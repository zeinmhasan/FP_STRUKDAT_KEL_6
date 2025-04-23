
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;
using namespace std::chrono;

struct Product {
    int id;
    string label;
    int attr1;
    int attr2;
};

vector<Product> readCSV(const string& filename) {
    vector<Product> data;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "❌ File tidak ditemukan: " << filename << endl;
        return data;
    }

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string id_str, label, attr1_str, attr2_str;
        getline(ss, id_str, ',');
        getline(ss, label, ',');
        getline(ss, attr1_str, ',');
        getline(ss, attr2_str, ',');

        try {
            int id = stoi(id_str);
            int attr1 = stoi(attr1_str);
            int attr2 = stoi(attr2_str);
            data.push_back({id, label, attr1, attr2});
        } catch (...) {
            continue;
        }
    }

    return data;
}

vector<Product> skylineUsingMap(const vector<Product>& products) {
    // urutkan berdasarkan attr1 (harga)
    vector<Product> sorted = products;
    sort(sorted.begin(), sorted.end(), [](const Product& a, const Product& b) {
        return a.attr1 < b.attr1;
    });

    // map<attr1, attr2>
    map<int, Product> skyline;

    for (const auto& p : sorted) {
        bool dominated = false;
        for (const auto& [_, q] : skyline) {
            if (q.attr1 <= p.attr1 && q.attr2 >= p.attr2) {
                dominated = true;
                break;
            }
        }

        if (!dominated) {
            skyline[p.attr1] = p;

            // Hapus produk yang sekarang didominasi oleh p
            auto it = skyline.upper_bound(p.attr1);
            while (it != skyline.end()) {
                if (it->second.attr2 <= p.attr2) {
                    it = skyline.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }

    // ubah ke vector hasil
    vector<Product> result;
    for (const auto& [_, p] : skyline) {
        result.push_back(p);
    }

    return result;
}

int main() {
    string filename = "ind_1000_2_product.csv";
    vector<Product> data = readCSV(filename);
    cout << "Jumlah produk dalam data: " << data.size() << endl;

    auto start = high_resolution_clock::now();
    vector<Product> skyline = skylineUsingMap(data);
    auto end = high_resolution_clock::now();
    auto durationMs = duration_cast<milliseconds>(end - start);
    auto durationUs = duration_cast<microseconds>(end - start);

    cout << "\nHasil Skyline:\n";
    for (const auto& p : skyline) {
        cout << p.label << " - attr1: " << p.attr1 << ", attr2: " << p.attr2 << endl;
    }

    cout << "\nJumlah produk dalam skyline: " << skyline.size() << endl;
    cout << "Waktu komputasi: " << durationMs.count() << " ms (" 
    << durationUs.count() << " µs)" << endl;

    return 0;
}
