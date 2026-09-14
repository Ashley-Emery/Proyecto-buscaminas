#include <QApplication>
#include <QWidget>

#include "Partida.h"
#include "Usuarios.h"

QWidget* crearMenusUI();

int main(int argc, char *argv[])
{
    QApplication aplicacion(argc, argv);

    QWidget* ventanaMenu = crearMenusUI();
    ventanaMenu->show();

    return aplicacion.exec();
}