// ============================================================
// E-TAKZIR: Entry Point Aplikasi Qt (QApplication)
// Paradigma: Pemrograman Prosedural Murni untuk Logika Bisnis
//            Qt Framework untuk Antarmuka Grafis Desktop
// ============================================================

#include <QApplication>
#include "antarmuka_utama.h"

// ============================================================
// Bagian dari materi: 2. FUNCTION (Entry Point)
// Fungsi main() yang sesungguhnya untuk aplikasi Qt Desktop.
// Alur logika: Siapkan mesin Qt (QApplication) -> Bangun jendela utama
//              -> Tampilkan jendela -> Jalankan loop event Qt.
// ============================================================
int main(int argc, char *argv[]) {
    // Inisialisasi mesin Qt (seperti menyalakan mesin mobil)
    QApplication aplikasi(argc, argv);

    // Atur informasi aplikasi
    aplikasi.setApplicationName("E-TAKZIR");
    aplikasi.setOrganizationName("PondokPesantren");

    // Bangun jendela utama aplikasi (seperti membuka toko)
    AntarmukaUtama jendelaUtama;
    jendelaUtama.show();    // Tampilkan jendela ke layar desktop

    // Jalankan loop event Qt (seperti menyalakan mesin toko agar siap melayani)
    // Loop ini akan berjalan terus sampai user menutup jendela utama
    return aplikasi.exec();
}