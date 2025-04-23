#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <chrono>  // Untuk pengukuran waktu

using namespace std;
using namespace std::chrono;

struct Produk {
    int id;
    string label;
    float attr1;
    float attr2;
};

// Fungsi dominasi: A mendominasi B jika attr1 lebih kecil DAN attr2 lebih besar
bool mendominasi(const Produk& a, const Produk& b) {
    return (a.attr1 <= b.attr1) && (a.attr2 >= b.attr2);
}

vector<Produk> bacaCSV(const string& namaFile) {
    vector<Produk> data;
    ifstream file(namaFile);
    string baris;

    getline(file, baris); // Lewati header

    while (getline(file, baris)) {
        stringstream ss(baris);
        string token;
        Produk p;

        getline(ss, token, ','); p.id = stoi(token);
        getline(ss, token, ','); p.label = token;
        getline(ss, token, ','); p.attr1 = stof(token);
        getline(ss, token, ','); p.attr2 = stof(token);

        data.push_back(p);
    }

    return data;
}

vector<Produk> skylineQuery(const vector<Produk>& produkList) {
    stack<Produk> skyline;

    for (const Produk& kandidat : produkList) {
        bool didominasi = false;
        stack<Produk> sementara;

        while (!skyline.empty()) {
            Produk top = skyline.top();
            skyline.pop();

            if (mendominasi(top, kandidat)) {
                didominasi = true;
                sementara.push(top);
                break;
            } else if (!mendominasi(kandidat, top)) {
                sementara.push(top);
            }
        }

        while (!sementara.empty()) {
            skyline.push(sementara.top());
            sementara.pop();
        }

        if (!didominasi) {
            skyline.push(kandidat);
        }
    }

    vector<Produk> hasil;
    while (!skyline.empty()) {
        hasil.push_back(skyline.top());
        skyline.pop();
    }

    return hasil;
}

void simpanHasilSkylineCSV(const vector<Produk>& hasil, const string& namaFile) {
    ofstream file(namaFile);
    file << "id,label,attr_1,attr_2\n";
    for (const Produk& p : hasil) {
        file << p.id << "," << p.label << "," << p.attr1 << "," << p.attr2 << "\n";
    }
    file.close();
}

int main() {
    string namaFile = "ind_1000_2_product (1).csv";

    // Mulai hitung waktu
    auto mulai = high_resolution_clock::now();

    vector<Produk> semuaProduk = bacaCSV(namaFile);
    vector<Produk> hasilSkyline = skylineQuery(semuaProduk);

    // Selesai hitung waktu
    auto selesai = high_resolution_clock::now();
    auto durasi = duration_cast<milliseconds>(selesai - mulai);

    // Tampilkan hasil
    cout << "Produk hasil Skyline Query:\n";
    for (const Produk& p : hasilSkyline) {
        cout << p.id << " - " << p.label
             << " | attr_1: " << p.attr1
             << " | attr_2: " << p.attr2 << endl;
    }

    // Tampilkan durasi
    cout << "\nWaktu eksekusi program: " << durasi.count() << " ms" << endl;
    simpanHasilSkylineCSV(hasilSkyline, "skyline_output.csv");
    return 0;
}

