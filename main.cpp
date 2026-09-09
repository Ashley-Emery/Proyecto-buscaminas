#include <QApplication>
#include <QWidget>

#include "Partida.h"
#include "Usuarios.h"


// -----------------------------------------------------------------------------
// PROTOTIPOS DE LAS INTERFACES
// -----------------------------------------------------------------------------

// MENU UI
// Cuando terminemos MenusUI.cpp, esta sera la funcion que abrira los menus.
QWidget* crearMenusUI();


// JUEGO UI
QWidget* crearJuegoUI(
    Usuario* usuarioActual,
    SistemaUsuarios* sistemaUsuarios,
    ModoJuego modo,
    int numeroNivel
);


// -----------------------------------------------------------------------------
// MAIN
// -----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    QApplication aplicacion(argc, argv);


    // =========================================================================
    // PRUEBA 1: MENUS UI
    // =========================================================================
    //
    // Para probar MenusUI.cpp:
    //
    // 1. Descomentar este bloque.
    // 2. Comentar el bloque de JuegoUI.
    //
    QWidget* ventanaMenu = crearMenusUI();
    ventanaMenu->show();


    // =========================================================================
    // PRUEBA 2: JUEGO UI
    // =========================================================================
    //
    // Para probar JuegoUI.cpp:
    //
    // Puedes cambiar:
    //
    // ModoJuego::PROGRESIVO
    // ModoJuego::MAPA
    //
    // y tambien cambiar numeroNivel entre 1 y 9.
    //
    // Por ahora se utilizan nullptr para Usuario y SistemaUsuarios porque
    // estamos probando JuegoUI de forma independiente, sin iniciar sesion.
    // =========================================================================

    // QWidget* ventanaJuego = crearJuegoUI(
    //     nullptr,
    //     nullptr,
    //     ModoJuego::PROGRESIVO,
    //     1
    // );

    // ventanaJuego->show();


    return aplicacion.exec();
}