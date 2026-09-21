#include <array>
#include <algorithm>
#include <cmath>
#include <random>
#include <string>

#include <iostream>

using namespace std;


// -----------------------------------------------------------------------------
// CONSTANTES DEL MODULO INVESTIGATIVO
// -----------------------------------------------------------------------------

const int FILAS_INVESTIGATIVO = 5;
const int COLUMNAS_INVESTIGATIVO = 5;
const int MINAS_INVESTIGATIVO = 5;


// -----------------------------------------------------------------------------
// POSICION DE UNA MINA
// -----------------------------------------------------------------------------

struct PosicionMinaInvestigativa {
    int fila;
    int columna;
};


// -----------------------------------------------------------------------------
// LIMPIAR MATRIZ DE MINAS
// -----------------------------------------------------------------------------

void limpiarMinasInvestigativas(bool minas[5][5]) {

    for (int fila = 0; fila < FILAS_INVESTIGATIVO; fila++) {
        for (int columna = 0; columna < COLUMNAS_INVESTIGATIVO; columna++) {

            minas[fila][columna] = false;
        }
    }
}


// -----------------------------------------------------------------------------
// GENERAR 5 MINAS ALEATORIAS
// -----------------------------------------------------------------------------

void generarMinasInvestigativas(bool minas[5][5]) {

    limpiarMinasInvestigativas(minas);

    array<int, 25> posiciones;

    for (int i = 0; i < 25; i++) {
        posiciones[i] = i;
    }

    static mt19937 generadorAleatorio(random_device{}());

    shuffle(posiciones.begin(), posiciones.end(), generadorAleatorio);

    for (int i = 0; i < MINAS_INVESTIGATIVO; i++) {

        int posicion = posiciones[i];
        int fila = posicion / COLUMNAS_INVESTIGATIVO;
        int columna = posicion % COLUMNAS_INVESTIGATIVO;

        minas[fila][columna] = true;
    }
}


// -----------------------------------------------------------------------------
// CONTAR MINAS VECINAS
// -----------------------------------------------------------------------------

int contarMinasVecinasInvestigativas(const bool minas[5][5], int fila, int columna) {

    int cantidad = 0;

    for (int desplazamientoFila = -1; desplazamientoFila <= 1; desplazamientoFila++) {
        
        for (int desplazamientoColumna = -1; desplazamientoColumna <= 1; desplazamientoColumna++) {

            if (desplazamientoFila == 0 && desplazamientoColumna == 0) {
                continue;
            }

            int nuevaFila = fila + desplazamientoFila;
            int nuevaColumna = columna + desplazamientoColumna;

            if (nuevaFila < 0 || nuevaFila >= FILAS_INVESTIGATIVO || nuevaColumna < 0 || nuevaColumna >= COLUMNAS_INVESTIGATIVO) {
                continue;
            }

            if (minas[nuevaFila][nuevaColumna]) {
                cantidad++;
            }
        }
    }

    return cantidad;
}


// -----------------------------------------------------------------------------
// DISTANCIA MANHATTAN
// -----------------------------------------------------------------------------

int calcularDistanciaManhattanInvestigativa(const PosicionMinaInvestigativa& mina1, const PosicionMinaInvestigativa& mina2) {

    return abs(mina1.fila - mina2.fila) + abs(mina1.columna - mina2.columna);
}


// -----------------------------------------------------------------------------
// PROMEDIO DE LAS 10 DISTANCIAS ENTRE MINAS
// -----------------------------------------------------------------------------

double calcularDistanciaPromedioInvestigativa(const bool minas[5][5]) {

    array<PosicionMinaInvestigativa, MINAS_INVESTIGATIVO> posiciones;

    int indice = 0;

    for (int fila = 0; fila < FILAS_INVESTIGATIVO; fila++) {
        for (int columna = 0; columna < COLUMNAS_INVESTIGATIVO; columna++) {

            if (!minas[fila][columna]) {
                continue;
            }

            posiciones[indice].fila = fila;
            posiciones[indice].columna = columna;

            indice++;
        }
    }

    int sumaDistancias = 0;
    int cantidadPares = 0;

    for (int i = 0; i < MINAS_INVESTIGATIVO; i++) {
        for (int j = i + 1; j < MINAS_INVESTIGATIVO; j++) {

            sumaDistancias += calcularDistanciaManhattanInvestigativa(posiciones[i], posiciones[j]);
            cantidadPares++;
        }
    }

    if (cantidadPares == 0) {
        return 0.0;
    }

    return static_cast<double>(sumaDistancias) / cantidadPares;
}


// -----------------------------------------------------------------------------
// CLASIFICAR DIFICULTAD ESPACIAL
// -----------------------------------------------------------------------------

string clasificarNivelInvestigativo(double promedio) {

    if (promedio <= 2.6) {
        return "Básico";
    }

    if (promedio <= 3.0) {
        return "Fácil";
    }

    if (promedio <= 3.4) {
        return "Medio";
    }

    if (promedio <= 3.8) {
        return "Difícil";
    }

    return "Experto";
}

// -----------------------------------------------------------------------------
// MOSTRAR GRID INVESTIGATIVO EN CONSOLA
// -----------------------------------------------------------------------------

void mostrarGridInvestigativo(const bool minas[5][5]) {

    cout << "\nGrid investigativo:\n\n";

    for (int fila = 0; fila < FILAS_INVESTIGATIVO; fila++) {
        for (int columna = 0; columna < COLUMNAS_INVESTIGATIVO; columna++) {

            if (minas[fila][columna]) {
                cout << "X ";
            } else {
                cout << ". ";
            }
        }

        cout << '\n';
    }

    cout << '\n';
}