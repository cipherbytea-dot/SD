#ifndef ANTARMUKA_UTAMA_H
#define ANTARMUKA_UTAMA_H

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
#include "kerangka_data.h"

class AntarmukaUtama : public QMainWindow {
    Q_OBJECT

public:
    explicit AntarmukaUtama(QWidget *parent = nullptr);
    ~AntarmukaUtama();

private slots:
    // Halaman Login
    void on_tombolLoginPimpinan_clicked();
    void on_tombolLoginPengurus_clicked();
    void on_tombolLoginSantri_clicked();

    // Halaman Pimpinan
    void on_pimpinan_tampilkanZonaMerah();
    void on_pimpinan_tampilkanAturan();
    void on_pimpinan_tambahAturan();
    void on_pimpinan_scrollAturanNext();
    void on_pimpinan_scrollAturanPrev();
    void on_pimpinan_updateBobot();
    void on_pimpinan_hapusAturan();
    void on_pimpinan_resetPoin();
    void on_pimpinan_statistikBulanan();
    void on_pimpinan_tampilkanJadwal();
    void on_pimpinan_tampilkanPeta();
    void on_pimpinan_logout();

    // Halaman Pengurus
    void on_pengurus_tampilkanSantri();
    void on_pengurus_tambahSantri();
    void on_pengurus_navigasiNext();
    void on_pengurus_navigasiPrev();
    void on_pengurus_hapusSantri();
    void on_pengurus_inputPelanggaran();
    void on_pengurus_prosesAntrean();
    void on_pengurus_undoTerakhir();
    void on_pengurus_cekAntrean();
    void on_pengurus_tampilkanAturan();
    void on_pengurus_tampilkanPeta();
    void on_pengurus_tampilkanJadwal();
    void on_pengurus_logout();
    void on_pengurus_pilihAturanChanged(int index);
    // V2: Fitur 1 - Update Santri
    void on_pengurus_updateSantri();
    // V2: Fitur 2 - Cari Santri BST
    void on_pengurus_cariSantri();

    // Halaman Santri
    void on_santri_tampilkanProfil();
    void on_santri_tampilkanRiwayat();
    void on_santri_tampilkanAturan();
    void on_santri_logout();

private:
    void bangunHalamanLogin();
    void bangunHalamanPimpinan();
    void bangunHalamanPengurus();
    void bangunHalamanSantri();

    void bersihkanTabel(QTableWidget *tabel);
    void tampilkanPesan(const QString &judul, const QString &isi);
    void perbaruiLabelSantriAktif();
    void perbaruiLabelAturanAktif();
    void perbaruiComboAturan();

    QStackedWidget *tumpukanHalaman;

    // Login
    QWidget *halamanLogin;
    QLineEdit *kotakUserPimpinan;
    QLineEdit *kotakPassPimpinan;
    QLineEdit *kotakUserPengurus;
    QLineEdit *kotakPassPengurus;
    QLineEdit *kotakNisSantri;
    QPushButton *tombolLoginPimpinan;
    QPushButton *tombolLoginPengurus;
    QPushButton *tombolLoginSantri;
    QLabel *labelStatusLogin;

    // Pimpinan
    QWidget *halamanPimpinan;
    QLabel *labelJudulPimpinan;
    QLabel *labelAturanAktif;
    QTableWidget *tabelPimpinan;
    QTextEdit *areaOutputPimpinan;
    QSpinBox *kotakKodeAturan;
    QLineEdit *kotakNamaAturan;
    QSpinBox *kotakBobotAturan;
    QSpinBox *kotakUpdateBobot;
    QLineEdit *kotakHapusKode;

    // Pengurus
    QWidget *halamanPengurus;
    QLabel *labelJudulPengurus;
    QLabel *labelSantriAktif;
    QTableWidget *tabelPengurus;
    QTextEdit *areaOutputPengurus;
    QLineEdit *kotakNisBaru;
    QLineEdit *kotakNamaBaru;
    QLineEdit *kotakKelasBaru;
    QLineEdit *kotakKamarBaru;
    QLineEdit *kotakNisPelanggaran;
    QComboBox *comboAturanPelanggaran;
    QLabel *labelBobotTerpilih;
    QLineEdit *kotakKeteranganPelanggaran;
    QLineEdit *kotakNisHapus;
    // V2: Widget Update Santri
    QLineEdit *kotakNisUpdate;
    QLineEdit *kotakNamaUpdate;
    QLineEdit *kotakKelasUpdate;
    QLineEdit *kotakKamarUpdate;
    QPushButton *tombolUpdateSantri;
    // V2: Widget Cari Santri BST
    QLineEdit *kotakNisCari;
    QPushButton *tombolCariSantri;

    // Santri
    QWidget *halamanSantri;
    QLabel *labelJudulSantri;
    QLabel *labelProfilNis;
    QLabel *labelProfilNama;
    QLabel *labelProfilKelas;
    QLabel *labelProfilKamar;
    QLabel *labelProfilPoin;
    QTableWidget *tabelSantri;
    QTextEdit *areaOutputSantri;

    struct NodeBST *dataSantriLogin;
};

#endif // ANTARMUKA_UTAMA_H