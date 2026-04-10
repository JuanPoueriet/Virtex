cmake_minimum_required(VERSION 3.16)

project(ChromeTabs VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)
pkg_check_modules(ADWAITA REQUIRED IMPORTED_TARGET libadwaita-1)

add_executable(ChromeTabs
    src/main.c
)

target_link_libraries(ChromeTabs PRIVATE
    PkgConfig::GTK4
    PkgConfig::ADWAITA
)
