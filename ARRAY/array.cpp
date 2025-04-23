#include <chrono> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

struct Shirt {
    int price;
    float review;
};

// Fungsi untuk menentukan dominasi
bool dominates(Shirt a, Shirt b) {
    return (a.price <= b.price && a.review >= b.review) &&
           (a.price < b.price || a.review > b.review);
}

// Fungsi utama Skyline Query
vector<Shirt> skylineQuery(const vector<Shirt>& shirts) {
    vector<Shirt> skyline;
    for (const auto& s : shirts) {
        bool isDominated = false;
        for (const auto& sk : skyline) {
            if (dominates(sk, s)) {
                isDominated = true;
                break;
            }
        }
        if (!isDominated) {
            vector<Shirt> newSkyline;
            for (const auto& sk : skyline) {
                if (!dominates(s, sk)) {
                    newSkyline.push_back(sk);
                }
            }
            newSkyline.push_back(s);
            skyline = newSkyline;
        }
    }
    return skyline;
}

vector<Shirt> readCSV(const string& filename) {
    vector<Shirt> shirts;
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cerr << "Gagal membuka file: " << filename << endl;
        return shirts;
    }

    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, nameStr, attr1Str, attr2Str;

        getline(ss, idStr, ',');    // ID
        getline(ss, nameStr, ',');  // Nama produk (product-1, etc.)
        getline(ss, attr1Str, ','); // Harga
        getline(ss, attr2Str, ','); // Review

        if (attr1Str.empty() || attr2Str.empty()) continue;

        try {
            int attr1 = stoi(attr1Str);     // Harga
            float attr2 = stof(attr2Str);   // Review
            shirts.push_back({attr1, attr2});
        } catch (const invalid_argument& e) {
            cerr << "Baris tidak valid, dilewati: " << line << endl;
            continue;
        }
    }

    return shirts;
}


int main() {
    string filename = "ind_1000_2_product.csv";
    vector<Shirt> shirts = readCSV(filename);

    auto start = chrono::high_resolution_clock::now();
    vector<Shirt> skyline = skylineQuery(shirts);
    auto end = chrono::high_resolution_clock::now();

    cout << "Total Items: " << shirts.size() << endl;
    cout << "Skyline Results (Best Shirts): " << skyline.size() << endl;

    for (const auto& s : skyline) {
        cout << "Price: " << s.price << ", Review: " << s.review << endl;
    }

    // Hitung waktu eksekusi
    auto elapsed = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "\nExecution Time: " << elapsed.count() << " microseconds" << endl;

    return 0;
}
