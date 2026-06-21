// ============================================================
// E-TAKZIR V2: Implementasi Antarmuka Grafis Qt (GUI)
// Penambahan: Update Santri + Cari Santri BST (Pengurus)
// FIX: Validasi NIS tidak ditemukan sebelum eksekusi operasi
// UI RESTORED: Tombol Jadwal Piket & Peta Rute di Pimpinan & Pengurus
// ============================================================

#include "antarmuka_utama.h"
#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTableWidget>
#include <QTextEdit>
#include <QMessageBox>
#include <QGroupBox>
#include <QComboBox>
#include <QSpinBox>
#include <QHeaderView>
#include <QSplitter>
#include <QScrollArea>
#include <QFrame>
#include <QFont>
#include <QPalette>
#include <QColor>
#include <QScreen>
#include <QGuiApplication>
#include <QSizePolicy>
#include <QIntValidator>

#include "kerangka_data.h"
#include "kumpulan_fungsi.h"

AntarmukaUtama::AntarmukaUtama(QWidget *parent) : QMainWindow(parent), dataSantriLogin(nullptr) {
    setWindowTitle("E-TAKZIR - Sistem Peringatan Dini Pelanggaran Santri");
    setMinimumSize(900, 600);

    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect available = screen->availableGeometry();
        int lebarAwal = qMin(1200, available.width() - 80);
        int tinggiAwal = qMin(800, available.height() - 80);
        resize(lebarAwal, tinggiAwal);
        move(available.x() + (available.width() - lebarAwal) / 2,
             available.y() + (available.height() - tinggiAwal) / 2);
    } else {
        resize(1200, 800);
    }

    QWidget *widgetPusat = new QWidget(this);
    setCentralWidget(widgetPusat);

    QVBoxLayout *layoutPusat = new QVBoxLayout(widgetPusat);
    layoutPusat->setContentsMargins(0, 0, 0, 0);
    layoutPusat->setSpacing(0);

    tumpukanHalaman = new QStackedWidget(this);
    layoutPusat->addWidget(tumpukanHalaman);

    bangunHalamanLogin();
    bangunHalamanPimpinan();
    bangunHalamanPengurus();
    bangunHalamanSantri();

    inisialisasiSistem();
    bacaDariTeks();

    tumpukanHalaman->setCurrentIndex(0);
}

AntarmukaUtama::~AntarmukaUtama() {
    simpanKeTeks();
    hapusMemori();
}

void AntarmukaUtama::bersihkanTabel(QTableWidget *tabel) {
    tabel->clearContents();
    tabel->setRowCount(0);
}

void AntarmukaUtama::tampilkanPesan(const QString &judul, const QString &isi) {
    QMessageBox::information(this, judul, isi);
}

void AntarmukaUtama::bangunHalamanLogin() {
    halamanLogin = new QWidget(this);
    QVBoxLayout *layoutUtama = new QVBoxLayout(halamanLogin);
    layoutUtama->setAlignment(Qt::AlignCenter);
    layoutUtama->setSpacing(20);

    QLabel *labelJudul = new QLabel("E-TAKZIR", halamanLogin);
    QFont fontJudul = labelJudul->font();
    fontJudul.setPointSize(36);
    fontJudul.setBold(true);
    labelJudul->setFont(fontJudul);
    labelJudul->setAlignment(Qt::AlignCenter);
    layoutUtama->addWidget(labelJudul);

    QLabel *labelSub = new QLabel("Sistem Peringatan Dini Pelanggaran Santri", halamanLogin);
    QFont fontSub = labelSub->font();
    fontSub.setPointSize(14);
    labelSub->setFont(fontSub);
    labelSub->setAlignment(Qt::AlignCenter);
    layoutUtama->addWidget(labelSub);

    layoutUtama->addSpacing(30);

    QHBoxLayout *layoutPanel = new QHBoxLayout();
    layoutPanel->setSpacing(20);

    QGroupBox *panelPimpinan = new QGroupBox("Login Pimpinan Pesantren", halamanLogin);
    QVBoxLayout *layoutPimpinan = new QVBoxLayout(panelPimpinan);
    kotakUserPimpinan = new QLineEdit(panelPimpinan);
    kotakUserPimpinan->setPlaceholderText("Username (pimpinan)");
    kotakPassPimpinan = new QLineEdit(panelPimpinan);
    kotakPassPimpinan->setPlaceholderText("Masukkan password");
    kotakPassPimpinan->setEchoMode(QLineEdit::Password);
    tombolLoginPimpinan = new QPushButton("Masuk sebagai Pimpinan", panelPimpinan);
    tombolLoginPimpinan->setStyleSheet("QPushButton { background-color: #2c3e50; color: white; padding: 10px; font-weight: bold; }");
    layoutPimpinan->addWidget(new QLabel("Username:"));
    layoutPimpinan->addWidget(kotakUserPimpinan);
    layoutPimpinan->addWidget(new QLabel("Password:"));
    layoutPimpinan->addWidget(kotakPassPimpinan);
    layoutPimpinan->addWidget(tombolLoginPimpinan);
    connect(tombolLoginPimpinan, &QPushButton::clicked, this, &AntarmukaUtama::on_tombolLoginPimpinan_clicked);

    QGroupBox *panelPengurus = new QGroupBox("Login Pengurus Keamanan", halamanLogin);
    QVBoxLayout *layoutPengurus = new QVBoxLayout(panelPengurus);
    kotakUserPengurus = new QLineEdit(panelPengurus);
    kotakUserPengurus->setPlaceholderText("Username (pengurus)");
    kotakPassPengurus = new QLineEdit(panelPengurus);
    kotakPassPengurus->setPlaceholderText("Masukkan password");
    kotakPassPengurus->setEchoMode(QLineEdit::Password);
    tombolLoginPengurus = new QPushButton("Masuk sebagai Pengurus", panelPengurus);
    tombolLoginPengurus->setStyleSheet("QPushButton { background-color: #27ae60; color: white; padding: 10px; font-weight: bold; }");
    layoutPengurus->addWidget(new QLabel("Username:"));
    layoutPengurus->addWidget(kotakUserPengurus);
    layoutPengurus->addWidget(new QLabel("Password:"));
    layoutPengurus->addWidget(kotakPassPengurus);
    layoutPengurus->addWidget(tombolLoginPengurus);
    connect(tombolLoginPengurus, &QPushButton::clicked, this, &AntarmukaUtama::on_tombolLoginPengurus_clicked);

    QGroupBox *panelSantri = new QGroupBox("Login Santri (NIS)", halamanLogin);
    QVBoxLayout *layoutSantri = new QVBoxLayout(panelSantri);
    kotakNisSantri = new QLineEdit(panelSantri);
    kotakNisSantri->setPlaceholderText("Masukkan NIS (Nomor Induk Santri)");
    kotakNisSantri->setValidator(new QIntValidator(0, 999999, panelSantri));
    tombolLoginSantri = new QPushButton("Masuk sebagai Santri", panelSantri);
    tombolLoginSantri->setStyleSheet("QPushButton { background-color: #2980b9; color: white; padding: 10px; font-weight: bold; }");
    layoutSantri->addWidget(new QLabel("Nomor Induk Santri (NIS):"));
    layoutSantri->addWidget(kotakNisSantri);
    layoutSantri->addWidget(tombolLoginSantri);
    connect(tombolLoginSantri, &QPushButton::clicked, this, &AntarmukaUtama::on_tombolLoginSantri_clicked);

    layoutPanel->addWidget(panelPimpinan);
    layoutPanel->addWidget(panelPengurus);
    layoutPanel->addWidget(panelSantri);
    layoutUtama->addLayout(layoutPanel);

    labelStatusLogin = new QLabel("Silakan pilih jenis login di atas.", halamanLogin);
    labelStatusLogin->setAlignment(Qt::AlignCenter);
    QFont fontStatus = labelStatusLogin->font();
    fontStatus.setItalic(true);
    labelStatusLogin->setFont(fontStatus);
    layoutUtama->addWidget(labelStatusLogin);

    tumpukanHalaman->addWidget(halamanLogin);
}

void AntarmukaUtama::bangunHalamanPimpinan() {
    halamanPimpinan = new QWidget(this);
    QHBoxLayout *layoutUtama = new QHBoxLayout(halamanPimpinan);
    layoutUtama->setSpacing(15);

    QWidget *panelKiri = new QWidget(halamanPimpinan);
    QVBoxLayout *layoutKiri = new QVBoxLayout(panelKiri);
    layoutKiri->setSpacing(10);

    labelJudulPimpinan = new QLabel("DASHBOARD PIMPINAN", panelKiri);
    QFont fontJudul = labelJudulPimpinan->font();
    fontJudul.setPointSize(18);
    fontJudul.setBold(true);
    labelJudulPimpinan->setFont(fontJudul);
    labelJudulPimpinan->setStyleSheet("color: #2c3e50;");
    layoutKiri->addWidget(labelJudulPimpinan);

    QGroupBox *grupMonitoring = new QGroupBox("Monitoring & Laporan", panelKiri);
    QVBoxLayout *layoutMonitoring = new QVBoxLayout(grupMonitoring);
    QPushButton *tombolZonaMerah = new QPushButton("Tampilkan Zona Merah", grupMonitoring);
    QPushButton *tombolStatistik = new QPushButton("Statistik Bulanan", grupMonitoring);
    QPushButton *tombolResetPoin = new QPushButton("Pemutihan Poin (Reset ke 0)", grupMonitoring);
    tombolResetPoin->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; }");
    layoutMonitoring->addWidget(tombolZonaMerah);
    layoutMonitoring->addWidget(tombolStatistik);
    layoutMonitoring->addWidget(tombolResetPoin);
    connect(tombolZonaMerah, &QPushButton::clicked, this, &AntarmukaUtama::on_pimpinan_tampilkanZonaMerah);
    connect(tombolStatistik, &QPushButton::clicked, this, &AntarmukaUtama::on_pimpinan_statistikBulanan);
    connect(tombolResetPoin, &QPushButton::clicked, this, &AntarmukaUtama::on_pimpinan_resetPoin);
    layoutKiri->addWidget(grupMonitoring);

    QGroupBox *grupAturan = new QGroupBox("Master Aturan Pelanggaran (DLL Circular)", panelKiri);
    QVBoxLayout *layoutAturan = new QVBoxLayout(grupAturan);

    labelAturanAktif = new QLabel("Aturan Aktif: (Belum ada)", grupAturan);
    layoutAturan->addWidget(labelAturanAktif);

    QHBoxLayout *layoutScroll = new QHBoxLayout();
    QPushButton *tombolPrev = new QPushButton("<< Prev", grupAturan);
    QPushButton *tombolNext = new QPushButton("Next >>", grupAturan);
    layoutScroll->addWidget(tombolPrev);
    layoutScroll->addWidget(tombolNext);
    layoutAturan->addLayout(layoutScroll);
    connect(tombolPrev, &QPushButton::clicked, this, &AntarmukaUtama::on_pimpinan_scrollAturanPrev);
    connect(tombolNext, &QPushButton::clicked, this, &AntarmukaUtama::on_pimpinan_scrollAturanNext);

    QPushButton *tombolTampilAturan = new QPushButton("Tampilkan Semua Aturan", grupAturan);
    layoutAturan->addWidget(tombolTampilAturan);
    connect(tombolTampilAturan, &QPushButton::clicked, this, &AntarmukaUtama::on_pimpinan_tampilkanAturan);

    layoutAturan->addWidget(new QLabel("Tambah Aturan Baru:"));
    kotakKodeAturan = new QSpinBox(grupAturan);
    kotakKodeAturan->setRange(1, 9999);
    kotakNamaAturan = new QLineEdit(grupAturan);
    kotakNamaAturan->setPlaceholderText("Nama pelanggaran (contoh: Kabur)");
    kotakBobotAturan = new QSpinBox(grupAturan);
    kotakBobotAturan->setRange(1, 200);
    QPushButton *tombolTambahAturan = new QPushButton("Tambah Aturan", grupAturan);
    layoutAturan->addWidget(new QLabel("Kode:"));
    layoutAturan->addWidget(kotakKodeAturan);
    layoutAturan->addWidget(new QLabel("Nama:"));
    layoutAturan->addWidget(kotakNamaAturan);
    layoutAturan->addWidget(new QLabel("Bobot Poin:"));
    layoutAturan->addWidget(kotakBobotAturan);
    layoutAturan->addWidget(tombolTambahAturan);
    connect(tombolTambahAturan, &QPushButton::clicked, this, &AntarmukaUtama::on_pimpinan_tambahAturan);

    layoutAturan->addWidget(new QLabel("Update Bobot (Kode + Poin Baru):"));
    kotakHapusKode = new QLineEdit(grupAturan);
    kotakHapusKode->setPlaceholderText("Kode aturan untuk update/hapus");
    kotakUpdateBobot = new QSpinBox(grupAturan);
    kotakUpdateBobot->setRange(1, 200);
    QHBoxLayout *layoutUpdateHapus = new QHBoxLayout();
    QPushButton *tombolUpdate = new QPushButton("Update Bobot", grupAturan);
    QPushButton *tombolHapus = new QPushButton("Hapus Aturan", grupAturan);
    layoutUpdateHapus->addWidget(tombolUpdate);
    layoutUpdateHapus->addWidget(tombolHapus);
    layoutAturan->addWidget(kotakHapusKode);
    layoutAturan->addWidget(new QLabel("Poin Baru:"));
    layoutAturan->addWidget(kotakUpdateBobot);
    layoutAturan->addLayout(layoutUpdateHapus);
    connect(tombolUpdate, &QPushButton::clicked, this, &AntarmukaUtama::on_pimpinan_updateBobot);
    connect(tombolHapus, &QPushButton::clicked, this, &AntarmukaUtama::on_pimpinan_hapusAturan);

    layoutKiri->addWidget(grupAturan);

    // UI RESTORED: Tombol Jadwal Piket & Peta Rute untuk Pimpinan
    QPushButton *tombolJadwalPimpinan = new QPushButton("Lihat Jadwal Piket", panelKiri);
    QPushButton *tombolPetaPimpinan = new QPushButton("Lihat Peta Rute", panelKiri);
    layoutKiri->addWidget(tombolJadwalPimpinan);
    layoutKiri->addWidget(tombolPetaPimpinan);
    connect(tombolJadwalPimpinan, &QPushButton::clicked, this, &AntarmukaUtama::on_pimpinan_tampilkanJadwal);
    connect(tombolPetaPimpinan, &QPushButton::clicked, this, &AntarmukaUtama::on_pimpinan_tampilkanPeta);

    QPushButton *tombolLogout = new QPushButton("Logout", panelKiri);
    tombolLogout->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; padding: 8px; }");
    layoutKiri->addWidget(tombolLogout);
    connect(tombolLogout, &QPushButton::clicked, this, &AntarmukaUtama::on_pimpinan_logout);

    layoutKiri->addStretch();

    QScrollArea *scrollKiri = new QScrollArea(halamanPimpinan);
    scrollKiri->setWidget(panelKiri);
    scrollKiri->setWidgetResizable(true);
    scrollKiri->setMinimumWidth(340);
    scrollKiri->setMaximumWidth(420);
    scrollKiri->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollKiri->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollKiri->setFrameShape(QFrame::NoFrame);

    layoutUtama->addWidget(scrollKiri, 1);

    QVBoxLayout *layoutKanan = new QVBoxLayout();

    tabelPimpinan = new QTableWidget(halamanPimpinan);
    tabelPimpinan->setColumnCount(4);
    tabelPimpinan->setHorizontalHeaderLabels(QStringList() << "NIS" << "NAMA" << "KAMAR" << "TOTAL POIN");
    tabelPimpinan->horizontalHeader()->setStretchLastSection(true);
    tabelPimpinan->setAlternatingRowColors(true);
    tabelPimpinan->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layoutKanan->addWidget(tabelPimpinan);

    areaOutputPimpinan = new QTextEdit(halamanPimpinan);
    areaOutputPimpinan->setReadOnly(true);
    areaOutputPimpinan->setPlaceholderText("Output log akan muncul di sini...");
    areaOutputPimpinan->setMaximumHeight(200);
    layoutKanan->addWidget(areaOutputPimpinan);

    layoutUtama->addLayout(layoutKanan, 2);

    tumpukanHalaman->addWidget(halamanPimpinan);
}

void AntarmukaUtama::bangunHalamanPengurus() {
    halamanPengurus = new QWidget(this);
    QHBoxLayout *layoutUtama = new QHBoxLayout(halamanPengurus);
    layoutUtama->setSpacing(15);

    QWidget *panelKiri = new QWidget(halamanPengurus);
    QVBoxLayout *layoutKiri = new QVBoxLayout(panelKiri);
    layoutKiri->setSpacing(10);

    labelJudulPengurus = new QLabel("DASHBOARD PENGURUS", panelKiri);
    QFont fontJudul = labelJudulPengurus->font();
    fontJudul.setPointSize(18);
    fontJudul.setBold(true);
    labelJudulPengurus->setFont(fontJudul);
    labelJudulPengurus->setStyleSheet("color: #27ae60;");
    layoutKiri->addWidget(labelJudulPengurus);

    labelSantriAktif = new QLabel("Santri Aktif: (Belum dipilih)", panelKiri);
    layoutKiri->addWidget(labelSantriAktif);

    QGroupBox *grupSantri = new QGroupBox("Kelola Data Santri (DLL Non-Circular)", panelKiri);
    QVBoxLayout *layoutSantri = new QVBoxLayout(grupSantri);

    QHBoxLayout *layoutNav = new QHBoxLayout();
    QPushButton *tombolPrevSantri = new QPushButton("<< Prev", grupSantri);
    QPushButton *tombolNextSantri = new QPushButton("Next >>", grupSantri);
    layoutNav->addWidget(tombolPrevSantri);
    layoutNav->addWidget(tombolNextSantri);
    layoutSantri->addLayout(layoutNav);
    connect(tombolPrevSantri, &QPushButton::clicked, this, &AntarmukaUtama::on_pengurus_navigasiPrev);
    connect(tombolNextSantri, &QPushButton::clicked, this, &AntarmukaUtama::on_pengurus_navigasiNext);

    QPushButton *tombolTampilSantri = new QPushButton("Tampilkan Semua Santri", grupSantri);
    layoutSantri->addWidget(tombolTampilSantri);
    connect(tombolTampilSantri, &QPushButton::clicked, this, &AntarmukaUtama::on_pengurus_tampilkanSantri);

    layoutSantri->addWidget(new QLabel("Tambah Santri Baru:"));
    kotakNisBaru = new QLineEdit(grupSantri);
    kotakNisBaru->setPlaceholderText("NIS");
    kotakNisBaru->setValidator(new QIntValidator(0, 999999, grupSantri));
    kotakNamaBaru = new QLineEdit(grupSantri);
    kotakNamaBaru->setPlaceholderText("Nama Lengkap");
    kotakKelasBaru = new QLineEdit(grupSantri);
    kotakKelasBaru->setPlaceholderText("Kelas");
    kotakKamarBaru = new QLineEdit(grupSantri);
    kotakKamarBaru->setPlaceholderText("Kamar (contoh: A1)");
    QPushButton *tombolTambahSantri = new QPushButton("Tambah Santri", grupSantri);
    layoutSantri->addWidget(kotakNisBaru);
    layoutSantri->addWidget(kotakNamaBaru);
    layoutSantri->addWidget(kotakKelasBaru);
    layoutSantri->addWidget(kotakKamarBaru);
    layoutSantri->addWidget(tombolTambahSantri);
    connect(tombolTambahSantri, &QPushButton::clicked, this, &AntarmukaUtama::on_pengurus_tambahSantri);

    layoutSantri->addWidget(new QLabel("Hapus Santri (Masukkan NIS):"));
    kotakNisHapus = new QLineEdit(grupSantri);
    kotakNisHapus->setPlaceholderText("NIS yang akan dihapus");
    kotakNisHapus->setValidator(new QIntValidator(0, 999999, grupSantri));
    QPushButton *tombolHapusSantri = new QPushButton("Hapus Santri", grupSantri);
    tombolHapusSantri->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; }");
    layoutSantri->addWidget(kotakNisHapus);
    layoutSantri->addWidget(tombolHapusSantri);
    connect(tombolHapusSantri, &QPushButton::clicked, this, &AntarmukaUtama::on_pengurus_hapusSantri);

    layoutSantri->addWidget(new QLabel("Update Data Santri (NIS + Data Baru):"));
    kotakNisUpdate = new QLineEdit(grupSantri);
    kotakNisUpdate->setPlaceholderText("NIS yang akan diupdate");
    kotakNisUpdate->setValidator(new QIntValidator(0, 999999, grupSantri));
    kotakNamaUpdate = new QLineEdit(grupSantri);
    kotakNamaUpdate->setPlaceholderText("Nama Baru");
    kotakKelasUpdate = new QLineEdit(grupSantri);
    kotakKelasUpdate->setPlaceholderText("Kelas Baru");
    kotakKamarUpdate = new QLineEdit(grupSantri);
    kotakKamarUpdate->setPlaceholderText("Kamar Baru");
    tombolUpdateSantri = new QPushButton("Update Data Santri", grupSantri);
    tombolUpdateSantri->setStyleSheet("QPushButton { background-color: #3498db; color: white; font-weight: bold; }");
    layoutSantri->addWidget(kotakNisUpdate);
    layoutSantri->addWidget(kotakNamaUpdate);
    layoutSantri->addWidget(kotakKelasUpdate);
    layoutSantri->addWidget(kotakKamarUpdate);
    layoutSantri->addWidget(tombolUpdateSantri);
    connect(tombolUpdateSantri, &QPushButton::clicked, this, &AntarmukaUtama::on_pengurus_updateSantri);

    layoutKiri->addWidget(grupSantri);

    QGroupBox *grupCari = new QGroupBox("Pencarian Cepat Santri (BST)", panelKiri);
    QVBoxLayout *layoutCari = new QVBoxLayout(grupCari);
    kotakNisCari = new QLineEdit(grupCari);
    kotakNisCari->setPlaceholderText("NIS yang dicari");
    kotakNisCari->setValidator(new QIntValidator(0, 999999, grupCari));
    tombolCariSantri = new QPushButton("Cari Santri (BST)", grupCari);
    tombolCariSantri->setStyleSheet("QPushButton { background-color: #9b59b6; color: white; font-weight: bold; }");
    layoutCari->addWidget(new QLabel("Cari berdasarkan NIS (Binary Search Tree):"));
    layoutCari->addWidget(kotakNisCari);
    layoutCari->addWidget(tombolCariSantri);
    connect(tombolCariSantri, &QPushButton::clicked, this, &AntarmukaUtama::on_pengurus_cariSantri);
    layoutKiri->addWidget(grupCari);

    QGroupBox *grupPelanggaran = new QGroupBox("Input Pelanggaran (Queue + Stack Undo)", panelKiri);
    QVBoxLayout *layoutPelanggaran = new QVBoxLayout(grupPelanggaran);

    kotakNisPelanggaran = new QLineEdit(grupPelanggaran);
    kotakNisPelanggaran->setPlaceholderText("NIS Santri yang melanggar");
    kotakNisPelanggaran->setValidator(new QIntValidator(0, 999999, grupPelanggaran));

    comboAturanPelanggaran = new QComboBox(grupPelanggaran);
    comboAturanPelanggaran->setPlaceholderText("Pilih jenis pelanggaran...");
    comboAturanPelanggaran->setEnabled(false);

    labelBobotTerpilih = new QLabel("Bobot Poin: -", grupPelanggaran);
    labelBobotTerpilih->setStyleSheet("color: #e74c3c; font-weight: bold;");

    kotakKeteranganPelanggaran = new QLineEdit(grupPelanggaran);
    kotakKeteranganPelanggaran->setPlaceholderText("Keterangan tambahan");

    layoutPelanggaran->addWidget(new QLabel("NIS Santri:"));
    layoutPelanggaran->addWidget(kotakNisPelanggaran);
    layoutPelanggaran->addWidget(new QLabel("Jenis Pelanggaran (dari Pimpinan):"));
    layoutPelanggaran->addWidget(comboAturanPelanggaran);
    layoutPelanggaran->addWidget(labelBobotTerpilih);
    layoutPelanggaran->addWidget(new QLabel("Keterangan:"));
    layoutPelanggaran->addWidget(kotakKeteranganPelanggaran);

    QPushButton *tombolInputPelanggaran = new QPushButton("Masukkan ke Antrean (Enqueue)", grupPelanggaran);
    tombolInputPelanggaran->setStyleSheet("QPushButton { background-color: #f39c12; color: white; font-weight: bold; }");
    layoutPelanggaran->addWidget(tombolInputPelanggaran);
    connect(tombolInputPelanggaran, &QPushButton::clicked, this, &AntarmukaUtama::on_pengurus_inputPelanggaran);

    connect(comboAturanPelanggaran, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AntarmukaUtama::on_pengurus_pilihAturanChanged);

    QHBoxLayout *layoutProses = new QHBoxLayout();
    QPushButton *tombolProses = new QPushButton("Proses Semua Antrean", grupPelanggaran);
    QPushButton *tombolCekAntrean = new QPushButton("Cek Jumlah Antrean", grupPelanggaran);
    layoutProses->addWidget(tombolProses);
    layoutProses->addWidget(tombolCekAntrean);
    layoutPelanggaran->addLayout(layoutProses);
    connect(tombolProses, &QPushButton::clicked, this, &AntarmukaUtama::on_pengurus_prosesAntrean);
    connect(tombolCekAntrean, &QPushButton::clicked, this, &AntarmukaUtama::on_pengurus_cekAntrean);

    QPushButton *tombolUndo = new QPushButton("UNDO Input Terakhir (Stack)", grupPelanggaran);
    tombolUndo->setStyleSheet("QPushButton { background-color: #c0392b; color: white; font-weight: bold; }");
    layoutPelanggaran->addWidget(tombolUndo);
    connect(tombolUndo, &QPushButton::clicked, this, &AntarmukaUtama::on_pengurus_undoTerakhir);

    layoutKiri->addWidget(grupPelanggaran);

    QGroupBox *grupLain = new QGroupBox("Informasi", panelKiri);
    QVBoxLayout *layoutLain = new QVBoxLayout(grupLain);
    QPushButton *tombolAturan = new QPushButton("Lihat Master Aturan", grupLain);
    layoutLain->addWidget(tombolAturan);
    connect(tombolAturan, &QPushButton::clicked, this, &AntarmukaUtama::on_pengurus_tampilkanAturan);

    // UI RESTORED: Tombol Jadwal Piket & Peta Rute untuk Pengurus
    QPushButton *tombolJadwalPengurus = new QPushButton("Lihat Jadwal Piket", grupLain);
    QPushButton *tombolPetaPengurus = new QPushButton("Lihat Peta Rute", grupLain);
    layoutLain->addWidget(tombolJadwalPengurus);
    layoutLain->addWidget(tombolPetaPengurus);
    connect(tombolJadwalPengurus, &QPushButton::clicked, this, &AntarmukaUtama::on_pengurus_tampilkanJadwal);
    connect(tombolPetaPengurus, &QPushButton::clicked, this, &AntarmukaUtama::on_pengurus_tampilkanPeta);

    layoutKiri->addWidget(grupLain);

    QPushButton *tombolLogout = new QPushButton("Logout", panelKiri);
    tombolLogout->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; padding: 8px; }");
    layoutKiri->addWidget(tombolLogout);
    connect(tombolLogout, &QPushButton::clicked, this, &AntarmukaUtama::on_pengurus_logout);

    layoutKiri->addStretch();

    QScrollArea *scrollKiri = new QScrollArea(halamanPengurus);
    scrollKiri->setWidget(panelKiri);
    scrollKiri->setWidgetResizable(true);
    scrollKiri->setMinimumWidth(340);
    scrollKiri->setMaximumWidth(420);
    scrollKiri->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollKiri->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollKiri->setFrameShape(QFrame::NoFrame);

    layoutUtama->addWidget(scrollKiri, 1);

    QVBoxLayout *layoutKanan = new QVBoxLayout();

    tabelPengurus = new QTableWidget(halamanPengurus);
    tabelPengurus->setColumnCount(5);
    tabelPengurus->setHorizontalHeaderLabels(QStringList() << "NIS" << "NAMA" << "KELAS" << "KAMAR" << "TOTAL POIN");
    tabelPengurus->horizontalHeader()->setStretchLastSection(true);
    tabelPengurus->setAlternatingRowColors(true);
    tabelPengurus->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layoutKanan->addWidget(tabelPengurus);

    areaOutputPengurus = new QTextEdit(halamanPengurus);
    areaOutputPengurus->setReadOnly(true);
    areaOutputPengurus->setPlaceholderText("Output log akan muncul di sini...");
    areaOutputPengurus->setMaximumHeight(200);
    layoutKanan->addWidget(areaOutputPengurus);

    layoutUtama->addLayout(layoutKanan, 2);

    tumpukanHalaman->addWidget(halamanPengurus);
}

void AntarmukaUtama::bangunHalamanSantri() {
    halamanSantri = new QWidget(this);
    QHBoxLayout *layoutUtama = new QHBoxLayout(halamanSantri);
    layoutUtama->setSpacing(15);

    QWidget *panelKiri = new QWidget(halamanSantri);
    QVBoxLayout *layoutKiri = new QVBoxLayout(panelKiri);
    layoutKiri->setSpacing(10);

    labelJudulSantri = new QLabel("DASHBOARD SANTRI", panelKiri);
    QFont fontJudul = labelJudulSantri->font();
    fontJudul.setPointSize(18);
    fontJudul.setBold(true);
    labelJudulSantri->setFont(fontJudul);
    labelJudulSantri->setStyleSheet("color: #2980b9;");
    layoutKiri->addWidget(labelJudulSantri);

    QGroupBox *grupProfil = new QGroupBox("Profil Santri", panelKiri);
    QVBoxLayout *layoutProfil = new QVBoxLayout(grupProfil);
    labelProfilNis = new QLabel("NIS: -", grupProfil);
    labelProfilNama = new QLabel("Nama: -", grupProfil);
    labelProfilKelas = new QLabel("Kelas: -", grupProfil);
    labelProfilKamar = new QLabel("Kamar: -", grupProfil);
    labelProfilPoin = new QLabel("Total Poin: -", grupProfil);
    QFont fontPoin = labelProfilPoin->font();
    fontPoin.setBold(true);
    fontPoin.setPointSize(14);
    labelProfilPoin->setFont(fontPoin);
    labelProfilPoin->setStyleSheet("color: #e74c3c;");

    layoutProfil->addWidget(labelProfilNis);
    layoutProfil->addWidget(labelProfilNama);
    layoutProfil->addWidget(labelProfilKelas);
    layoutProfil->addWidget(labelProfilKamar);
    layoutProfil->addWidget(labelProfilPoin);
    layoutKiri->addWidget(grupProfil);

    QPushButton *tombolRiwayat = new QPushButton("Lihat Riwayat Pelanggaran", panelKiri);
    QPushButton *tombolAturan = new QPushButton("Lihat Master Aturan", panelKiri);
    QPushButton *tombolLogout = new QPushButton("Logout", panelKiri);
    tombolLogout->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; padding: 8px; }");

    layoutKiri->addWidget(tombolRiwayat);
    layoutKiri->addWidget(tombolAturan);
    layoutKiri->addWidget(tombolLogout);
    connect(tombolRiwayat, &QPushButton::clicked, this, &AntarmukaUtama::on_santri_tampilkanRiwayat);
    connect(tombolAturan, &QPushButton::clicked, this, &AntarmukaUtama::on_santri_tampilkanAturan);
    connect(tombolLogout, &QPushButton::clicked, this, &AntarmukaUtama::on_santri_logout);

    layoutKiri->addStretch();

    QScrollArea *scrollKiri = new QScrollArea(halamanSantri);
    scrollKiri->setWidget(panelKiri);
    scrollKiri->setWidgetResizable(true);
    scrollKiri->setMinimumWidth(280);
    scrollKiri->setMaximumWidth(380);
    scrollKiri->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollKiri->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollKiri->setFrameShape(QFrame::NoFrame);

    layoutUtama->addWidget(scrollKiri, 1);

    QVBoxLayout *layoutKanan = new QVBoxLayout();

    tabelSantri = new QTableWidget(halamanSantri);
    tabelSantri->setColumnCount(5);
    tabelSantri->setHorizontalHeaderLabels(QStringList() << "NO" << "TANGGAL" << "JENIS" << "POIN" << "KETERANGAN");
    tabelSantri->horizontalHeader()->setStretchLastSection(true);
    tabelSantri->setAlternatingRowColors(true);
    tabelSantri->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layoutKanan->addWidget(tabelSantri);

    areaOutputSantri = new QTextEdit(halamanSantri);
    areaOutputSantri->setReadOnly(true);
    areaOutputSantri->setPlaceholderText("Output log akan muncul di sini...");
    areaOutputSantri->setMaximumHeight(200);
    layoutKanan->addWidget(areaOutputSantri);

    layoutUtama->addLayout(layoutKanan, 2);

    tumpukanHalaman->addWidget(halamanSantri);
}

void AntarmukaUtama::on_tombolLoginPimpinan_clicked() {
    QString user = kotakUserPimpinan->text();
    QString pass = kotakPassPimpinan->text();
    QByteArray userBytes = user.toUtf8();
    QByteArray passBytes = pass.toUtf8();
    int hasil = verifikasiLoginPengurus(userBytes.data(), passBytes.data());
    if (hasil == 1) {
        labelStatusLogin->setText("Login Pimpinan berhasil!");
        kotakUserPimpinan->clear();
        kotakPassPimpinan->clear();
        tumpukanHalaman->setCurrentIndex(1);
    } else {
        labelStatusLogin->setText("Login gagal! Username atau password salah.");
        tampilkanPesan("Login Gagal", "Username atau password Pimpinan tidak valid.");
    }
}

void AntarmukaUtama::on_tombolLoginPengurus_clicked() {
    QString user = kotakUserPengurus->text();
    QString pass = kotakPassPengurus->text();
    QByteArray userBytes = user.toUtf8();
    QByteArray passBytes = pass.toUtf8();
    int hasil = verifikasiLoginPengurus(userBytes.data(), passBytes.data());
    if (hasil == 2) {
        labelStatusLogin->setText("Login Pengurus berhasil!");
        kotakUserPengurus->clear();
        kotakPassPengurus->clear();
        tumpukanHalaman->setCurrentIndex(2);
        perbaruiComboAturan();
    } else {
        labelStatusLogin->setText("Login gagal! Username atau password salah.");
        tampilkanPesan("Login Gagal", "Username atau password Pengurus tidak valid.");
    }
}

void AntarmukaUtama::on_tombolLoginSantri_clicked() {
    int nis = kotakNisSantri->text().toInt();
    dataSantriLogin = verifikasiLoginSantri(nis);
    if (dataSantriLogin != NULL) {
        labelStatusLogin->setText(QString("Login Santri berhasil: %1").arg(dataSantriLogin->data.nama));
        kotakNisSantri->clear();

        labelProfilNis->setText(QString("NIS: %1").arg(dataSantriLogin->data.nis));
        labelProfilNama->setText(QString("Nama: %1").arg(dataSantriLogin->data.nama));
        labelProfilKelas->setText(QString("Kelas: %1").arg(dataSantriLogin->data.kelas));
        labelProfilKamar->setText(QString("Kamar: %1").arg(dataSantriLogin->data.kamar));
        labelProfilPoin->setText(QString("Total Poin: %1").arg(dataSantriLogin->data.totalPoin));

        tumpukanHalaman->setCurrentIndex(3);
    } else {
        labelStatusLogin->setText("Login gagal! NIS tidak ditemukan.");
        tampilkanPesan("Login Gagal", "NIS yang dimasukkan tidak terdaftar di database.");
    }
}

void AntarmukaUtama::on_pimpinan_tampilkanZonaMerah() {
    bersihkanTabel(tabelPimpinan);
    tabelPimpinan->setColumnCount(4);
    tabelPimpinan->setHorizontalHeaderLabels(QStringList() << "NIS" << "NAMA" << "KAMAR" << "TOTAL POIN");

    struct NodeSantri *current = headSantri;
    int baris = 0;
    while (current != NULL) {
        if (current->data.totalPoin >= 50) {
            tabelPimpinan->insertRow(baris);
            tabelPimpinan->setItem(baris, 0, new QTableWidgetItem(QString::number(current->data.nis)));
            tabelPimpinan->setItem(baris, 1, new QTableWidgetItem(current->data.nama));
            tabelPimpinan->setItem(baris, 2, new QTableWidgetItem(current->data.kamar));
            QTableWidgetItem *itemPoin = new QTableWidgetItem(QString::number(current->data.totalPoin));
            itemPoin->setBackground(QColor(231, 76, 60));
            itemPoin->setForeground(QColor(255, 255, 255));
            tabelPimpinan->setItem(baris, 3, itemPoin);
            baris++;
        }
        current = current->next;
    }

    if (baris == 0) {
        areaOutputPimpinan->append("Alhamdulillah, tidak ada santri di zona merah.");
    } else {
        areaOutputPimpinan->append(QString("Ditemukan %1 santri di ZONA MERAH (poin >= 50).").arg(baris));
    }
}

void AntarmukaUtama::on_pimpinan_tampilkanAturan() {
    bersihkanTabel(tabelPimpinan);
    tabelPimpinan->setColumnCount(3);
    tabelPimpinan->setHorizontalHeaderLabels(QStringList() << "KODE" << "NAMA PELANGGARAN" << "BOBOT POIN");

    if (headAturan == NULL) {
        areaOutputPimpinan->append("Belum ada aturan pelanggaran.");
        return;
    }

    struct NodeAturan *current = headAturan;
    int baris = 0;
    do {
        tabelPimpinan->insertRow(baris);
        tabelPimpinan->setItem(baris, 0, new QTableWidgetItem(QString::number(current->data.kodePelanggaran)));
        tabelPimpinan->setItem(baris, 1, new QTableWidgetItem(current->data.namaPelanggaran));
        tabelPimpinan->setItem(baris, 2, new QTableWidgetItem(QString::number(current->data.bobotPoin)));
        baris++;
        current = current->next;
    } while (current != headAturan);

    areaOutputPimpinan->append(QString("Total %1 aturan pelanggaran ditampilkan.").arg(baris));
}

void AntarmukaUtama::on_pimpinan_tambahAturan() {
    struct DataMaster dm;
    dm.kodePelanggaran = kotakKodeAturan->value();
    QByteArray bytesNamaAturan = kotakNamaAturan->text().toUtf8();
    strncpy(dm.namaPelanggaran, bytesNamaAturan.constData(), sizeof(dm.namaPelanggaran) - 1);
    dm.namaPelanggaran[sizeof(dm.namaPelanggaran) - 1] = '\0';
    dm.bobotPoin = kotakBobotAturan->value();
    tambahAturan(dm);
    areaOutputPimpinan->append(QString("Aturan '%1' (Kode: %2, Poin: %3) ditambahkan.")
        .arg(kotakNamaAturan->text()).arg(dm.kodePelanggaran).arg(dm.bobotPoin));
    kotakNamaAturan->clear();
    perbaruiComboAturan();
}

void AntarmukaUtama::on_pimpinan_scrollAturanNext() {
    struct NodeAturan *na = scrollAturanNext();
    if (na != NULL) {
        labelAturanAktif->setText(QString("Aturan Aktif: [%1] %2 (Poin: %3)")
            .arg(na->data.kodePelanggaran).arg(na->data.namaPelanggaran).arg(na->data.bobotPoin));
        areaOutputPimpinan->append(QString("Scroll Next: %1").arg(na->data.namaPelanggaran));
    }
}

void AntarmukaUtama::on_pimpinan_scrollAturanPrev() {
    struct NodeAturan *na = scrollAturanPrev();
    if (na != NULL) {
        labelAturanAktif->setText(QString("Aturan Aktif: [%1] %2 (Poin: %3)")
            .arg(na->data.kodePelanggaran).arg(na->data.namaPelanggaran).arg(na->data.bobotPoin));
        areaOutputPimpinan->append(QString("Scroll Prev: %1").arg(na->data.namaPelanggaran));
    }
}

void AntarmukaUtama::on_pimpinan_updateBobot() {
    int kode = kotakHapusKode->text().toInt();
    int bobot = kotakUpdateBobot->value();
    updateBobotPoin(kode, bobot);
    areaOutputPimpinan->append(QString("Update bobot aturan kode %1 menjadi %2 poin.").arg(kode).arg(bobot));
    perbaruiComboAturan();
}

void AntarmukaUtama::on_pimpinan_hapusAturan() {
    int kode = kotakHapusKode->text().toInt();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Konfirmasi Hapus",
        QString("Yakin ingin menghapus aturan dengan kode %1?").arg(kode),
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        areaOutputPimpinan->append("Penghapusan aturan dibatalkan.");
        return;
    }

    hapusAturan(kode);
    areaOutputPimpinan->append(QString("Menghapus aturan dengan kode %1.").arg(kode));
    perbaruiComboAturan();
}

void AntarmukaUtama::on_pimpinan_resetPoin() {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Konfirmasi Reset",
        "Yakin ingin mereset SEMUA poin santri ke 0? Tindakan ini tidak bisa dibatalkan.",
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        areaOutputPimpinan->append("Pemutihan poin dibatalkan.");
        return;
    }

    resetTotalPoin();
    areaOutputPimpinan->append("Pemutihan poin berhasil! Semua poin santri direset ke 0.");
    tampilkanPesan("Pemutihan", "Semua poin santri telah direset ke 0.");
}

void AntarmukaUtama::on_pimpinan_statistikBulanan() {
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);
    int bulanIni = info->tm_mon + 1;
    int tahunIni = info->tm_year + 1900;
    char bulanStr[12];
    sprintf(bulanStr, "%02d", bulanIni);

    int totalPelanggaran = 0;
    int totalPoinBulanan = 0;

    struct NodeSantri *santri = headSantri;
    while (santri != NULL) {
        struct NodeRiwayat *riwayat = santri->headRiwayat;
        while (riwayat != NULL) {
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

    areaOutputPimpinan->append(QString("=== STATISTIK BULAN %1/%2 ===").arg(bulanStr).arg(tahunIni));
    areaOutputPimpinan->append(QString("Total Kejadian Pelanggaran: %1").arg(totalPelanggaran));
    areaOutputPimpinan->append(QString("Total Poin Dikenakan: %1").arg(totalPoinBulanan));
}

void AntarmukaUtama::on_pimpinan_tampilkanJadwal() {
    bersihkanTabel(tabelPimpinan);
    tabelPimpinan->setColumnCount(3);
    tabelPimpinan->setHorizontalHeaderLabels(QStringList() << "NAMA PENGURUS" << "HARI" << "SHIFT");

    if (headPiket == NULL) {
        areaOutputPimpinan->append("Belum ada jadwal piket.");
        return;
    }

    struct NodePiket *current = headPiket;
    int baris = 0;
    do {
        tabelPimpinan->insertRow(baris);
        tabelPimpinan->setItem(baris, 0, new QTableWidgetItem(current->data.namaPengurus));
        tabelPimpinan->setItem(baris, 1, new QTableWidgetItem(current->data.hari));
        tabelPimpinan->setItem(baris, 2, new QTableWidgetItem(current->data.shift));
        baris++;
        current = current->next;
    } while (current != headPiket);

    areaOutputPimpinan->append(QString("Total %1 jadwal piket ditampilkan.").arg(baris));
}

void AntarmukaUtama::on_pimpinan_tampilkanPeta() {
    bersihkanTabel(tabelPimpinan);
    tabelPimpinan->setColumnCount(4);
    tabelPimpinan->setHorizontalHeaderLabels(QStringList() << "BLOK ASAL" << "KAMAR" << "TERHUBUNG KE" << "JARAK (m)");

    if (headVertex == NULL) {
        areaOutputPimpinan->append("Peta asrama masih kosong!");
        return;
    }

    int baris = 0;
    struct Vertex *v = headVertex;
    while (v != NULL) {
        struct Edge *e = v->firstEdge;
        while (e != NULL) {
            tabelPimpinan->insertRow(baris);
            tabelPimpinan->setItem(baris, 0, new QTableWidgetItem(v->namaBlok));
            tabelPimpinan->setItem(baris, 1, new QTableWidgetItem(v->namaKamar));
            tabelPimpinan->setItem(baris, 2, new QTableWidgetItem(e->blokTujuan));
            tabelPimpinan->setItem(baris, 3, new QTableWidgetItem(QString::number(e->jarakMeter)));
            baris++;
            e = e->nextEdge;
        }
        v = v->nextVertex;
    }

    areaOutputPimpinan->append(QString("Total %1 jalur/rute ditampilkan.").arg(baris));
}

void AntarmukaUtama::on_pimpinan_logout() {
    simpanKeTeks();
    areaOutputPimpinan->clear();
    bersihkanTabel(tabelPimpinan);
    tumpukanHalaman->setCurrentIndex(0);
    labelStatusLogin->setText("Logout berhasil. Silakan login kembali.");
}

void AntarmukaUtama::on_pengurus_tampilkanSantri() {
    bersihkanTabel(tabelPengurus);
    tabelPengurus->setColumnCount(5);
    tabelPengurus->setHorizontalHeaderLabels(QStringList() << "NIS" << "NAMA" << "KELAS" << "KAMAR" << "TOTAL POIN");

    struct NodeSantri *current = headSantri;
    int baris = 0;
    while (current != NULL) {
        tabelPengurus->insertRow(baris);
        tabelPengurus->setItem(baris, 0, new QTableWidgetItem(QString::number(current->data.nis)));
        tabelPengurus->setItem(baris, 1, new QTableWidgetItem(current->data.nama));
        tabelPengurus->setItem(baris, 2, new QTableWidgetItem(current->data.kelas));
        tabelPengurus->setItem(baris, 3, new QTableWidgetItem(current->data.kamar));
        tabelPengurus->setItem(baris, 4, new QTableWidgetItem(QString::number(current->data.totalPoin)));
        baris++;
        current = current->next;
    }

    areaOutputPengurus->append(QString("Total %1 santri ditampilkan.").arg(baris));
}

void AntarmukaUtama::on_pengurus_tambahSantri() {
    // Validasi: semua field wajib diisi
    int nis = kotakNisBaru->text().toInt();
    QString nama = kotakNamaBaru->text().trimmed();
    QString kelas = kotakKelasBaru->text().trimmed();
    QString kamar = kotakKamarBaru->text().trimmed();

    if (nis == 0) {
        areaOutputPengurus->append("Gagal: NIS tidak valid atau kosong.");
        tampilkanPesan("Gagal", "NIS tidak valid. Masukkan NIS dengan benar.");
        return;
    }
    if (nama.isEmpty()) {
        areaOutputPengurus->append("Gagal: Nama santri wajib diisi.");
        tampilkanPesan("Gagal", "Nama santri tidak boleh kosong.");
        return;
    }
    if (kelas.isEmpty()) {
        areaOutputPengurus->append("Gagal: Kelas santri wajib diisi.");
        tampilkanPesan("Gagal", "Kelas santri tidak boleh kosong.");
        return;
    }
    if (kamar.isEmpty()) {
        areaOutputPengurus->append("Gagal: Kamar santri wajib diisi.");
        tampilkanPesan("Gagal", "Kamar santri tidak boleh kosong.");
        return;
    }

    // Cek duplikat NIS
    struct NodeSantri *cek = cariLinearSantri(nis);
    if (cek != NULL) {
        areaOutputPengurus->append(QString("Gagal: NIS %1 sudah terdaftar (atas nama %2).").arg(nis).arg(cek->data.nama));
        tampilkanPesan("Gagal", QString("NIS %1 sudah terdaftar di database.").arg(nis));
        return;
    }

    struct DataSantri ds;
    ds.nis = nis;
    QByteArray bytesNama = nama.toUtf8();
    QByteArray bytesKelas = kelas.toUtf8();
    QByteArray bytesKamar = kamar.toUtf8();
    strncpy(ds.nama, bytesNama.constData(), sizeof(ds.nama) - 1);
    ds.nama[sizeof(ds.nama) - 1] = '\0';
    strncpy(ds.kelas, bytesKelas.constData(), sizeof(ds.kelas) - 1);
    ds.kelas[sizeof(ds.kelas) - 1] = '\0';
    strncpy(ds.kamar, bytesKamar.constData(), sizeof(ds.kamar) - 1);
    ds.kamar[sizeof(ds.kamar) - 1] = '\0';
    ds.totalPoin = 0;
    tambahSantri(ds);

    areaOutputPengurus->append(QString("Santri %1 (NIS: %2) berhasil ditambahkan.").arg(ds.nama).arg(ds.nis));
    kotakNisBaru->clear();
    kotakNamaBaru->clear();
    kotakKelasBaru->clear();
    kotakKamarBaru->clear();
}

void AntarmukaUtama::on_pengurus_navigasiNext() {
    struct NodeSantri *ns = navigasiNext();
    if (ns != NULL) {
        labelSantriAktif->setText(QString("Santri Aktif: [%1] %2 | Kelas: %3 | Kamar: %4 | Poin: %5")
            .arg(ns->data.nis).arg(ns->data.nama).arg(ns->data.kelas).arg(ns->data.kamar).arg(ns->data.totalPoin));
        areaOutputPengurus->append(QString("Navigasi Next: %1").arg(ns->data.nama));
    } else {
        areaOutputPengurus->append("Sudah di santri terakhir.");
    }
}

void AntarmukaUtama::on_pengurus_navigasiPrev() {
    struct NodeSantri *ns = navigasiPrev();
    if (ns != NULL) {
        labelSantriAktif->setText(QString("Santri Aktif: [%1] %2 | Kelas: %3 | Kamar: %4 | Poin: %5")
            .arg(ns->data.nis).arg(ns->data.nama).arg(ns->data.kelas).arg(ns->data.kamar).arg(ns->data.totalPoin));
        areaOutputPengurus->append(QString("Navigasi Prev: %1").arg(ns->data.nama));
    } else {
        areaOutputPengurus->append("Sudah di santri pertama.");
    }
}

void AntarmukaUtama::on_pengurus_hapusSantri() {
    int nis = kotakNisHapus->text().toInt();
    if (nis == 0) {
        areaOutputPengurus->append("Gagal: NIS tidak valid.");
        tampilkanPesan("Gagal", "NIS tidak valid.");
        return;
    }
    struct NodeSantri *santri = cariLinearSantri(nis);
    if (santri == NULL) {
        areaOutputPengurus->append(QString("Data dengan NIS %1 tidak ditemukan.").arg(nis));
        tampilkanPesan("Gagal", QString("Data dengan NIS %1 tidak ditemukan.").arg(nis));
        kotakNisHapus->clear();
        return;
    }
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Konfirmasi Hapus",
        QString("Yakin ingin menghapus santri dengan NIS %1? Data ini tidak bisa dikembalikan.").arg(nis),
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        areaOutputPengurus->append("Penghapusan santri dibatalkan.");
        kotakNisHapus->clear();
        return;
    }

    hapusSantri(nis);
    areaOutputPengurus->append(QString("Menghapus santri dengan NIS %1.").arg(nis));
    kotakNisHapus->clear();
}

void AntarmukaUtama::on_pengurus_updateSantri() {
    int nis = kotakNisUpdate->text().toInt();
    if (nis == 0) {
        areaOutputPengurus->append("Gagal update: NIS tidak valid.");
        tampilkanPesan("Gagal", "NIS tidak valid.");
        return;
    }
    struct NodeSantri *existing = cariLinearSantri(nis);
    if (existing == NULL) {
        areaOutputPengurus->append(QString("Data dengan NIS %1 tidak ditemukan.").arg(nis));
        tampilkanPesan("Gagal", QString("Data dengan NIS %1 tidak ditemukan.").arg(nis));
        kotakNisUpdate->clear();
        kotakNamaUpdate->clear();
        kotakKelasUpdate->clear();
        kotakKamarUpdate->clear();
        return;
    }

    struct DataSantri dataBaru;
    dataBaru.nis = nis;
    QByteArray bytesNamaUp = kotakNamaUpdate->text().toUtf8();
    QByteArray bytesKelasUp = kotakKelasUpdate->text().toUtf8();
    QByteArray bytesKamarUp = kotakKamarUpdate->text().toUtf8();
    strncpy(dataBaru.nama, bytesNamaUp.constData(), sizeof(dataBaru.nama) - 1);
    dataBaru.nama[sizeof(dataBaru.nama) - 1] = '\0';
    strncpy(dataBaru.kelas, bytesKelasUp.constData(), sizeof(dataBaru.kelas) - 1);
    dataBaru.kelas[sizeof(dataBaru.kelas) - 1] = '\0';
    strncpy(dataBaru.kamar, bytesKamarUp.constData(), sizeof(dataBaru.kamar) - 1);
    dataBaru.kamar[sizeof(dataBaru.kamar) - 1] = '\0';
    dataBaru.totalPoin = existing->data.totalPoin;

    updateSantri(nis, dataBaru);
    areaOutputPengurus->append(QString("Data santri NIS %1 berhasil diupdate: %2 | %3 | %4")
        .arg(nis).arg(dataBaru.nama).arg(dataBaru.kelas).arg(dataBaru.kamar));

    kotakNisUpdate->clear();
    kotakNamaUpdate->clear();
    kotakKelasUpdate->clear();
    kotakKamarUpdate->clear();

    on_pengurus_tampilkanSantri();
}

void AntarmukaUtama::on_pengurus_inputPelanggaran() {
    if (comboAturanPelanggaran->currentIndex() == 0 || comboAturanPelanggaran->currentData().toInt() < 0 || headAturan == NULL) {
        areaOutputPengurus->append("Gagal: Pilih jenis pelanggaran yang valid!");
        tampilkanPesan("Peringatan", "Silakan pilih jenis pelanggaran dari dropdown. Pilih 'Tidak Jadi' jika ingin membatalkan.");
        return;
    }

    int nis = kotakNisPelanggaran->text().toInt();
    if (nis == 0) {
        areaOutputPengurus->append("Gagal: NIS tidak valid.");
        tampilkanPesan("Gagal", "NIS tidak valid.");
        return;
    }
    struct NodeSantri *santri = cariLinearSantri(nis);
    if (santri == NULL) {
        areaOutputPengurus->append(QString("Data dengan NIS %1 tidak ditemukan.").arg(nis));
        tampilkanPesan("Gagal", QString("Data dengan NIS %1 tidak ditemukan.").arg(nis));
        return;
    }

    struct DataPelanggaran dp;
    dp.idPelanggaran = 0;
    dp.nisSantri = nis;

    QByteArray bytesJenis = comboAturanPelanggaran->currentText().toUtf8();
    QByteArray bytesKeterangan = kotakKeteranganPelanggaran->text().toUtf8();
    strncpy(dp.jenis, bytesJenis.constData(), sizeof(dp.jenis) - 1);
    dp.jenis[sizeof(dp.jenis) - 1] = '\0';
    dp.bobotPoin = comboAturanPelanggaran->currentData().toInt();
    strncpy(dp.keterangan, bytesKeterangan.constData(), sizeof(dp.keterangan) - 1);
    dp.keterangan[sizeof(dp.keterangan) - 1] = '\0';
    strcpy(dp.tanggal, "");

    prosesInputPelanggaran(dp);
    areaOutputPengurus->append(QString("Pelanggaran masuk antrean: NIS %1 | %2 | %3 poin")
        .arg(dp.nisSantri).arg(dp.jenis).arg(dp.bobotPoin));

    kotakNisPelanggaran->clear();
    kotakKeteranganPelanggaran->clear();
    comboAturanPelanggaran->setCurrentIndex(0);
}

void AntarmukaUtama::on_pengurus_prosesAntrean() {
    int jumlah = hitungAntrean();
    if (jumlah == 0) {
        areaOutputPengurus->append("Antrean kosong, tidak ada yang diproses.");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Konfirmasi Proses",
        QString("Yakin ingin memproses %1 pelanggaran dalam antrean?").arg(jumlah),
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        areaOutputPengurus->append("Proses antrean dibatalkan.");
        return;
    }

    while (!isEmptyQueue()) {
        struct DataPelanggaran dp = dequeue();
        eksekusiPelanggaran(dp);
        areaOutputPengurus->append(QString("Proses: ID %1 | NIS %2 | %3 | Poin: %4")
            .arg(dp.idPelanggaran).arg(dp.nisSantri).arg(dp.jenis).arg(dp.bobotPoin));
    }

    areaOutputPengurus->append(QString("%1 pelanggaran telah diproses.").arg(jumlah));
    tampilkanPesan("Proses Selesai", QString("%1 pelanggaran berhasil diproses.").arg(jumlah));
}

void AntarmukaUtama::on_pengurus_undoTerakhir() {
    undoInputTerakhir();
    areaOutputPengurus->append("Undo telah dieksekusi.");
}

void AntarmukaUtama::on_pengurus_cekAntrean() {
    int jumlah = hitungAntrean();
    areaOutputPengurus->append(QString("Jumlah antrean menunggu: %1").arg(jumlah));
}

void AntarmukaUtama::on_pengurus_tampilkanAturan() {
    bersihkanTabel(tabelPengurus);
    tabelPengurus->setColumnCount(3);
    tabelPengurus->setHorizontalHeaderLabels(QStringList() << "KODE" << "NAMA PELANGGARAN" << "BOBOT POIN");

    if (headAturan == NULL) {
        areaOutputPengurus->append("Belum ada aturan pelanggaran.");
        return;
    }

    struct NodeAturan *current = headAturan;
    int baris = 0;
    do {
        tabelPengurus->insertRow(baris);
        tabelPengurus->setItem(baris, 0, new QTableWidgetItem(QString::number(current->data.kodePelanggaran)));
        tabelPengurus->setItem(baris, 1, new QTableWidgetItem(current->data.namaPelanggaran));
        tabelPengurus->setItem(baris, 2, new QTableWidgetItem(QString::number(current->data.bobotPoin)));
        baris++;
        current = current->next;
    } while (current != headAturan);

    areaOutputPengurus->append(QString("Total %1 aturan pelanggaran (read-only).").arg(baris));
}

void AntarmukaUtama::on_pengurus_tampilkanPeta() {
    bersihkanTabel(tabelPengurus);
    tabelPengurus->setColumnCount(4);
    tabelPengurus->setHorizontalHeaderLabels(QStringList() << "BLOK ASAL" << "KAMAR" << "TERHUBUNG KE" << "JARAK (m)");

    if (headVertex == NULL) {
        areaOutputPengurus->append("Peta asrama masih kosong!");
        return;
    }

    int baris = 0;
    struct Vertex *v = headVertex;
    while (v != NULL) {
        struct Edge *e = v->firstEdge;
        while (e != NULL) {
            tabelPengurus->insertRow(baris);
            tabelPengurus->setItem(baris, 0, new QTableWidgetItem(v->namaBlok));
            tabelPengurus->setItem(baris, 1, new QTableWidgetItem(v->namaKamar));
            tabelPengurus->setItem(baris, 2, new QTableWidgetItem(e->blokTujuan));
            tabelPengurus->setItem(baris, 3, new QTableWidgetItem(QString::number(e->jarakMeter)));
            baris++;
            e = e->nextEdge;
        }
        v = v->nextVertex;
    }

    areaOutputPengurus->append(QString("Total %1 jalur/rute ditampilkan.").arg(baris));
}

void AntarmukaUtama::on_pengurus_tampilkanJadwal() {
    bersihkanTabel(tabelPengurus);
    tabelPengurus->setColumnCount(3);
    tabelPengurus->setHorizontalHeaderLabels(QStringList() << "NAMA PENGURUS" << "HARI" << "SHIFT");

    if (headPiket == NULL) {
        areaOutputPengurus->append("Belum ada jadwal piket.");
        return;
    }

    struct NodePiket *current = headPiket;
    int baris = 0;
    do {
        tabelPengurus->insertRow(baris);
        tabelPengurus->setItem(baris, 0, new QTableWidgetItem(current->data.namaPengurus));
        tabelPengurus->setItem(baris, 1, new QTableWidgetItem(current->data.hari));
        tabelPengurus->setItem(baris, 2, new QTableWidgetItem(current->data.shift));
        baris++;
        current = current->next;
    } while (current != headPiket);

    areaOutputPengurus->append(QString("Total %1 jadwal piket ditampilkan.").arg(baris));
}

void AntarmukaUtama::on_pengurus_logout() {
    simpanKeTeks();
    areaOutputPengurus->clear();
    bersihkanTabel(tabelPengurus);
    labelSantriAktif->setText("Santri Aktif: (Belum dipilih)");
    tumpukanHalaman->setCurrentIndex(0);
    labelStatusLogin->setText("Logout berhasil. Silakan login kembali.");
}

void AntarmukaUtama::on_pengurus_pilihAturanChanged(int index) {
    if (index <= 0 || headAturan == NULL) {
        labelBobotTerpilih->setText("Bobot Poin: -");
        return;
    }
    int bobot = comboAturanPelanggaran->currentData().toInt();
    if (bobot < 0) {
        labelBobotTerpilih->setText("Bobot Poin: -");
        return;
    }
    labelBobotTerpilih->setText(QString("Bobot Poin: %1").arg(bobot));
}

void AntarmukaUtama::perbaruiComboAturan() {
    if (!comboAturanPelanggaran) return;

    comboAturanPelanggaran->clear();

    if (headAturan == NULL) {
        comboAturanPelanggaran->addItem("Belum ada aturan (hubungi Pimpinan)", QVariant(-1));
        comboAturanPelanggaran->setEnabled(false);
        labelBobotTerpilih->setText("Bobot Poin: -");
        return;
    }

    comboAturanPelanggaran->setEnabled(true);
    comboAturanPelanggaran->addItem("Tidak Jadi", QVariant(-1));

    struct NodeAturan *current = headAturan;
    do {
        comboAturanPelanggaran->addItem(
            QString(current->data.namaPelanggaran),
            QVariant(current->data.bobotPoin)
        );
        current = current->next;
    } while (current != headAturan);

    comboAturanPelanggaran->setCurrentIndex(0);
    on_pengurus_pilihAturanChanged(0);
}

void AntarmukaUtama::on_pengurus_cariSantri() {
    int nis = kotakNisCari->text().toInt();
    if (nis == 0) {
        areaOutputPengurus->append("Gagal cari: NIS tidak valid.");
        tampilkanPesan("Gagal", "NIS tidak valid.");
        return;
    }

    struct NodeBST *hasil = cariNisBST(rootBST, nis);

    bersihkanTabel(tabelPengurus);
    tabelPengurus->setColumnCount(5);
    tabelPengurus->setHorizontalHeaderLabels(QStringList() << "NIS" << "NAMA" << "KELAS" << "KAMAR" << "TOTAL POIN");

    if (hasil != NULL) {
        tabelPengurus->insertRow(0);
        tabelPengurus->setItem(0, 0, new QTableWidgetItem(QString::number(hasil->data.nis)));
        tabelPengurus->setItem(0, 1, new QTableWidgetItem(hasil->data.nama));
        tabelPengurus->setItem(0, 2, new QTableWidgetItem(hasil->data.kelas));
        tabelPengurus->setItem(0, 3, new QTableWidgetItem(hasil->data.kamar));
        tabelPengurus->setItem(0, 4, new QTableWidgetItem(QString::number(hasil->data.totalPoin)));
        areaOutputPengurus->append(QString("Santri ditemukan (BST): NIS %1 | %2 | Kelas: %3 | Kamar: %4 | Poin: %5")
            .arg(hasil->data.nis).arg(hasil->data.nama).arg(hasil->data.kelas).arg(hasil->data.kamar).arg(hasil->data.totalPoin));
    } else {
        areaOutputPengurus->append(QString("Data dengan NIS %1 tidak ditemukan di BST.").arg(nis));
        tampilkanPesan("Gagal", QString("Data dengan NIS %1 tidak ditemukan.").arg(nis));
    }

    kotakNisCari->clear();
}

void AntarmukaUtama::on_santri_tampilkanProfil() {
    if (dataSantriLogin == NULL) return;
    labelProfilNis->setText(QString("NIS: %1").arg(dataSantriLogin->data.nis));
    labelProfilNama->setText(QString("Nama: %1").arg(dataSantriLogin->data.nama));
    labelProfilKelas->setText(QString("Kelas: %1").arg(dataSantriLogin->data.kelas));
    labelProfilKamar->setText(QString("Kamar: %1").arg(dataSantriLogin->data.kamar));
    labelProfilPoin->setText(QString("Total Poin: %1").arg(dataSantriLogin->data.totalPoin));

    if (dataSantriLogin->data.totalPoin >= 50) {
        labelProfilPoin->setStyleSheet("color: #e74c3c; font-weight: bold; font-size: 14px;");
    } else {
        labelProfilPoin->setStyleSheet("color: #27ae60; font-weight: bold; font-size: 14px;");
    }
}

void AntarmukaUtama::on_santri_tampilkanRiwayat() {
    if (dataSantriLogin == NULL) return;

    struct NodeSantri *ns = cariLinearSantri(dataSantriLogin->data.nis);
    if (ns == NULL) {
        areaOutputSantri->append(QString("Data dengan NIS %1 tidak ditemukan di DLL.").arg(dataSantriLogin->data.nis));
        tampilkanPesan("Gagal", QString("Data dengan NIS %1 tidak ditemukan.").arg(dataSantriLogin->data.nis));
        return;
    }

    bersihkanTabel(tabelSantri);
    tabelSantri->setColumnCount(5);
    tabelSantri->setHorizontalHeaderLabels(QStringList() << "NO" << "TANGGAL" << "JENIS" << "POIN" << "KETERANGAN");

    struct NodeRiwayat *riwayat = ns->headRiwayat;
    int baris = 0;
    while (riwayat != NULL) {
        tabelSantri->insertRow(baris);
        tabelSantri->setItem(baris, 0, new QTableWidgetItem(QString::number(baris + 1)));
        tabelSantri->setItem(baris, 1, new QTableWidgetItem(riwayat->data.tanggal));
        tabelSantri->setItem(baris, 2, new QTableWidgetItem(riwayat->data.jenis));
        tabelSantri->setItem(baris, 3, new QTableWidgetItem(QString::number(riwayat->data.bobotPoin)));
        tabelSantri->setItem(baris, 4, new QTableWidgetItem(riwayat->data.keterangan));
        baris++;
        riwayat = riwayat->next;
    }

    if (baris == 0) {
        areaOutputSantri->append("Belum ada riwayat pelanggaran. Alhamdulillah.");
    } else {
        areaOutputSantri->append(QString("Total %1 riwayat pelanggaran ditampilkan.").arg(baris));
    }
}

void AntarmukaUtama::on_santri_tampilkanAturan() {
    bersihkanTabel(tabelSantri);
    tabelSantri->setColumnCount(3);
    tabelSantri->setHorizontalHeaderLabels(QStringList() << "KODE" << "NAMA PELANGGARAN" << "BOBOT POIN");

    if (headAturan == NULL) {
        areaOutputSantri->append("Belum ada aturan pelanggaran.");
        return;
    }

    struct NodeAturan *current = headAturan;
    int baris = 0;
    do {
        tabelSantri->insertRow(baris);
        tabelSantri->setItem(baris, 0, new QTableWidgetItem(QString::number(current->data.kodePelanggaran)));
        tabelSantri->setItem(baris, 1, new QTableWidgetItem(current->data.namaPelanggaran));
        tabelSantri->setItem(baris, 2, new QTableWidgetItem(QString::number(current->data.bobotPoin)));
        baris++;
        current = current->next;
    } while (current != headAturan);

    areaOutputSantri->append(QString("Total %1 aturan pelanggaran (read-only).").arg(baris));
}

void AntarmukaUtama::on_santri_logout() {
    simpanKeTeks();
    dataSantriLogin = NULL;
    areaOutputSantri->clear();
    bersihkanTabel(tabelSantri);
    tumpukanHalaman->setCurrentIndex(0);
    labelStatusLogin->setText("Logout berhasil. Silakan login kembali.");
}