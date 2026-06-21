# DOKUMEN SPESIFIKASI TEKNIS SISTEM
## E-TAKZIR: Sistem Peringatan Dini Pelanggaran Santri
**Paradigma:** Pemrograman Prosedural (C++) | **UI Framework:** Qt | **Storage:** File Teks (.txt)

---

## 1. DESKRIPSI SINGKAT SISTEM

E-TAKZIR adalah aplikasi desktop berbasis **Qt Framework** yang dibangun dengan paradigma **pemrograman prosedural murni** menggunakan bahasa C++. Sistem ini menggantikan pencatatan pelanggaran manual (buku hitam) di pondok pesantren menjadi digital terpusat.

**Inti sistem:** Setiap pelanggaran memiliki bobot poin. Ketika akumulasi poin santri mencapai ambang batas tertentu, sistem secara otomatis memunculkan **peringatan visual (Early Warning)** berupa zona merah pada dashboard.

**Mekanisme Penyimpanan:** Seluruh data (master pelanggaran, data santri, riwayat transaksi, jadwal piket, peta rute) disimpan dalam **file `.txt` murni**. Saat aplikasi dijalankan, data di-*load* dari `.txt` ke memori dalam bentuk struktur data. Setelah operasi selesai, data di-*save* kembali ke `.txt`.

---

## 2. ALUR LOGIKA SISTEM

| Tahap | Kegiatan | Detail Teknis |
|-------|----------|---------------|
| **1. Start** | User menjalankan aplikasi Qt | `main()` memanggil `inisialisasiQt()` |
| **2. Load .txt** | Sistem membaca seluruh file database | `loadDariTxt()` → membaca `santri.txt`, `pelanggaran.txt`, `master_pelanggaran.txt`, `piket.txt`, `rute.txt` |
| **3. Login** | User memasukkan kredensial | `cariNisBST()` atau `verifikasiLogin()` memvalidasi di Binary Search Tree |
| **4. Dashboard** | Qt menampilkan antarmuka sesuai role | `tampilkanDashboard()` memanggil layout berbeda berdasarkan hasil login |
| **5. Eksekusi Fungsi** | User memicu aksi via tombol Qt | Signal Qt memanggil function prosedural (misal: `tambahPelanggaran()`, `undoStack()`) |
| **6. Save .txt** | Data di memori ditulis ke file | `simpanKeTxt()` menulis ulang seluruh file `.txt` dari struktur data di memori |
| **7. Logout** | Sesi berakhir, memori dibersihkan | `hapusMemori()` meng-*free* pointer dan linked list, kembali ke layar login |

---

## 3. HAK AKSES & MATRIKS OPERASI

### A. PIMPINAN PESANTREN (Regulator & Eksekutif)

| Fitur | Operasi | Keterangan |
|-------|---------|------------|
| **Master Pelanggaran** | Create, Read, Update, Delete | Menentukan jenis pelanggaran & bobot poin (contoh: "Kabur" = 75 poin) |
| **Monitoring** | Read | Melihat statistik bulanan & daftar "Santri Zona Merah" |
| **Pemutihan Poin** | Delete (Reset) | Reset seluruh poin santri ke 0 di akhir semester |
| **Jadwal Piket** | Read | Melihat rotasi jadwal piket pengurus |

### B. PENGURUS KEAMANAN (Operator & Eksekutor)

| Fitur | Operasi | Keterangan |
|-------|---------|------------|
| **Data Santri** | Create, Read, Update, Delete | Kelola profil santri (NIS, Nama, Kelas, Kamar) |
| **Input Pelanggaran** | Create, Read | Input kejadian pelanggaran harian |
| **Koreksi Input** | Update, Delete | Hanya untuk data hari yang sama (via fitur Undo/Stack) |
| **Master Pelanggaran** | Read Only | Hanya melihat aturan, tidak bisa ubah poin |
| **Peta Rute** | Read | Melihat peta rute patroli malam |

### C. SANTRI (Viewer)

| Fitur | Operasi | Keterangan |
|-------|---------|------------|
| **Profil Sendiri** | Read | Login pakai NIS, lihat total poin & riwayat |
| **Master Pelanggaran** | Read | Melihat daftar aturan & konsekuensi |
| **Peta Rute** | Read | Melihat peta asrama (read-only) |

---

## 4. SPESIFIKASI 11 KOMPONEN STRUKTUR DATA & ALGORITMA

### 1. STRUCT — Blueprint Data di Memori

**Definisi:** `struct` adalah cetak biru tipe data yang mengelompokkan beberapa variabel berbeda tipe menjadi satu kesatuan record.

**Skenario dalam E-TAKZIR:**

```cpp
struct DataSantri {
    int nis;
    char nama[50];
    char kelas[20];
    char kamar[10];
    int totalPoin;
};

struct DataPelanggaran {
    int idPelanggaran;
    int nisSantri;
    char jenis[50];
    int bobotPoin;
    char tanggal[11];
    char keterangan[100];
};

struct DataMaster {
    int kodePelanggaran;
    char namaPelanggaran[50];
    int bobotPoin;
};
```

**Fungsi Prosedural Terkait:**
- `buatStructSantri()` — mengisi field struct dari input Qt
- `tampilkanStructSantri()` — menampilkan data struct ke QLabel/QTableWidget

**Alur Kerja:** Saat load file, setiap baris `.txt` di-parse dan dimasukkan ke dalam struct. Struct ini menjadi "wadah" data sebelum disusun ke struktur data yang lebih kompleks (linked list, tree, graph).

---

### 2. FUNCTION — Modul Prosedural yang Dipicu Qt

**Definisi:** Function adalah blok kode prosedural yang modular, menerima parameter, dan dapat dipanggil berulang kali.

**Skenario dalam E-TAKZIR:**

Semua aksi user di Qt (klik tombol, pilih menu) memicu function prosedural:

| Tombol Qt | Function yang Dipanggil | Hasil |
|-----------|------------------------|-------|
| **Btn Simpan Santri** | `simpanDataSantri(struct DataSantri baru)` | Menambah node ke Double Linked List + append ke `santri.txt` |
| **Btn Input Pelanggaran** | `prosesInputPelanggaran()` | Enqueue ke Queue, lalu eksekusi |
| **Btn Undo** | `undoInputTerakhir()` | Pop dari Stack, hapus pelanggaran terakhir |
| **Btn Cari** | `cariNisBST(int nis)` | Traversal Tree, return pointer ke struct santri |
| **Btn Simpan Semua** | `simpanKeTxt()` | Write seluruh struktur data ke file `.txt` |

**Prinsip:** Tidak ada "method" — hanya **function global** atau **function static** yang menerima pointer struct sebagai parameter.

---

### 3. POINTER — Penghubung Node di Memori

**Definisi:** Pointer adalah variabel yang menyimpan alamat memori. Pointer digunakan untuk mengaitkan satu struct dengan struct lainnya di heap.

**Skenario dalam E-TAKZIR:**

```cpp
struct NodeSantri {
    struct DataSantri data;
    struct NodeSantri *next;   // pointer ke node berikutnya
    struct NodeSantri *prev;   // pointer ke node sebelumnya
};

struct NodePelanggaran {
    struct DataPelanggaran data;
    struct NodePelanggaran *next;  // pointer riwayat berikutnya
};
```

**Penggunaan Pointer:**
- `struct NodeSantri *headSantri` — pointer ke node pertama daftar santri
- `struct NodeSantri *current` — pointer penunjuk node aktif saat navigasi Prev/Next
- `struct NodePelanggaran *top` — pointer top untuk Stack (Undo)

**Alur Kerja:** Saat load data, `malloc()` atau `new` (untuk struct) dialokasikan untuk setiap record. Pointer `next`/`prev` disambungkan sehingga membentuk linked list di memori.

---

### 4. STACK (LIFO) — Fitur "Undo" Input Pelanggaran

**Definisi:** Stack adalah struktur data Last-In-First-Out. Data yang terakhir masuk akan pertama keluar.

**Skenario dalam E-TAKZIR:**

Pengurus salah input pelanggaran. Sebelum data masuk ke database permanen, sistem menyimpan setiap input ke Stack:

```cpp
struct StackNode {
    struct DataPelanggaran data;
    struct StackNode *top;   // pointer ke elemen puncak
};
```

**Operasi Prosedural:**

| Function | Kegiatan |
|----------|----------|
| `pushStack(struct DataPelanggaran x)` | Menambah input terbaru ke puncak Stack |
| `popStack()` | Mengambil & menghapus input terakhir (UNDO) |
| `isEmptyStack()` | Cek apakah Stack kosong |

**Alur Kerja:**
1. Pengurus klik "Input Pelanggaran" → data masuk Stack (push)
2. Pengurus sadar salah input → klik "Undo"
3. `popStack()` mengeluarkan data terakhir
4. Data dihapus dari Single Linked List riwayat + total poin dikembalikan
5. Hanya bisa undo untuk input **hari yang sama** (Stack di-reset tiap pergantian hari)

---

### 5. QUEUE (FIFO) — Antrean Pemrosesan Input Pelanggaran

**Definisi:** Queue adalah struktur data First-In-First-Out. Data yang pertama masuk akan pertama diproses.

**Skenario dalam E-TAKZIR:**

Ketika banyak santri melakukan pelanggaran bersamaan (misal: razia malam), pengurus menginput beberapa pelanggaran sekaligus. Sistem tidak langsung eksekusi, melainkan menampung ke Queue agar berurutan:

```cpp
struct QueueNode {
    struct DataPelanggaran data;
    struct QueueNode *next;
};

struct Queue {
    struct QueueNode *front;  // pointer depan
    struct QueueNode *rear;   // pointer belakang
};
```

**Operasi Prosedural:**

| Function | Kegiatan |
|----------|----------|
| `enqueue(struct DataPelanggaran x)` | Menambah pelanggaran ke antrean belakang |
| `dequeue()` | Mengambil pelanggaran paling depan untuk diproses |
| `isEmptyQueue()` | Cek apakah antrean kosong |

**Alur Kerja:**
1. Pengurus input 5 pelanggaran razia → semua `enqueue()`
2. Sistem menampilkan "5 pelanggaran dalam antrean"
3. Klik "Proses Semua" → `dequeue()` satu per satu
4. Setiap dequeue: poin dihitung → masuk ke Linked List riwayat → update total poin santri
5. Urutan diproses sesuai urutan input (yang pertama diinput, pertama diproses)

---

### 6. SINGLE LINKED LIST NON-CIRCULAR — Riwayat Pelanggaran per Santri

**Definisi:** Linked list satu arah (hanya pointer `next`) yang tidak berputar (tail menunjuk NULL).

**Skenario dalam E-TAKZIR:**

Setiap santri memiliki daftar riwayat pelanggaran pribadi. Riwayat ini disimpan sebagai Single Linked List:

```cpp
struct NodeRiwayat {
    struct DataPelanggaran data;
    struct NodeRiwayat *next;
};
```

**Struktur Penyimpanan:**

```
[Santri A] → [NodeRiwayat 1] → [NodeRiwayat 2] → [NodeRiwayat 3] → NULL
[Santri B] → [NodeRiwayat 1] → [NodeRiwayat 2] → NULL
```

**Operasi Prosedural:**

| Function | Kegiatan |
|----------|----------|
| `tambahRiwayat(struct NodeSantri *santri, struct DataPelanggaran x)` | Insert node baru di akhir list milik santri tersebut |
| `tampilkanRiwayat(struct NodeSantri *santri)` | Traversal dari head sampai NULL, tampilkan ke QTableWidget |
| `hapusRiwayatTerakhir(struct NodeSantri *santri)` | Hapus node tail (digunakan saat Undo dari Stack) |

**Alur Kerja:** Saat santri dipilih di Qt, sistem mengikuti pointer `headRiwayat` milik santri tersebut, lalu traversal `next` sampai NULL untuk menampilkan seluruh riwayat di UI.

---

### 7. DOUBLE LINKED LIST NON-CIRCULAR — Data Induk Santri

**Definisi:** Linked list dua arah (pointer `next` dan `prev`) yang tidak berputar. Memungkinkan navigasi maju-mundur.

**Skenario dalam E-TAKZIR:**

Daftar seluruh santri disimpan dalam Double Linked List agar Pengurus mudah menavigasi data dengan tombol "Previous" dan "Next" di Qt:

```cpp
struct NodeSantri {
    struct DataSantri data;
    struct NodeSantri *next;
    struct NodeSantri *prev;
};
```

**Operasi Prosedural:**

| Function | Kegiatan |
|----------|----------|
| `tambahSantri(struct DataSantri x)` | Insert node di akhir list |
| `hapusSantri(int nis)` | Cari node lalu hapus dengan relink `prev` dan `next` |
| `navigasiNext()` | `current = current->next`, tampilkan ke form Qt |
| `navigasiPrev()` | `current = current->prev`, tampilkan ke form Qt |
| `cariLinear(int nis)` | Traversal linear untuk edit/hapus |

**Alur Kerja:** Pengurus buka menu "Data Santri". Qt menampilkan data santri pertama. Tombol "Next" memicu `navigasiNext()` yang menggeser pointer `current` ke `next`. Tombol "Prev" memicu `navigasiPrev()` ke `prev`. Sangat cocok untuk browsing data santri satu per satu.

---

### 8. SINGLE LINKED LIST CIRCULAR — Jadwal Piket Pengurus

**Definisi:** Linked list satu arah yang berputar: tail menunjuk kembali ke head. Tidak ada ujung.

**Skenario dalam E-TAKZIR:**

Sistem memiliki daftar rotasi jadwal piket malam pengurus. Setelah pengurus terakhir, jadwal kembali ke pengurus pertama:

```cpp
struct NodePiket {
    char namaPengurus[50];
    char hari[10];
    char shift[10];
    struct NodePiket *next;
};
```

**Struktur Penyimpanan:**

```
[Pengurus A] → [Pengurus B] → [Pengurus C] → [Pengurus D] → (kembali ke A)
```

**Operasi Prosedural:**

| Function | Kegiatan |
|----------|----------|
| `tambahJadwalPiket(struct NodePiket x)` | Insert node ke circular list |
| `tampilkanJadwalHariIni()` | Traversal dari head, tampilkan sesuai hari aktual |
| `rotasiPiket()` | `current = current->next`, pindah ke jadwal berikutnya |

**Alur Kerja:** Pimpinan buka menu "Jadwal Piket". Qt menampilkan jadwal hari ini. Setiap hari berganti, sistem otomatis `rotasiPiket()` ke node berikutnya. Karena circular, setelah pengurus terakhir, pointer kembali ke pengurus pertama tanpa error NULL.

---

### 9. DOUBLE LINKED LIST CIRCULAR — Master Aturan Pelanggaran

**Definisi:** Linked list dua arah yang berputar: tail->next menunjuk head, dan head->prev menunjuk tail.

**Skenario dalam E-TAKZIR:**

Pimpinan mengelola daftar aturan pelanggaran. Saat membuka form master, Pimpinan bisa scroll tanpa batas bolak-balik:

```cpp
struct NodeMaster {
    struct DataMaster data;
    struct NodeMaster *next;
    struct NodeMaster *prev;
};
```

**Struktur Penyimpanan:**

```
[Aturan 1] ↔ [Aturan 2] ↔ [Aturan 3] ↔ [Aturan 4] ↔ (kembali ke 1)
```

**Operasi Prosedural:**

| Function | Kegiatan |
|----------|----------|
| `tambahAturan(struct DataMaster x)` | Insert node ke circular list |
| `scrollAturanNext()` | `current = current->next`, tampilkan ke form |
| `scrollAturanPrev()` | `current = current->prev`, tampilkan ke form |
| `updateBobotPoin(int kode, int bobotBaru)` | Edit data di node aktif |

**Alur Kerja:** Pimpinan buka "Master Pelanggaran". Form menampilkan aturan pertama. Tombol "→" (Next) memicu `scrollAturanNext()` ke aturan berikutnya. Dari aturan terakhir, next kembali ke aturan pertama. Tombol "←" (Prev) berlawanan. Sangat nyaman untuk review seluruh aturan tanpa takut mentok di ujung.

---

### 10. TREE (BST — Binary Search Tree) — Mesin Pencari Utama

**Definisi:** Tree adalah struktur data hierarki. BST (Binary Search Tree) memiliki aturan: nilai kiri < parent < nilai kanan. Pencarian sangat cepat: O(log n).

**Skenario dalam E-TAKZIR:**

NIS (Nomor Induk Santri) adalah integer unik. Seluruh NIS disusun dalam BST untuk pencarian super cepat saat login dan pencarian data:

```cpp
struct NodeBST {
    int nis;                      // key pencarian
    struct DataSantri data;       // data lengkap santri
    struct NodeRiwayat *headRiwayat;  // pointer ke linked list riwayat
    struct NodeBST *left;         // pointer anak kiri (nis lebih kecil)
    struct NodeBST *right;        // pointer anak kanan (nis lebih besar)
};
```

**Operasi Prosedural:**

| Function | Kegiatan | Kompleksitas |
|----------|----------|--------------|
| `insertBST(struct NodeBST **root, int nis, struct DataSantri x)` | Menambah node ke tree | O(log n) |
| `cariNisBST(struct NodeBST *root, int nis)` | Mencari santri berdasarkan NIS | O(log n) |
| `hapusNodeBST(struct NodeBST **root, int nis)` | Menghapus node dari tree | O(log n) |
| `inorderTraversal(struct NodeBST *root)` | Tampilkan urut NIS ke Qt | O(n) |

**Alur Kerja:**
1. **Login:** Santri masukkan NIS → `cariNisBST(root, nis)` → jika ditemukan, berhasil login
2. **Cari Data:** Pengurus ketik NIS di search box → BST langsung temukan dalam waktu singkat
3. **Load Awal:** Saat aplikasi start, seluruh data `santri.txt` di-insert ke BST berdasarkan NIS
4. **Keunggulan:** Dengan 1000 santri, pencarian linear butuh ~500 langkah. BST hanya butuh ~10 langkah.

---

### 11. GRAPH — Peta Rute Patroli Asrama

**Definisi:** Graph adalah struktur data yang terdiri dari **vertex** (simpul/titik) dan **edge** (sisi/jalur). Graph merepresentasikan hubungan antar titik.

**Skenario dalam E-TAKZIR:**

Asrama terdiri dari beberapa blok kamar yang saling terhubung oleh lorong. Pengurus malam harus patroli dengan rute efisien. Sistem menyimpan peta ini sebagai Graph:

```cpp
struct Vertex {
    char namaBlok[10];        // contoh: "A1", "B2", "C3"
    char namaKamar[20];       // contoh: "Kamar Putra 1"
    struct Edge *firstEdge;   // pointer ke daftar edge (jalur)
};

struct Edge {
    char namaBlokTujuan[10];  // blok yang terhubung
    int jarakMeter;           // jarak antar blok (untuk hitung efisiensi)
    struct Edge *nextEdge;    // edge berikutnya dari vertex yang sama
};
```

**Representasi:** Adjacency List — setiap vertex memiliki linked list edge sendiri.

**Operasi Prosedural:**

| Function | Kegiatan |
|----------|----------|
| `tambahVertex(char blok[])` | Menambah titik/blok asrama |
| `tambahEdge(char blokAsal[], char blokTujuan[], int jarak)` | Menambah jalur antar blok |
| `tampilkanGraph()` | Traversal semua vertex & edge, gambar ke QGraphicsView Qt |
| `cariRuteTerpendek(char start[], char end[])` | Algoritma Dijkstra/DFS untuk rute patroli optimal |

**Alur Kerja:**
1. Peta asrama di-load dari `rute.txt` saat start aplikasi
2. Format `rute.txt`: setiap baris berisi `BLOK_ASAL|BLOK_TUJUAN|JARAK`
3. Qt menampilkan peta secara visual menggunakan QGraphicsView dengan garis antar blok
4. Pengurus malam bisa melihat blok mana yang terhubung langsung dan merencanakan rute patroli
5. Jika ada kamar yang dilaporkan rawan, sistem bisa hitung rute tercepat dari pos jaga ke kamar tersebut

---

## 5. STRUKTUR FILE .TXT (DATABASE LOKAL)

| Nama File | Isi | Format Baris |
|-----------|-----|--------------|
| `santri.txt` | Data induk santri | `NIS|NAMA|KELAS|KAMAR|TOTAL_POIN` |
| `master_pelanggaran.txt` | Aturan & bobot poin | `KODE|NAMA_PELANGGARAN|BOBOT` |
| `riwayat_pelanggaran.txt` | Riwayat transaksi | `ID|NIS|KODE_PELANGGARAN|TANGGAL|KETERANGAN` |
| `piket.txt` | Jadwal piket pengurus | `NAMA_PENGURUS|HARI|SHIFT` |
| `rute.txt` | Peta rute asrama | `BLOK_ASAL|BLOK_TUJUAN|JARAK_METER` |

**Catatan:** Semua file di-load ke memori (struct & pointer) saat aplikasi start. Semua perubahan di memori baru ditulis ke `.txt` saat user klik "Simpan" atau "Logout".

---

## 6. KESIMPULAN ARSITEKTUR

```
┌─────────────────────────────────────────┐
│           UI Qt (QMainWindow)           │
│    ┌─────────┐ ┌─────────┐ ┌─────────┐  │
│    │  Button │ │  Form   │ │  Table  │  │
│    │ Trigger │ │  Input  │ │  View   │  │
│    └────┬────┘ └────┬────┘ └────┬────┘  │
└─────────┼───────────┼───────────┼─────────┘
          │           │           │
          ▼           ▼           ▼
┌─────────────────────────────────────────┐
│      FUNCTION PROSEDURAL (C++)          │
│  ┌─────────┐ ┌─────────┐ ┌─────────┐   │
│  │  Stack  │ │  Queue  │ │  Tree   │   │
│  │  (Undo) │ │(Antrean)│ │  (BST)  │   │
│  └─────────┘ └─────────┘ └─────────┘   │
│  ┌─────────┐ ┌─────────┐ ┌─────────┐   │
│  │ SLL-Non │ │ DLL-Non │ │ SLL-Circ│   │
│  │(Riwayat)│ │(Santri) │ │ (Piket) │   │
│  └─────────┘ └─────────┘ └─────────┘   │
│  ┌─────────┐ ┌─────────┐              │
│  │DLL-Circ │ │  Graph  │              │
│  │(Aturan) │ │ (Rute)  │              │
│  └─────────┘ └─────────┘              │
└─────────────────────────────────────────┘
          │
          ▼
┌─────────────────────────────────────────┐
│      FILE .TXT (Read/Write Murni)       │
│  santri.txt | master.txt | riwayat.txt  │
│  piket.txt  | rute.txt                 │
└─────────────────────────────────────────┘
```

**Paradigma:** 100% Prosedural. Tidak ada Class, Object, atau Method. Hanya **Struct**, **Function**, **Pointer**, dan **File Teks**.

---

**Disusun oleh:** Senior System Analyst & Dosen Algoritma Struktur Data  
**Proyek:** E-TAKZIR (Sistem Peringatan Dini Pelanggaran Santri)  
**Platform:** C++ Prosedural + Qt Framework + File `.txt` Lokal