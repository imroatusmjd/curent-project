#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_set>
#include <cstdlib>
#include <queue>
#include <stack>

using namespace std;

// Deklarasi fungsi-fungsi
string generateIdDriver(string nama, char kelamin, string tglLahir, unordered_set<string>& duplikasiID);
string generateIdOrder(string platNomor, string idSupir, string tujuan, string namaPelanggan);
void prosesPesanan();

// struct untuk mobil
struct Mobil {
    string platNomor;
    string jenisMobil;
    string brandMobil; 

    Mobil* next; 
};

// struct data order
struct Order {
    string id;
    string namaKust;
    string supir;
    string platNomor;
    string tujuan;
    
    Order* next;
    Order* prev;
};

// struct data supir
struct Driver {
    string Id;
    string namaSupir;
    string TglLahir;
    char Kelamin;
    string Alamat;

    Driver* next;
    Driver* prev;
};

// pointer ke mobil paling atas dalam garasi
Mobil* top = nullptr;

// fungsi untuk menambahkan mobil baru ke garasi ( push )
void tambahMobil( string platNomor, string jenisMobil, string brandMobil ) {
    Mobil* newMobil = new Mobil;

    newMobil->platNomor = platNomor;
    newMobil->jenisMobil = jenisMobil;
    newMobil->brandMobil = brandMobil;

    newMobil->next = top;
    top = newMobil;

    cout << "Data berhasil dimasukkan!" << endl;
}

// fungsi untuk mengeluarkan mobil dari garasi ( pop )
Mobil* keluarkanMobil() {

    if ( top == nullptr ) {
        cout << "Garasi mobil kosong!" << endl;
        return nullptr;
    } else {
        Mobil* mobilKeluar = top;
        top = top->next;
        return mobilKeluar;
    }
}

// fungsi untuk menampilkan informasi mobil
void displayMobil( Mobil* mobil ) {

    if (mobil == nullptr ) {
        cout << "Tidak ada mobil yang tersedia!" << endl;
    } else {
        cout << "Plat Nomor     : " << mobil->platNomor << endl;
        cout << "Jenis Mobil    : " << mobil->jenisMobil << endl;
        cout << "Brand Mobil    : " << mobil->brandMobil << endl;
    }

}

// pointer head dan tail untuk membuat antrian order
Order* headOrder = nullptr;
Order* tailOrder = nullptr;

// fungsi enqueue untuk menambahkan order baru ke dalam antrian
void enqueueOrder(const string& id, const string& nama, const string& supir, const string& platNomor, const string& tujuan) {
    Order* newOrder = new Order{id, nama, supir, platNomor, tujuan, nullptr, nullptr};
    if (!headOrder) {
        headOrder = newOrder;
        tailOrder = newOrder;
    } else {
        tailOrder->next = newOrder;
        newOrder->prev = tailOrder;
        tailOrder = newOrder;
    }
}

// fungsi dequeue untuk menghapus order dari antrian
void dequeueOrder() {
    if (!headOrder) {
        cout << "Antrian pesanan kosong!" << endl;
        return;
    }

    Order* temp = headOrder;
    headOrder = headOrder->next;
    delete temp;

    if (!headOrder) {
        tailOrder = nullptr;
    } else {
        headOrder->prev = nullptr;
    }
}

// fungsi menampilkan data supir
void displayDriver(Driver* currentDriver) {
    
    cout << "|------------------------------|" << endl;
    cout << " Id        : " << setw(5) << setfill('0') << currentDriver->Id << endl;
    cout << " Nama      : " << currentDriver->namaSupir << endl;
    cout << " Tgl Lahir : " << currentDriver->TglLahir << endl;
    cout << " Kelamin   : " << currentDriver->Kelamin << endl;
    cout << " Alamat    : " << currentDriver->Alamat << endl;
    cout << "|------------------------------|" << endl;
    cout << endl;
}

//fungsi untuk menampilkan daftar supir yang ada di Linked list dengan next dan prev
void displayDriverList( Driver *currentDriver ) {
    displayDriver(currentDriver);
    cout << "1. Next    " << endl;
    cout << "2. Previous" << endl;
    cout << "0. Exit    " << endl;
}

// fungsi menambah driver baru ( menu admin )
Driver* addDriver(Driver* tail, unordered_set<string>& duplikasiID) {
    string nama, alamat, tglLahir;
    char kelamin;

    cout << "Nama : ";
    cin.ignore();
    getline( cin, nama );
    cout << "Tanggal Lahir (dd-mm-yyyy) : ";
    cin >> tglLahir;
    cout << "Kelamin (L/P) : ";
    cin >> kelamin;
    cout << "Alamat : ";
    cin.ignore(); 
    getline(cin, alamat);

    string newId = generateIdDriver(nama, kelamin, tglLahir, duplikasiID);
    Driver* newDriver = new Driver{newId, nama, tglLahir, kelamin, alamat, nullptr, nullptr};

    if (!tail) {    // jika linkedlist masih kosong
        tail = newDriver;
        tail->next = newDriver;
        tail->prev = newDriver;
    } else { 
        newDriver->next = tail->next; 
        newDriver->prev = tail;
        tail->next->prev = newDriver;
        tail->next = newDriver;
    }
    cout << "Data supir berhasil ditambahkan!" << endl;
    return tail;
}

// fungsi mengubah data driver berdasarkan id ( menu admin )
void editDriver(Driver*& tail, unordered_set<string>& duplikasiId, const string& targetId) {
    if (!tail) {
        cout << "Daftar supir masih kosong!" << endl;
        return;
    }
    Driver* currentDriver = tail->next;
    do {
        cout << "DEBUG : memeriksa Id " << currentDriver->Id << " terhadap Id target " << targetId << endl;

        if (currentDriver->Id == targetId) {
            displayDriver(currentDriver);
            cout << "Mengubah data supir dengan Id " << targetId << endl;
            cout << "Pilih data supir yang ingin diubah:" << endl;
            cout << "1. Nama            " << endl;
            cout << "2. Tanggal Lahir   " << endl;
            cout << "3. Kelamin         " << endl;
            cout << "4. Alamat          " << endl;
            cout << "0. Exit            " << endl;
            cout << "Pilihan : ";

            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    cout << "Masukkan nama baru: ";
                    cin.ignore(); 
                    getline(cin, currentDriver->namaSupir);
                    break;
                case 2:
                    cout << "Masukkan tanggal lahir baru: ";
                    cin >> currentDriver->TglLahir;
                    break;
                case 3:
                    cout << "Masukkan kelamin baru (L/P): ";
                    cin >> currentDriver->Kelamin;
                    while (currentDriver->Kelamin != 'L' && currentDriver->Kelamin != 'P') {
                        cout << "Input tidak valid!. masukkan sesuai contoh! "<< endl;
                        cout << ">> Kelamin (L/P)  : ";
                        cin >> currentDriver->Kelamin;
                    }
                    break;
                case 4:
                    cout << "Masukkan alamat baru: ";
                    cin.ignore(); 
                    getline(cin, currentDriver->Alamat);
                    break;
                case 0:
                    cout << "Keluar ke menu utama." << endl;
                    return;
                default:
                    cout << "Pilihan tidak valid." << endl;
                    break;
            }

            string oldId = currentDriver->Id; // declare oldID here
            duplikasiId.erase(oldId); // hapus ID lama.
            // memperbarui ID baru untuk data ID yang diubah.
            currentDriver->Id = generateIdDriver(currentDriver->namaSupir, currentDriver->Kelamin, currentDriver->TglLahir, duplikasiId);
            displayDriver(currentDriver);
            cout << "Data supir dengan Id " << oldId << " berhasil diubah menjadi Id " << currentDriver->Id << endl;
            
            return;
        }

        currentDriver = currentDriver->next;
    } while (currentDriver != tail->next);

    cout << "Id supir tidak ditemukan!" << endl;
}

// fungsi untuk mencari data driver berdasarkan id ( menu admin )
Driver* findDriver(Driver* tail, const string& targetId) {
    if (!tail) {
        cout << "Daftar supir masih kosong." << endl;
        return nullptr;
    }

    Driver* currentDriver = tail->next;
    do {
        if (currentDriver->Id == targetId) {
            displayDriver(currentDriver);
            return currentDriver;
        }
        currentDriver = currentDriver->next;
    } while (currentDriver != tail->next);

    cout << "Id supir tidak ditemukan!" << endl;
    return nullptr;
}

// fungsi untuk menghapus data driver berdasarkan id ( menu admin )
Driver* deleteDriver(Driver*& tail, const string& targetId) {
    if (!tail) {
        cout << "Daftar supir masih kosong." << endl;
        return nullptr;
    }

    Driver* currentDriver = tail->next;
    do {
        if (currentDriver->Id == targetId) {
            if (currentDriver == tail) {
                tail = tail->prev;
            }
            currentDriver->prev->next = currentDriver->next;
            currentDriver->next->prev = currentDriver->prev;

            delete currentDriver;
            cout << "Data supir dengan Id " << targetId << " telah berhasil dihapus." << endl;
            return tail;
        }
        currentDriver = currentDriver->next;
    } while (currentDriver != tail->next);

    cout << "Id supir tidak ditemukan!" << endl;
    return tail;
}

// fungsi untuk melakukan order taxi ( menu user )
// fungsi untuk melakukan order taxi ( menu user )
void orderTaxi(Driver* currentDriver, const string& platNomor) {
    if (top == nullptr) {
        cout << "Garasi mobil kosong!" << endl;
        return;
    }

    Order* newOrder = new Order;
    newOrder->namaKust = "";
    newOrder->tujuan = "";

    cout << ">> Nama     : "; // nama pelanggan
    cin.ignore(); getline(cin, newOrder->namaKust);
    cout << ">> Tujuan   : "; // tujuan pelanggan
    getline(cin, newOrder->tujuan);
    newOrder->supir = currentDriver->namaSupir;
    
    newOrder->id = generateIdOrder(platNomor, currentDriver->Id, newOrder->tujuan, newOrder->namaKust); // membuat id

    enqueueOrder(newOrder->id, newOrder->namaKust, newOrder->supir, platNomor, newOrder->tujuan);

    cout << "Order telah dilakukan!" << endl;
    cout << "| Id         : " << newOrder->id << endl;
    cout << "| Nama User  : " << newOrder->namaKust << endl;
    cout << "| Nama Supir : " << currentDriver->namaSupir << endl;
    cout << "| Plat nomor : " << platNomor << endl;
    cout << "| Tujuan     : " << newOrder->tujuan << endl;
    system("pause");
}

string generateIdOrder(string platNomor, string idSupir, string tujuan, string namaPelanggan) {
    // mendapatkan nilai dari karakter pertama platNomor!
    int digitPlatNomor = (tolower(platNomor[0]) - 'a' + 1); // untuk digit 1 dan 2
    // kalau nilai karakternya <10, tambahin nol di awal. Misal A = 1, jadinya 01.
    string digitPlatNomorStr = (digitPlatNomor < 10) ? "0" + to_string(digitPlatNomor) : to_string(digitPlatNomor);
    // mendapatkan ID supir. klasik...
    string digitIDSupirStr = (idSupir.length() < 5) ? string(5 - idSupir.length(), '0') + idSupir : idSupir; // untuk digit 3 4 5 6 7

    int digitTujuan = 0; // untuk digit 8 9
    // kalau string tujuannya >= 2, dia jalan
    if (tujuan.length() >= 2) {
        char lastChar1 = tolower(tujuan[tujuan.length() - 2]);
        char lastChar2 = tolower(tujuan[tujuan.length() - 1]);
        digitTujuan = (lastChar1 - 'a' + 1) + (lastChar2 - 'a' + 1);
    } else { // kalau <2 PROGRAMNYA MOKAD, misal cuma masukin 'A' doang atau 'Z' doang.
        // ini gak usah dicopas juga ya!
        // daripada dibuat gini, mending ketika user masukin tujuan, dibuat perulangan aja sampe si user masukin minimal 2 huruf
        // atau tambahin nol di awal jika <10.
        cerr << "Error: MINIMAL HARUS ADA DUA HURUF PADA DESTINASI TUJUAN!" << endl;
        exit(EXIT_FAILURE);
    }

    int jumlahHurufNama = 0; // digit 10
    for (char huruf : namaPelanggan) {
        if (isalpha(huruf)) {
            jumlahHurufNama += tolower(huruf) - 'a' + 1;
        }
    }
    int digitTerakhir = jumlahHurufNama % 10; // ambil angka terakhir

    // gabungin semuanya menjadi 10 digit ID order.
    return digitPlatNomorStr +
           digitIDSupirStr +
           (digitTujuan < 10 ? "0" + to_string(digitTujuan) : to_string(digitTujuan)) + // kalau hasil penjumlahannya <10, tambahin nol di awal!
           to_string(digitTerakhir);
}


// menu admin
void menuAdmin(Driver*& tail, unordered_set<string>& duplikasiId) {
    int choice;
    string targetId;

    do {
        cout << endl;
        cout << "|---------------------------------|" << endl;
        cout << "1. Mencari Data Supir" << endl;
        cout << "2. Menghapus Data Supir" << endl;
        cout << "3. Mengubah Data Supir" << endl;
        cout << "4. Menambah Data Supir" << endl;
        cout << "5. Proses Pesanan" << endl;
        cout << "6. Tambah unit mobil" << endl;
        cout << "0. Exit" << endl;
        cout << "Pilihan : ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Masukkan Id Supir yang ingin dicari : ";
                cin >> targetId;
                findDriver(tail, targetId);
                system("pause");
                break;
            case 2:
                cout << "Masukkan Id Supir yang Ingin Dihapus: ";
                cin >> targetId;
                tail = deleteDriver(tail, targetId);
                system("pause");
                break;
            case 3:
                cout << "Masukkan Id Supir yang Ingin Diubah: ";
                cin >> targetId;
                editDriver(tail, duplikasiId, targetId);
                system("pause");                
                break;
            case 4:
                tail = addDriver(tail, duplikasiId); // menambah driver ke akhir linked list
                system("pause");
                break;
            case 5:
                prosesPesanan();
                break;
            case 6:
            {
                string platNomor, jenisMobil, brandMobil;
                cout << "Masukkan Plat Nomor Mobil: ";
                cin.ignore(); // Membersihkan newline yang masih tersisa di buffer
                getline(cin, platNomor); // Membaca seluruh baris masukan untuk plat nomor
                cout << "Masukkan Jenis Mobil: ";
                cin >> jenisMobil;
                cout << "Masukkan Brand Mobil: ";
                cin >> brandMobil;
                tambahMobil(platNomor, jenisMobil, brandMobil);
                system("pause");
                break;
            }
            case 0:
                cout << "Keluar dari Menu Admin." << endl;
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi!" << endl;
                break;
        }
    } while (choice != 0);
}

// fungsi menampilkan informasi order
void displayOrder(Order* currentOrder) {
    cout << "|------------------------------|" << endl;
    cout << "  Order ID : " << currentOrder->id << endl;
    cout << "  Nama     : " << currentOrder->namaKust << endl;
    cout << "  Supir    : " << currentOrder->supir << endl;
    cout << "  Plat     : " << currentOrder->platNomor << endl;
    cout << "  Tujuan   : " << currentOrder->tujuan << endl;
    cout << "|------------------------------|" << endl;
}

// fungsi untuk memproses pesanan ( menu admin )
void prosesPesanan() {
    Order* currentOrder = headOrder;

    if (!currentOrder) {
        cout << "Tidak ada pesanan." << endl;
        return;
    }

    while (currentOrder) {
        displayOrder(currentOrder);

        int opsi;
        cout << "1. Accept" << endl;
        cout << "2. Reject" << endl;
        cout << "0. Exit" << endl;
        cout << "Pilihan : ";
        cin >> opsi;

        switch (opsi) {
            case 1: {
                cout << "Pesanan dengan Id " << currentOrder->id << " telah diproses!!!" << endl;
                dequeueOrder();
                break;
            }
            case 2: {
                cout << "Pesanan dengan Id " << currentOrder->id << " telah ditolak." << endl;
                dequeueOrder();
                break;
            }
            case 0:
                cout << "Keluar dari proses pesanan." << endl;
                return;
            default:
                cout << "Pilihan tidak valid." << endl;
                break;
        }

        cin.ignore();

        // Update currentOrder setelah di hapus dari antrian
        currentOrder = headOrder;
    }
}

// fungsi untuk menampilkan menu user
void menuUser(Driver* tail) {
    int choice;
    do {
        cout << endl;
        cout << "|------------------------------|" << endl;
        cout << "1. Lihat Data Supir" << endl;
        cout << "0. Exit" << endl;
        cout << "Pilihan : ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (tail) {
                    Driver* currentDriver = tail->next;
                    do {
                        displayDriver(currentDriver);
                        cout << "1. Next" << endl;
                        cout << "2. Previous" << endl;
                        cout << "3. Order" << endl; // menuju ke fungsi order
                        cout << "0. Exit" << endl;
                        cout << "Pilihan : ";
                        cin >> choice;
                        switch (choice)
                        {
                        case 1:
                            currentDriver = currentDriver->next;
                            break;
                        case 2:
                            currentDriver = currentDriver->prev;
                            break;
                        case 3:
                            orderTaxi(currentDriver, top->platNomor);
                            break;
                        case 0:
                            cout << "Berhasil keluar!" << endl;
                            break;
                        default:
                            cout << "Pilihan tidak valid!" << endl;
                            break;
                        }
                    } while (choice != 0);
                } else {
                    cout << "Daftar supir masih kosong." << endl;
                }
                break;
            case 0:
                cout << "Keluar dari Menu User." << endl;
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
                break;
        }
    } while (choice != 0);
}

//fungsi untuk menghasilkan id 
string generateIdDriver(string Nama, char Kelamin, string TglLahir, unordered_set<string>& duplikasiId) {
    int digitDuaPertama = abs((tolower(Nama[0]) - 'a' + 1) - (tolower(Nama[Nama.length() - 1]) - 'a' + 1));
    int digitKetiga = (Kelamin == 'L') ? 1 : 0;
    int digitKeempat = ((TglLahir[1] - '0') + (TglLahir[4] - '0') + (TglLahir[9] - '0')) % 9;
    int digitKelima = 0;
    string hasilId;
    do {
        hasilId = to_string(digitDuaPertama) +
                   to_string(digitKetiga) +
                   to_string(digitKeempat) +
                   to_string(digitKelima);

        // +0 di awal jika jumlah digit kurang dari 5
        if (hasilId.length() < 5) {
            hasilId = string(5 - hasilId.length(), '0') + hasilId;
        }
        digitKelima++;

        if (digitKelima > 99) { // untuk test eror 
            cerr << "Error: Infinite loop detected. Unable to generate a unique ID." << endl;
            exit(EXIT_FAILURE);
        }
    } while (duplikasiId.find(hasilId) != duplikasiId.end());

    duplikasiId.insert(hasilId);
    cout << "Id yang dihasilkan: " << hasilId << endl;
    
    return hasilId;
}

// program utama
int main() {
    unordered_set<string> duplikasiId;
    Driver* tail = nullptr;
    int privilege;

    do {
        system("cls");
        cout << " " << endl;
        cout << "    Taksi Pak Sugeng!    " << endl;
        cout << "|----------------------|" << endl;
        cout << "1. Masuk sebagai Admin " << endl;
        cout << "2. Masuk sebagai User  " << endl;
        cout << "0. Exit                " << endl;
        cout << "Pilihan : ";
        cin >> privilege;

        switch (privilege) {
            case 1:
                menuAdmin(tail, duplikasiId);
                break;
            case 2:
                menuUser(tail);
                break;
            case 0:
                cout << "Keluar dari program." << endl;
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi!" << endl;
                break;
        }
    } while (privilege != 0);
    return 0;
}