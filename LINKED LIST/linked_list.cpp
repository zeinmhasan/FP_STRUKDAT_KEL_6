#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
using namespace std;
using namespace std::chrono;

struct Item {
    string id;
    string category;
    double price;
    double reviewScore;
    Item* next;
};

class LinkedList {
private:
    Item* head;
    int totalDataCount = 0;

public:
    LinkedList() : head(nullptr) {}

    void insert(string id, string category, double price, double reviewScore) {
        Item* newItem = new Item{id, category, price, reviewScore, nullptr};
        if (!head) {
            head = newItem;
        } else {
            Item* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newItem;
        }
        totalDataCount++;
    }

    bool dominates(Item* a, Item* b) {
        return (a->price <= b->price && a->reviewScore >= b->reviewScore) &&
               (a->price < b->price || a->reviewScore > b->reviewScore);
    }

    void skylineQuery() {
      LinkedList skyline;
  
      for (Item* curr = head; curr != nullptr; curr = curr->next) {
          bool isDominated = false;
          Item* temp = skyline.head;
          Item* prev = nullptr;
  
          while (temp != nullptr) {
              if (dominates(temp, curr)) {
                  isDominated = true;
                  break;
              } else if (dominates(curr, temp)) {
                  if (prev == nullptr) {
                      skyline.head = temp->next;
                  } else {
                      prev->next = temp->next;
                  }
                  Item* toDelete = temp;
                  temp = temp->next;
                  delete toDelete;
                  continue;
              }
              prev = temp;
              temp = temp->next;
          }
  
          if (!isDominated) {
              skyline.insert(curr->id, curr->category, curr->price, curr->reviewScore);
          }
      }
  
      cout << "Skyline Items (Best Clothes):\n";
      int skylineCount = 0;
      for (Item* temp = skyline.head; temp != nullptr; temp = temp->next) {
          string idNum = temp->id;  
          cout << idNum << " | product-" << idNum
               << " | Price: " << static_cast<int>(temp->price)
               << " | Review Score: " << static_cast<int>(temp->reviewScore) << "\n";
          skylineCount++;
      }
  
      cout << "\nJumlah produk dalam skyline: " << skylineCount << endl;
      cout << "Jumlah produk dalam data: " << totalDataCount << endl;
  }
  
    void loadFromCSV(const string& filename) {
        ifstream file(filename);
        string line;
        getline(file, line); // skip header

        while (getline(file, line)) {
            stringstream ss(line);
            string id, category, priceStr, reviewStr;
            getline(ss, id, ',');
            getline(ss, category, ',');
            getline(ss, priceStr, ',');
            getline(ss, reviewStr, ',');

            double price = stod(priceStr);
            double reviewScore = stod(reviewStr);

            insert(id, category, price, reviewScore);
        }
    }
};

int main() {
    LinkedList clothes;

    auto start = high_resolution_clock::now();

    clothes.loadFromCSV("ind_1000_2_product.csv");
    clothes.skylineQuery();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "\nWaktu komputasi: " << duration.count() << " ms" << endl;

    return 0;
}

