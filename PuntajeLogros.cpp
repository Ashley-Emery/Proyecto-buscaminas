#include "Usuarios.h"

#include <algorithm>

int SistemaPuntajes::calcularPuntajePersonalizado(int filas, int columnas) {

    const int celdasMinimas = FILAS_MINIMAS * COLUMNAS_MINIMAS;   // 64
    const int celdasMaximas = FILAS_MAXIMAS * COLUMNAS_MAXIMAS;   // 480

    int celdasActuales = filas * columnas;

    celdasActuales = max( celdasMinimas, min(celdasMaximas, celdasActuales) );

    // Relación lineal:        8x8  = 64 celdas  -> 100 puntos        12x40 = 480 celdas -> 500 puntos
    int puntaje = 100 + ((celdasActuales - celdasMinimas) * 400) / (celdasMaximas - celdasMinimas);

    return puntaje;
}

int SistemaPuntajes::calcularPuntaje( const Partida &partida) {

    if (!partida.gano()) {
        return 0;
    }

    const ConfiguracionPartida &configuracion =  partida.obtenerConfiguracion();

    if (configuracion.obtenerModo() == ModoJuego::PERSONALIZADO) {
        return calcularPuntajePersonalizado(
            configuracion.obtenerFilas(),
            configuracion.obtenerColumnas()
        );
    }

    if (configuracion.obtenerModo() == ModoJuego::PROGRESIVO) {

        switch (configuracion.obtenerDificultad()) {
            case Dificultad::BASICO:{
                return 500;
            }
            case Dificultad::INTERMEDIO:{
                return 1000;
            }
            case Dificultad::EXPERTO:{
                return 1500;
            }
            case Dificultad::PERSONALIZADO:{
                return 0;
            }
        }
    }

    if (configuracion.obtenerModo() == ModoJuego::MAPA) {

        switch (configuracion.obtenerDificultad()) {
            case Dificultad::BASICO:{
                return 100;
            }
            case Dificultad::INTERMEDIO:{
                return 250;
            }
            case Dificultad::EXPERTO:{
                return 500;
            }
            case Dificultad::PERSONALIZADO:{
                return 0;
            }
        }
    }
    return 0;
}

int SistemaPuntajes::procesarResultado(Usuario &usuario, Partida &partida, SistemaUsuarios &sistemaUsuarios) {

    if (!partida.gano() || partida.yaRegistroResultado()) {
        return 0;
    }

    int puntajeGanado = calcularPuntaje(partida);
    usuario.agregarPuntaje(puntajeGanado);

    const ConfiguracionPartida &configuracion = partida.obtenerConfiguracion();

    int numeroNivel = configuracion.obtenerNumeroNivel();

    if (numeroNivel >= 1 && numeroNivel <= CANTIDAD_NIVELES) {

        usuario.marcarNivelCompletado(numeroNivel);

        if (configuracion.obtenerModo() == ModoJuego::PROGRESIVO) {

            usuario.avanzarNivelProgresivoSiCorresponde(numeroNivel);
        }
    }

    SistemaLogros::evaluarLogros(usuario, partida);

    partida.marcarResultadoRegistrado();

    // El ranking se obtiene directamente del arreglo de usuarios.
    // Al guardar aquí, cualquier consulta posterior refleja el nuevo puntaje.
    sistemaUsuarios.guardar();

    return puntajeGanado;
}

// IMPLEMENTACION DE LOGROS
void SistemaLogros::evaluarLogros(Usuario &usuario, const Partida &partida) {

    if (!partida.gano()) {
        return;
    }

    // FIRST RESCUE: Se concede con la primera victoria de cualquier tipo.
    usuario.otorgarLogro( TipoLogro::FIRST_RESCUE);

    const Tablero* tablero = partida.obtenerTablero();

    if (tablero != nullptr && tablero->todasLasMinasMarcadasCorrectamente()) {
        usuario.otorgarLogro( TipoLogro::MINE_TAMER);
    }

    // QUICK FUSE: Ganar antes de que termine el tiempo.
    if (partida.obtenerTiempoRestante() > 0) {

        usuario.otorgarLogro(TipoLogro::QUICK_FUSE);
    }

    bool completoTodos = true;

    for (int nivel = 1; nivel <= CANTIDAD_NIVELES; nivel++) {

        if (!usuario.completoNivel(nivel)) {
            completoTodos = false;
            break;
        }
    }

    if (completoTodos) {
        usuario.otorgarLogro(TipoLogro::NINE_LIVES);
    }

    if (partida.obtenerConfiguracion().obtenerDificultad() == Dificultad::EXPERTO) {
        usuario.otorgarLogro(TipoLogro::BOMB_DISPOSAL_EXPERT);
    }

    if (!partida.usoBanderas()) {
        usuario.otorgarLogro(TipoLogro::CLEAN_SWEEP);
    }
}

string SistemaLogros::obtenerNombre(TipoLogro logro) {

    switch (logro) {
        case TipoLogro::FIRST_RESCUE:{
            return "First Rescue";
        }
        case TipoLogro::MINE_TAMER:{
            return "Mine Tamer";
        }
        case TipoLogro::QUICK_FUSE:{
            return "Quick Fuse";
        }
        case TipoLogro::NINE_LIVES:{
            return "Nine Lives";
        }
        case TipoLogro::BOMB_DISPOSAL_EXPERT:{
            return "Bomb Disposal Expert";
        }
        case TipoLogro::CLEAN_SWEEP:{
            return "Clean Sweep";
        }
    }

    return "Logro desconocido";
}

string SistemaLogros::obtenerDescripcion(TipoLogro logro) {

    switch (logro) {
        case TipoLogro::FIRST_RESCUE:{
            return "Clear your very first minefield and bring Julius home in one piece.";
        }
        case TipoLogro::MINE_TAMER:{
            return "Mark every single one correctly and this medal's yours!";
        }
        case TipoLogro::QUICK_FUSE:{
            return "Tick-tock, kid! Clear the minefield before time runs out and show that fuse who's boss!";
        }
        case TipoLogro::NINE_LIVES:{
            return "Nine minefields, nine rescues! Complete all nine levels to earn this one.";
        }
        case TipoLogro::BOMB_DISPOSAL_EXPERT:{
            return "Ready for the big leagues? Beat an Expert-level minefield and prove yourself!";
        }
        case TipoLogro::CLEAN_SWEEP:{
            return "Clear an entire minefield without placing a single flag. Now that's fancy footwork!";
        }
    }
    
    return "";
}
