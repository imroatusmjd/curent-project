#include <iostream>
#include <fstream>
#include <string>

using namespace std;


// Struktur untuk menu makanan
struct Food {
    string idmakanan;
    string namamakanan;
    double price;
    string kategori;

    Food* next;
    Food* prev;
};

// Struktur untuk pelanggan
struct CustomerOrder {
    string idorder;
    string namacust;
    string addresCust;
    string phone;
    string makananpesan;
    string idmakananpesan;

    CustomerOrder* next;
    CustomerOrder* prev;
};

struct CustomerOrderRiwayat {
    string idorder;
    string namacust;
    string addresCust;
    string phone;
    string makananpesan;
    string idmakananpesan;
    string status;

    CustomerOrderRiwayat* next;
    CustomerOrderRiwayat* prev;
};

//struct untuk tree
struct Tree {
    Food data;
    Tree* left;
    Tree* right;
};

// Node untuk stack dan queue
struct Node {
    CustomerOrder* customer;
    Node* next;
};

//deklarasi jumlah maksimal food yang ditemukan
const int maxFood = 100;
const int MAX_NODES = 100;
const int TABLE_SIZE = 100;

// Queue untuk antrian pesanan
struct Queue {
    Node* front;
    Node* rear;
    Queue() : front(nullptr), rear(nullptr) {}
    
    void enqueue(CustomerOrder* customer) { //mengubah beberapa pointer, kompleksitas waktunya = O(1).
        Node* newNode = new Node{customer, nullptr};
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    } // O(1) untuk enqueue dan dequeue
    
    void dequeue() { //mengubah beberapa pointer dan melakukan 1 penghapusan,kompleksitas waktunya = O(1).
        if (front == nullptr) {
            cout << "Queue is empty!" << endl;
            return;
        }
        Node* temp = front;
        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        }
        delete temp;
    }
    
    bool isEmpty() {
        return front == nullptr;
    }
};

// Stack untuk riwayat pesanan
struct Stack {
    Node* top;
    Stack() : top(nullptr) {}
    
    void push(CustomerOrder* customer) { //mengubah beberapa pointer, kompleksitas waktunya = O(1)
        Node* newNode = new Node{customer, top};
        top = newNode;
    }
    
    void pop() { //mengubah beberapa pointer dan melakukan 1 penghapusan, kompleksitas waktunya = O(1)
        if (top == nullptr) {
            cout << "Stack is empty!" << endl;
            return;
        }
        Node* temp = top;
        top = top->next;
        delete temp;
    }
    
    bool isEmpty() {
        return top == nullptr;
    }
};

// Fungsi rekursif untuk menambah node ke dalam tree
Tree* tambahTreeRekursif(Tree* root, Food foodtree) {
    if (root == nullptr) {
        root = new Tree;
        root->data = foodtree;
        root->left = root->right = nullptr;
    } else if (foodtree.idmakanan < root->data.idmakanan) {
        root->left = tambahTreeRekursif(root->left, foodtree);
    } else {
        root->right = tambahTreeRekursif(root->right, foodtree);
    }
    return root;
}

// Fungsi rekursif untuk melakukan in-order traversal pada tree
void inOrderTraversalRekursif(Tree* root) {
    if (root != nullptr) {
        inOrderTraversalRekursif(root->left);
        cout << "ID makanan: " << root->data.idmakanan <<endl <<  "Nama makanan: " << root->data.namamakanan
             << endl << "Harga: " << root->data.price << endl << "Kategori: " << root->data.kategori<< endl;
        cout << "--------------------------------"<<endl;
        inOrderTraversalRekursif(root->right);
    }
}


// Fungsi rekursif untuk mencari makanan berdasarkan ID
Tree* searchTreeRekursif(Tree* root, const string& idmakanan) {
    if (root == nullptr || root->data.idmakanan == idmakanan) {
        return root;
    } else if (idmakanan < root->data.idmakanan) {
        return searchTreeRekursif(root->left, idmakanan);
    } else {
        return searchTreeRekursif(root->right, idmakanan);
    }
}

//deklarasi variabel unruk kategori makanan
const int jumKategori = 4;

//fungsi untuk membuat ID makanan
string buatIDmakanan(int kategori, int price, string makanan) {
    // Baca file untuk mendapatkan daftar ID yang sudah ada
    ifstream file("daftar_menu.txt");
    string existingIDs[1000]; // Anggap maksimal ada 1000 ID dalam file
    int idCount = 0;
    string line;
    while (getline(file, line)) {
        existingIDs[idCount++] = line;
    }
    file.close();

    // Kode kategori
    string kodekategori;
    switch (kategori) {
        case 1: kodekategori = "MB"; break;
        case 2: kodekategori = "SN"; break;
        case 3: kodekategori = "MN"; break;
        case 4: kodekategori = "DS"; break;
        default: kodekategori = "XX"; break;
    }

    // Dua huruf awal dari harga makanan
    string kodeharga = to_string(price);
    if (kodeharga.length() < 2) {
        kodeharga = "0" + kodeharga; // Tambah nol di depan jika harga kurang dari 10
    }
    kodeharga = kodeharga.substr(0, 2);

    // Huruf depan nama makanan
    char hurufMakanan = toupper(makanan[0]);

    // Mulai dengan collision counter 0
    int collisionCounter = 0;
    string id = kodekategori + kodeharga + hurufMakanan + '0';

    // Cek dan tangani collision
    bool collision;
    do {
        collision = false;
        for (int i = 0; i < idCount; ++i) {
            if (existingIDs[i] == id) {
                collision = true;
                collisionCounter++;
                id = kodekategori + kodeharga + hurufMakanan + to_string(collisionCounter);
                break;
            }
        }
    } while (collision);

    return id;
}//ID Generation: Proses pembangkitan ID memerlukan membaca file dan memeriksa ID yang sudah ada. 
//Membaca file secara linear memiliki kompleksitas O(m), di mana m adalah jumlah baris dalam file. 
//Pemeriksaan kolisi juga memiliki kompleksitas O(m). Kompleksitas total untuk pembangkitan ID adalah O(m).

//deklarasi ukuran hash table
const int HASH_TABLE_SIZE = 50;

//fungsi hashing
int hashFunction(const string& key) {
    int hashValue = 0;
    for (char c : key) {
        hashValue += c;
    }
    return hashValue % HASH_TABLE_SIZE;
}


// Fungsi untuk menambahkan makanan baru melalui tree
void tambahFoodtree(Tree*& root) {

    ofstream file("daftar_menu.txt", ios::app);

    if (!file.is_open()) {
        cout << "Gagal membuka file.\n";
        return;
    }

    int jumlah;
    cout << "\n--------------------------------------------------\n";
    cout << "Jumlah menu yang akan ditambahkan : ";
    cin >> jumlah;
    cin.ignore();

    for (int i = 0; i < jumlah; ++i) {
        Food foodtree;
        cout << "\n--------------------------------------------------\n";
        cout << "Masukkan detail menu " << i + 1 << ":\n";
        cout << "Makanan        : ";
        getline(cin, foodtree.namamakanan);
        cout << "Harga          : ";
        cin >> foodtree.price;
        cout << "\n--------------------------------------------------\n";
        cout << endl;
        cin.ignore();

        int nomorkategori;
        do {
            cout << "Kategori Makanan: " << endl;
            cout << "\n1. Makanan Berat\n2. Snack\n3. Minuman\n4. Dessert\n";
            cout << "Kategori (1-4)    : ";
            cin >> nomorkategori;
            cin.ignore();
        } while (nomorkategori < 1 || nomorkategori > 4);

        string kodekategori[] = {"Makanan Berat", "Snack", "Minuman", "Dessert"};
        foodtree.kategori = kodekategori[nomorkategori - 1];

        foodtree.idmakanan = buatIDmakanan(nomorkategori, foodtree.price, foodtree.namamakanan);

        //root = tambahTree(root, foodtree, file);
        root = tambahTreeRekursif(root, foodtree);
        file << foodtree.idmakanan << "                ,"  << foodtree.namamakanan << "         ,"<< foodtree.price <<"         ,"<< foodtree.kategori  << "," << '\n';
    }

    file.close();
    cout << "Makanan berhasil ditambahkan!\n";
}

string generateIdOrder(string makananpesan, string idmakananpesan, string addressCust, string namacust) {
    // Digit pertama: huruf pertama dari nama makanan
    char digitMakanan = toupper(makananpesan[0]);

    // Digit kedua dan ketiga: dua huruf pertama dari ID makanan
    string digitIDmakanan = idmakananpesan.substr(0, 2);

    // Digit keempat: huruf pertama dari alamat pelanggan
    char digitAlamat = toupper(addressCust[0]);

    // Digit kelima: huruf pertama dari nama pelanggan
    char digitNama = toupper(namacust[0]);

    // Mulai dengan collision counter 0
    int collisionCounter = 0;
    string idOrder = digitMakanan + digitIDmakanan + digitAlamat + digitNama + '0';

    string existingOrders[1000]; // Anggap maksimal ada 1000 ID dalam file
    int orderCount = 0;
    string line;

    // Cek dan tangani collision
    bool collision;
    do {
        collision = false;
        for (int i = 0; i < orderCount; ++i) {
            if (existingOrders[i] == idOrder) {
                collision = true;
                collisionCounter++;
                idOrder = digitMakanan + digitIDmakanan + digitAlamat + digitNama + to_string(collisionCounter);
                break;
            }
        }
    } while (collision);

    return idOrder;
}

string generateIdOrderRiwayat(string makananpesan, string idmakananpesan, string addressCust, string namacust) {
    // Digit pertama: huruf pertama dari nama makanan
    char digitMakanan = toupper(makananpesan[0]);

    // Digit kedua dan ketiga: dua huruf pertama dari ID makanan
    string digitIDmakanan = idmakananpesan.substr(0, 2);

    // Digit keempat: huruf pertama dari alamat pelanggan
    char digitAlamat = toupper(addressCust[0]);

    // Digit kelima: huruf pertama dari nama pelanggan
    char digitNama = toupper(namacust[0]);

    // Mulai dengan collision counter 0
    int collisionCounter = 0;
    string idOrder = digitMakanan + digitIDmakanan + digitAlamat + digitNama + '0';

    string existingOrders[1000]; // Anggap maksimal ada 1000 ID dalam file
    int orderCount = 0;
    string line;
    

    // Cek dan tangani collision
    bool collision;
    do {
        collision = false;
        for (int i = 0; i < orderCount; ++i) {
            if (existingOrders[i] == idOrder) {
                collision = true;
                collisionCounter++;
                idOrder = digitMakanan + digitIDmakanan + digitAlamat + digitNama + to_string(collisionCounter);
                break;
            }
        }
    } while (collision);

    return idOrder;
}


void tambahOrder(Queue& queueOrder) {
    CustomerOrder* newOrder = new CustomerOrder;
    
        Food foodtree;
        cout << "\n--------------------------------------------------\n";
        cout << "Masukkan detail order baru :\n";
    cout << "Nama pelanggan            : ";
    getline(cin, newOrder->namacust);
    cout << "Alamat pelanggan          : ";
    getline(cin, newOrder->addresCust);
    cout << "Nomor telepon             : ";
    getline(cin, newOrder->phone);
    cout << "Makanan yang dipesan      : ";
    getline(cin, newOrder->makananpesan);
    cout << "ID makanan yang dipesan   : ";
    getline(cin, newOrder->idmakananpesan);

    newOrder->idorder = generateIdOrder(newOrder->makananpesan, newOrder->idmakananpesan, newOrder->addresCust, newOrder->namacust);

    queueOrder.enqueue(newOrder);

    cout << "Order berhasil ditambahkan!\n";
}

void tampilkanOrder(Queue& queueOrder) {
    if (queueOrder.isEmpty()) {
        cout << "Tidak ada order dalam antrian.\n";
        return;
    }

    Node* current = queueOrder.front;
    cout << "\n--------------------------------------------------\n";
    cout << "Daftar order dalam antrian:\n";
    while (current != nullptr) {
        CustomerOrder* order = current->customer;
        cout << "ID Order             : " << order->idorder << endl;
        cout << "Nama pelanggan       : " << order->namacust << endl;
        cout << "Alamat               : " << order->addresCust << endl;
        cout << "No telepon           : " << order->phone << endl;
        cout << "Makanan yang dipesan : " << order->makananpesan << endl;
        cout << "ID Makanan           : " << order->idmakananpesan << endl;
        cout << "---------------------------------------\n";
        current = current->next;
    }
}

void tambahOrderriwayat(Queue& queueOrder, Stack& stackOrderRiwayat) {
    if (queueOrder.isEmpty()) {
        cout << "Tidak ada order dalam antrian.\n";
        return;
    }

    CustomerOrder* order = queueOrder.front->customer;
    CustomerOrderRiwayat* newOrderRiwayat = new CustomerOrderRiwayat;
    newOrderRiwayat->idorder = order->idorder;
    newOrderRiwayat->namacust = order->namacust;
    newOrderRiwayat->addresCust = order->addresCust;
    newOrderRiwayat->phone = order->phone;
    newOrderRiwayat->makananpesan = order->makananpesan;
    newOrderRiwayat->idmakananpesan = order->idmakananpesan;
    newOrderRiwayat->status = "Selesai";

    stackOrderRiwayat.push(order);
    queueOrder.dequeue();

    cout << "Order berhasil ditambahkan ke riwayat!\n";
}

void tampilkanOrderriwayat(Stack& stackOrderRiwayat) {
    if (stackOrderRiwayat.isEmpty()) {
        cout << "Tidak ada order dalam riwayat.\n";
        return;
    }

    Node* current = stackOrderRiwayat.top;
    cout << "\n--------------------------------------------------\n";
    cout << "Daftar order dalam riwayat:\n";
    while (current != nullptr) {
        CustomerOrder* order = current->customer;
        cout << "ID Order               : " << order->idorder << endl;
        cout << "Nama pelanggan         : " << order->namacust << endl;
        cout << "Alamat                 : " << order->addresCust << endl;
        cout << "No telepon             : " << order->phone << endl;
        cout << "Makanan yang dipesan   : " << order->makananpesan << endl;
        cout << "ID Makanan             : " << order->idmakananpesan << endl;
        cout << "Status                 : Selesai" << endl;
        cout << "---------------------------------------\n";
        current = current->next;
    }
}


// Deklarasi variabel global Queue
Queue queueOrder;

//fungsi untuk mengurutkan makanan yang digunakan saat menampilkan daftar makanan
void quickSort(Food makanan[], int low, int high, int pilihDisplay) {
    if (low < high) {
        string pivot;
        switch (pilihDisplay) {
            case 1:
                pivot = makanan[high].idmakanan;
                break;
            case 2:
                pivot = makanan[high].namamakanan;
                break;
            case 3:
                pivot = makanan[high].kategori;
                break;
        }

        int i = low - 1;

        for (int j = low; j <= high - 1; j++) {
            string current;
            switch (pilihDisplay) {
                case 1:
                    current = makanan[j].idmakanan;
                    break;
                case 2:
                    current = makanan[j].namamakanan;
                    break;
                case 3:
                    current = makanan[j].kategori;
                    break;
            }

            if (current < pivot) {
                i++;
                Food temp = makanan[i];
                makanan[i] = makanan[j];
                makanan[j] = temp;
            }
        }

        Food temp = makanan[i + 1];
        makanan[i + 1] = makanan[high];
        makanan[high] = temp;

        quickSort(makanan, low, i, pilihDisplay); // O(n log n) on average, O(n^2) in the worst case
        quickSort(makanan, i + 2, high, pilihDisplay); // O(n log n) on average, O(n^2) in the worst case

    }
}

//fungsi untuk menampilkan makanan
void displayFoodSort(Food makanan[], int& jumlahFood) {
    ifstream file("daftar_menu.txt");
    if (!file.is_open()) {
        cout << "Gagal membuka file.\n";
        return;
    }

    jumlahFood = 0;
    string line;
    while (getline(file, line) && jumlahFood < 100) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        size_t pos4 = line.find(',', pos3 + 1);

        makanan[jumlahFood].idmakanan = line.substr(0, pos1);
        makanan[jumlahFood].namamakanan = line.substr(pos1 + 1, pos2 - pos1 - 1);
        makanan[jumlahFood].price = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
        makanan[jumlahFood].kategori = line.substr(pos3 + 1, pos4 - pos3 - 1);
        jumlahFood++;
    }
    file.close();

    int pilihDisplay;
    cout << "\n--------------------------------------------------\n";
    cout << "Pilihan pengurutan :\n";
    cout << "1. ID Makanan\n2. Nama makanan\n3. Kategori\n";
    cout << "Masukkan pilihan: ";
    cin >> pilihDisplay;
    cout << "\n--------------------------------------------------\n";
    
    quickSort(makanan, 0, jumlahFood - 1, pilihDisplay);

    for (int i = 0; i < jumlahFood; ++i) {
        cout << "ID Makanan      : " << makanan[i].idmakanan<< endl;
        cout << "Makanan         : " << makanan[i].namamakanan<<endl;
        cout << "Harga           : " << makanan[i].price<<endl;
        cout << "Kategori        : " << makanan[i].kategori<<endl;
        cout << "--------------------------------------------------\n";
    }
}


void lihatMenu(Food makanan[], int& jumlahFood) {

    cout << "\n--------------------------------------------------\n";
    cout << "=================================================================" << endl;
    cout << "                              DAFTAR MENU                              \n";
    cout << "=================================================================" << endl;

     ifstream file("daftar_menu.txt");
    if (!file.is_open()) {
        cout << "Gagal membuka file.\n";
        return;
    }

    jumlahFood = 0;
    string line;
    while (getline(file, line) && jumlahFood < 100) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        size_t pos4 = line.find(',', pos3 + 1);

        makanan[jumlahFood].idmakanan = line.substr(0, pos1);
        makanan[jumlahFood].namamakanan = line.substr(pos1 + 1, pos2 - pos1 - 1);
        makanan[jumlahFood].price = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
        makanan[jumlahFood].kategori = line.substr(pos3 + 1, pos4 - pos3 - 1);
        jumlahFood++;
    }
    file.close();

    int pilihDisplay = 3;
    
    quickSort(makanan, 0, jumlahFood - 1, pilihDisplay);

    for (int i = 0; i < jumlahFood; ++i) {
        cout << "ID Makanan      : " << makanan[i].idmakanan<< endl;
        cout << "Makanan         : " << makanan[i].namamakanan<<endl;
        cout << "Harga           : " << makanan[i].price<<endl;
        cout << "Kategori        : " << makanan[i].kategori<<endl;
        cout << "--------------------------------------------------\n";
    }
}

//fungsi untuk mencari makanan berdasarkan sebuah keyword
void cariFood() {
    ifstream file("daftar_menu.txt");
    if (!file.is_open()) {
        cout << "Gagal membuka file.\n";
        return;
    }

    Food foodDitemukan[100];
    int jumlahDitemukan = 0;

    string pencarian;
    cout << "--------------------------------------------------\n";
    cout << "Masukkan pencarian: ";
    cin.ignore();
    getline(cin, pencarian);

    string line;
    while (getline(file, line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        size_t pos4 = line.find(',', pos3 + 1);

        Food makanan;
        makanan.idmakanan = line.substr(0, pos1);
        makanan.namamakanan = line.substr(pos1 + 1, pos2 - pos1 - 1);
        makanan.price = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
        makanan.kategori = line.substr(pos3 + 1, pos4 - pos3 - 1);

        if (makanan.idmakanan.find(pencarian) != string::npos ||
            makanan.namamakanan.find(pencarian) != string::npos ||
            to_string(makanan.price).find(pencarian) != string::npos ||
            makanan.kategori.find(pencarian) != string::npos ) {

            if (jumlahDitemukan < maxFood) {
                foodDitemukan[jumlahDitemukan] = makanan;
                jumlahDitemukan++;
            }
        }
    }

    file.close();

    if (jumlahDitemukan == 0) {
        cout << "Makanan dengan pencarian '" << pencarian << "' tidak ditemukan.\n";
    } else {
        cout << "--------------------------------------------------\n";
        cout << "Makanan ditemukan!! \n\n";
        for (int i = 0; i < jumlahDitemukan; i++) {
            cout << "Informasi makanan :\n";
            cout << "ID makanan        : " << foodDitemukan[i].idmakanan << endl;
            cout << "Nama makanan      : " << foodDitemukan[i].namamakanan << endl;
            cout << "Harga             : " << foodDitemukan[i].price << endl;
            cout << "Kategori          : " << foodDitemukan[i].kategori << endl;
            cout << "--------------------------------------------------\n";
        }
    }
}

// Kelas Graph
class Graph {
public:
    string nodes[MAX_NODES];
    int edges[MAX_NODES][MAX_NODES];
    int distances[MAX_NODES][MAX_NODES];
    int node_count;

    Graph() {
        node_count = 0;
        for (int i = 0; i < MAX_NODES; i++) {
            for (int j = 0; j < MAX_NODES; j++) {
                edges[i][j] = 0;
                distances[i][j] = INT_MAX;
            }
        }
    }

    void add_node(const string& value) {
        nodes[node_count] = value;
        node_count++;
    }

    void add_edge(int from_node, int to_node, int distance) {
        edges[from_node][to_node] = 1;
        edges[to_node][from_node] = 1; // jika graph tidak berarah
        distances[from_node][to_node] = distance;
        distances[to_node][from_node] = distance; // jika graph tidak berarah
    }

    int get_node_index(const string& value) {
        for (int i = 0; i < node_count; i++) {
            if (nodes[i] == value) {
                return i;
            }
        }
        return -1;
    }
};

// Algoritma Dijkstra
void dijkstra(Graph& graph, int initial, int* visited, int* path) {
    bool processed[MAX_NODES] = { false };
    for (int i = 0; i < graph.node_count; i++) {
        visited[i] = INT_MAX;
        path[i] = -1;
    }
    visited[initial] = 0;

    for (int i = 0; i < graph.node_count; i++) {
        int min_node = -1;
        for (int j = 0; j < graph.node_count; j++) {
            if (!processed[j] && (min_node == -1 || visited[j] < visited[min_node])) {
                min_node = j;
            }
        }

        if (visited[min_node] == INT_MAX) {
            break;
        }

        processed[min_node] = true;
        for (int j = 0; j < graph.node_count; j++) {
            if (graph.edges[min_node][j] && visited[min_node] + graph.distances[min_node][j] < visited[j]) {
                visited[j] = visited[min_node] + graph.distances[min_node][j];
                path[j] = min_node;
            }
        }
    }
}

// Hash table sederhana menggunakan chaining
class HashTable {
public:
    struct Node {
        string key;
        int value;
        Node* next;
    };

    Node* table[TABLE_SIZE];

    HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }

    int hash(const string& key) {
        int hash_value = 0;
        for (char c : key) {
            hash_value = (hash_value * 31 + c) % TABLE_SIZE;
        }
        return hash_value;
    }

    void add(const string& key, int value) {
        int index = hash(key);
        Node* new_node = new Node{ key, value, table[index] };
        table[index] = new_node;
    }

    int get(const string& key) {
        int index = hash(key);
        Node* node = table[index];
        while (node) {
            if (node->key == key) {
                return node->value;
            }
            node = node->next;
        }
        return INT_MAX; // Mengembalikan INT_MAX jika tidak ditemukan
    }
};

void menuCustomer (Queue& queueOrder){
    Food makanan[100];
    int jumlahFood = 0;
    Stack stackOrderRiwayat;
    Tree* root = nullptr;

    int pilihan;

    do {
        cout << "\n===========================\n";
        cout << "    APLIKASI FOOD ORDER\n";
        cout << "===========================\n";
        cout << "1. Lihat Daftar Menu\n";
        cout << "2. Tambah Order Baru\n";
        cout << "0. Keluar\n";
        cout << "===========================\n";
        cout << "Pilihan: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 1:
                lihatMenu(makanan, jumlahFood);
                break;
            case 2:
                tambahOrder(queueOrder);
                break;
            case 0:
                cout << "Terima kasih telah menggunakan aplikasi ini.\n";
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        }
    } while (pilihan != 0);
}


void menuAdmin(Queue& queueOrder){
    Food makanan[100];
    int jumlahFood = 0;
    Stack stackOrderRiwayat;
    Graph graph;
    string initial_location, destination_location;
    HashTable hash_table;
    Tree* root = nullptr;
    CustomerOrder* orderHead = nullptr; // Inisialisasi linked list untuk pesanan
   
    int pilihan;

    // Menambahkan node (lokasi)
    graph.add_node("Kantin");
    graph.add_node("FEB");
    graph.add_node("FH");
    graph.add_node("FMIPA");
    graph.add_node("FT");
    graph.add_node("FV");
    graph.add_node("FISIP");

    // Menambahkan edges (jarak antar lokasi dalam menit)
    graph.add_edge(graph.get_node_index("Kantin"), graph.get_node_index("FH"), 3);
    graph.add_edge(graph.get_node_index("Kantin"), graph.get_node_index("FMIPA"), 3);
    graph.add_edge(graph.get_node_index("FMIPA"), graph.get_node_index("FT"), 7);
    graph.add_edge(graph.get_node_index("FMIPA"), graph.get_node_index("FV"), 10);
    graph.add_edge(graph.get_node_index("FH"), graph.get_node_index("FEB"), 7);
    graph.add_edge(graph.get_node_index("FEB"), graph.get_node_index("FV"), 10);
    graph.add_edge(graph.get_node_index("FEB"), graph.get_node_index("FISIP"), 5);



    do {
        cout << "\n===========================\n";
        cout << "     Menu Admin\n";
        cout << "===========================\n";
        cout << "1. Tambah Menu Baru\n";
        cout << "2. Cek Menu Baru\n";
        cout << "3. Lihat Daftar Semua Menu\n";
        cout << "4. Tampilkan Order dalam Antrian\n";
        cout << "5. Selesaikan Order\n";
        cout << "6. Tampilkan Riwayat Order\n";
        cout << "7. Display Urutan Menu\n";
        cout << "8. Cari Menu (Nama Makanan) \n";
        cout << "9. Cari Menu (ID dalam tree)\n";
        cout << "10. Rute pengantaran pesanan \n";
        cout << "0. Keluar\n";
        cout << "===========================\n";
        cout << "Pilihan: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 0:
                cout << "Terima kasih telah menggunakan aplikasi ini.\n";
                break;
            case 1:
                tambahFoodtree(root);
                break;
            case 2:
                inOrderTraversalRekursif(root);
                break;
            case 3:
                lihatMenu(makanan, jumlahFood);
                break;
            case 4:
                tampilkanOrder(queueOrder);
                break;
            case 5:
                tambahOrderriwayat(queueOrder, stackOrderRiwayat);
                break;
            case 6:
                tampilkanOrderriwayat(stackOrderRiwayat);
                break;
            case 7:
                displayFoodSort(makanan, jumlahFood);
                break;
            case 8 :
                cariFood();
                break;
            case 9 :
                { string idmakanan;
                cout << "Masukkan ID makanan yang ingin dicari: ";
                getline(cin, idmakanan);
                Tree* result = searchTreeRekursif(root, idmakanan);
                if (result) {
                    cout << "Makanan ditemukan: ID makanan: " << result->data.idmakanan 
                         << ", Nama makanan: " << result->data.namamakanan 
                         << ", Harga: " << result->data.price 
                         << ", Kategori: " << result->data.kategori << endl;
                } else {
                    cout << "Makanan dengan ID " << idmakanan << " tidak ditemukan.\n";
                }
                break;
                }
            case 10 :
            {
                cout << "Masukkan lokasi awal (Kantin): ";
                getline(cin, initial_location);

                cout << "Masukkan lokasi akhir (Alamat customer): ";
                getline(cin, destination_location);

                // Cari indeks node untuk lokasi awal dan tujuan
                int initial_index = graph.get_node_index(initial_location);
                int destination_index = graph.get_node_index(destination_location);

                if (initial_index != -1 && destination_index != -1) {
                // Lakukan pencarian rute tercepat menggunakan algoritma Dijkstra
                int visited[MAX_NODES];
                int path[MAX_NODES];
                dijkstra(graph, initial_index, visited, path);

                // Tampilkan rute tercepat
                int route[MAX_NODES];
                int route_size = 0;
                int current_location = destination_index;
                while (current_location != -1) {
                route[route_size++] = current_location;
                current_location = path[current_location];
                }

                cout << "Rute tercepat dari " << initial_location << " ke " << destination_location << ": ";
                for (int i = route_size - 1; i >= 0; i--) {
                cout << graph.nodes[route[i]];
                if (i > 0) {
                cout << " -> "; }
                }

                cout << endl;

                // Menampilkan waktu tempuh
                cout << "Waktu tempuh dari " << initial_location << " ke " << destination_location << ": " << visited[destination_index] << " menit" << endl;
                } else {
                cout << "Lokasi awal atau tujuan tidak ditemukan dalam daftar lokasi.\n";
                }}
                break;
            default:
                 cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        }
    } while (pilihan != 0);
}

int main() {
    Queue queueOrder;
    Stack stackOrderRiwayat;

    int pilihan;

    do {
        cout << "\n===========================\n";
        cout << "     APLIKASI FOOD ORDER \n";
        cout << "===========================\n";
        cout << "1. Masuk sebagai Admin    \n";
        cout << "2. Masuk sebagai Customer \n";;
        cout << "0. Keluar\n";
        cout << "===========================\n";
        cout << "Pilihan: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 1:
                menuAdmin(queueOrder);
                break;
            case 2:
                menuCustomer(queueOrder);
                break;
            case 0:
                cout << "Terima kasih telah menggunakan aplikasi ini.\n";
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        }
    } while (pilihan != 0);

    return 0;
}

// Fungsi untuk membaca data makanan dari file teks dan membangun pohon pencarian biner
Tree* bacaDataMenu(const string& namafile) {
    ifstream file(namafile);
    if (!file) {
        cerr << "Gagal membuka file " << namafile << endl;
        return nullptr;
    }

    Tree* root = nullptr;
    Food food;
    while (file >> food.idmakanan >> food.namamakanan >> food.price >> food.kategori) {
        root = tambahTreeRekursif(root, food);
    }

    file.close();
    return root;
}