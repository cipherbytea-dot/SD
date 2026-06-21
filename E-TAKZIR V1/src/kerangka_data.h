#ifndef KERANGKA_DATA_H
#define KERANGKA_DATA_H

// ============================================================
// E-TAKZIR: Sistem Peringatan Dini Pelanggaran Santri
// Paradigma: Pemrograman Prosedural Murni (C++)
// File ini berisi cetak biru data (struct) untuk 11 materi
// ============================================================

// ============================================================
// Bagian dari materi: 1. STRUCT
// Struct adalah cetak biru yang mengelompokkan variabel beda tipe
// jadi satu kesatuan record, seperti formulir data di dunia nyata
// ============================================================

// Struct dasar untuk data santri, ini seperti kartu identitas digital
struct DataSantri {
    int nis;                    // Nomor Induk Santri (unik, seperti NIK)
    char nama[50];              // Nama lengkap santri
    char kelas[20];             // Kelas atau tingkatan santri
    char kamar[10];             // Nomor kamar asrama
    int totalPoin;              // Akumulasi poin pelanggaran
};

// Struct dasar untuk data pelanggaran, ini seperti bukti tilang digital
struct DataPelanggaran {
    int idPelanggaran;          // ID unik setiap kejadian
    int nisSantri;              // NIS santri yang melanggar
    char jenis[50];             // Jenis pelanggaran (contoh: "Kabur")
    int bobotPoin;              // Berapa poin yang didapat
    char tanggal[11];           // Tanggal kejadian, format DD/MM/YYYY
    char keterangan[100];       // Keterangan tambahan kejadian
};

// Struct dasar untuk master aturan pelanggaran
// Ini seperti daftar hukum yang ditentukan pimpinan pesantren
struct DataMaster {
    int kodePelanggaran;        // Kode unik jenis pelanggaran
    char namaPelanggaran[50];   // Nama pelanggaran (contoh: "Tidak Sholat")
    int bobotPoin;              // Bobot poin yang dikenakan
};

// Struct dasar untuk jadwal piket pengurus malam
struct DataPiket {
    char namaPengurus[50];      // Nama pengurus yang bertugas
    char hari[10];              // Hari piket (Senin, Selasa, dst)
    char shift[10];             // Shift malam (Shift 1, Shift 2)
};

// Struct dasar untuk rute antar blok asrama
struct DataRute {
    char blokAsal[10];          // Blok asal (contoh: "A1")
    char blokTujuan[10];        // Blok tujuan (contoh: "B2")
    int jarakMeter;             // Jarak dalam meter
};

// ============================================================
// Bagian dari materi: 3. POINTER
// Pointer adalah variabel yang nyimpan alamat memori, seperti
// petunjuk arah yang menunjuk ke rumah tertentu di komplek
// ============================================================

// ============================================================
// Bagian dari materi: 7. DOUBLE LINKED LIST NON-CIRCULAR
// NodeSantri punya pointer next (maju) dan prev (mundur)
// Jadi pengurus bisa pencet tombol "Next" atau "Previous" di Qt
// ============================================================
struct NodeSantri {
    struct DataSantri data;         // Data santri yang disimpan
    struct NodeSantri *next;        // Pointer ke node berikutnya (maju)
    struct NodeSantri *prev;        // Pointer ke node sebelumnya (mundur)
    struct NodeRiwayat *headRiwayat; // Pointer ke riwayat pelanggaran santri ini
};

// ============================================================
// Bagian dari materi: 6. SINGLE LINKED LIST NON-CIRCULAR
// Riwayat pelanggaran cuma punya pointer next (satu arah)
// Seperti rantai yang cuma bisa ditarik ke satu arah
// ============================================================
struct NodeRiwayat {
    struct DataPelanggaran data;    // Data pelanggaran
    struct NodeRiwayat *next;       // Pointer ke riwayat berikutnya
};

// ============================================================
// Bagian dari materi: 4. STACK (LIFO)
// Stack untuk fitur Undo, seperti tumpukan piring:
// yang terakhir diletakkan, pertama diambil
// ============================================================
struct NodeUndo {
    struct DataPelanggaran data;    // Data pelanggaran yang di-undo
    struct NodeUndo *next;          // Pointer ke data undo sebelumnya (di bawahnya)
};

// ============================================================
// Bagian dari materi: 5. QUEUE (FIFO)
// Queue untuk antrean input pelanggaran, seperti antrean di bank:
// yang pertama datang, pertama dilayani
// ============================================================
struct NodeAntrean {
    struct DataPelanggaran data;    // Data pelanggaran dalam antrean
    struct NodeAntrean *next;       // Pointer ke antrean berikutnya
};

// Struct pembungkus Queue yang punya pointer depan dan belakang
struct Queue {
    struct NodeAntrean *front;      // Pointer ke antrean paling depan
    struct NodeAntrean *rear;       // Pointer ke antrean paling belakang
};

// ============================================================
// Bagian dari materi: 9. DOUBLE LINKED LIST CIRCULAR
// Master aturan bisa di-scroll bolak-balik tanpa mentok
// Setelah aturan terakhir, next kembali ke aturan pertama
// ============================================================
struct NodeAturan {
    struct DataMaster data;         // Data aturan pelanggaran
    struct NodeAturan *next;        // Pointer ke aturan berikutnya (circular)
    struct NodeAturan *prev;        // Pointer ke aturan sebelumnya (circular)
};

// ============================================================
// Bagian dari materi: 8. SINGLE LINKED LIST CIRCULAR
// Jadwal piket berputar terus, setelah pengurus terakhir
// kembali ke pengurus pertama, seperti jadwal jaga yang muter
// ============================================================
struct NodePiket {
    struct DataPiket data;          // Data jadwal piket
    struct NodePiket *next;         // Pointer ke jadwal berikutnya (circular)
};

// ============================================================
// Bagian dari materi: 10. TREE (BST)
// BST untuk pencarian santri berdasarkan NIS
// Aturan: nilai kiri < parent < nilai kanan
// Seperti pohon keluarga yang tersusun rapi berdasarkan nomor
// ============================================================
struct NodeBST {
    int nis;                        // Key pencarian (Nomor Induk Santri)
    struct DataSantri data;         // Data lengkap santri
    struct NodeRiwayat *headRiwayat; // Pointer ke riwayat pelanggaran santri
    struct NodeBST *left;           // Pointer anak kiri (nis lebih kecil)
    struct NodeBST *right;          // Pointer anak kanan (nis lebih besar)
};

// ============================================================
// Bagian dari materi: 11. GRAPH
// Graph merepresentasikan peta rute asrama
// Ada vertex (titik/blok) dan edge (jalur/lorong antar blok)
// Seperti peta jalan yang menghubungkan rumah-rumah
// ============================================================

// Edge adalah jalur yang menghubungkan dua blok asrama
struct Edge {
    char blokTujuan[10];            // Blok tujuan yang terhubung
    int jarakMeter;                 // Jarak antar blok dalam meter
    struct Edge *nextEdge;          // Pointer ke edge berikutnya dari vertex yang sama
};

// Vertex adalah titik/blok asrama, seperti rumah di perumahan
struct Vertex {
    char namaBlok[10];              // Nama blok (contoh: "A1", "B2")
    char namaKamar[20];             // Nama kamar (contoh: "Kamar Putra 1")
    struct Edge *firstEdge;         // Pointer ke daftar edge (jalur dari blok ini)
    struct Vertex *nextVertex;      // Pointer ke vertex berikutnya (daftar vertex)
};

// ============================================================
// Variabel Global Pointer untuk Mengelola Struktur Data di Memori
// Bagian dari materi: 3. POINTER
// Pointer-poiner ini adalah "jembatan" yang menghubungkan node-node
// di memori sehingga membentuk struktur data yang utuh
// ============================================================

// Pointer untuk Double Linked List Non-Circular (Data Santri)
extern struct NodeSantri *headSantri;       // Pointer ke node pertama santri
extern struct NodeSantri *tailSantri;       // Pointer ke node terakhir santri
extern struct NodeSantri *currentSantri;    // Pointer node aktif saat navigasi Prev/Next

// Pointer untuk Double Linked List Circular (Master Aturan)
extern struct NodeAturan *headAturan;       // Pointer ke node pertama aturan
extern struct NodeAturan *tailAturan;       // Pointer ke node terakhir aturan
extern struct NodeAturan *currentAturan;    // Pointer aturan aktif saat scroll

// Pointer untuk Single Linked List Circular (Jadwal Piket)
extern struct NodePiket *headPiket;         // Pointer ke jadwal piket pertama
extern struct NodePiket *currentPiket;      // Pointer jadwal piket aktif

// Pointer untuk Tree BST (Pencarian Santri)
extern struct NodeBST *rootBST;             // Pointer akar pohon BST

// Pointer untuk Graph (Peta Rute Asrama)
extern struct Vertex *headVertex;           // Pointer ke vertex pertama

// Pointer untuk Stack (Undo)
extern struct NodeUndo *topUndo;            // Pointer ke elemen puncak stack

// Pointer untuk Queue (Antrean Pelanggaran)
extern struct Queue *antreanPelanggaran;    // Pointer ke antrean pelanggaran

// Counter global untuk ID otomatis
extern int idPelanggaranTerakhir;           // ID terakhir yang digunakan

#endif // KERANGKA_DATA_H
