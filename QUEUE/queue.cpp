#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>
#include <string>
#include <chrono>
using namespace std;

struct Baju {
    int id;
    string namaProduk;
    int harga;
    int ulasan;
};

bool mendominasi(const Baju& a, const Baju& b) {
    return (a.harga <= b.harga && a.ulasan >= b.ulasan) &&
    (a.harga < b.harga || a.ulasan > b.ulasan);
}           

int main() {
    queue<Baju> queueBaju;
    vector<Baju> dataBaju;
    vector<Baju> hasilSkyline;

    // Membaca file CSV
    ifstream file("ind_1000_2_product.csv");
    if (!file.is_open()) {
        cout << "Gagal membuka file CSV!" << endl;
        system("pause");
        return 1;
    }

    string line;
    getline(file, line); // Lewati header

    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, namaStr, hargaStr, ulasanStr;
        getline(ss, idStr, ',');
        getline(ss, namaStr, ',');
        getline(ss, hargaStr, ',');
        getline(ss, ulasanStr, ',');

        if (hargaStr.empty() || ulasanStr.empty())
            continue;

        try {
            Baju baju;
            baju.id = stoi(idStr);
            baju.namaProduk = namaStr;
            baju.harga = stoi(hargaStr);
            baju.ulasan = stoi(ulasanStr);
            dataBaju.push_back(baju);
        } catch (...) {
            continue;
        }
    }

    cout << "========================================\n";
    cout << "         SKYLINE PRODUCTS (QUEUE)       \n";
    cout << "========================================\n";
    cout << "Total data terbaca: " << dataBaju.size() << "\n\n";

    for (const auto& baju : dataBaju) {
        queueBaju.push(baju);
    }

    // Mulai ukur waktu
    auto start = chrono::high_resolution_clock::now();

    while (!queueBaju.empty()) {
        Baju current = queueBaju.front();
        queueBaju.pop();
        bool didominasi = false;

        for (const auto& hasil : hasilSkyline) {
            if (mendominasi(hasil, current)) {
                didominasi = true;
                break;
            }
        }

        if (!didominasi) {
            // Hapus dari skyline yang didominasi oleh current
            vector<Baju> temp;
            for (const auto& hasil : hasilSkyline) {
                if (!mendominasi(current, hasil)) {
                    temp.push_back(hasil);
                }
            }
            temp.push_back(current);
            hasilSkyline = temp;
        }        
        
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    // Output hasil akhir
    cout << "Skyline Products:\n";
    for (const auto& b : hasilSkyline) {
        cout << "ID: " << b.id
             << ", Name: " << b.namaProduk
             << ", Price: " << b.harga
             << ", Review: " << b.ulasan << "\n";
    }

    cout << "\nTotal produk dalam skyline: " << hasilSkyline.size() << "\n";
    cout << "Waktu eksekusi program: "
         << chrono::duration_cast<chrono::milliseconds>(duration).count()
         << " ms\n";
    cout << "========================================\n";

    return 0;
}
