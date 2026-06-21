// ============================================================
// E-TAKZIR: Modul Proses Pelanggaran (Queue + SLL Riwayat + Stack Undo)
// Paradigma: Pemrograman Prosedural Murni (C++)
// Fungsi: Mengelola antrean input pelanggaran, riwayat per santri,
//         dan fitur undo untuk koreksi input hari yang sama
// ============================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "kerangka_data.h"
#include "kumpulan_fungsi.h"

// ============================================================
// ============================================================
// BAGIAN 1: QUEUE (FIFO) - ANTREAN INPUT PELANGGARAN
// ============================================================
// ============================================================

// ============================================================
// Bagian dari materi: 5. QUEUE (FIFO)
// Inisialisasi Queue agar siap menerima antrean pelanggaran
// Alur logika: Siapkan loket antrean baru, pastikan belum ada orang mengantre
// ============================================================
void inisialisasiQueue() {
    if (antreanPelanggaran == NULL) {
        antreanPelanggaran = (struct Queue*)malloc(sizeof(struct Queue));
    }
    if (antreanPelanggaran != NULL) {
        antreanPelanggaran->front = NULL;   // Belum ada orang di depan loket
        antreanPelanggaran->rear = NULL;    // Belum ada orang di belakang loket
    }
}

// ============================================================
// Bagian dari materi: 5. QUEUE (FIFO)
// Menambah pelanggaran ke belakang antrean (enqueue)
// Alur logika: Orang baru datang, antre di belakang orang terakhir
// ============================================================
void enqueue(struct DataPelanggaran data) {
    if (antreanPelanggaran == NULL) {
        inisialisasiQueue();    // Kalau loket belum siap, siapkan dulu
    }

    struct NodeAntrean *baru = (struct NodeAntrean*)malloc(sizeof(struct NodeAntrean));
    if (baru == NULL) {
        printf("Gagal alokasi memori untuk antrean!\n");
        return;
    }
    baru->data = data;
    baru->next = NULL;          // Orang baru belum punya orang di belakangnya

    if (antreanPelanggaran->rear == NULL) {
        // Loket masih kosong, orang baru jadi yang pertama dan terakhir
        antreanPelanggaran->front = antreanPelanggaran->rear = baru;
    } else {
        // Tambah di belakang orang terakhir yang ada
        antreanPelanggaran->rear->next = baru;
        antreanPelanggaran->rear = baru;
    }
}

// ============================================================
// Bagian dari materi: 5. QUEUE (FIFO)
// Mengambil pelanggaran dari depan antrean (dequeue)
// Alur logika: Orang paling depan dilayani, keluar dari antrean
// ============================================================
struct DataPelanggaran dequeue() {
    struct DataPelanggaran kosong;
    memset(&kosong, 0, sizeof(struct DataPelanggaran));  // Isi dengan nol semua

    if (antreanPelanggaran == NULL || antreanPelanggaran->front == NULL) {
        printf("Antrean kosong, tidak ada pelanggaran yang bisa diproses!\n");
        return kosong;
    }

    struct NodeAntrean *temp = antreanPelanggaran->front;
    struct DataPelanggaran hasil = temp->data;  // Ambil data orang paling depan

    // Geser antrean depan ke orang berikutnya
    antreanPelanggaran->front = antreanPelanggaran->front->next;
    if (antreanPelanggaran->front == NULL) {
        antreanPelanggaran->rear = NULL;    // Antrean sudah habis semua
    }

    free(temp);     // Orang yang sudah dilayani tidak perlu antre lagi
    return hasil;
}

// ============================================================
// Bagian dari materi: 5. QUEUE (FIFO)
// Mengecek apakah Queue kosong
// Alur logika: Cek apakah loket antrean masih kosong
// ============================================================
int isEmptyQueue() {
    if (antreanPelanggaran == NULL || antreanPelanggaran->front == NULL) {
        return 1;   // Ya, loket masih kosong
    }
    return 0;       // Tidak kosong, masih ada yang mengantre
}

// ============================================================
// Bagian dari materi: 5. QUEUE (FIFO)
// Menghitung jumlah antrean yang menunggu
// Alur logika: Hitung berapa orang yang masih mengantre di loket
// ============================================================
int hitungAntrean() {
    if (antreanPelanggaran == NULL || antreanPelanggaran->front == NULL) {
        return 0;
    }
    int jumlah = 0;
    struct NodeAntrean *current = antreanPelanggaran->front;
    while (current != NULL) {
        jumlah++;
        current = current->next;    // Geser ke orang berikutnya di antrean
    }
    return jumlah;
}

// ============================================================
// ============================================================
// BAGIAN 2: SINGLE LINKED LIST NON-CIRCULAR - RIWAYAT PELANGGARAN
// ============================================================
// ============================================================

// ============================================================
// Bagian dari materi: 6. SINGLE LINKED LIST NON-CIRCULAR
// Menambah riwayat pelanggaran di akhir list milik santri tertentu
// Alur logika: Tambah catatan baru di buku riwayat paling belakang
// ============================================================
void tambahRiwayat(struct NodeSantri *santri, struct DataPelanggaran data) {
    if (santri == NULL) {
        printf("Santri tidak valid, tidak bisa tambah riwayat!\n");
        return;
    }

    struct NodeRiwayat *baru = (struct NodeRiwayat*)malloc(sizeof(struct NodeRiwayat));
    if (baru == NULL) {
        printf("Gagal alokasi memori untuk riwayat!\n");
        return;
    }
    baru->data = data;
    baru->next = NULL;          // Catatan baru jadi halaman terakhir

    if (santri->headRiwayat == NULL) {
        // Buku riwayat masih kosong, catatan ini jadi halaman pertama
        santri->headRiwayat = baru;
    } else {
        // Selipkan di halaman terakhir buku riwayat
        struct NodeRiwayat *akhir = santri->headRiwayat;
        while (akhir->next != NULL) {
            akhir = akhir->next;    // Cari halaman terakhir
        }
        akhir->next = baru;         // Tambah halaman baru di belakang
    }
}

// ============================================================
// Bagian dari materi: 6. SINGLE LINKED LIST NON-CIRCULAR
// Menampilkan seluruh riwayat pelanggaran santri ke konsol
// Alur logika: Buka buku riwayat dari halaman pertama sampai habis
// ============================================================
void tampilkanRiwayat(struct NodeSantri *santri) {
    if (santri == NULL) {
        printf("Santri tidak ditemukan!\n");
        return;
    }

    printf("\n=== RIWAYAT PELANGGARAN: %s (NIS: %d) ===\n",
           santri->data.nama, santri->data.nis);
    printf("%-6s %-12s %-20s %-8s %-30s\n",
           "NO", "TANGGAL", "JENIS", "POIN", "KETERANGAN");
    printf("--------------------------------------------------------------------------\n");

    struct NodeRiwayat *current = santri->headRiwayat;
    int no = 1;
    while (current != NULL) {
        printf("%-6d %-12s %-20s %-8d %-30s\n",
               no++,
               current->data.tanggal,
               current->data.jenis,
               current->data.bobotPoin,
               current->data.keterangan);
        current = current->next;    // Buka halaman berikutnya
    }

    if (no == 1) {
        printf("Belum ada riwayat pelanggaran. Alhamdulillah.\n");
    }
    printf("==========================================\n\n");
}

// ============================================================
// Bagian dari materi: 6. SINGLE LINKED LIST NON-CIRCULAR
// Menghapus riwayat terakhir santri (digunakan saat Undo dari Stack)
// Alur logika: Sobek halaman terakhir dari buku riwayat
// ============================================================
void hapusRiwayatTerakhir(struct NodeSantri *santri) {
    if (santri == NULL || santri->headRiwayat == NULL) {
        printf("Tidak ada riwayat yang bisa dihapus!\n");
        return;
    }

    if (santri->headRiwayat->next == NULL) {
        // Hanya ada satu catatan di buku, buang semua
        free(santri->headRiwayat);
        santri->headRiwayat = NULL;
        return;
    }

    // Cari catatan sebelum halaman terakhir
    struct NodeRiwayat *current = santri->headRiwayat;
    while (current->next->next != NULL) {
        current = current->next;    // Maju sampai sebelum halaman terakhir
    }

    free(current->next);        // Sobek halaman terakhir
    current->next = NULL;       // Yang sekarang jadi halaman terakhir
}

// ============================================================
// Bagian dari materi: 6. SINGLE LINKED LIST NON-CIRCULAR
// Mencari riwayat berdasarkan ID pelanggaran
// Alur logika: Cari catatan spesifik di buku riwayat berdasarkan nomor ID
// ============================================================
struct NodeRiwayat* cariRiwayat(struct NodeSantri *santri, int idPelanggaran) {
    if (santri == NULL) return NULL;

    struct NodeRiwayat *current = santri->headRiwayat;
    while (current != NULL) {
        if (current->data.idPelanggaran == idPelanggaran) {
            return current;     // Ketemu! Catatan dengan ID tersebut ada
        }
        current = current->next;    // Buka halaman berikutnya
    }
    return NULL;    // Sampai habis tidak ketemu
}

// ============================================================
// Bagian dari materi: 6. SINGLE LINKED LIST NON-CIRCULAR
// Menghitung total riwayat pelanggaran seorang santri
// Alur logika: Hitung berapa banyak catatan di buku riwayat
// ============================================================
int hitungJumlahRiwayat(struct NodeSantri *santri) {
    if (santri == NULL || santri->headRiwayat == NULL) {
        return 0;
    }

    int jumlah = 0;
    struct NodeRiwayat *current = santri->headRiwayat;
    while (current != NULL) {
        jumlah++;
        current = current->next;    // Buka halaman berikutnya
    }
    return jumlah;
}

// ============================================================
// ============================================================
// BAGIAN 3: STACK (LIFO) - FITUR UNDO INPUT PELANGGARAN
// ============================================================
// ============================================================

// ============================================================
// Bagian dari materi: 4. STACK (LIFO)
// Menambah data pelanggaran ke puncak Stack (Undo)
// Alur logika: Taruh piring baru di atas tumpukan piring yang sudah ada
// ============================================================
void pushUndo(struct DataPelanggaran data) {
    struct NodeUndo *baru = (struct NodeUndo*)malloc(sizeof(struct NodeUndo));
    if (baru == NULL) {
        printf("Gagal alokasi memori untuk undo!\n");
        return;
    }
    baru->data = data;
    baru->next = topUndo;       // Yang baru menunjuk ke piring yang sebelumnya di atas
    topUndo = baru;             // Yang baru sekarang jadi piring paling atas
}

// ============================================================
// Bagian dari materi: 4. STACK (LIFO)
// Mengambil dan menghapus data teratas dari Stack (Undo)
// Alur logika: Ambil piring paling atas dari tumpukan
// ============================================================
struct DataPelanggaran popUndo() {
    struct DataPelanggaran kosong;
    memset(&kosong, 0, sizeof(struct DataPelanggaran));  // Isi nol semua

    if (topUndo == NULL) {
        printf("Stack Undo kosong, tidak ada yang bisa dibatalkan!\n");
        return kosong;
    }

    struct NodeUndo *temp = topUndo;
    struct DataPelanggaran hasil = temp->data;  // Ambil data piring paling atas

    topUndo = topUndo->next;    // Yang di bawahnya naik jadi piring paling atas
    free(temp);                 // Buang piring yang sudah diambil
    return hasil;
}

// ============================================================
// Bagian dari materi: 4. STACK (LIFO)
// Mengecek apakah Stack Undo kosong
// Alur logika: Cek apakah tumpukan piringnya sudah habis
// ============================================================
int isEmptyUndo() {
    return (topUndo == NULL) ? 1 : 0;
}

// ============================================================
// Bagian dari materi: 4. STACK (LIFO)
// Reset Stack Undo (bersihkan semua isi tumpukan)
// Dipanggil setiap pergantian hari agar undo hanya untuk hari yang sama
// Alur logika: Bersihkan semua piring di tumpukan, mulai dari nol
// ============================================================
void resetUndo() {
    while (topUndo != NULL) {
        struct NodeUndo *temp = topUndo;
        topUndo = topUndo->next;
        free(temp);
    }
    topUndo = NULL;
}

// ============================================================
// ============================================================
// BAGIAN 4: UTILITAS TANGGAL (Pembatas Undo Harian)
// ============================================================
// ============================================================

// ============================================================
// Bagian dari materi: 2. FUNCTION
// Mendapatkan tanggal hari ini dalam format DD/MM/YYYY
// Alur logika: Lihat kalender hari ini, tulis format tanggal Indonesia
// ============================================================
void ambilTanggalHariIni(char hasil[]) {
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);
    sprintf(hasil, "%02d/%02d/%04d", info->tm_mday, info->tm_mon + 1, info->tm_year + 1900);
}

// ============================================================
// Bagian dari materi: 2. FUNCTION
// Mengecek apakah string tanggal sama dengan hari ini
// Digunakan untuk batasi fitur Undo hanya untuk hari yang sama
// Alur logika: Bandingkan tanggal di catatan dengan tanggal hari ini
// ============================================================
int cekTanggalHariIni(char tanggal[]) {
    char hariIni[11];
    ambilTanggalHariIni(hariIni);
    return (strcmp(tanggal, hariIni) == 0) ? 1 : 0;
}

// ============================================================
// ============================================================
// BAGIAN 5: ORKESTRASI PROSES PELANGGARAN (Queue + Stack + SLL + DLL + BST)
// ============================================================
// ============================================================

// ============================================================
// Bagian dari materi: 5. QUEUE (FIFO) & 2. FUNCTION
// Proses input pelanggaran: masukkan ke Queue untuk diproses berurutan
// Alur logika: Catat pelanggaran, masukkan antrean, tunggu giliran diproses
// ============================================================
void prosesInputPelanggaran(struct DataPelanggaran data) {
    // Beri ID unik untuk pelanggaran ini (auto-increment)
    idPelanggaranTerakhir++;
    data.idPelanggaran = idPelanggaranTerakhir;

    // Isi tanggal hari ini kalau belum ada
    if (strlen(data.tanggal) == 0) {
        ambilTanggalHariIni(data.tanggal);
    }

    // Masukkan ke antrean (Queue) untuk diproses berurutan
    enqueue(data);
    printf("Pelanggaran ID %d masuk antrean. Total menunggu: %d\n",
           data.idPelanggaran, hitungAntrean());
}

// ============================================================
// Bagian dari materi: 5. QUEUE (FIFO) & 6. SINGLE LINKED LIST
// & 4. STACK (LIFO) & 7. DOUBLE LINKED LIST & 10. TREE (BST)
// Eksekusi satu pelanggaran dari Queue (dequeue + hitung poin + masuk riwayat)
// Alur logika: Ambil pelanggaran dari antrean, catat di buku riwayat santri,
//              tambah poin di kartu identitas, cek apakah sudah bahaya
// ============================================================
void eksekusiPelanggaran(struct DataPelanggaran data) {
    // Cari santri yang bersangkutan di DLL (Rak Kartu Identitas)
    struct NodeSantri *santri = cariLinearSantri(data.nisSantri);
    if (santri == NULL) {
        printf("Santri dengan NIS %d tidak ditemukan! Pelanggaran dibatalkan.\n", data.nisSantri);
        return;
    }

    // Simpan ke Stack Undo (bisa dibatalkan kalau salah input hari ini)
    // Alur logika: Sebelum catat permanen, simpan salinan di tumpukan undo
    pushUndo(data);

    // Masukkan ke buku riwayat santri (Single Linked List)
    // Alur logika: Tulis catatan pelanggaran di buku riwayat santri
    tambahRiwayat(santri, data);

    // Tambah total poin santri di DLL (Rak Kartu Identitas)
    santri->data.totalPoin += data.bobotPoin;

    // Update juga di BST agar data pencarian tetap sinkron
    struct NodeBST *nodeBST = cariNisBST(rootBST, data.nisSantri);
    if (nodeBST != NULL) {
        nodeBST->data.totalPoin = santri->data.totalPoin;
    }

    printf("Pelanggaran ID %d berhasil diproses. %s mendapat %d poin. Total poin: %d\n",
           data.idPelanggaran, santri->data.nama, data.bobotPoin, santri->data.totalPoin);

    // Cek apakah santri masuk zona merah setelah ditambah poin
    cekZonaMerah(data.nisSantri);
}

// ============================================================
// Bagian dari materi: 4. STACK (LIFO) & 6. SINGLE LINKED LIST
// & 7. DOUBLE LINKED LIST & 10. TREE (BST)
// Fitur Undo: batalkan input pelanggaran terakhir
// Alur logika: Ambil catatan terakhir dari tumpukan, cek masih hari ini,
//              hapus dari buku riwayat, kembalikan poin santri seperti semula
// ============================================================
void undoInputTerakhir() {
    if (isEmptyUndo()) {
        printf("Tidak ada data yang bisa di-undo!\n");
        return;
    }

    // Ambil data terakhir dari tumpukan (Stack)
    struct DataPelanggaran data = popUndo();

    // Cek apakah masih bisa di-undo (hanya untuk hari yang sama)
    if (!cekTanggalHariIni(data.tanggal)) {
        printf("Tidak bisa undo: data sudah dari hari lain! (Tanggal: %s)\n", data.tanggal);
        // Push kembali ke stack karena tidak jadi di-undo
        pushUndo(data);
        return;
    }

    // Cari santri yang bersangkutan di DLL
    struct NodeSantri *santri = cariLinearSantri(data.nisSantri);
    if (santri == NULL) {
        printf("Santri tidak ditemukan untuk proses undo!\n");
        return;
    }

    // Hapus riwayat terakhir santri (yang paling baru diinput)
    // Asumsi: riwayat selalu ditambah di akhir SLL, jadi terakhir = yang di-undo
    hapusRiwayatTerakhir(santri);

    // Kembalikan poin santri (kurangi dengan bobot yang di-undo)
    santri->data.totalPoin -= data.bobotPoin;
    if (santri->data.totalPoin < 0) {
        santri->data.totalPoin = 0;     // Jangan sampai poin negatif
    }

    // Update juga di BST agar sinkron
    struct NodeBST *nodeBST = cariNisBST(rootBST, data.nisSantri);
    if (nodeBST != NULL) {
        nodeBST->data.totalPoin = santri->data.totalPoin;
    }

    printf("Undo berhasil: Pelanggaran ID %d dibatalkan. Poin %s dikembalikan (%d poin). Total sekarang: %d\n",
           data.idPelanggaran, santri->data.nama, data.bobotPoin, santri->data.totalPoin);
}