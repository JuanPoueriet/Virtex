cmake_minimum_required(VERSION 3.16)

project(ChromeTabsQt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Gui Qml Quick)

qt_add_executable(ChromeTabsQt
    src/main.cpp
    src/TabSession.h
    src/TabSession.cpp
    src/TabView.h
    src/TabView.cpp
    src/WindowManager.h
    src/WindowManager.cpp
)

qt_add_qml_module(ChromeTabsQt
    URI ChromeTabsQt
    VERSION 1.0
    QML_FILES
        src/Main.qml
        src/TabContainer.qml
        src/TabItemView.qml
)

target_link_libraries(ChromeTabsQt PRIVATE Qt6::Gui Qt6::Qml Qt6::Quick)
