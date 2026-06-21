#ifndef KUMPULAN_FUNGSI_H
#define KUMPULAN_FUNGSI_H

#include "kerangka_data.h"

// ============================================================
// E-TAKZIR: Kumpulan Prototipe Fungsi Prosedural
// Semua fungsi di sini adalah deklarasi saja (header)
// Implementasi ada di file .cpp terpisah
// Paradigma: 100% Prosedural, tidak ada Class/Method
// ============================================================

// ============================================================
// BAGIAN INISIALISASI & UTILITAS GLOBAL
// Bagian dari materi: 2. FUNCTION
// Function adalah blok kode modular yang bisa dipanggil berulang kali
// seperti resep masakan yang bisa dipakai setiap kali masak
// ============================================================

// Inisialisasi semua pointer global ke NULL saat aplikasi pertama kali jalan
// Alur logika: Sebelum pakai alat, pastikan alatnya bersih dulu
void inisialisasiSistem();

// Membersihkan semua memori (free pointer) saat logout atau exit
// Alur logika: Setelah selesai pakai, rapikan dan bersihkan meja kerja
void hapusMemori();

// Mengecek apakah string tanggal hari ini sama dengan tanggal input
// Digunakan untuk batasi fitur Undo hanya untuk hari yang sama
int cekTanggalHariIni(char tanggal[]);

// Mendapatkan tanggal hari ini dalam format DD/MM/YYYY
void ambilTanggalHariIni(char hasil[]);

// ============================================================
// BAGIAN BACA TULIS FILE .TXT (DATABASE LOKAL)
// Bagian dari materi: 2. FUNCTION
// Fungsi-fungsi ini adalah jembatan antara memori dan file teks
// ============================================================

// Membaca seluruh file database dan membangun struktur data di memori
// Alur logika: Buka laci arsip, ambil semua berkas, susun di meja kerja
void bacaDariTeks();

// Menulis ulang seluruh data dari memori ke file .txt
// Alur logika: Setelah selesai edit di meja kerja, simpan kembali ke laci arsip
void simpanKeTeks();

// Fungsi spesifik untuk membaca masing-masing file
void bacaDataSantri();            // Baca santri.txt, bangun DLL & BST
void bacaDataMaster();            // Baca master_pelanggaran.txt, bangun DLL Circular
void bacaRiwayatPelanggaran();   // Baca riwayat_pelanggaran.txt, bangun SLL per santri
void bacaJadwalPiket();           // Baca piket.txt, bangun SLL Circular
void bacaPetaRute();              // Baca rute.txt, bangun Graph

// Fungsi spesifik untuk menyimpan masing-masing file
void simpanDataSantri();          // Tulis ulang santri.txt dari DLL
void simpanDataMaster();          // Tulis ulang master_pelanggaran.txt dari DLL Circular
void simpanRiwayatPelanggaran();  // Tulis ulang riwayat_pelanggaran.txt dari semua SLL
void simpanJadwalPiket();         // Tulis ulang piket.txt dari SLL Circular
void simpanPetaRute();            // Tulis ulang rute.txt dari Graph

// ============================================================
// BAGIAN STACK (LIFO) - FITUR UNDO
// Bagian dari materi: 4. STACK (LIFO)
// Stack seperti tumpukan piring: yang terakhir diletakkan, pertama diambil
// ============================================================

// Menambah data pelanggaran ke puncak Stack (Undo)
// Alur logika: Taruh piring baru di atas tumpukan piring yang sudah ada
void pushUndo(struct DataPelanggaran data);

// Mengambil dan menghapus data teratas dari Stack (Undo)
// Alur logika: Ambil piring paling atas dari tumpukan
struct DataPelanggaran popUndo();

// Mengecek apakah Stack Undo kosong
// Alur logika: Cek apakah tumpukan piringnya sudah habis
int isEmptyUndo();

// Reset Stack Undo (dipanggil setiap pergantian hari)
// Alur logika: Bersihkan semua piring di tumpukan, mulai dari nol
void resetUndo();

// ============================================================
// BAGIAN QUEUE (FIFO) - ANTREAN INPUT PELANGGARAN
// Bagian dari materi: 5. QUEUE (FIFO)
// Queue seperti antrean di bank: yang pertama datang, pertama dilayani
// ============================================================

// Inisialisasi Queue agar front dan rear siap dipakai
// Alur logika: Siapkan loket antrean baru, pastikan belum ada orang
void inisialisasiQueue();

// Menambah pelanggaran ke belakang antrean (enqueue)
// Alur logika: Orang baru datang, antre di belakang orang terakhir
void enqueue(struct DataPelanggaran data);

// Mengambil pelanggaran dari depan antrean (dequeue)
// Alur logika: Orang paling depan dilayani, keluar dari antrean
struct DataPelanggaran dequeue();

// Mengecek apakah Queue kosong
// Alur logika: Cek apakah loket antrean masih kosong
int isEmptyQueue();

// Menghitung jumlah antrean yang menunggu
// Alur logika: Hitung berapa orang yang masih mengantre
int hitungAntrean();

// ============================================================
// BAGIAN SINGLE LINKED LIST NON-CIRCULAR - RIWAYAT PELANGGARAN
// Bagian dari materi: 6. SINGLE LINKED LIST NON-CIRCULAR
// Seperti rantai satu arah: cuma bisa maju, tidak bisa mundur
// ============================================================

// Menambah riwayat pelanggaran di akhir list milik santri tertentu
// Alur logika: Tambah catatan baru di buku riwayat paling belakang
void tambahRiwayat(struct NodeSantri *santri, struct DataPelanggaran data);

// Menampilkan seluruh riwayat pelanggaran santri ke UI (QTableWidget)
// Alur logika: Buka buku riwayat dari halaman pertama sampai habis
void tampilkanRiwayat(struct NodeSantri *santri);

// Menghapus riwayat terakhir santri (digunakan saat Undo dari Stack)
// Alur logika: Sobek halaman terakhir dari buku riwayat
void hapusRiwayatTerakhir(struct NodeSantri *santri);

// Mencari riwayat berdasarkan ID pelanggaran
// Alur logika: Cari catatan spesifik di buku riwayat berdasarkan nomor
struct NodeRiwayat* cariRiwayat(struct NodeSantri *santri, int idPelanggaran);

// Menghitung total riwayat pelanggaran seorang santri
// Alur logika: Hitung berapa banyak catatan di buku riwayat
int hitungJumlahRiwayat(struct NodeSantri *santri);

// ============================================================
// BAGIAN DOUBLE LINKED LIST NON-CIRCULAR - DATA INDUK SANTRI
// Bagian dari materi: 7. DOUBLE LINKED LIST NON-CIRCULAR
// Seperti rantai dua arah: bisa maju (next) dan mundur (prev)
// ============================================================

// Menambah santri baru di akhir list
// Alur logika: Tambah kartu identitas baru di rak paling belakang
void tambahSantri(struct DataSantri data);

// Menghapus santri berdasarkan NIS
// Alur logika: Buang kartu identitas dari rak, rapikan sisa kartu
void hapusSantri(int nis);

// Mencari santri dengan pencarian linear (untuk edit/hapus manual)
// Alur logika: Cek kartu identitas satu per satu dari depan sampai belakang
struct NodeSantri* cariLinearSantri(int nis);

// Navigasi ke santri berikutnya (Next) untuk tombol Qt
// Alur logika: Geser ke kartu identitas berikutnya di rak
struct NodeSantri* navigasiNext();

// Navigasi ke santri sebelumnya (Previous) untuk tombol Qt
// Alur logika: Geser ke kartu identitas sebelumnya di rak
struct NodeSantri* navigasiPrev();

// Mengupdate data santri berdasarkan NIS
// Alur logika: Ganti isi kartu identitas yang sudah ada
void updateSantri(int nis, struct DataSantri dataBaru);

// Menampilkan seluruh data santri ke UI
// Alur logika: Tampilkan semua kartu identitas dari depan sampai belakang
void tampilkanSemuaSantri();

// ============================================================
// BAGIAN SINGLE LINKED LIST CIRCULAR - JADWAL PIKET PENGURUS
// Bagian dari materi: 8. SINGLE LINKED LIST CIRCULAR
// Seperti jam dinding: setelah angka 12, kembali ke angka 1
// ============================================================

// Menambah jadwal piket baru ke circular list
// Alur logika: Tambah jadwal jaga baru di putaran jadwal
void tambahJadwalPiket(struct DataPiket data);

// Menghapus jadwal piket berdasarkan nama pengurus dan hari
// Alur logika: Hapus jadwal jaga tertentu dari putaran
void hapusJadwalPiket(char namaPengurus[], char hari[]);

// Menampilkan jadwal piket hari ini
// Alur logika: Tampilkan siapa yang jaga malam ini
void tampilkanJadwalHariIni();

// Menampilkan seluruh jadwal piket (satu putaran penuh)
// Alur logika: Tampilkan semua jadwal jaga dari awal sampai kembali ke awal
void tampilkanSemuaJadwalPiket();

// Rotasi jadwal ke pengurus berikutnya (circular)
// Alur logika: Putar jadwal ke orang berikutnya, kalau habis balik ke awal
void rotasiPiket();

// Mencari jadwal piket berdasarkan nama pengurus
// Alur logika: Cari jadwal jaga si Fulan di putaran jadwal
struct NodePiket* cariJadwalPiket(char namaPengurus[]);

// ============================================================
// BAGIAN DOUBLE LINKED LIST CIRCULAR - MASTER ATURAN PELANGGARAN
// Bagian dari materi: 9. DOUBLE LINKED LIST CIRCULAR
// Seperti musik playlist yang repeat: bisa next/prev tanpa batas
// ============================================================

// Menambah aturan pelanggaran baru ke circular list
// Alur logika: Tambah hukum baru di daftar aturan yang muter
void tambahAturan(struct DataMaster data);

// Menghapus aturan pelanggaran berdasarkan kode
// Alur logika: Hapus hukum tertentu dari daftar aturan
void hapusAturan(int kode);

// Mengupdate bobot poin aturan yang sedang aktif (current)
// Alur logika: Ubah berat hukuman di aturan yang sedang ditampilkan
void updateBobotPoin(int kode, int bobotBaru);

// Scroll ke aturan berikutnya (Next) untuk form Qt
// Alur logika: Geser ke hukum berikutnya, kalau habis balik ke awal
struct NodeAturan* scrollAturanNext();

// Scroll ke aturan sebelumnya (Prev) untuk form Qt
// Alur logika: Geser ke hukum sebelumnya, kalau habis balik ke akhir
struct NodeAturan* scrollAturanPrev();

// Mencari aturan berdasarkan kode pelanggaran
// Alur logika: Cari hukum berdasarkan nomor kode di daftar aturan
struct NodeAturan* cariAturan(int kode);

// Menampilkan semua aturan pelanggaran
// Alur logika: Tampilkan semua hukum dari awal sampai kembali ke awal
void tampilkanSemuaAturan();

// ============================================================
// BAGIAN TREE (BST) - MESIN PENCARI UTAMA
// Bagian dari materi: 10. TREE (BST)
// BST seperti pohon keluarga yang tersusun rapi:
// anak kiri lebih kecil, anak kanan lebih besar, sangat cepat dicari
// ============================================================

// Menambah node santri ke BST berdasarkan NIS
// Alur logika: Tempatkan kartu identitas di posisi yang tepat di pohon
void insertBST(struct NodeBST **root, int nis, struct DataSantri data, struct NodeRiwayat *headRiwayat);

// Mencari santri berdasarkan NIS (super cepat, O(log n))
// Alur logika: Cari kartu identitas di pohon, mulai dari akar, belok kiri/kanan
struct NodeBST* cariNisBST(struct NodeBST *root, int nis);

// Menghapus node dari BST berdasarkan NIS
// Alur logika: Buang kartu identitas dari pohon, rapikan cabang-cabangnya
void hapusNodeBST(struct NodeBST **root, int nis);

// Traversal inorder (kiri-akar-kanan) untuk tampilkan urut NIS
// Alur logika: Baca kartu identitas dari nomor kecil ke besar
void inorderTraversal(struct NodeBST *root);

// Membangun ulang BST dari Double Linked List Santri
// Alur logika: Susun ulang semua kartu identitas ke pohon pencarian
void bangunBSTdariDLL();

// ============================================================
// BAGIAN GRAPH - PETA RUTE PATROLI ASRAMA
// Bagian dari materi: 11. GRAPH
// Graph seperti peta jalan: ada titik (blok) dan jalan (lorong) yang menghubungkan
// ============================================================

// Menambah vertex (titik/blok asrama) baru ke graph
// Alur logika: Tambah lokasi baru di peta asrama
void tambahVertex(char blok[], char kamar[]);

// Menambah edge (jalur/lorong) antar dua blok dengan jarak tertentu
// Alur logika: Tambah jalan baru yang menghubungkan dua lokasi
void tambahEdge(char blokAsal[], char blokTujuan[], int jarak);

// Menampilkan seluruh graph (vertex dan edge) ke UI
// Alur logika: Tampilkan peta lengkap dengan semua lokasi dan jalannya
void tampilkanGraph();

// Mencari vertex berdasarkan nama blok
// Alur logika: Cari lokasi "A1" di peta asrama
struct Vertex* cariVertex(char blok[]);

// Mencari rute terpendek dari blok start ke blok end (Dijkstra)
// Alur logika: Cari jalan tercepat dari pos jaga ke kamar tujuan
void cariRuteTerpendek(char start[], char end[]);

// Menampilkan daftar blok yang terhubung langsung dari suatu blok
// Alur logika: Tampilkan lokasi-lokasi yang bisa dicapai langsung dari sini
void tampilkanTetangga(char blok[]);

// ============================================================
// BAGIAN LOGIN & VERIFIKASI
// Bagian dari materi: 2. FUNCTION & 10. TREE (BST)
// ============================================================

// Verifikasi login santri menggunakan NIS via BST
// Alur logika: Cek apakah NIS yang dimasukkan ada di database pohon
struct NodeBST* verifikasiLoginSantri(int nis);

// Verifikasi login pengurus/pimpinan menggunakan kredensial
// Alur logika: Cek username dan password pengurus di database
int verifikasiLoginPengurus(char username[], char password[]);

// ============================================================
// BAGIAN DASHBOARD & MONITORING (PIMPINAN)
// Bagian dari materi: 2. FUNCTION & 7. DOUBLE LINKED LIST
// ============================================================

// Menampilkan daftar santri yang masuk Zona Merah (total poin tinggi)
// Alur logika: Cek semua santri, tandai yang poinnya sudah bahaya
void tampilkanZonaMerah();

// Reset seluruh poin santri ke 0 (Pemutihan Poin akhir semester)
// Alur logika: Hapus semua poin pelanggaran, mulai semester baru dengan bersih
void resetTotalPoin();

// Menampilkan statistik pelanggaran bulanan
// Alur logika: Hitung berapa pelanggaran terjadi bulan ini
void tampilkanStatistikBulanan();

// ============================================================
// BAGIAN OPERASI PELANGGARAN (PENGURUS)
// Bagian dari materi: 2. FUNCTION, 4. STACK, 5. QUEUE, 6. SLL
// ============================================================

// Proses input pelanggaran: masukkan ke Queue, lalu eksekusi
// Alur logika: Catat pelanggaran, masukkan antrean, proses sesuai urutan
void prosesInputPelanggaran(struct DataPelanggaran data);

// Eksekusi satu pelanggaran dari Queue (dequeue + hitung poin + masuk riwayat)
// Alur logika: Ambil pelanggaran dari antrean, hitung denda, catat di buku
void eksekusiPelanggaran(struct DataPelanggaran data);

// Fitur Undo: batalkan input pelanggaran terakhir (pop dari Stack)
// Alur logika: Ambil catatan terakhir, hapus, kembalikan poin santri
void undoInputTerakhir();

// Cek apakah santri masuk Zona Merah setelah ditambah poin
// Alur logika: Hitung total poin, kalau sudah bahaya tampilkan peringatan
void cekZonaMerah(int nis);

// ============================================================
#endif // KUMPULAN_FUNGSI_H
