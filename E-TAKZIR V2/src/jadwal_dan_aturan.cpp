// ============================================================
// E-TAKZIR: Modul Jadwal, Aturan, Graph, dan Login
// Paradigma: Pemrograman Prosedural Murni (C++)
// Fungsi: Master Aturan (DLL Circular), Jadwal Piket (SLL Circular),
//         Peta Rute (Graph), Login Pimpinan/Pengurus, Statistik
// ============================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "kerangka_data.h"
#include "kumpulan_fungsi.h"

// ============================================================
// Konstanta Sistem
// ============================================================
#define MAKS_VERTEX 100
#define INF 999999

// ============================================================
// ============================================================
// BAGIAN 1: DOUBLE LINKED LIST CIRCULAR - MASTER ATURAN PELANGGARAN
// ============================================================
// ============================================================

// ============================================================
// Bagian dari materi: 9. DOUBLE LINKED LIST CIRCULAR
// Menambah aturan pelanggaran baru ke circular list
// Alur logika: Tambah hukum baru di daftar aturan yang muter tanpa ujung
// ============================================================
void tambahAturan(struct DataMaster data) {
    struct NodeAturan *baru = (struct NodeAturan*)malloc(sizeof(struct NodeAturan));
    if (baru == NULL) {
        printf("Gagal alokasi memori untuk aturan baru!\n");
        return;
    }
    baru->data = data;
    baru->next = NULL;
    baru->prev = NULL;

    if (headAturan == NULL) {
        // Daftar aturan masih kosong, yang pertama menunjuk dirinya sendiri
        headAturan = tailAturan = currentAturan = baru;
        baru->next = baru;
        baru->prev = baru;
    } else {
        // Sambungkan di belakang aturan terakhir, lalu muter kembali ke awal
        tailAturan->next = baru;
        baru->prev = tailAturan;
        baru->next = headAturan;    // Yang baru menunjuk ke awal (circular)
        headAturan->prev = baru;    // Awal menunjuk ke yang baru
        tailAturan = baru;
    }
    printf("Aturan '%s' (Kode: %d, Poin: %d) berhasil ditambahkan.\n",
           data.namaPelanggaran, data.kodePelanggaran, data.bobotPoin);
}

// ============================================================
// Bagian dari materi: 9. DOUBLE LINKED LIST CIRCULAR
// Menghapus aturan pelanggaran berdasarkan kode
// Alur logika: Hapus hukum tertentu dari daftar aturan yang muter
// ============================================================
void hapusAturan(int kode) {
    if (headAturan == NULL) {
        printf("Daftar aturan masih kosong!\n");
        return;
    }

    struct NodeAturan *cari = headAturan;
    do {
        if (cari->data.kodePelanggaran == kode) {
            // Ketemu aturan yang mau dihapus
            if (cari->next == cari) {
                // Hanya ada satu aturan, daftar jadi kosong
                free(cari);
                headAturan = tailAturan = currentAturan = NULL;
            } else {
                // Rapikan pointer sekitar node yang dihapus
                cari->prev->next = cari->next;
                cari->next->prev = cari->prev;

                if (cari == headAturan) headAturan = cari->next;
                if (cari == tailAturan) tailAturan = cari->prev;
                if (currentAturan == cari) currentAturan = cari->next;

                free(cari);
            }
            printf("Aturan dengan kode %d berhasil dihapus.\n", kode);
            return;
        }
        cari = cari->next;
    } while (cari != headAturan);

    printf("Aturan dengan kode %d tidak ditemukan!\n", kode);
}

// ============================================================
// Bagian dari materi: 9. DOUBLE LINKED LIST CIRCULAR
// Mengupdate bobot poin aturan berdasarkan kode
// Alur logika: Ubah berat hukuman di aturan yang sudah ada
// ============================================================
void updateBobotPoin(int kode, int bobotBaru) {
    struct NodeAturan *cari = headAturan;
    if (cari == NULL) return;

    do {
        if (cari->data.kodePelanggaran == kode) {
            cari->data.bobotPoin = bobotBaru;
            printf("Bobot poin aturan '%s' diupdate menjadi %d.\n",
                   cari->data.namaPelanggaran, bobotBaru);
            return;
        }
        cari = cari->next;
    } while (cari != headAturan);

    printf("Aturan dengan kode %d tidak ditemukan untuk update!\n", kode);
}

// ============================================================
// Bagian dari materi: 9. DOUBLE LINKED LIST CIRCULAR
// Scroll ke aturan berikutnya (Next) untuk form Qt
// Alur logika: Geser ke hukum berikutnya, kalau habis balik ke awal
// ============================================================
struct NodeAturan* scrollAturanNext() {
    if (currentAturan == NULL) return NULL;
    currentAturan = currentAturan->next;    // Geser ke aturan berikutnya
    return currentAturan;
}

// ============================================================
// Bagian dari materi: 9. DOUBLE LINKED LIST CIRCULAR
// Scroll ke aturan sebelumnya (Prev) untuk form Qt
// Alur logika: Geser ke hukum sebelumnya, kalau habis balik ke akhir
// ============================================================
struct NodeAturan* scrollAturanPrev() {
    if (currentAturan == NULL) return NULL;
    currentAturan = currentAturan->prev;    // Geser ke aturan sebelumnya
    return currentAturan;
}

// ============================================================
// Bagian dari materi: 9. DOUBLE LINKED LIST CIRCULAR
// Mencari aturan berdasarkan kode pelanggaran
// Alur logika: Cari hukum berdasarkan nomor kode di daftar aturan yang muter
// ============================================================
struct NodeAturan* cariAturan(int kode) {
    if (headAturan == NULL) return NULL;

    struct NodeAturan *cari = headAturan;
    do {
        if (cari->data.kodePelanggaran == kode) {
            return cari;    // Ketemu! Aturan dengan kode tersebut ada
        }
        cari = cari->next;
    } while (cari != headAturan);
    return NULL;    // Muter satu putaran penuh, tidak ketemu
}

// ============================================================
// Bagian dari materi: 9. DOUBLE LINKED LIST CIRCULAR
// Menampilkan semua aturan pelanggaran (satu putaran penuh)
// Alur logika: Tampilkan semua hukum dari awal sampai kembali ke awal
// ============================================================
void tampilkanSemuaAturan() {
    if (headAturan == NULL) {
        printf("Belum ada aturan pelanggaran yang terdaftar.\n");
        return;
    }

    printf("\n=== DAFTAR MASTER ATURAN PELANGGARAN ===\n");
    printf("%-8s %-25s %-8s\n", "KODE", "NAMA PELANGGARAN", "POIN");
    printf("-------------------------------------------\n");

    struct NodeAturan *current = headAturan;
    do {
        printf("%-8d %-25s %-8d\n",
               current->data.kodePelanggaran,
               current->data.namaPelanggaran,
               current->data.bobotPoin);
        current = current->next;
    } while (current != headAturan);    // Muter sampai kembali ke awal

    printf("=========================================\n\n");
}

// ============================================================
// ============================================================
// BAGIAN 2: SINGLE LINKED LIST CIRCULAR - JADWAL PIKET PENGURUS
// ============================================================
// ============================================================

// ============================================================
// Bagian dari materi: 8. SINGLE LINKED LIST CIRCULAR
// Menambah jadwal piket baru ke circular list
// Alur logika: Tambah jadwal jaga baru di putaran jadwal yang tidak berujung
// ============================================================
void tambahJadwalPiket(struct DataPiket data) {
    struct NodePiket *baru = (struct NodePiket*)malloc(sizeof(struct NodePiket));
    if (baru == NULL) {
        printf("Gagal alokasi memori untuk jadwal piket!\n");
        return;
    }
    baru->data = data;
    baru->next = NULL;

    if (headPiket == NULL) {
        // Jadwal masih kosong, yang pertama menunjuk dirinya sendiri
        headPiket = currentPiket = baru;
        baru->next = baru;
    } else {
        // Selipkan di belakang yang terakhir, lalu muter ke awal
        struct NodePiket *akhir = headPiket;
        while (akhir->next != headPiket) {
            akhir = akhir->next;    // Cari yang terakhir di putaran
        }
        akhir->next = baru;         // Yang terakhir menunjuk ke yang baru
        baru->next = headPiket;     // Yang baru menunjuk ke awal (muter)
    }
    printf("Jadwal piket %s (%s - %s) berhasil ditambahkan.\n",
           data.namaPengurus, data.hari, data.shift);
}

// ============================================================
// Bagian dari materi: 8. SINGLE LINKED LIST CIRCULAR
// Menghapus jadwal piket berdasarkan nama pengurus dan hari
// Alur logika: Hapus jadwal jaga tertentu dari putaran jadwal
// ============================================================
void hapusJadwalPiket(char namaPengurus[], char hari[]) {
    if (headPiket == NULL) {
        printf("Jadwal piket masih kosong!\n");
        return;
    }

    struct NodePiket *cari = headPiket;
    struct NodePiket *sebelumnya = NULL;

    do {
        if (strcmp(cari->data.namaPengurus, namaPengurus) == 0 &&
            strcmp(cari->data.hari, hari) == 0) {

            if (cari->next == cari) {
                // Hanya ada satu jadwal
                free(cari);
                headPiket = currentPiket = NULL;
            } else {
                // Cari node sebelumnya
                struct NodePiket *temp = headPiket;
                while (temp->next != cari) {
                    temp = temp->next;
                }
                temp->next = cari->next;    // Lewati node yang dihapus
                if (cari == headPiket) headPiket = cari->next;
                if (currentPiket == cari) currentPiket = cari->next;
                free(cari);
            }
            printf("Jadwal piket %s hari %s berhasil dihapus.\n", namaPengurus, hari);
            return;
        }
        cari = cari->next;
    } while (cari != headPiket);

    printf("Jadwal piket tidak ditemukan!\n");
}

// ============================================================
// Bagian dari materi: 8. SINGLE LINKED LIST CIRCULAR
// Menampilkan jadwal piket hari ini
// Alur logika: Cek putaran jadwal, tampilkan siapa yang jaga hari ini
// ============================================================
void tampilkanJadwalHariIni() {
    if (headPiket == NULL) {
        printf("Belum ada jadwal piket yang terdaftar.\n");
        return;
    }

    // Ambil nama hari dalam bahasa Indonesia
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);
    char hariIni[10];
    switch (info->tm_wday) {
        case 0: strcpy(hariIni, "Minggu"); break;
        case 1: strcpy(hariIni, "Senin"); break;
        case 2: strcpy(hariIni, "Selasa"); break;
        case 3: strcpy(hariIni, "Rabu"); break;
        case 4: strcpy(hariIni, "Kamis"); break;
        case 5: strcpy(hariIni, "Jumat"); break;
        case 6: strcpy(hariIni, "Sabtu"); break;
    }

    printf("\n=== JADWAL PIKET HARI INI (%s) ===\n", hariIni);
    struct NodePiket *current = headPiket;
    int ada = 0;
    do {
        if (strcmp(current->data.hari, hariIni) == 0) {
            printf("- %s | Shift: %s\n", current->data.namaPengurus, current->data.shift);
            ada = 1;
        }
        current = current->next;
    } while (current != headPiket);

    if (!ada) printf("Tidak ada jadwal piket untuk hari ini.\n");
    printf("==================================\n\n");
}

// ============================================================
// Bagian dari materi: 8. SINGLE LINKED LIST CIRCULAR
// Menampilkan seluruh jadwal piket (satu putaran penuh)
// Alur logika: Tampilkan semua jadwal jaga dari awal sampai kembali ke awal
// ============================================================
void tampilkanSemuaJadwalPiket() {
    if (headPiket == NULL) {
        printf("Belum ada jadwal piket yang terdaftar.\n");
        return;
    }

    printf("\n=== DAFTAR JADWAL PIKET PENGURUS ===\n");
    printf("%-20s %-10s %-10s\n", "NAMA PENGURUS", "HARI", "SHIFT");
    printf("------------------------------------------\n");

    struct NodePiket *current = headPiket;
    do {
        printf("%-20s %-10s %-10s\n",
               current->data.namaPengurus,
               current->data.hari,
               current->data.shift);
        current = current->next;
    } while (current != headPiket);     // Muter sampai kembali ke awal

    printf("====================================\n\n");
}

// ============================================================
// Bagian dari materi: 8. SINGLE LINKED LIST CIRCULAR
// Rotasi jadwal ke pengurus berikutnya (circular)
// Alur logika: Putar jadwal ke orang berikutnya, kalau habis balik ke awal
// ============================================================
void rotasiPiket() {
    if (currentPiket == NULL) {
        printf("Belum ada jadwal piket untuk dirotasi.\n");
        return;
    }
    currentPiket = currentPiket->next;    // Geser ke jadwal berikutnya
    printf("Rotasi piket: sekarang aktif -> %s (%s - %s)\n",
           currentPiket->data.namaPengurus,
           currentPiket->data.hari,
           currentPiket->data.shift);
}

// ============================================================
// Bagian dari materi: 8. SINGLE LINKED LIST CIRCULAR
// Mencari jadwal piket berdasarkan nama pengurus
// Alur logika: Cari jadwal jaga si Fulan di putaran jadwal
// ============================================================
struct NodePiket* cariJadwalPiket(char namaPengurus[]) {
    if (headPiket == NULL) return NULL;

    struct NodePiket *cari = headPiket;
    do {
        if (strcmp(cari->data.namaPengurus, namaPengurus) == 0) {
            return cari;    // Ketemu! Jadwal jaga Fulan ditemukan
        }
        cari = cari->next;
    } while (cari != headPiket);
    return NULL;    // Muter satu putaran, tidak ketemu
}

// ============================================================
// ============================================================
// BAGIAN 3: GRAPH - PETA RUTE PATROLI ASRAMA
// ============================================================
// ============================================================

// ============================================================
// Bagian dari materi: 11. GRAPH
// Mencari vertex berdasarkan nama blok di Graph
// Alur logika: Cari lokasi di peta asrama dengan pencarian satu per satu
// ============================================================
struct Vertex* cariVertex(char blok[]) {
    struct Vertex *current = headVertex;
    while (current != NULL) {
        if (strcmp(current->namaBlok, blok) == 0) {
            return current;     // Ketemu! Lokasi ditemukan di peta
        }
        current = current->nextVertex;  // Cek lokasi berikutnya
    }
    return NULL;    // Lokasi tidak ditemukan di peta asrama
}

// ============================================================
// Bagian dari materi: 11. GRAPH
// Menambah vertex (titik/blok asrama) baru ke graph
// Alur logika: Tambah lokasi baru di peta asrama
// ============================================================
void tambahVertex(char blok[], char kamar[]) {
    // Cek dulu apakah blok sudah ada
    if (cariVertex(blok) != NULL) {
        printf("Blok %s sudah ada di peta!\n", blok);
        return;
    }

    struct Vertex *baru = (struct Vertex*)malloc(sizeof(struct Vertex));
    if (baru == NULL) {
        printf("Gagal alokasi memori untuk vertex!\n");
        return;
    }
    strcpy(baru->namaBlok, blok);
    strcpy(baru->namaKamar, kamar);
    baru->firstEdge = NULL;
    baru->nextVertex = NULL;

    // Tambahkan ke daftar vertex (SLL untuk daftar vertex)
    if (headVertex == NULL) {
        headVertex = baru;      // Peta masih kosong, lokasi ini jadi titik pertama
    } else {
        struct Vertex *temp = headVertex;
        while (temp->nextVertex != NULL) {
            temp = temp->nextVertex;    // Cari titik terakhir di peta
        }
        temp->nextVertex = baru;        // Tambah titik baru di belakang
    }
    printf("Vertex blok %s (%s) berhasil ditambahkan ke peta.\n", blok, kamar);
}

// ============================================================
// Bagian dari materi: 11. GRAPH
// Menambah edge (jalur/lorong) antar dua blok dengan jarak tertentu
// Alur logika: Tarik jalan baru yang menghubungkan dua lokasi di peta
// ============================================================
void tambahEdge(char blokAsal[], char blokTujuan[], int jarak) {
    struct Vertex *vAsal = cariVertex(blokAsal);
    struct Vertex *vTujuan = cariVertex(blokTujuan);

    if (vAsal == NULL) {
        printf("Blok asal %s tidak ditemukan! Tambahkan vertex dulu.\n", blokAsal);
        return;
    }
    if (vTujuan == NULL) {
        printf("Blok tujuan %s tidak ditemukan! Tambahkan vertex dulu.\n", blokTujuan);
        return;
    }

    // Buat edge dari asal ke tujuan (arah satu arah, bisa dibuat dua arah jika perlu)
    struct Edge *edgeBaru = (struct Edge*)malloc(sizeof(struct Edge));
    if (edgeBaru == NULL) {
        printf("Gagal alokasi memori untuk edge!\n");
        return;
    }
    strcpy(edgeBaru->blokTujuan, blokTujuan);
    edgeBaru->jarakMeter = jarak;
    edgeBaru->nextEdge = NULL;

    // Sambungkan edge ke vertex asal (seperti menarik jalan dari titik ini)
    if (vAsal->firstEdge == NULL) {
        vAsal->firstEdge = edgeBaru;    // Titik ini belum punya jalan, tarik yang pertama
    } else {
        struct Edge *temp = vAsal->firstEdge;
        while (temp->nextEdge != NULL) {
            temp = temp->nextEdge;      // Cari jalan terakhir dari titik ini
        }
        temp->nextEdge = edgeBaru;      // Tambah jalan baru
    }
    printf("Edge %s -> %s (%d meter) berhasil ditambahkan.\n", blokAsal, blokTujuan, jarak);
}

// ============================================================
// Bagian dari materi: 11. GRAPH
// Menampilkan seluruh graph (vertex dan edge) ke konsol
// Alur logika: Tampilkan peta lengkap dengan semua lokasi dan jalannya
// ============================================================
void tampilkanGraph() {
    if (headVertex == NULL) {
        printf("Peta asrama masih kosong!\n");
        return;
    }

    printf("\n=== PETA RUTE ASRAMA (GRAPH) ===\n");
    struct Vertex *v = headVertex;
    while (v != NULL) {
        printf("\n[%s - %s]\n", v->namaBlok, v->namaKamar);
        printf("  Terhubung ke: ");
        struct Edge *e = v->firstEdge;
        if (e == NULL) {
            printf("(tidak ada jalur langsung)");
        }
        while (e != NULL) {
            printf("%s(%dm)", e->blokTujuan, e->jarakMeter);
            if (e->nextEdge != NULL) printf(", ");
            e = e->nextEdge;
        }
        printf("\n");
        v = v->nextVertex;
    }
    printf("\n=================================\n\n");
}

// ============================================================
// Bagian dari materi: 11. GRAPH
// Menampilkan daftar blok yang terhubung langsung dari suatu blok
// Alur logika: Tampilkan lokasi-lokasi yang bisa dicapai langsung dari sini
// ============================================================
void tampilkanTetangga(char blok[]) {
    struct Vertex *v = cariVertex(blok);
    if (v == NULL) {
        printf("Blok %s tidak ditemukan di peta!\n", blok);
        return;
    }

    printf("\nBlok %s terhubung langsung dengan: ", blok);
    struct Edge *e = v->firstEdge;
    if (e == NULL) {
        printf("(tidak ada)\n");
        return;
    }
    while (e != NULL) {
        printf("%s(%dm)", e->blokTujuan, e->jarakMeter);
        if (e->nextEdge != NULL) printf(", ");
        e = e->nextEdge;
    }
    printf("\n\n");
}

// ============================================================
// Bagian dari materi: 11. GRAPH
// Mencari rute terpendek dari blok start ke blok end (Algoritma Dijkstra sederhana)
// Alur logika: Cari jalan tercepat dari pos jaga ke kamar tujuan,
//              seperti mencari jalan terpendek di Google Maps versi sederhana
// ============================================================
void cariRuteTerpendek(char start[], char end[]) {
    struct Vertex *vStart = cariVertex(start);
    struct Vertex *vEnd = cariVertex(end);

    if (vStart == NULL || vEnd == NULL) {
        printf("Blok asal atau tujuan tidak ditemukan di peta!\n");
        return;
    }

    // Kumpulkan semua vertex ke array untuk algoritma Dijkstra
    struct Vertex *daftarVertex[MAKS_VERTEX];
    int jarak[MAKS_VERTEX];
    int sudahDikunjungi[MAKS_VERTEX];
    char jalur[MAKS_VERTEX][200];   // Menyimpan jalur sebagai string
    int jumlahVertex = 0;

    struct Vertex *v = headVertex;
    while (v != NULL && jumlahVertex < MAKS_VERTEX) {
        daftarVertex[jumlahVertex] = v;
        jarak[jumlahVertex] = INF;          // Awalnya semua jarak tak terhingga
        sudahDikunjungi[jumlahVertex] = 0; // Belum dikunjungi
        strcpy(jalur[jumlahVertex], v->namaBlok);
        jumlahVertex++;
        v = v->nextVertex;
    }

    // Cari index start
    int idxStart = -1, idxEnd = -1;
    for (int i = 0; i < jumlahVertex; i++) {
        if (strcmp(daftarVertex[i]->namaBlok, start) == 0) idxStart = i;
        if (strcmp(daftarVertex[i]->namaBlok, end) == 0) idxEnd = i;
    }

    if (idxStart == -1 || idxEnd == -1) {
        printf("Error indexing vertex!\n");
        return;
    }

    jarak[idxStart] = 0;    // Jarak ke diri sendiri = 0
    strcpy(jalur[idxStart], start);

    // Algoritma Dijkstra sederhana
    for (int i = 0; i < jumlahVertex; i++) {
        // Cari vertex dengan jarak terkecil yang belum dikunjungi
        int minJarak = INF;
        int idxMin = -1;
        for (int j = 0; j < jumlahVertex; j++) {
            if (!sudahDikunjungi[j] && jarak[j] < minJarak) {
                minJarak = jarak[j];
                idxMin = j;
            }
        }

        if (idxMin == -1) break;    // Semua sudah dikunjungi atau tidak terhubung
        sudahDikunjungi[idxMin] = 1;

        // Update jarak ke tetangga-tetangga
        struct Edge *e = daftarVertex[idxMin]->firstEdge;
        while (e != NULL) {
            // Cari index vertex tujuan
            for (int k = 0; k < jumlahVertex; k++) {
                if (strcmp(daftarVertex[k]->namaBlok, e->blokTujuan) == 0) {
                    int jarakBaru = jarak[idxMin] + e->jarakMeter;
                    if (jarakBaru < jarak[k]) {
                        jarak[k] = jarakBaru;
                        // Update jalur
                        char temp[200];
                        sprintf(temp, "%s -> %s", jalur[idxMin], e->blokTujuan);
                        strcpy(jalur[k], temp);
                    }
                    break;
                }
            }
            e = e->nextEdge;
        }
    }

    if (jarak[idxEnd] == INF) {
        printf("Tidak ada rute dari %s ke %s!\n", start, end);
    } else {
        printf("\n=== RUTE TERPENDEK ===\n");
        printf("Dari %s ke %s\n", start, end);
        printf("Jarak: %d meter\n", jarak[idxEnd]);
        printf("Jalur: %s\n", jalur[idxEnd]);
        printf("======================\n\n");
    }
}

// ============================================================
// ============================================================
// BAGIAN 4: LOGIN & VERIFIKASI (PIMPINAN & PENGURUS)
// ============================================================
// ============================================================

// ============================================================
// Bagian dari materi: 2. FUNCTION
// Verifikasi login pimpinan/pengurus menggunakan kredensial sederhana
// Alur logika: Cek username dan password pengurus di database sederhana
//              (Di produksi nyata, ini akan membaca dari file kredensial)
// ============================================================
int verifikasiLoginPengurus(char username[], char password[]) {
    // Database kredensial sederhana (hardcoded untuk simulasi)
    // Dalam implementasi nyata, ini akan membaca dari file teks terenkripsi
    if (strcmp(username, "pimpinan") == 0 && strcmp(password, "pimpinan123") == 0) {
        printf("Login PIMPINAN berhasil! Selamat datang, Kiyai.\n");
        return 1;   // Role: Pimpinan
    }
    if (strcmp(username, "pengurus") == 0 && strcmp(password, "pengurus123") == 0) {
        printf("Login PENGURUS berhasil! Selamat datang, Pengurus Keamanan.\n");
        return 2;   // Role: Pengurus
    }
    printf("Login gagal! Username atau password salah.\n");
    return 0;       // Gagal login
}

// ============================================================
// ============================================================
// BAGIAN 5: STATISTIK BULANAN
// ============================================================
// ============================================================

// ============================================================
// Bagian dari materi: 2. FUNCTION & 6. SINGLE LINKED LIST
// Menampilkan statistik pelanggaran bulanan
// Alur logika: Hitung berapa pelanggaran terjadi bulan ini dari semua buku riwayat
// ============================================================
void tampilkanStatistikBulanan() {
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);
    int bulanIni = info->tm_mon + 1;
    int tahunIni = info->tm_year + 1900;

    char bulanStr[3];
    sprintf(bulanStr, "%02d", bulanIni);

    int totalPelanggaran = 0;
    int totalPoinBulanan = 0;

    struct NodeSantri *santri = headSantri;
    while (santri != NULL) {
        struct NodeRiwayat *riwayat = santri->headRiwayat;
        while (riwayat != NULL) {
            // Parse bulan dari format DD/MM/YYYY
            char bulanRiwayat[3];
            strncpy(bulanRiwayat, riwayat->data.tanggal + 3, 2);
            bulanRiwayat[2] = '\0';
            char tahunRiwayat[5];
            strncpy(tahunRiwayat, riwayat->data.tanggal + 6, 4);
            tahunRiwayat[4] = '\0';

            if (strcmp(bulanRiwayat, bulanStr) == 0 && atoi(tahunRiwayat) == tahunIni) {
                totalPelanggaran++;
                totalPoinBulanan += riwayat->data.bobotPoin;
            }
            riwayat = riwayat->next;
        }
        santri = santri->next;
    }

    printf("\n=== STATISTIK PELANGGARAN BULAN %s/%d ===\n", bulanStr, tahunIni);
    printf("Total Kejadian Pelanggaran: %d\n", totalPelanggaran);
    printf("Total Poin Dikenakan      : %d\n", totalPoinBulanan);
    printf("=========================================\n\n");
}