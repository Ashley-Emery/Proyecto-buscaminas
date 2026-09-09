#include "Partida.h"

#include <algorithm>

namespace {
    int obtenerTiempoNivel(int numeroNivel) {
        switch (numeroNivel) {

            case 1: return 180;
            case 2: return 180;
            case 3: return 180;

            case 4: return 420;
            case 5: return 420;
            case 6: return 420;

            case 7: return 720;
            case 8: return 720;
            case 9: return 720;

            default: return 0;
        }
    }

    int calcularTiempoPersonalizado(int filas, int columnas) {
        const int celdasMinimas = FILAS_MINIMAS * COLUMNAS_MINIMAS;
        const int celdasMaximas = FILAS_MAXIMAS * COLUMNAS_MAXIMAS;
        const int celdasActuales = filas * columnas;

        // Escala lineal propuesta: 180 s para 8x8 hasta 720 s para 12x40.
        int tiempo = 180 + ((celdasActuales - celdasMinimas) * (720 - 180)) / (celdasMaximas - celdasMinimas);

        return max(180, min(720, tiempo));
    }
}

ConfiguracionPartida::ConfiguracionPartida()
    : filas(0),
      columnas(0),
      minas(0),
      tiempoLimiteSegundos(0),
      numeroNivel(0),
      dificultad(Dificultad::PERSONALIZADO),
      modo(ModoJuego::PERSONALIZADO) {
}

ConfiguracionPartida::ConfiguracionPartida(
    int filas,
    int columnas,
    int minas,
    int tiempoLimiteSegundos,
    int numeroNivel,
    Dificultad dificultad,
    ModoJuego modo
)
    : filas(filas),
      columnas(columnas),
      minas(minas),
      tiempoLimiteSegundos(tiempoLimiteSegundos),
      numeroNivel(numeroNivel),
      dificultad(dificultad),
      modo(modo) {
}

int ConfiguracionPartida::obtenerFilas() const {
    return filas;
}

int ConfiguracionPartida::obtenerColumnas() const {
    return columnas;
}

int ConfiguracionPartida::obtenerMinas() const {
    return minas;
}

int ConfiguracionPartida::obtenerTiempoLimiteSegundos() const {
    return tiempoLimiteSegundos;
}

int ConfiguracionPartida::obtenerNumeroNivel() const {
    return numeroNivel;
}

Dificultad ConfiguracionPartida::obtenerDificultad() const {
    return dificultad;
}

ModoJuego ConfiguracionPartida::obtenerModo() const {
    return modo;
}

bool ConfiguracionPartida::esValida() const {
    return filas > 0 && columnas > 0 && minas > 0 && minas < filas * columnas && tiempoLimiteSegundos > 0;
}

ConfiguracionPartida ConfiguracionPartida::crearNivel(int numeroNivel, ModoJuego modo) {

    if (numeroNivel < 1 || numeroNivel > CANTIDAD_NIVELES) {
        return ConfiguracionPartida();
    }

    if (modo == ModoJuego::PERSONALIZADO) {
        return ConfiguracionPartida();
    }

    if (numeroNivel <= 3) {
        return ConfiguracionPartida(8, 8, 10, obtenerTiempoNivel(numeroNivel), numeroNivel, Dificultad::BASICO, modo);
    }

    if (numeroNivel <= 6) {
        return ConfiguracionPartida(8, 32, 40, obtenerTiempoNivel(numeroNivel), numeroNivel, Dificultad::INTERMEDIO, modo);
    }

    return ConfiguracionPartida(12, 40, 99, obtenerTiempoNivel(numeroNivel), numeroNivel, Dificultad::EXPERTO, modo);
}

ConfiguracionPartida ConfiguracionPartida::crearPersonalizada(int filas, int columnas) {

    string mensajeError;

    if (!validarConfiguracionPersonalizada(filas, columnas, mensajeError)) {
        return ConfiguracionPartida();
    }

    int minas = calcularMinasPersonalizadas(filas, columnas);

    return ConfiguracionPartida(
        filas,
        columnas,
        minas,
        calcularTiempoPersonalizado(filas, columnas),
        0,
        Dificultad::PERSONALIZADO,
        ModoJuego::PERSONALIZADO
    );
}

int calcularMinasPersonalizadas(int filas, int columnas) {

    const int celdasMinimas = FILAS_MINIMAS * COLUMNAS_MINIMAS;
    const int celdasMaximas = FILAS_MAXIMAS * COLUMNAS_MAXIMAS;

    int cantidadCeldas = filas * columnas;

    cantidadCeldas = max(celdasMinimas, min(celdasMaximas, cantidadCeldas));

    int minas = 10 + ((cantidadCeldas - celdasMinimas) * 89) / (celdasMaximas - celdasMinimas);

    return minas;
}

bool validarConfiguracionPersonalizada(int filas, int columnas, string &mensajeError) {

    if (filas < FILAS_MINIMAS || filas > FILAS_MAXIMAS) {
        mensajeError = "Las filas deben estar entre 8 y 12.";
        return false;
    }

    if (columnas < COLUMNAS_MINIMAS || columnas > COLUMNAS_MAXIMAS) {
        mensajeError = "Las columnas deben estar entre 8 y 40.";
        return false;
    }

    mensajeError.clear();
    return true;
}

string obtenerTextoDificultad(Dificultad dificultad) {
    switch (dificultad) {
        case Dificultad::BASICO:{
            return "Basico";
        }
        case Dificultad::INTERMEDIO:{
            return "Intermedio";
        }
        case Dificultad::EXPERTO:{
            return "Experto";
        }
        case Dificultad::PERSONALIZADO:{
            return "Personalizado";
        }
    }

    return "Desconocida";
}

string obtenerTextoModo(ModoJuego modo) {

    switch (modo) {
        case ModoJuego::PROGRESIVO:{
            return "Progresivo";
        }
        case ModoJuego::MAPA:{
            return "Mapa";
        }
        case ModoJuego::PERSONALIZADO:{
            return "Personalizado";
        }
    }

    return "Desconocido";
}
