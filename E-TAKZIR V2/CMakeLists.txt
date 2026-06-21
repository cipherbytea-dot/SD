# ============================================================
# E-TAKZIR: Konfigurasi Build CMake untuk Qt6
# Paradigma: Pemrograman Prosedural Murni (C++) + Qt Widgets Framework
# ============================================================

cmake_minimum_required(VERSION 3.16)

# Nama proyek dan bahasa
project(E-TAKZIR VERSION 1.0 LANGUAGES CXX)

# Standar C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Cari package Qt6 (Widgets, Core, Gui)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

# ============================================================
# Daftar file sumber .cpp (logika prosedural + UI Qt)
# ============================================================
set(SUMBER
    src/main.cpp
    src/antarmuka_utama.cpp
    src/baca_tulis_file.cpp
    src/kelola_data_santri.cpp
    src/proses_pelanggaran.cpp
    src/jadwal_dan_aturan.cpp
)

# ============================================================
# Daftar file header .h (agar MOC Qt memproses signal/slot)
# ============================================================
set(HEADER
    src/kerangka_data.h
    src/kumpulan_fungsi.h
    src/antarmuka_utama.h
)

# ============================================================
# Buat executable
# ============================================================
add_executable(${PROJECT_NAME} ${SUMBER} ${HEADER})

# ============================================================
# Link dengan library Qt6
# ============================================================
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
)

# ============================================================
# Include direktori src agar header bisa ditemukan
# ============================================================
target_include_directories(${PROJECT_NAME} PRIVATE src)

# ============================================================
# Platform-specific: Windows
# ============================================================
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# ============================================================
# Catatan Build:
#   mkdir build && cd build && cmake .. && cmake --build .
# ============================================================
