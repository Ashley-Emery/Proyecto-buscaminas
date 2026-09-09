#include "Partida.h"

Partida::Partida() : tablero(nullptr), estado(EstadoPartida::NO_INICIADA), tiempoRestanteSegundos(0), utilizoBanderas(false), resultadoRegistrado(false) {
}

Partida::Partida(const ConfiguracionPartida &configuracion) : tablero(nullptr), estado(EstadoPartida::NO_INICIADA),
    tiempoRestanteSegundos(0), utilizoBanderas(false), resultadoRegistrado(false) {
    
        iniciar(configuracion);
}

Partida::~Partida() {
    delete tablero;
    tablero = nullptr;
}

bool Partida::iniciar(const ConfiguracionPartida &nuevaConfiguracion) {
    if (!nuevaConfiguracion.esValida()) {
        return false;
    }

    delete tablero;
    tablero = nullptr;

    configuracion = nuevaConfiguracion;

    tablero = new Tablero( configuracion.obtenerFilas(), configuracion.obtenerColumnas(), configuracion.obtenerMinas());

    estado = EstadoPartida::NO_INICIADA;
    tiempoRestanteSegundos = configuracion.obtenerTiempoLimiteSegundos();
    utilizoBanderas = false;
    resultadoRegistrado = false;

    return true;
}

bool Partida::procesarClicIzquierdo(int fila, int columna) {
    if (tablero == nullptr || estaFinalizada()) {
        return false;
    }

    Celda* celda = tablero->obtenerCelda(fila, columna);

    if (celda == nullptr || celda->estaMarcada() || celda->estaRevelada()) {
        return false;
    }

    // Las minas se crean despues de conocer el primer clic.
    if (!tablero->estanSembradasLasMinas()) {
        if (!tablero->sembrarMinas(fila, columna)) {
            return false;
        }

        estado = EstadoPartida::EN_CURSO;
    }

    tablero->revelarCelda(fila, columna);

    celda = tablero->obtenerCelda(fila, columna);

    if (celda != nullptr && celda->tieneMina()) {
        estado = EstadoPartida::PERDIDA;
        return true;
    }

    if (tablero->verificarVictoria()) {
        estado = EstadoPartida::GANADA;
    }

    return true;
}

bool Partida::procesarClicDerecho(int fila, int columna) {
    if (tablero == nullptr || estaFinalizada()) {
        return false;
    }

    // Antes del primer clic izquierdo se permiten marcadores, pero las minas aun no existen.
    if (tablero->alternarBandera(fila, columna)) {
        utilizoBanderas = true;
        return true;
    }

    return false;
}

void Partida::actualizarCronometro() {
    if (estado != EstadoPartida::EN_CURSO) {
        return;
    }

    if (tiempoRestanteSegundos > 0) {
        tiempoRestanteSegundos--;
    }

    if (tiempoRestanteSegundos <= 0) {
        tiempoRestanteSegundos = 0;
        estado = EstadoPartida::TIEMPO_AGOTADO;
    }
}

void Partida::revelarMinasAlFinalizar() {

    if (tablero == nullptr) {
        return;
    }

    if (estado != EstadoPartida::PERDIDA && estado != EstadoPartida::TIEMPO_AGOTADO) {
        return;
    }

    tablero->revelarTodasLasMinas();
}

bool Partida::estaFinalizada() const {
    return estado == EstadoPartida::GANADA || estado == EstadoPartida::PERDIDA || estado == EstadoPartida::TIEMPO_AGOTADO;
}

bool Partida::gano() const {
    return estado == EstadoPartida::GANADA;
}

bool Partida::perdio() const {
    return estado == EstadoPartida::PERDIDA;
}

bool Partida::seAgotoElTiempo() const {
    return estado == EstadoPartida::TIEMPO_AGOTADO;
}

int Partida::obtenerTiempoRestante() const {
    return tiempoRestanteSegundos;
}

int Partida::obtenerMinasRestantes() const {
    if (tablero == nullptr) {
        return 0;
    }

    int restantes = tablero->obtenerCantidadMinas() - tablero->contarBanderas();

    return restantes < 0 ? 0 : restantes;
}

bool Partida::usoBanderas() const {
    return utilizoBanderas;
}

bool Partida::yaRegistroResultado() const {
    return resultadoRegistrado;
}

void Partida::marcarResultadoRegistrado() {
    resultadoRegistrado = true;
}

EstadoPartida Partida::obtenerEstado() const {
    return estado;
}

const ConfiguracionPartida &Partida::obtenerConfiguracion() const {
    return configuracion;
}

const Tablero* Partida::obtenerTablero() const {
    return tablero;
}

Tablero* Partida::obtenerTablero() {
    return tablero;
}
