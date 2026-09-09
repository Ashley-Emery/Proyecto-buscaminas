cmake_minimum_required(VERSION 3.5)

project(Proyecto-buscaminas LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS
    Widgets
    Multimedia
    MultimediaWidgets
)

add_executable(Proyecto-buscaminas
    main.cpp
    Tablero.cpp
    Partida.cpp
    Niveles.cpp
    Usuarios.cpp
    Persistencia.cpp
    PuntajeLogros.cpp
    Assets.cpp
    Bosses.cpp
    JuegoUI.cpp
    MenusUI.cpp
)

target_link_libraries(Proyecto-buscaminas
    Qt5::Widgets
    Qt5::Multimedia
    Qt5::MultimediaWidgets
)