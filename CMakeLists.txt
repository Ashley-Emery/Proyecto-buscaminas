cmake_minimum_required(VERSION 3.5)

project(Proyecto-buscaminas LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Proyecto-buscaminas
    main.cpp
    Tablero.cpp
    Partida.cpp
    Niveles.cpp
    Usuarios.cpp
    Persistencia.cpp
    PuntajeLogros.cpp
)