// ============================================================
// E-TAKZIR: Modul Kelola Data Santri (DLL + BST + Zona Merah)
// Paradigma: Pemrograman Prosedural Murni (C++)
// Fungsi: Pengelolaan data induk santri menggunakan Double Linked List
//         dan Binary Search Tree untuk pencarian super cepat
// ============================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kerangka_data.h"
#include "kumpulan_fungsi.h"

// ============================================================
// Konstanta Sistem
// ============================================================
#define BATAS_ZONA_MERAH 50   // Ambang batas poin untuk peringatan dini

// ============================================================
// Bagian dari materi: 2. FUNCTION
// Fungsi pembantu internal: membuat node santri baru di memori heap
// Alur logika: Siapkan kartu identitas kosong, isi data, siap disimpan di rak
// ============================================================
struct NodeSantri* buatNodeSantri(struct DataSantri data) {
    struct NodeSantri *baru = (struct NodeSantri*)malloc(sizeof(struct NodeSantri));
    if (baru == NULL) {
        printf("Gagal alokasi memori untuk santri baru!\n");
        return NULL;
    }
    baru->data = data;
    baru->next = NULL;
    baru->prev = NULL;
    baru->headRiwayat = NULL;   // Belum ada riwayat pelanggaran
    return baru;
}

// ============================================================
// Bagian dari materi: 2. FUNCTION
// Inisialisasi tambahan sistem (Queue dan status awal)
// Alur logika: Sebelum buka toko, pastikan loket antrean sudah siap
// ============================================================
void inisialisasiSistem() {
    inisialisasiQueue();        // Siapkan loket antrean pelanggaran
    printf("Sistem E-TAKZIR berhasil diinisialisasi.\n");
}

// ============================================================
// ============================================================
// BAGIAN 1: DOUBLE LINKED LIST NON-CIRCULAR (DATA INDUK SANTRI)
// ============================================================
// ============================================================

// ============================================================
// Bagian dari materi: 7. DOUBLE LINKED LIST NON-CIRCULAR
// Menambah santri baru di akhir list (rak paling belakang)
// Alur logika: Kartu identitas baru diselipkan di rak paling belakang,
//              sekaligus ditanam di pohon pencarian (BST)
// ============================================================
void tambahSantri(struct DataSantri data) {
    struct NodeSantri *baru = buatNodeSantri(data);
    if (baru == NULL) return;

    if (headSantri == NULL) {
        // Rak masih kosong, kartu ini jadi yang pertama dan terakhir
        headSantri = tailSantri = currentSantri = baru;
    } else {
        // Selipkan di belakang kartu terakhir yang ada
        tailSantri->next = baru;
        baru->prev = tailSantri;
        tailSantri = baru;
    }

    // Tanam juga ke BST agar pencarian berdasarkan NIS super cepat
    // Alur logika: Selain disimpan di rak, kartu ini juga ditancapkan di pohon
    insertBST(&rootBST, data.nis, data, NULL);

    printf("Santri %s (NIS: %d) berhasil ditambahkan.\n", data.nama, data.nis);
}

// ============================================================
// Bagian dari materi: 7. DOUBLE LINKED LIST NON-CIRCULAR
// Menghapus santri berdasarkan NIS dari DLL dan BST
// Alur logika: Buang kartu identitas dari rak, rapikan sisa kartu di sekitarnya,
//              lalu buang juga dari pohon pencarian (BST)
// ============================================================
void hapusSantri(int nis) {
    struct NodeSantri *cari = headSantri;

    // Cari kartu identitas yang mau dibuang
    while (cari != NULL && cari->data.nis != nis) {
        cari = cari->next;
    }

    if (cari == NULL) {
        printf("Santri dengan NIS %d tidak ditemukan!\n", nis);
        return;
    }

    // Rapikan pointer next dan prev di sekitar node yang akan dibuang
    if (cari->prev != NULL) {
        cari->prev->next = cari->next;
    } else {
        headSantri = cari->next;    // Yang dibuang adalah kartu pertama di rak
    }

    if (cari->next != NULL) {
        cari->next->prev = cari->prev;
    } else {
        tailSantri = cari->prev;    // Yang dibuang adalah kartu terakhir di rak
    }

    // Kalau pointer current sedang menunjuk ke kartu yang dibuang, pindahkan
    if (currentSantri == cari) {
        currentSantri = (cari->next != NULL) ? cari->next : cari->prev;
    }

    // Bersihkan riwayat pelanggaran santri ini dulu (free SLL)
    struct NodeRiwayat *riwayat = cari->headRiwayat;
    while (riwayat != NULL) {
        struct NodeRiwayat *temp = riwayat;
        riwayat = riwayat->next;
        free(temp);
    }

    free(cari);     // Buang kartu identitas dari memori

    // Buang juga dari pohon BST
    hapusNodeBST(&rootBST, nis);

    printf("Santri dengan NIS %d berhasil dihapus dari sistem.\n", nis);
}

// ============================================================
// Bagian dari materi: 7. DOUBLE LINKED LIST NON-CIRCULAR
// Mencari santri dengan pencarian linear (cek satu per satu dari depan rak)
// Alur logika: Cek kartu identitas dari rak depan sampai belakang
// ============================================================
struct NodeSantri* cariLinearSantri(int nis) {
    struct NodeSantri *current = headSantri;
    while (current != NULL) {
        if (current->data.nis == nis) {
            return current;     // Ketemu! Kartu identitas ditemukan di rak
        }
        current = current->next;
    }
    return NULL;    // Sampai habis tidak ketemu
}

// ============================================================
// Bagian dari materi: 7. DOUBLE LINKED LIST NON-CIRCULAR
// Navigasi ke santri berikutnya (tombol Next di Qt)
// Alur logika: Geser jari ke kartu identitas berikutnya di rak
// ============================================================
struct NodeSantri* navigasiNext() {
    if (currentSantri != NULL && currentSantri->next != NULL) {
        currentSantri = currentSantri->next;
    }
    return currentSantri;
}

// ============================================================
// Bagian dari materi: 7. DOUBLE LINKED LIST NON-CIRCULAR
// Navigasi ke santri sebelumnya (tombol Previous di Qt)
// Alur logika: Geser jari ke kartu identitas sebelumnya di rak
// ============================================================
struct NodeSantri* navigasiPrev() {
    if (currentSantri != NULL && currentSantri->prev != NULL) {
        currentSantri = currentSantri->prev;
    }
    return currentSantri;
}

// ============================================================
// Bagian dari materi: 7. DOUBLE LINKED LIST NON-CIRCULAR
// Mengupdate data santri berdasarkan NIS
// Alur logika: Ganti isi kartu identitas yang sudah ada di rak,
//              lalu update juga di pohon pencarian (BST)
// ============================================================
void updateSantri(int nis, struct DataSantri dataBaru) {
    struct NodeSantri *cari = cariLinearSantri(nis);
    if (cari == NULL) {
        printf("Santri dengan NIS %d tidak ditemukan untuk diupdate!\n", nis);
        return;
    }

    // Update data di DLL (rak kartu identitas)
    cari->data = dataBaru;

    // Update juga di BST agar data sinkron
    struct NodeBST *nodeBST = cariNisBST(rootBST, nis);
    if (nodeBST != NULL) {
        nodeBST->data = dataBaru;
    }

    printf("Data santri NIS %d berhasil diupdate.\n", nis);
}

// ============================================================
// Bagian dari materi: 7. DOUBLE LINKED LIST NON-CIRCULAR
// Menampilkan seluruh data santri dari depan sampai belakang rak
// Alur logika: Tampilkan semua kartu identitas di rak satu per satu
// ============================================================
void tampilkanSemuaSantri() {
    struct NodeSantri *current = headSantri;
    printf("\n=== DAFTAR SELURUH SANTRI ===\n");
    printf("%-8s %-20s %-12s %-10s %-8s\n", "NIS", "NAMA", "KELAS", "KAMAR", "POIN");
    printf("------------------------------------------------------------\n");
    while (current != NULL) {
        printf("%-8d %-20s %-12s %-10s %-8d\n",
               current->data.nis,
               current->data.nama,
               current->data.kelas,
               current->data.kamar,
               current->data.totalPoin);
        current = current->next;
    }
    printf("=============================\n\n");
}

// ============================================================
// Bagian dari materi: 7. DOUBLE LINKED LIST NON-CIRCULAR
// Menampilkan daftar santri yang masuk Zona Merah (poin tinggi)
// Alur logika: Cek semua kartu identitas di rak, tandai yang poinnya sudah bahaya
// ============================================================
void tampilkanZonaMerah() {
    struct NodeSantri *current = headSantri;
    printf("\n=== DAFTAR SANTRI ZONA MERAH (POIN >= %d) ===\n", BATAS_ZONA_MERAH);
    int ada = 0;
    while (current != NULL) {
        if (current->data.totalPoin >= BATAS_ZONA_MERAH) {
            printf("!!! ZONA MERAH -> NIS: %-6d | Nama: %-20s | Poin: %d !!!\n",
                   current->data.nis, current->data.nama, current->data.totalPoin);
            ada = 1;
        }
        current = current->next;
    }
    if (!ada) {
        printf("Alhamdulillah, tidak ada santri di zona merah.\n");
    }
    printf("=============================================\n\n");
}

// ============================================================
// Bagian dari materi: 7. DOUBLE LINKED LIST NON-CIRCULAR
// Reset seluruh poin santri ke 0 (Pemutihan akhir semester)
// Alur logika: Hapus semua angka poin di kartu identitas,
//              mulai semester baru dengan bersih
// ============================================================
void resetTotalPoin() {
    struct NodeSantri *current = headSantri;
    while (current != NULL) {
        current->data.totalPoin = 0;

        // Update juga di BST agar sinkron
        struct NodeBST *nodeBST = cariNisBST(rootBST, current->data.nis);
        if (nodeBST != NULL) {
            nodeBST->data.totalPoin = 0;
        }
        current = current->next;
    }
    printf("Semua poin santri telah direset ke 0 (Pemutihan Semester).\n");
}

// ============================================================
// Bagian dari materi: 7. DOUBLE LINKED LIST NON-CIRCULAR
// Cek apakah satu santri masuk Zona Merah setelah ditambah poin
// Alur logika: Lihat kartu identitas santri, kalau poin sudah bahaya beri peringatan
// ============================================================
void cekZonaMerah(int nis) {
    struct NodeSantri *santri = cariLinearSantri(nis);
    if (santri == NULL) return;

    if (santri->data.totalPoin >= BATAS_ZONA_MERAH) {
        printf("\n>>> PERINGATAN DINI: Santri %s (NIS: %d) masuk ZONA MERAH dengan poin %d! <<<\n\n",
               santri->data.nama, nis, santri->data.totalPoin);
    }
}

// ============================================================
// ============================================================
// BAGIAN 2: TREE (BST) - MESIN PENCARI BERDASARKAN NIS
// ============================================================
// ============================================================

// ============================================================
// Bagian dari materi: 10. TREE (BST)
// Menambah node santri ke BST berdasarkan NIS
// Alur logika: Tanam kartu identitas di pohon, posisi tergantung nomor NIS:
//              NIS lebih kecil ke cabang kiri, lebih besar ke cabang kanan
// ============================================================
void insertBST(struct NodeBST **root, int nis, struct DataSantri data, struct NodeRiwayat *headRiwayat) {
    if (*root == NULL) {
        // Pohon masih kosong di posisi ini, tanam kartu baru
        struct NodeBST *baru = (struct NodeBST*)malloc(sizeof(struct NodeBST));
        if (baru == NULL) return;
        baru->nis = nis;
        baru->data = data;
        baru->headRiwayat = headRiwayat;
        baru->left = NULL;
        baru->right = NULL;
        *root = baru;
        return;
    }

    if (nis < (*root)->nis) {
        // NIS lebih kecil, belok ke cabang kiri pohon
        insertBST(&((*root)->left), nis, data, headRiwayat);
    } else if (nis > (*root)->nis) {
        // NIS lebih besar, belok ke cabang kanan pohon
        insertBST(&((*root)->right), nis, data, headRiwayat);
    } else {
        // NIS sama, update data yang sudah ada (tidak boleh duplikat)
        (*root)->data = data;
        (*root)->headRiwayat = headRiwayat;
    }
}

// ============================================================
// Bagian dari materi: 10. TREE (BST)
// Mencari santri berdasarkan NIS dengan cepat (O(log n))
// Alur logika: Cari kartu identitas di pohon, belok kiri kalau NIS lebih kecil,
//              belok kanan kalau NIS lebih besar, sangat cepat!
// ============================================================
struct NodeBST* cariNisBST(struct NodeBST *root, int nis) {
    if (root == NULL) {
        return NULL;    // Pohon kosong atau sampai ujung, tidak ketemu
    }

    if (nis == root->nis) {
        return root;    // Ketemu! Kartu identitas ditemukan di pohon
    } else if (nis < root->nis) {
        return cariNisBST(root->left, nis);     // Cari di cabang kiri (nomor kecil)
    } else {
        return cariNisBST(root->right, nis);    // Cari di cabang kanan (nomor besar)
    }
}

// ============================================================
// Bagian dari materi: 10. TREE (BST)
// Fungsi pembantu: cari node minimum di subtree kanan (successor)
// Digunakan saat hapus node yang punya dua anak
// ============================================================
struct NodeBST* cariMinimumBST(struct NodeBST *root) {
    while (root->left != NULL) {
        root = root->left;      // Terus ke kiri sampai yang paling kecil
    }
    return root;
}

// ============================================================
// Bagian dari materi: 10. TREE (BST)
// Menghapus node dari BST berdasarkan NIS
// Alur logika: Buang kartu dari pohon, rapikan cabang-cabang yang tersisa
// ============================================================
void hapusNodeBST(struct NodeBST **root, int nis) {
    if (*root == NULL) return;  // Pohon kosong, tidak ada yang dibuang

    if (nis < (*root)->nis) {
        hapusNodeBST(&((*root)->left), nis);    // Cari di cabang kiri
    } else if (nis > (*root)->nis) {
        hapusNodeBST(&((*root)->right), nis);   // Cari di cabang kanan
    } else {
        // Ketemu node yang mau dibuang
        struct NodeBST *temp = *root;

        if ((*root)->left == NULL) {
            // Hanya punya anak kanan, gantikan dengan anak kanan
            *root = (*root)->right;
            free(temp);
        } else if ((*root)->right == NULL) {
            // Hanya punya anak kiri, gantikan dengan anak kiri
            *root = (*root)->left;
            free(temp);
        } else {
            // Punya dua anak, cari pengganti (successor) dari cabang kanan
            struct NodeBST *successor = cariMinimumBST((*root)->right);
            (*root)->nis = successor->nis;
            (*root)->data = successor->data;
            (*root)->headRiwayat = successor->headRiwayat;
            hapusNodeBST(&((*root)->right), successor->nis);
        }
    }
}

// ============================================================
// Bagian dari materi: 10. TREE (BST)
// Traversal inorder: kiri - akar - kanan, hasil urutan NIS terurut naik
// Alur logika: Baca kartu identitas dari nomor kecil ke besar
// ============================================================
void inorderTraversal(struct NodeBST *root) {
    if (root == NULL) return;

    inorderTraversal(root->left);       // Baca cabang kiri dulu (nomor kecil)
    printf("NIS: %-6d | Nama: %-20s | Kelas: %-10s | Poin: %d\n",
           root->nis, root->data.nama, root->data.kelas, root->data.totalPoin);
    inorderTraversal(root->right);      // Baca cabang kanan (nomor besar)
}

// Forward declaration untuk menghindari error scope
void hapusMemoriBST(struct NodeBST *root);

// ============================================================
// Bagian dari materi: 10. TREE (BST) & 7. DOUBLE LINKED LIST
// Membangun ulang BST dari seluruh data di DLL
// Alur logika: Ambil semua kartu dari rak, tanam ulang ke pohon pencarian
//              (digunakan setelah operasi besar yang membuat pohon tidak sinkron)
// ============================================================
void bangunBSTdariDLL() {
    // Hancurkan pohon lama dulu
    hapusMemoriBST(rootBST);
    rootBST = NULL;

    struct NodeSantri *current = headSantri;
    while (current != NULL) {
        insertBST(&rootBST, current->data.nis, current->data, current->headRiwayat);
        current = current->next;
    }
    printf("BST berhasil dibangun ulang dari data DLL.\n");
}

// ============================================================
// Bagian dari materi: 10. TREE (BST)
// Verifikasi login santri menggunakan NIS via BST (super cepat)
// Alur logika: Cek apakah nomor NIS yang dimasukkan ada di database pohon kartu identitas
// ============================================================
struct NodeBST* verifikasiLoginSantri(int nis) {
    struct NodeBST *hasil = cariNisBST(rootBST, nis);
    if (hasil != NULL) {
        printf("Login santri berhasil: %s (NIS: %d)\n", hasil->data.nama, nis);
    } else {
        printf("Login gagal: NIS %d tidak ditemukan di database.\n", nis);
    }
    return hasil;
}

// ============================================================
// ============================================================
// BAGIAN 3: PEMBERSIHAN MEMORI (FREE POINTER & LINKED LIST)
// ============================================================
// ============================================================

// ============================================================
// Bagian dari materi: 3. POINTER & 10. TREE (BST)
// Membersihkan memori BST secara rekursif (post-order)
// Alur logika: Buang semua kartu identitas dari pohon, cabang demi cabang
// ============================================================
void hapusMemoriBST(struct NodeBST *root) {
    if (root == NULL) return;
    hapusMemoriBST(root->left);     // Bersihkan cabang kiri dulu
    hapusMemoriBST(root->right);    // Bersihkan cabang kanan
    free(root);                     // Baru buang akar (induk)
}

// ============================================================
// Bagian dari materi: 3. POINTER & 2. FUNCTION
// Membersihkan semua memori yang teralokasi di heap saat logout/exit
// Alur logika: Sebelum tutup toko, bersihkan semua rak, pohon, tumpukan, dan peta
// ============================================================
void hapusMemori() {
    // 1. Bersihkan Double Linked List Santri (termasuk riwayat SLL di dalamnya)
    struct NodeSantri *cs = headSantri;
    while (cs != NULL) {
        struct NodeSantri *ts = cs;

        // Bersihkan buku riwayat pelanggaran santri ini (SLL)
        struct NodeRiwayat *riwayat = cs->headRiwayat;
        while (riwayat != NULL) {
            struct NodeRiwayat *tr = riwayat;
            riwayat = riwayat->next;
            free(tr);
        }

        cs = cs->next;
        free(ts);
    }
    headSantri = tailSantri = currentSantri = NULL;

    // 2. Bersihkan BST (Pohon Pencarian)
    hapusMemoriBST(rootBST);
    rootBST = NULL;

    // 3. Bersihkan Double Linked List Circular Aturan
    if (headAturan != NULL) {
        struct NodeAturan *ca = headAturan;
        struct NodeAturan *awal = headAturan;
        do {
            struct NodeAturan *ta = ca;
            ca = ca->next;
            free(ta);
        } while (ca != awal);
        headAturan = tailAturan = currentAturan = NULL;
    }

    // 4. Bersihkan Single Linked List Circular Piket
    if (headPiket != NULL) {
        struct NodePiket *cp = headPiket;
        struct NodePiket *awalP = headPiket;
        do {
            struct NodePiket *tp = cp;
            cp = cp->next;
            free(tp);
        } while (cp != awalP);
        headPiket = currentPiket = NULL;
    }

    // 5. Bersihkan Graph (Vertex & Edge)
    struct Vertex *cv = headVertex;
    while (cv != NULL) {
        struct Edge *ce = cv->firstEdge;
        while (ce != NULL) {
            struct Edge *te = ce;
            ce = ce->nextEdge;
            free(te);
        }
        struct Vertex *tv = cv;
        cv = cv->nextVertex;
        free(tv);
    }
    headVertex = NULL;

    // 6. Bersihkan Queue (Antrean Pelanggaran)
    if (antreanPelanggaran != NULL) {
        struct NodeAntrean *cq = antreanPelanggaran->front;
        while (cq != NULL) {
            struct NodeAntrean *tq = cq;
            cq = cq->next;
            free(tq);
        }
        free(antreanPelanggaran);
        antreanPelanggaran = NULL;
    }

    // 7. Bersihkan Stack Undo (memanggil fungsi dari modul proses_pelanggaran)
    resetUndo();

    printf("Semua memori telah dibersihkan. Sistem siap dimatikan.\n");
}