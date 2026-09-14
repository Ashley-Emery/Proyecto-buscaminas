cmake_minimum_required(VERSION 3.16)

project(Proyecto-buscaminas LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Multimedia
    MultimediaWidgets
)

add_executable(Proyecto-buscaminas
    main.cpp
    Tablero.cpp Tablero.h
    Partida.cpp Partida.h
    Niveles.cpp
    Usuarios.cpp Usuarios.h
    Persistencia.cpp
    PuntajeLogros.cpp
    Assets.cpp Assets.h
    Bosses.cpp

)

target_link_libraries(Proyecto-buscaminas PRIVATE
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
)