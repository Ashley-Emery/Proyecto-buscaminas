#include "Tablero.h"

#include <algorithm>
#include <random>

using namespace std;

// CELDA
Celda::Celda() : contieneMina(false), revelada(false), marcada(false), minasVecinas(0) {
}

bool Celda::tieneMina() const {
    return contieneMina;
}

bool Celda::estaRevelada() const {
    return revelada;
}

bool Celda::estaMarcada() const {
    return marcada;
}

int Celda::obtenerMinasVecinas() const {
    return minasVecinas;
}

void Celda::establecerMina(bool valor) {
    contieneMina = valor;
}

void Celda::establecerRevelada(bool valor) {
    revelada = valor;
}

void Celda::establecerMarcada(bool valor) {
    marcada = valor;
}

void Celda::establecerMinasVecinas(int cantidad) {
    minasVecinas = cantidad;
}


// TABLERO
Tablero::Tablero() : celdas(nullptr), filas(0), columnas(0), cantidadMinas(0), minasSembradas(false) {
}

Tablero::Tablero(int filas, int columnas, int cantidadMinas)
    : celdas(nullptr), filas(0), columnas(0), cantidadMinas(0), minasSembradas(false) {

    inicializar(filas, columnas, cantidadMinas);
}

Tablero::~Tablero() {
    liberarMemoria();
}

void Tablero::reservarMemoria() {
    celdas = new Celda*[filas];

    for (int fila = 0; fila < filas; fila++) {
        celdas[fila] = new Celda[columnas];
    }
}

void Tablero::liberarMemoria() {
    if (celdas == nullptr) {
        return;
    }

    for (int fila = 0; fila < filas; fila++) {
        delete[] celdas[fila];
        celdas[fila] = nullptr;
    }

    delete[] celdas;
    celdas = nullptr;

    filas = 0;
    columnas = 0;
    cantidadMinas = 0;
    minasSembradas = false;
}

bool Tablero::posicionValida(int fila, int columna) const {
    return fila >= 0 && fila < filas && columna >= 0 && columna < columnas;
}

void Tablero::inicializar(int nuevasFilas, int nuevasColumnas, int nuevasMinas) {
    liberarMemoria();

    filas = nuevasFilas;
    columnas = nuevasColumnas;
    cantidadMinas = nuevasMinas;
    minasSembradas = false;

    reservarMemoria();
}

void Tablero::reiniciar() {
    if (filas <= 0 || columnas <= 0) {
        return;
    }

    for (int fila = 0; fila < filas; fila++) {
        for (int columna = 0; columna < columnas; columna++) {
            celdas[fila][columna] = Celda();
        }
    }

    minasSembradas = false;
}

bool Tablero::sembrarMinas(int filaSegura, int columnaSegura) {
    if (!posicionValida(filaSegura, columnaSegura) || minasSembradas) {
        return false;
    }

    const int totalCeldas = filas * columnas;
    const int disponibles = totalCeldas - 1;

    if (cantidadMinas <= 0 || cantidadMinas > disponibles) {
        return false;
    }

    int* posiciones = new int[disponibles];
    int indice = 0;

    for (int fila = 0; fila < filas; fila++) {
        for (int columna = 0; columna < columnas; columna++) {
            if (fila == filaSegura && columna == columnaSegura) {
                continue;
            }

            posiciones[indice] = fila * columnas + columna;
            indice++;
        }
    }

    random_device dispositivoAleatorio;
    mt19937 generador(dispositivoAleatorio());
    shuffle(posiciones, posiciones + disponibles, generador);

    for (int i = 0; i < cantidadMinas; i++) {
        int posicion = posiciones[i];
        int fila = posicion / columnas;
        int columna = posicion % columnas;

        celdas[fila][columna].establecerMina(true);
    }

    delete[] posiciones;
    posiciones = nullptr;

    minasSembradas = true;
    calcularNumeros();
    return true;
}

int Tablero::contarMinasAlrededor(int fila, int columna) const {
    int total = 0;

    for (int desplazamientoFila = -1; desplazamientoFila <= 1; desplazamientoFila++) {
        for (int desplazamientoColumna = -1; desplazamientoColumna <= 1; desplazamientoColumna++) {
            if (desplazamientoFila == 0 && desplazamientoColumna == 0) {
                continue;
            }

            int filaVecina = fila + desplazamientoFila;
            int columnaVecina = columna + desplazamientoColumna;

            if (posicionValida(filaVecina, columnaVecina) &&
                celdas[filaVecina][columnaVecina].tieneMina()) {
                total++;
            }
        }
    }

    return total;
}

void Tablero::calcularNumeros() {
    for (int fila = 0; fila < filas; fila++) {
        for (int columna = 0; columna < columnas; columna++) {
            if (!celdas[fila][columna].tieneMina()) {
                celdas[fila][columna].establecerMinasVecinas(
                    contarMinasAlrededor(fila, columna)
                );
            }
        }
    }
}

void Tablero::expansionRecursiva(int fila, int columna) {
    if (!posicionValida(fila, columna)) {
        return;
    }

    Celda &celda = celdas[fila][columna];

    if (celda.estaRevelada() ||
        celda.estaMarcada() ||
        celda.tieneMina()) {
        return;
    }

    celda.establecerRevelada(true);

    if (celda.obtenerMinasVecinas() > 0) {
        return;
    }

    for (int desplazamientoFila = -1; desplazamientoFila <= 1; desplazamientoFila++) {
        for (int desplazamientoColumna = -1; desplazamientoColumna <= 1; desplazamientoColumna++) {
            if (desplazamientoFila == 0 && desplazamientoColumna == 0) {
                continue;
            }

            expansionRecursiva(
                fila + desplazamientoFila,
                columna + desplazamientoColumna
            );
        }
    }
}

bool Tablero::revelarCelda(int fila, int columna) {
    if (!posicionValida(fila, columna)) {
        return false;
    }

    Celda &celda = celdas[fila][columna];

    if (celda.estaRevelada() || celda.estaMarcada()) {
        return false;
    }

    if (celda.tieneMina()) {
        celda.establecerRevelada(true);
        return true;
    }

    expansionRecursiva(fila, columna);
    return true;
}

bool Tablero::alternarBandera(int fila, int columna) {
    if (!posicionValida(fila, columna)) {
        return false;
    }

    Celda &celda = celdas[fila][columna];

    if (celda.estaRevelada()) {
        return false;
    }

    celda.establecerMarcada(!celda.estaMarcada());
    return true;
}

void Tablero::revelarTodasLasMinas() {

    if (celdas == nullptr || !minasSembradas) {
        return;
    }

    for (int fila = 0; fila < filas; fila++) {

        for (int columna = 0; columna < columnas; columna++) {

            if (celdas[fila][columna].tieneMina()) {
                celdas[fila][columna].establecerRevelada(true);
            }
        }
    }
}

bool Tablero::verificarVictoria() const {
    if (!minasSembradas) {
        return false;
    }

    for (int fila = 0; fila < filas; fila++) {
        for (int columna = 0; columna < columnas; columna++) {
            const Celda &celda = celdas[fila][columna];

            if (!celda.tieneMina() && !celda.estaRevelada()) {
                return false;
            }
        }
    }

    return true;
}

bool Tablero::todasLasMinasMarcadasCorrectamente() const {
    if (!minasSembradas) {
        return false;
    }

    int minasMarcadas = 0;

    for (int fila = 0; fila < filas; fila++) {
        for (int columna = 0; columna < columnas; columna++) {
            const Celda &celda = celdas[fila][columna];

            if (celda.estaMarcada() && !celda.tieneMina()) {
                return false;
            }

            if (celda.estaMarcada() && celda.tieneMina()) {
                minasMarcadas++;
            }
        }
    }

    return minasMarcadas == cantidadMinas;
}

int Tablero::contarBanderas() const {
    int total = 0;

    for (int fila = 0; fila < filas; fila++) {
        for (int columna = 0; columna < columnas; columna++) {
            if (celdas[fila][columna].estaMarcada()) {
                total++;
            }
        }
    }

    return total;
}

int Tablero::contarCeldasReveladas() const {
    int total = 0;

    for (int fila = 0; fila < filas; fila++) {
        for (int columna = 0; columna < columnas; columna++) {
            if (celdas[fila][columna].estaRevelada()) {
                total++;
            }
        }
    }

    return total;
}

int Tablero::obtenerFilas() const {
    return filas;
}

int Tablero::obtenerColumnas() const {
    return columnas;
}

int Tablero::obtenerCantidadMinas() const {
    return cantidadMinas;
}

bool Tablero::estanSembradasLasMinas() const {
    return minasSembradas;
}

const Celda* Tablero::obtenerCelda(int fila, int columna) const {
    if (!posicionValida(fila, columna)) {
        return nullptr;
    }

    return &celdas[fila][columna];
}

Celda* Tablero::obtenerCelda(int fila, int columna) {
    if (!posicionValida(fila, columna)) {
        return nullptr;
    }

    return &celdas[fila][columna];
}
