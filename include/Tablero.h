#ifndef TABLERO_H
#define TABLERO_H

class ArchivoPersistencia;


// -----------------------------------------------------------------------------
// CLASE 1: CELDA
// Representa una casilla individual del tablero.
// -----------------------------------------------------------------------------
class Celda {
    private:
        bool contieneMina;
        bool revelada;
        bool marcada;
        int minasVecinas;

    public:
        Celda();

        bool tieneMina() const;
        bool estaRevelada() const;
        bool estaMarcada() const;
        int obtenerMinasVecinas() const;

        void establecerMina(bool valor);
        void establecerRevelada(bool valor);
        void establecerMarcada(bool valor);
        void establecerMinasVecinas(int cantidad);

        friend class ArchivoPersistencia;
};


// -----------------------------------------------------------------------------
// CLASE 2: TABLERO
// Administra la matriz dinámica Celda** y toda la lógica de la grilla.
// -----------------------------------------------------------------------------
class Tablero {
    private:
        Celda** celdas;
        int filas;
        int columnas;
        int cantidadMinas;
        bool minasSembradas;

        void reservarMemoria();
        void liberarMemoria();
        bool posicionValida(int fila, int columna) const;
        int contarMinasAlrededor(int fila, int columna) const;
        void calcularNumeros();
        void expansionRecursiva(int fila, int columna);

    public:
        Tablero();
        Tablero(int filas, int columnas, int cantidadMinas);
        ~Tablero();

        Tablero(const Tablero&) = delete;
        Tablero &operator=(const Tablero&) = delete;

        void inicializar(int filas, int columnas, int cantidadMinas);
        void reiniciar();

        // Primer clic seguro: se llama después de conocer la primera casilla pulsada.
        bool sembrarMinas(int filaSegura, int columnaSegura);

        bool revelarCelda(int fila, int columna);
        bool alternarBandera(int fila, int columna);
        
        void revelarTodasLasMinas();

        bool verificarVictoria() const;
        bool todasLasMinasMarcadasCorrectamente() const;

        int contarBanderas() const;
        int contarCeldasReveladas() const;

        int obtenerFilas() const;
        int obtenerColumnas() const;
        int obtenerCantidadMinas() const;
        bool estanSembradasLasMinas() const;

        const Celda* obtenerCelda(int fila, int columna) const;
        Celda* obtenerCelda(int fila, int columna);

        friend class ArchivoPersistencia;
};

#endif