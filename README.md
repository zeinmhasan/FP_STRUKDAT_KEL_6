# FP_STRUKDAT_KEL_6

| Nama  | NRP | STRUKTUR DATA |
| ------------- | ------------- | ------------- |
| Mey Rosalina | 5027241004 | QUEUE |
| Clarissa Aydin Rahmazea | 5027241014 | ARRAY |
| Zein Muhammad Hasan  | 5027241035  | STACK |
| Ahmad Idza Anaafin | 5027241017 | HASH TABLE |
| Muhammad Fatihul Qolbi Ash Shiddiqi | 5027241023 | LINKED LIST |
| Danar Bagus Rasendriya | 5027231055 | MAP |

<nav>
  <ul>
    <li><a href="#1">Array</a></li>
    <li><a href="#2">Linked list</a></li>
    <li><a href="#3">Stack</a></li>
    <li><a href="#4">Queue</a></li>
    <li><a href="#5">Hash table</a></li>
    <li><a href="#6">Map</a></li>
  </ul>
</nav>

<h2 id= 1>Array</h2> 

  ```
  kode
  ```
  Penjelasan
  screensot
<h2 id= 2>Linked List</h2>

  ```
  kode
  ```
  Penjelasan
  screensot
<h2 id= 3>Stack</h2>

untuk mengimplementasikan skyline query dengan struktur data stack kita memerlukan library ini
  ```cpp
#include <iostream>   // Untuk input dan output standar seperti std::cout dan std::cin
#include <fstream>    // Untuk operasi file seperti membaca dan menulis file
#include <sstream>    // Untuk manipulasi string stream, seperti std::istringstream untuk parsing string
#include <vector>     // Untuk penggunaan struktur data vektor (array dinamis)
#include <stack>      // Untuk penggunaan struktur data stack (tumpukan)
#include <chrono>     // Untuk pengukuran waktu, misalnya menghitung lama eksekusi program
  ```
lalu buat struck untuk menampung data data yang nanti akan kita compare 
```cpp
struct Produk {
    int id;
    string label;
    float attr1;
    float attr2;
};
```

pada kasus kali ini kita membuat sebuah syarat dominasi yaitu: <br>
attr1 = harga dan attr2 = review <br>
jika kondisi 
``` cpp
bool mendominasi(const Produk& a, const Produk& b) {
    return (a.attr1 <= b.attr1) && (a.attr2 >= b.attr2);
}
```
maka produk a akan mendominasi produk b dan pastinya probuk b akan keluar dari list skyline query

untuk menginput data dari file csv kita perlu sedikit ngoding seperti ini:

```cpp
vector<Produk> bacaCSV(const string& namaFile) {
    vector<Produk> data;          // Vektor untuk menyimpan semua data produk yang dibaca dari file
    ifstream file(namaFile);      // Membuka file CSV menggunakan input file stream
    string baris;

    getline(file, baris);         // Membaca baris pertama (header) dan melewatinya

    while (getline(file, baris)) {       // Membaca setiap baris berikutnya dalam file
        stringstream ss(baris);          // Mengubah baris menjadi stream untuk di-parse per kolom
        string token;
        Produk p;                        // Objek Produk sementara untuk menyimpan data satu baris

        // Membaca token (data antar koma) satu per satu dan mengisi atribut Produk
        getline(ss, token, ','); p.id = stoi(token);     // Mengambil dan mengonversi kolom pertama ke integer
        getline(ss, token, ','); p.label = token;        // Mengambil kolom kedua sebagai string
        getline(ss, token, ','); p.attr1 = stof(token);  // Mengambil kolom ketiga sebagai float
        getline(ss, token, ','); p.attr2 = stof(token);  // Mengambil kolom keempat sebagai float

        data.push_back(p);         // Menambahkan objek Produk ke dalam vektor data
    }

    return data;   // Mengembalikan semua data produk yang telah dibaca
}
```
Fungsi bacaCSV bertugas membaca data dari sebuah file CSV, lalu mengubah setiap baris (kecuali header) menjadi objek bertipe Produk, dan mengembalikan semua objek itu dalam bentuk vector<Produk>.<br>

```cpp
vector<Produk> data;
ifstream file(namaFile);
```
Membuat vektor kosong bernama data untuk menyimpan semua produk yang akan dibaca.
Lalu membuka file dengan nama yang diberikan lewat parameter namaFile, menggunakan stream file input (ifstream).

```cpp
while (getline(file, baris)) {
 stringstream ss(baris);
string token;
    Produk p;
```
Selama masih ada baris yang bisa dibaca dari file, lakukan proses parsing: Mengubah satu baris string menjadi objek stringstream agar bisa dipecah berdasarkan koma (,). token digunakan untuk menyimpan data antar koma, dan p adalah objek Produk sementara untuk setiap baris.

```cpp
getline(ss, token, ','); p.id = stoi(token);
getline(ss, token, ','); p.label = token;
getline(ss, token, ','); p.attr1 = stof(token);
getline(ss, token, ','); p.attr2 = stof(token);
```
membaca kolom pertama, kedua, ketiga, dan ke empat dan di simpan di masing masing variabel nya.

```cpp
return data;
```
kembalikan semua data yang dibaca fungsi ini

```cpp
vector<Produk> skylineQuery(const vector<Produk>& produkList) {
    stack<Produk> skyline;  // Stack untuk menyimpan kandidat produk yang masuk ke dalam hasil skyline

    // Iterasi semua produk dalam list
    for (const Produk& kandidat : produkList) {
        bool didominasi = false;          // Penanda apakah kandidat didominasi oleh produk lain
        stack<Produk> sementara;          // Stack sementara untuk menyimpan elemen yang sudah diproses

        // Proses untuk membandingkan kandidat dengan elemen-elemen dalam stack skyline
        while (!skyline.empty()) {
            Produk top = skyline.top();   // Ambil produk paling atas dari stack
            skyline.pop();                // Keluarkan produk tersebut dari stack

            if (mendominasi(top, kandidat)) {
                // Jika produk di stack mendominasi kandidat, maka kandidat tidak lolos skyline
                didominasi = true;
                sementara.push(top);      // Simpan kembali produk ini ke stack sementara
                break;                    // Langsung hentikan pengecekan lebih lanjut
            } else if (!mendominasi(kandidat, top)) {
                // Jika kandidat tidak mendominasi produk di stack, simpan produk itu ke stack sementara
                sementara.push(top);
            }
            // Catatan: Jika kandidat mendominasi top, maka top akan di-drop (tidak dikembalikan ke stack)
        }

        // Kembalikan semua elemen dari stack sementara ke stack utama
        while (!sementara.empty()) {
            skyline.push(sementara.top());
            sementara.pop();
        }

        // Jika kandidat tidak didominasi oleh siapapun, masukkan ke dalam skyline
        if (!didominasi) {
            skyline.push(kandidat);
        }
    }

    // Pindahkan isi stack skyline ke dalam vektor hasil
    vector<Produk> hasil;
    while (!skyline.empty()) {
        hasil.push_back(skyline.top());
        skyline.pop();
    }

    return hasil;  // Kembalikan hasil akhir produk yang termasuk dalam skyline
}

```
Fungsi skylineQuery digunakan untuk menentukan produk-produk unggulan (skyline) dari daftar produkList, berdasarkan kriteria dominasi. Produk yang tidak didominasi oleh produk lain akan dimasukkan ke dalam hasil.<br>
Konsep Dominasi : <br>
Produk A mendominasi produk B jika:
<br>
A.attr1 < B.attr1 dan
<br>
A.attr1 > B.attr2
<br>
Artinya, A lebih baik dalam dua arah: attr1 lebih kecil dari B (harga), tapi juga lebih besar dari attr2 B (review).
```cpp
for (const Produk& kandidat : produkList) {
bool didominasi = false;
stack<Produk> sementara;
```
Fungsi akan mengecek setiap kandidat apakah layak masuk ke dalam skyline.
<li>
  didominasi akan jadi true jika kandidat terbukti dikalahkan oleh produk yang sudah ada.
</li>
<li>
  Stack sementara dipakai untuk menyimpan produk yang diambil sementara dari skyline saat pengecekan.
</li>

```cpp
while (!skyline.empty()) {
    Produk top = skyline.top();
    skyline.pop();
if (mendominasi(top, kandidat)) {
    didominasi = true;
    sementara.push(top);
    break;
}
else if (!mendominasi(kandidat, top)) {
    sementara.push(top);
}
```
Bandingkan dengan Stack Skyline <br>
<li>Ambil satu per satu produk dari skyline untuk dibandingkan dengan kandidat.</li>
<li>Kalau produk yang sudah ada (top) mendominasi kandidat, maka kandidat tidak dimasukkan.</li>
<li>Kalau kandidat tidak mendominasi produk yang sudah ada, maka produk itu tetap disimpan.</li>
<li>Jika kandidat mendominasi top, maka top dibuang dari skyline.</li>
<br>
Kembalikan Stack Sementara

```cpp
while (!sementara.empty()) {
    skyline.push(sementara.top());
    sementara.pop();
}
```

Kalau kandidat aman dari dominasi, ia masuk ke skyline. (push)

```cpp
if (!didominasi) {
    skyline.push(kandidat);
}
```

Isi stack dipindahkan ke vector agar hasil bisa dikembalikan dalam bentuk yang lebih fleksibel. **(yang udah di pindahkan ini hasil yang nggk terdominasi alias list fiks dari skyline query)**.
```cpp
vector<Produk> hasil;
while (!skyline.empty()) {
    hasil.push_back(skyline.top());
    skyline.pop();
}
```
<br>
untuk dokumentasi atau penyimpanan data hasil tadi alangkah baiknya kita simpan di file word, begini kode nya

```cpp
void simpanHasilSkylineCSV(const vector<Produk>& hasil, const string& namaFile) {
    ofstream file(namaFile);  // Membuka file output untuk ditulis (akan menimpa jika sudah ada)

    file << "id,label,attr_1,attr_2\n";  // Menuliskan baris header ke file CSV

    // Menuliskan setiap data produk hasil skyline ke dalam file
    for (const Produk& p : hasil) {
        file << p.id << ","              // Menulis ID produk
             << p.label << ","           // Menulis label produk
             << p.attr1 << ","           // Menulis nilai atribut 1
             << p.attr2 << "\n";         // Menulis nilai atribut 2 diikuti newline
    }

    file.close();  // Menutup file setelah selesai menulis
}
```

penjelasan singkat <br>
<li>Fungsi ini menyimpan data hasil dari skyline query ke dalam file .csv.</li>
<li>Format output tetap konsisten: kolom dipisahkan koma (CSV).</li>
<li>Header ditulis terlebih dahulu, lalu setiap baris data ditulis sesuai isi vector<Produk>.</li>

fungi main

```cpp
int main() {
    string namaFile = "ind_1000_2_product (1).csv";

    // Mulai hitung waktu
    auto mulai = high_resolution_clock::now();

    vector<Produk> semuaProduk = bacaCSV(namaFile);
    vector<Produk> hasilSkyline = skylineQuery(semuaProduk);

    // Selesai hitung waktu
    auto selesai = high_resolution_clock::now();
    auto durasi = duration_cast<microseconds>(selesai - mulai);

    // Tampilkan hasil
    cout << "Produk hasil Skyline Query:\n";
    for (const Produk& p : hasilSkyline) {
        cout << p.id << " - " << p.label
             << " | attr_1: " << p.attr1
             << " | attr_2: " << p.attr2 << endl;
    }

    // Tampilkan durasi
    cout << "\nWaktu eksekusi program: " << durasi.count() << " micro" << endl;
    simpanHasilSkylineCSV(hasilSkyline, "skyline_output.csv");
    return 0;
}
```

Tujuan Fungsi main()
Fungsi ini menjalankan keseluruhan proses:
<br>
1.Membaca file CSV
<br>
2.Melakukan skyline query
<br>
3.Mengukur waktu eksekusi
<br>
4.Menampilkan hasil
<br>
5.Menyimpan hasil ke file output

```cpp
string namaFile = "ind_1000_2_product (1).csv";
auto mulai = high_resolution_clock::now();
```
Menyimpan nama file CSV sumber data yang akan dibaca.
Menyimpan waktu saat program mulai menjalankan proses utama, menggunakan std::chrono untuk mengukur durasi eksekusi.

```cpp
vector<Produk> semuaProduk = bacaCSV(namaFile);
vector<Produk> hasilSkyline = skylineQuery(semuaProduk);
auto selesai = high_resolution_clock::now();
auto durasi = duration_cast<microseconds>(selesai - mulai);
```
Membaca seluruh data produk dari file CSV dan menyimpannya dalam vektor semuaProduk.
<br>
Melakukan skyline query terhadap semua produk untuk mendapatkan produk-produk yang tidak didominasi.

```cpp
cout << "Produk hasil Skyline Query:\n";
for (const Produk& p : hasilSkyline) {
    cout << p.id << " - " << p.label
         << " | attr_1: " << p.attr1
         << " | attr_2: " << p.attr2 << endl;
cout << "\nWaktu eksekusi program: " << durasi.count() << " micro" << endl;
}
```
Menampilkan setiap produk hasil skyline lengkap dengan ID, label, dan dua atributnya serta waktu yang diperlukannya.

```cpp
simpanHasilSkylineCSV(hasilSkyline, "skyline_output.csv");
```
Menyimpan hasil skyline ke dalam file CSV bernama skyline_output.csv.


<h2 id= 4>Queue</h2> 

  ```
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

// Fungsi untuk menentukan apakah A mendominasi B
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
  ```
  Penjelasan
  screensot
<h2 id= 5>Hash Table</h2>

  ```
  kode
  ```
  Penjelasan
  screensot
<h2 id= 6>Map</h2> 

  ```
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
  ```
  ### Penjelasan
Struktur Data: Map (Dictionary)

Kompleksitas Waktu: O(n log n)
  
  ### Output
  ![Screenshot 2025-04-23 at 12 43 26](https://github.com/user-attachments/assets/744df6ca-ff9c-486e-8f0d-f46fa54f6a51)

## Analisis
  ```
  pake code dijadiin satu ga? terus nanti outputnya kek pake struktur data ini brp ms
  ```

  atau cuman penjelasan dan kesimpulan sama ssan

