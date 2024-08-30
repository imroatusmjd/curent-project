#include <iostream>
#include <string>
#include <ctime>
#include <sstream>

using namespace std;
char pilihan;

// Fungsi pembantu untuk mengonversi integer menjadi string
template <typename T>
string to_string(T value)
{
    ostringstream os;
    os << value;
    return os.str();
}

// Struktur untuk menyimpan informasi buku
struct Buku
{
    string judul;
    string pengarang;
    string isbn; // Variable untuk ID buku
    int stok;
    string edisi;
    string kategori;
    string lokasi;
    bool tersedia;
    Buku *next;
    Buku *pengarangNext; // Pointer ke buku berikutnya oleh pengarang yang sama
};

// Node awal Linked list untuk menyimpan buku
Buku *daftarBuku = NULL;


// Struktur untuk menyimpan informasi mahasiswa
struct Mahasiswa
{
    string nama;
    string id;
    int tahun;
    int semester;
    string tanggalPeminjaman;
    Buku *bukuDipinjam;
    Mahasiswa *next;
};

// Node awal Linked list untuk menyimpan mahasiswa
Mahasiswa *daftarMahasiswa = NULL;

// Struktur untuk menyimpan catatan peminjaman dalam hash table
struct Peminjaman
{
    string isbn;
    string namaMahasiswa;
    string idMahasiswa;
    string tanggalPeminjaman;
    Peminjaman *next; // Pointer untuk handling collision
    string kodePeminjaman;
};

// Ukuran hash table
const int TABLE_SIZE = 100;

// Hash table untuk menyimpan catatan peminjaman
Peminjaman *hashTable[TABLE_SIZE];

// Struktur untuk BST yang mengklasifikasikan buku berdasarkan pengarang
struct PengarangNode
{
    string pengarang;
    Buku *daftarBuku;
    PengarangNode *left;
    PengarangNode *right;
};

// Root dari BST untuk mengklasifikasikan buku berdasarkan pengarang
PengarangNode *pengarangRoot = NULL;

// Fungsi hash untuk menghitung indeks dari ISBN
int hashFunction(string isbn)
{
    int hash = 0;
    for (char c : isbn)
    {
        hash = (hash * 31 + c) % TABLE_SIZE;
    }
    return hash;
}

// Fungsi untuk halaman login admin
bool loginAdmin() {
    string username, password;
    cout << "Masukkan username: ";
    cin >> username;
    cout << "Masukkan password: ";
    cin >> password;
    if (username == "admin" && password == "admin123") {
        return true;
    } else {
        cout << "Username atau password salah." << endl;
        return false;
    }
}

//Fungsi untuk membuat id buku
string generateIdBuku(const string& judul, const string& pengarang, const string& edisi) {
    return judul.substr(0, 1) + pengarang.substr(0, 1) + edisi.substr(0, 1);
}

// Fungsi untuk mencari buku berdasarkan judul
Buku *cariBuku(string judul)
{
    Buku *temp = daftarBuku;
    while (temp != NULL)
    {
        if (temp->judul == judul)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL; // Buku tidak ditemukan
}

// Fungsi untuk membuat node pengarang baru
PengarangNode* buatNodePengarang(string pengarang, Buku* buku)
{
    PengarangNode* newNode = new PengarangNode();
    newNode->pengarang = pengarang;
    newNode->daftarBuku = buku;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Fungsi untuk memasukkan buku ke dalam BST berdasarkan pengarang
PengarangNode* masukkanBukuBerdasarkanPengarang(PengarangNode* root, Buku* buku)
{
    if (root == NULL)
    {
        return buatNodePengarang(buku->pengarang, buku);
    }

    if (buku->pengarang < root->pengarang)
    {
        root->left = masukkanBukuBerdasarkanPengarang(root->left, buku);
    }
    else if (buku->pengarang > root->pengarang)
    {
        root->right = masukkanBukuBerdasarkanPengarang(root->right, buku);
    }
    else // Jika pengarang sudah ada
    {
        buku->pengarangNext = root->daftarBuku;
        root->daftarBuku = buku;
    }

    return root;
}

// Fungsi untuk mengklasifikasikan buku berdasarkan pengarang
void klasifikasikanBukuBerdasarkanPengarang()
{
    Buku* temp = daftarBuku;
    while (temp != NULL)
    {
        pengarangRoot = masukkanBukuBerdasarkanPengarang(pengarangRoot, temp);
        temp = temp->next;
    }
}

// Fungsi pembantu untuk menampilkan buku oleh pengarang tertentu
void tampilkanBukuBerdasarkanPengarang(Buku* buku)
{
    while (buku != NULL)
    {
        cout << "Judul: " << buku->judul << ", Edisi: " << buku->edisi << ", ID: "<< buku->isbn << "Kategori: " << buku->kategori << ", Stok: " << buku->stok << ", Tersedia: " << (buku->tersedia ? "Ya" : "Tidak") << endl;
        buku = buku->pengarangNext;
    }
}

// Fungsi untuk menampilkan BST dalam traversal in-order
void tampilkanPengarangInOrder(PengarangNode* root)
{
    if (root != NULL)
    {
        tampilkanPengarangInOrder(root->left);
        cout << "Pengarang: " << root->pengarang << endl;
        tampilkanBukuBerdasarkanPengarang(root->daftarBuku);
        tampilkanPengarangInOrder(root->right);
    }
}

// Fungsi untuk menampilkan buku yang sudah diklasifikasikan berdasarkan pengarang
void tampilkanBukuTerkelasifikasi()
{
    cout << "=== Buku Terkelasifikasi Berdasarkan Pengarang ===" << endl;
    tampilkanPengarangInOrder(pengarangRoot);
}

//Fungsi Mengurutkan buku berdasarkan kategori
void urutkanBukuKategori(Buku*& head) {
    if (head == nullptr || head->next == nullptr) return;

    Buku* sorted = nullptr;
    Buku* current = head;

    while (current != nullptr) {
        Buku* next = current->next;
        if (sorted == nullptr || sorted->kategori > current->kategori) {
            current->next = sorted;
            sorted = current;
        } else {
            Buku* temp = sorted;
            while (temp->next != nullptr && temp->next->kategori <= current->kategori) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    head = sorted;
}


//Fungsi mengurutkan buku sesuia abjad judul
void urutkanBukuAbjad(Buku*& head) {
    if (head == nullptr || head->next == nullptr) return;

    Buku* sorted = nullptr;
    Buku* current = head;

    while (current != nullptr) {
        Buku* next = current->next;
        if (sorted == nullptr || sorted->judul > current->judul) {
            current->next = sorted;
            sorted = current;
        } else {
            Buku* temp = sorted;
            while (temp->next != nullptr && temp->next->judul <= current->judul) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    head = sorted;
}

// Fungsi untuk menambah buku
void tambahBuku()
{
    cout << "=== Tambah Buku ===" << endl;

    int jumlahBuku, stok;
    cout << "Masukkan jumlah buku yang ingin ditambahkan: ";
    cin >> jumlahBuku;
    cin.ignore(); // Menangani karakter newline yang tertinggal

    for (int i = 0; i < jumlahBuku; i++)
    {
        cin.ignore(); // Mengabaikan karakter newline
        string judul, pengarang, edisi, isbn, kategori, tahunterbit;
        cout << "\nBuku " << i + 1 << ":\n";
        cout << "Masukkan judul buku: ";
        getline(cin, judul);
        cout << "Masukkan penulis: ";
        getline(cin, pengarang);
        cout << "Masukkan edisi: ";
        getline(cin, edisi);
        cout << "Masukkan kategori: ";
        getline(cin, kategori);
        cout << "Masukkan stok: ";
        cin >> stok;

        Buku *bukuBaru = new Buku();
        bukuBaru->judul = judul;
        bukuBaru->pengarang = pengarang;
        bukuBaru->stok = stok;
        bukuBaru->edisi = edisi;
        bukuBaru->lokasi = bukuBaru->kategori.substr(0, 3) + to_string(rand() % 10); // Lokasi dummy
        bukuBaru->isbn = generateIdBuku(bukuBaru->judul, bukuBaru->pengarang, bukuBaru->edisi);
        bukuBaru->kategori = kategori;
        bukuBaru->tersedia = true;
        bukuBaru->next = NULL;
        bukuBaru->pengarangNext = NULL;

        if (daftarBuku == NULL)
        {
            daftarBuku = bukuBaru;
        }
        else
        {
            Buku *temp = daftarBuku;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = bukuBaru;
        }

        cout << "Buku berhasil ditambahkan!" << endl;
    }
}

// Fungsi untuk menghapus buku
void hapusBuku()
{
    string judul;

    cout << "=== Hapus Buku ===" << endl;
    cout << "Masukkan judul buku yang akan dihapus: ";
    cin.ignore();
    getline(cin, judul);

    Buku *temp = daftarBuku;
    Buku *prev = NULL;
    bool ditemukan = false;

    Mahasiswa *temp2 = daftarMahasiswa;
    Mahasiswa *prev2 = NULL;
    bool ditemukan2 = false;

    while (temp2 != NULL)
    {
        if (temp2->bukuDipinjam->judul == judul)
        {
            ditemukan2 = true;
            break;
        }
        prev2 = temp2;
        temp2 = temp2->next;
    }

    while (temp != NULL)
    {
        if (temp->judul == judul)
        {
            ditemukan = true;
            break;
        }
        prev = temp;
        temp = temp->next;
    }

    if (ditemukan && temp->tersedia && !ditemukan2)
    {
        if (prev == NULL)
        {
            daftarBuku = temp->next;
        }
        else
        {
            prev->next = temp->next;
        }
        delete temp;
        cout << "Buku berhasil dihapus!" << endl;
    }
    else
    {
        if (!temp->tersedia || ditemukan2)
        {
            cout << "Buku sedang dipinjam" << endl;
        }
        else
        {
            cout << "Buku tidak ditemukan!" << endl;
        }
    }
}

// Fungsi untuk menghasilkan kode peminjaman unik
string generateKodePeminjaman(string isbn, string idMahasiswa)
{
    // Menggunakan ISBN dan ID Mahasiswa untuk menghasilkan kode unik
    return isbn + "-" + idMahasiswa;
}

// Fungsi untuk menambah catatan peminjaman ke dalam hash table
void tambahPeminjamanKeHashTable(string isbn, string namaMahasiswa, string idMahasiswa, string tanggalPeminjaman)
{
    int index = hashFunction(isbn);
    Peminjaman *newPeminjaman = new Peminjaman();
    newPeminjaman->isbn = isbn;
    newPeminjaman->namaMahasiswa = namaMahasiswa;
    newPeminjaman->idMahasiswa = idMahasiswa;
    newPeminjaman->tanggalPeminjaman = tanggalPeminjaman;
    newPeminjaman->next = NULL;
    newPeminjaman->kodePeminjaman = generateKodePeminjaman(isbn, idMahasiswa); // Menghasilkan kode peminjaman

    if (hashTable[index] == NULL)
    {
        hashTable[index] = newPeminjaman;
    }
    else
    {
        Peminjaman *temp = hashTable[index];
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newPeminjaman;
    }
}

// Fungsi untuk menghapus catatan peminjaman dari hash table
void hapusPeminjamanDariHashTable(string isbn)
{
    int index = hashFunction(isbn);
    Peminjaman *temp = hashTable[index];
    Peminjaman *prev = NULL;

    while (temp != NULL && temp->isbn != isbn)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        return; // Tidak ditemukan
    }

    if (prev == NULL)
    {
        hashTable[index] = temp->next;
    }
    else
    {
        prev->next = temp->next;
    }

    delete temp;
}

// Fungsi untuk meminjam buku
void pinjamBuku()
{
    string judul, pengarang, isbn;
    cout << "=== Pinjam Buku ===" << endl;
    cout << "Masukkan judul buku: ";
    cin.ignore();
    getline(cin, judul);
    cout << "Masukkan nama pengarang: ";
    getline(cin, pengarang);

    Buku *temp = daftarBuku;
    bool ditemukan = false;

    while (temp != NULL)
    {
        if (temp->judul == judul && temp->pengarang == pengarang && temp->tersedia == true && temp->isbn == isbn) // Menambahkan syarat pencarian berdasarkan ISBN
        {
            ditemukan = true;
            break;
        }
        temp = temp->next;
    }

    if (ditemukan && temp->tersedia)
    {
        string nama, id;
        int semester;
        cout << "Masukkan nama mahasiswa: ";
        getline(cin, nama);
        cout << "Masukkan ID mahasiswa: ";
        getline(cin, id);
        cout << "Masukkan semester mahasiswa: ";
        cin >> semester;

        time_t now = time(0);
        tm *currentDate = localtime(&now);
        string tanggalPeminjaman = to_string(currentDate->tm_mday) + "/" + to_string(currentDate->tm_mon + 1) + "/" + to_string(currentDate->tm_year + 1900);

        if (temp != NULL)
        {
            Mahasiswa *mahasiswaBaru = new Mahasiswa();
            mahasiswaBaru->nama = nama;
            mahasiswaBaru->id = id;
            mahasiswaBaru->semester = semester;
            mahasiswaBaru->tanggalPeminjaman = tanggalPeminjaman;
            mahasiswaBaru->bukuDipinjam = temp;
            mahasiswaBaru->next = NULL;

            temp->stok = temp->stok - 1;
            if (temp->stok < 1)
            {
                temp->tersedia = false;
            }

            if (daftarMahasiswa == NULL)
            {
                daftarMahasiswa = mahasiswaBaru;
            }
            else
            {
                Mahasiswa *temp2 = daftarMahasiswa;
                while (temp2->next != NULL)
                {
                    temp2 = temp2->next;
                }
                temp2->next = mahasiswaBaru;
            }

            // Masukkan data peminjaman ke hash table
            tambahPeminjamanKeHashTable(isbn, nama, id, tanggalPeminjaman);

            cout << "Buku berhasil dipinjam!" << endl;
        }
        else
        {
            cout << "Tidak ada buku yang tersedia untuk dipinjam." << endl;
        }
    }
    else
    {
        cout << "Buku tidak tersedia atau tidak ditemukan!" << endl;
    }
}



// Fungsi untuk mengembalikan buku
void kembalikanBuku()
{
    cout << "=== Kembalikan Buku ===" << endl;
    string nama, id, isbn;
    cout << "Masukkan nama mahasiswa: ";
    cin.ignore();
    getline(cin, nama);
    cout << "Masukkan ID mahasiswa: ";
    getline(cin, id);
    cout << "Masukkan ID buku: ";
    getline(cin, isbn);

    Mahasiswa *temp = daftarMahasiswa;
    Buku *temp2 = daftarBuku;
    Mahasiswa *previous = NULL;
    bool ditemukan = false;

    while (temp != NULL)
    {
        if (temp->nama == nama && temp->id == id)
        {
            ditemukan = true;
            break;
        }
        previous = temp;
        temp = temp->next;
    }

    if (ditemukan)
    {
        if (previous == NULL)
        {
            daftarMahasiswa = temp->next;
        }
        else
        {
            previous->next = temp->next;
        }

        Buku *buku = temp->bukuDipinjam;
        buku->stok += 1;
        buku->tersedia = true;

        delete temp;

        // Hapus catatan peminjaman dari hash table
        hapusPeminjamanDariHashTable(isbn);

        cout << "Buku berhasil dikembalikan!" << endl;
    }
    else
    {
        cout << "Tidak ada catatan peminjaman untuk mahasiswa ini." << endl;
    }
}

// Fungsi untuk menampilkan daftar buku
void tampilkanDaftarBuku()
{
    cout << "=== Daftar Buku di Toko Buku ===" << endl;
    Buku *temp = daftarBuku;
    while (temp != NULL)
    {
        cout << "Judul: " << temp->judul << ", Pengarang: " << temp->pengarang << ", Edisi: " << temp->edisi << "ID: " << temp->isbn << ", Stok: " << temp->stok << ", Tersedia: " << (temp->tersedia ? "Ya" : "Tidak") << endl;
        temp = temp->next;
    }
}

// Fungsi untuk menampilkan info peminjaman
// Fungsi untuk menampilkan info peminjaman dari hash table
void tampilkanInfoPeminjaman()
{
    cout << "=== Info Peminjaman ===" << endl;
    
    // Tampilkan info peminjaman dari daftar mahasiswa
    Mahasiswa *temp = daftarMahasiswa;
    while (temp != NULL)
    {
        cout << "Nama: " << temp->nama << ", ID: " << temp->id << ", Semester: " << temp->semester << ", Tanggal Peminjaman: " << temp->tanggalPeminjaman << ", Buku Dipinjam: " << temp->bukuDipinjam->judul << endl;
        temp = temp->next;
    }

    // Tampilkan info peminjaman dari hash table
    cout << "=== Info Peminjaman dari HashTable ===" << endl;

    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        Peminjaman *tempPeminjaman = hashTable[i];
        while (tempPeminjaman != NULL)
        {
            cout << "ID: " << tempPeminjaman->isbn << ", Nama Mahasiswa: " << tempPeminjaman->namaMahasiswa << ", ID Mahasiswa: " << tempPeminjaman->idMahasiswa << ", Tanggal Peminjaman: " << tempPeminjaman->tanggalPeminjaman << ", Kode Peminjaman: " << tempPeminjaman->kodePeminjaman << endl;
            tempPeminjaman = tempPeminjaman->next;
        }
    }
}



// Fungsi untuk menampilkan menu
void tampilkanMenu()
{
    cout << "=== Toko Buku ===" << endl;
    cout << "1. Tambah Buku" << endl;
    cout << "2. Hapus Buku" << endl;
    cout << "3. Tampilkan Daftar Buku" << endl;
    cout << "4. Tampilkan Info Peminjaman" << endl;
    cout << "5. Pinjam Buku" << endl;
    cout << "6. Kembalikan Buku" << endl;
    cout << "7. Cari Buku (Judul)" << endl;
    cout << "8. Klasifikasikan Buku Berdasarkan Pengarang" << endl;
    cout << "9. Tampilkan Buku(Abjad)" << endl;
    cout << "0. Keluar" << endl;
    cout << "Pilih menu: ";
    cin >> pilihan;
}

// Fungsi untuk mencari buku
void cariBuku()
{
    string judul;
    cout << "=== Cari Buku ===" << endl;
    cout << "Masukkan judul buku yang ingin dicari: ";
    cin.ignore();
    getline(cin, judul);

    Buku *temp = cariBuku(judul);

    if (temp != NULL)
    {
        cout << "Judul: " << temp->judul << ", Pengarang: " << temp->pengarang << ", Edisi: " << temp->edisi << ", ID: " << temp->isbn << ", Stok: " << temp->stok << ", Tersedia: " << (temp->tersedia ? "Ya" : "Tidak") << endl;
    }
    else
    {
        cout << "Buku tidak ditemukan!" << endl;
    }
}

int main()
{
    if (loginAdmin()) {
        tampilkanMenu();
    }
    // Inisialisasi hash table
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        hashTable[i] = NULL;
    }

    do
    {
        tampilkanMenu();
        switch (pilihan)
        {
        case '0':
            cout << "Keluar dari program." << endl;
            break;
        case '1':
            tambahBuku();
            break;
        case '2':
            hapusBuku();
            break;
        case '3':
            tampilkanDaftarBuku();
            break;
        case '4':
            tampilkanInfoPeminjaman();
            break;
        case '5':
            pinjamBuku();
            break;
        case '6':
            kembalikanBuku();
            break;
        case '7':
            cariBuku();
            break;
        case '8':
            klasifikasikanBukuBerdasarkanPengarang();
            tampilkanBukuTerkelasifikasi();
            break;
        case '9':
            urutkanBukuAbjad(daftarBuku);
            tampilkanDaftarBuku();
        default:
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }

    } while (pilihan != '9');

    return 0;
}
