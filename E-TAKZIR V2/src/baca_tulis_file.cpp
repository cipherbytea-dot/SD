// ============================================================
// E-TAKZIR: Modul Baca Tulis File Teks (.txt)
// Paradigma: Pemrograman Prosedural Murni (C++)
// Fungsi: Jembatan antara memori (struct & pointer) dengan file database lokal
// MODIFIKASI: Auto-resolve path database (working dir -> parent dir)
// ============================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kerangka_data.h"
#include "kumpulan_fungsi.h"

// ============================================================
// Bagian dari materi: 3. POINTER
// Definisi variabel global pointer yang dideklarasikan extern di header
// ============================================================
struct NodeSantri *headSantri = NULL;
struct NodeSantri *tailSantri = NULL;
struct NodeSantri *currentSantri = NULL;

struct NodeAturan *headAturan = NULL;
struct NodeAturan *tailAturan = NULL;
struct NodeAturan *currentAturan = NULL;

struct NodePiket *headPiket = NULL;
struct NodePiket *currentPiket = NULL;

struct NodeBST *rootBST = NULL;

struct Vertex *headVertex = NULL;

struct NodeUndo *topUndo = NULL;

struct Queue *antreanPelanggaran = NULL;

int idPelanggaranTerakhir = 0;

// ============================================================
// HELPER: Auto-resolve path database
// Coba working directory dulu, kalau gagal coba parent directory
// Alur logika: Cek laci arsip di meja kerja, kalau tidak ada cek di ruang depan
// ============================================================
FILE* bukaFileDatabase(const char* namaFile, const char* mode) {
    FILE* file = fopen(namaFile, mode);
    if (file != NULL) {
        return file;
    }
    // Kalau tidak ketemu, coba di parent directory (../database/xxx.txt)
    char pathParent[200];
    sprintf(pathParent, "../%s", namaFile);
    file = fopen(pathParent, mode);
    return file;
}

// ============================================================
// Bagian dari materi: 2. FUNCTION
// Fungsi pembantu untuk memecah baris teks berdasarkan tanda pemisah "|"
// ============================================================
void pecahString(char baris[], char hasil[][100], int *jumlah) {
    *jumlah = 0;
    char *token = strtok(baris, "|");
    while (token != NULL) {
        strcpy(hasil[*jumlah], token);
        (*jumlah)++;
        token = strtok(NULL, "|");
    }
}

// ============================================================
// Fungsi untuk membersihkan karakter newline (\n) dari akhir string
// ============================================================
void bersihkanNewline(char str[]) {
    int panjang = strlen(str);
    if (panjang > 0 && str[panjang - 1] == '\n') {
        str[panjang - 1] = '\0';
    }
    if (panjang > 1 && str[panjang - 2] == '\r') {
        str[panjang - 2] = '\0';
    }
}

// ============================================================
// Bagian dari materi: 2. FUNCTION & 7. DOUBLE LINKED LIST NON-CIRCULAR
// Membaca file santri.txt dan membangun Double Linked List + BST di memori
// ============================================================
void bacaDataSantri() {
    FILE *file = bukaFileDatabase("database/santri.txt", "r");
    if (file == NULL) {
        printf("File santri.txt tidak ditemukan, memulai dengan database kosong.\n");
        return;
    }

    char baris[200];
    char potongan[5][100];
    int jumlahPotongan;

    while (fgets(baris, sizeof(baris), file) != NULL) {
        bersihkanNewline(baris);
        if (strlen(baris) < 3) continue;

        pecahString(baris, potongan, &jumlahPotongan);
        if (jumlahPotongan < 5) continue;

        struct NodeSantri *baru = (struct NodeSantri *)malloc(sizeof(struct NodeSantri));
        if (baru == NULL) continue;

        baru->data.nis = atoi(potongan[0]);
        strcpy(baru->data.nama, potongan[1]);
        strcpy(baru->data.kelas, potongan[2]);
        strcpy(baru->data.kamar, potongan[3]);
        baru->data.totalPoin = atoi(potongan[4]);
        baru->headRiwayat = NULL;
        baru->next = NULL;
        baru->prev = NULL;

        if (headSantri == NULL) {
            headSantri = tailSantri = baru;
        } else {
            tailSantri->next = baru;
            baru->prev = tailSantri;
            tailSantri = baru;
        }

        insertBST(&rootBST, baru->data.nis, baru->data, NULL);
    }

    fclose(file);
    printf("Data santri berhasil dimuat dari file.\n");
}

// ============================================================
// Bagian dari materi: 2. FUNCTION & 9. DOUBLE LINKED LIST CIRCULAR
// Membaca file master_pelanggaran.txt dan membangun DLL Circular
// ============================================================
void bacaDataMaster() {
    FILE *file = bukaFileDatabase("database/master_pelanggaran.txt", "r");
    if (file == NULL) {
        printf("File master_pelanggaran.txt tidak ditemukan, memulai dengan master kosong.\n");
        return;
    }

    char baris[200];
    char potongan[3][100];
    int jumlahPotongan;

    while (fgets(baris, sizeof(baris), file) != NULL) {
        bersihkanNewline(baris);
        if (strlen(baris) < 3) continue;

        pecahString(baris, potongan, &jumlahPotongan);
        if (jumlahPotongan < 3) continue;

        struct NodeAturan *baru = (struct NodeAturan *)malloc(sizeof(struct NodeAturan));
        if (baru == NULL) continue;

        baru->data.kodePelanggaran = atoi(potongan[0]);
        strcpy(baru->data.namaPelanggaran, potongan[1]);
        baru->data.bobotPoin = atoi(potongan[2]);
        baru->next = NULL;
        baru->prev = NULL;

        if (headAturan == NULL) {
            headAturan = tailAturan = baru;
            baru->next = baru;
            baru->prev = baru;
        } else {
            tailAturan->next = baru;
            baru->prev = tailAturan;
            baru->next = headAturan;
            headAturan->prev = baru;
            tailAturan = baru;
        }
    }

    currentAturan = headAturan;
    fclose(file);
    printf("Data master pelanggaran berhasil dimuat dari file.\n");
}

// ============================================================
// Bagian dari materi: 2. FUNCTION & 6. SINGLE LINKED LIST NON-CIRCULAR
// Membaca file riwayat_pelanggaran.txt dan menyambungkan ke masing-masing santri
// ============================================================
void bacaRiwayatPelanggaran() {
    FILE *file = bukaFileDatabase("database/riwayat_pelanggaran.txt", "r");
    if (file == NULL) {
        printf("File riwayat_pelanggaran.txt tidak ditemukan.\n");
        return;
    }

    char baris[300];
    char potongan[5][100];
    int jumlahPotongan;

    while (fgets(baris, sizeof(baris), file) != NULL) {
        bersihkanNewline(baris);
        if (strlen(baris) < 3) continue;

        pecahString(baris, potongan, &jumlahPotongan);
        if (jumlahPotongan < 5) continue;

        int id = atoi(potongan[0]);
        int nis = atoi(potongan[1]);

        if (id > idPelanggaranTerakhir) {
            idPelanggaranTerakhir = id;
        }

        struct NodeRiwayat *baru = (struct NodeRiwayat *)malloc(sizeof(struct NodeRiwayat));
        if (baru == NULL) continue;

        baru->data.idPelanggaran = id;
        baru->data.nisSantri = nis;
        strcpy(baru->data.jenis, potongan[2]);
        strcpy(baru->data.tanggal, potongan[3]);
        strcpy(baru->data.keterangan, potongan[4]);

        struct NodeAturan *aturan = headAturan;
        int bobot = 0;
        if (aturan != NULL) {
            do {
                if (strcmp(aturan->data.namaPelanggaran, baru->data.jenis) == 0) {
                    bobot = aturan->data.bobotPoin;
                    break;
                }
                aturan = aturan->next;
            } while (aturan != headAturan);
        }
        baru->data.bobotPoin = bobot;
        baru->next = NULL;

        struct NodeSantri *cari = headSantri;
        while (cari != NULL) {
            if (cari->data.nis == nis) {
                if (cari->headRiwayat == NULL) {
                    cari->headRiwayat = baru;
                } else {
                    struct NodeRiwayat *akhir = cari->headRiwayat;
                    while (akhir->next != NULL) {
                        akhir = akhir->next;
                    }
                    akhir->next = baru;
                }
                break;
            }
            cari = cari->next;
        }
    }

    fclose(file);
    printf("Data riwayat pelanggaran berhasil dimuat dari file.\n");
}

// ============================================================
// Bagian dari materi: 2. FUNCTION & 8. SINGLE LINKED LIST CIRCULAR
// Membaca file piket.txt dan membangun SLL Circular (Jadwal Piket)
// ============================================================
void bacaJadwalPiket() {
    FILE *file = bukaFileDatabase("database/piket.txt", "r");
    if (file == NULL) {
        printf("File piket.txt tidak ditemukan.\n");
        return;
    }

    char baris[200];
    char potongan[3][100];
    int jumlahPotongan;

    while (fgets(baris, sizeof(baris), file) != NULL) {
        bersihkanNewline(baris);
        if (strlen(baris) < 3) continue;

        pecahString(baris, potongan, &jumlahPotongan);
        if (jumlahPotongan < 3) continue;

        struct NodePiket *baru = (struct NodePiket *)malloc(sizeof(struct NodePiket));
        if (baru == NULL) continue;

        strcpy(baru->data.namaPengurus, potongan[0]);
        strcpy(baru->data.hari, potongan[1]);
        strcpy(baru->data.shift, potongan[2]);
        baru->next = NULL;

        if (headPiket == NULL) {
            headPiket = baru;
            baru->next = baru;
        } else {
            struct NodePiket *akhir = headPiket;
            while (akhir->next != headPiket) {
                akhir = akhir->next;
            }
            akhir->next = baru;
            baru->next = headPiket;
        }
    }

    currentPiket = headPiket;
    fclose(file);
    printf("Data jadwal piket berhasil dimuat dari file.\n");
}

// ============================================================
// Bagian dari materi: 2. FUNCTION & 11. GRAPH
// Membaca file rute.txt dan membangun Graph (Vertex + Edge)
// ============================================================
void bacaPetaRute() {
    FILE *file = bukaFileDatabase("database/rute.txt", "r");
    if (file == NULL) {
        printf("File rute.txt tidak ditemukan.\n");
        return;
    }

    char baris[200];
    char potongan[3][100];
    int jumlahPotongan;

    while (fgets(baris, sizeof(baris), file) != NULL) {
        bersihkanNewline(baris);
        if (strlen(baris) < 3) continue;

        pecahString(baris, potongan, &jumlahPotongan);
        if (jumlahPotongan < 3) continue;

        char blokAsal[10], blokTujuan[10];
        int jarak;
        strcpy(blokAsal, potongan[0]);
        strcpy(blokTujuan, potongan[1]);
        jarak = atoi(potongan[2]);

        struct Vertex *vAsal = cariVertex(blokAsal);
        if (vAsal == NULL) {
            vAsal = (struct Vertex *)malloc(sizeof(struct Vertex));
            if (vAsal == NULL) continue;
            strcpy(vAsal->namaBlok, blokAsal);
            strcpy(vAsal->namaKamar, "");
            vAsal->firstEdge = NULL;
            vAsal->nextVertex = NULL;

            if (headVertex == NULL) {
                headVertex = vAsal;
            } else {
                struct Vertex *temp = headVertex;
                while (temp->nextVertex != NULL) {
                    temp = temp->nextVertex;
                }
                temp->nextVertex = vAsal;
            }
        }

        struct Vertex *vTujuan = cariVertex(blokTujuan);
        if (vTujuan == NULL) {
            vTujuan = (struct Vertex *)malloc(sizeof(struct Vertex));
            if (vTujuan == NULL) continue;
            strcpy(vTujuan->namaBlok, blokTujuan);
            strcpy(vTujuan->namaKamar, "");
            vTujuan->firstEdge = NULL;
            vTujuan->nextVertex = NULL;

            if (headVertex == NULL) {
                headVertex = vTujuan;
            } else {
                struct Vertex *temp = headVertex;
                while (temp->nextVertex != NULL) {
                    temp = temp->nextVertex;
                }
                temp->nextVertex = vTujuan;
            }
        }

        struct Edge *edgeBaru = (struct Edge *)malloc(sizeof(struct Edge));
        if (edgeBaru == NULL) continue;
        strcpy(edgeBaru->blokTujuan, blokTujuan);
        edgeBaru->jarakMeter = jarak;
        edgeBaru->nextEdge = NULL;

        if (vAsal->firstEdge == NULL) {
            vAsal->firstEdge = edgeBaru;
        } else {
            struct Edge *temp = vAsal->firstEdge;
            while (temp->nextEdge != NULL) {
                temp = temp->nextEdge;
            }
            temp->nextEdge = edgeBaru;
        }
    }

    fclose(file);
    printf("Data peta rute berhasil dimuat dari file.\n");
}

// ============================================================
// Fungsi utama pembaca: memanggil semua fungsi baca file
// ============================================================
void bacaDariTeks() {
    printf("=== MEMUAT DATA DARI FILE TEKS ===\n");

    bacaDataSantri();
    bacaDataMaster();
    bacaRiwayatPelanggaran();
    bacaJadwalPiket();
    bacaPetaRute();

    printf("=== SEMUA DATA BERHASIL DIMUAT ===\n");
}

// ============================================================
// Bagian dari materi: 2. FUNCTION & 7. DOUBLE LINKED LIST NON-CIRCULAR
// Menyimpan data santri dari DLL ke file santri.txt
// ============================================================
void simpanDataSantri() {
    FILE *file = bukaFileDatabase("database/santri.txt", "w");
    if (file == NULL) {
        printf("Gagal membuka file santri.txt untuk menyimpan!\n");
        return;
    }

    struct NodeSantri *current = headSantri;
    while (current != NULL) {
        fprintf(file, "%d|%s|%s|%s|%d\n",
                current->data.nis,
                current->data.nama,
                current->data.kelas,
                current->data.kamar,
                current->data.totalPoin);
        current = current->next;
    }

    fclose(file);
    printf("Data santri berhasil disimpan ke file.\n");
}

// ============================================================
// Bagian dari materi: 2. FUNCTION & 9. DOUBLE LINKED LIST CIRCULAR
// Menyimpan data master pelanggaran dari DLL Circular ke file
// ============================================================
void simpanDataMaster() {
    FILE *file = bukaFileDatabase("database/master_pelanggaran.txt", "w");
    if (file == NULL) {
        printf("Gagal membuka file master_pelanggaran.txt untuk menyimpan!\n");
        return;
    }

    if (headAturan != NULL) {
        struct NodeAturan *current = headAturan;
        do {
            fprintf(file, "%d|%s|%d\n",
                    current->data.kodePelanggaran,
                    current->data.namaPelanggaran,
                    current->data.bobotPoin);
            current = current->next;
        } while (current != headAturan);
    }

    fclose(file);
    printf("Data master pelanggaran berhasil disimpan ke file.\n");
}

// ============================================================
// Bagian dari materi: 2. FUNCTION & 6. SINGLE LINKED LIST NON-CIRCULAR
// Menyimpan seluruh riwayat pelanggaran dari semua santri ke file
// ============================================================
void simpanRiwayatPelanggaran() {
    FILE *file = bukaFileDatabase("database/riwayat_pelanggaran.txt", "w");
    if (file == NULL) {
        printf("Gagal membuka file riwayat_pelanggaran.txt untuk menyimpan!\n");
        return;
    }

    struct NodeSantri *santri = headSantri;
    while (santri != NULL) {
        struct NodeRiwayat *riwayat = santri->headRiwayat;
        while (riwayat != NULL) {
            fprintf(file, "%d|%d|%s|%s|%s\n",
                    riwayat->data.idPelanggaran,
                    riwayat->data.nisSantri,
                    riwayat->data.jenis,
                    riwayat->data.tanggal,
                    riwayat->data.keterangan);
            riwayat = riwayat->next;
        }
        santri = santri->next;
    }

    fclose(file);
    printf("Data riwayat pelanggaran berhasil disimpan ke file.\n");
}

// ============================================================
// Bagian dari materi: 2. FUNCTION & 8. SINGLE LINKED LIST CIRCULAR
// Menyimpan jadwal piket dari SLL Circular ke file
// ============================================================
void simpanJadwalPiket() {
    FILE *file = bukaFileDatabase("database/piket.txt", "w");
    if (file == NULL) {
        printf("Gagal membuka file piket.txt untuk menyimpan!\n");
        return;
    }

    if (headPiket != NULL) {
        struct NodePiket *current = headPiket;
        do {
            fprintf(file, "%s|%s|%s\n",
                    current->data.namaPengurus,
                    current->data.hari,
                    current->data.shift);
            current = current->next;
        } while (current != headPiket);
    }

    fclose(file);
    printf("Data jadwal piket berhasil disimpan ke file.\n");
}

// ============================================================
// Bagian dari materi: 2. FUNCTION & 11. GRAPH
// Menyimpan peta rute dari Graph ke file
// ============================================================
void simpanPetaRute() {
    FILE *file = bukaFileDatabase("database/rute.txt", "w");
    if (file == NULL) {
        printf("Gagal membuka file rute.txt untuk menyimpan!\n");
        return;
    }

    struct Vertex *v = headVertex;
    while (v != NULL) {
        struct Edge *e = v->firstEdge;
        while (e != NULL) {
            fprintf(file, "%s|%s|%d\n",
                    v->namaBlok,
                    e->blokTujuan,
                    e->jarakMeter);
            e = e->nextEdge;
        }
        v = v->nextVertex;
    }

    fclose(file);
    printf("Data peta rute berhasil disimpan ke file.\n");
}

// ============================================================
// Fungsi utama penyimpanan: memanggil semua fungsi simpan file
// ============================================================
void simpanKeTeks() {
    printf("=== MENYIMPAN DATA KE FILE TEKS ===\n");

    simpanDataSantri();
    simpanDataMaster();
    simpanRiwayatPelanggaran();
    simpanJadwalPiket();
    simpanPetaRute();

    printf("=== SEMUA DATA BERHASIL DISIMPAN ===\n");
}