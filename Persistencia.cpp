#include "Usuarios.h"

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;


template <typename Tipo>
bool escribirValor(ofstream &archivo, const Tipo &valor) {

    archivo.write(reinterpret_cast<const char*>(&valor), sizeof(Tipo));
    return archivo.good();
}

template <typename Tipo>
bool leerValor(ifstream &archivo,Tipo &valor) {
    
    archivo.read(reinterpret_cast<char*>(&valor), sizeof(Tipo));
    return archivo.good();
}

bool escribirCadena(ofstream &archivo,const string &texto) {
    
    uint32_t longitud = static_cast<uint32_t>(texto.size());

    if (!escribirValor(archivo, longitud)) {
        return false;
    }

    if (longitud > 0) {
        archivo.write(texto.data(), longitud);
    }

    return archivo.good();
}

bool leerCadena(ifstream &archivo, string &texto) {
    uint32_t longitud = 0;

    if (!leerValor(archivo, longitud)) {
        return false;
    }

    texto.resize(longitud);

    if (longitud > 0) {
        archivo.read(&texto[0], longitud);
    }

    return archivo.good();
}

string obtenerNombreSeguroUsuario(const string &nombreUsuario) {
    string nombreSeguro = normalizarNombreUsuario(nombreUsuario);

    for (char &caracter : nombreSeguro) {
        bool permitido =
            (caracter >= 'a' && caracter <= 'z') ||
            (caracter >= '0' && caracter <= '9') ||
            caracter == '_' ||
            caracter == '-';

        if (!permitido) {
            caracter = '_';
        }
    }

    return nombreSeguro;
}

string obtenerNombreArchivoModo(ModoJuego modo) {
    switch (modo) {
        case ModoJuego::PROGRESIVO:
            return "progresivo.dat";
        case ModoJuego::MAPA:
            return "mapa.dat";
        case ModoJuego::PERSONALIZADO:
            return "personalizado.dat";
    }

    return "";
}

bool crearCarpeta(const string &ruta) {

    try {
        fs::create_directories(ruta);
        return true;
    }
    catch (...) {
        return false;
    }
}

string construirRutaCarpetaUsuario(const string &nombreUsuario) {
    return "data/" + obtenerNombreSeguroUsuario(nombreUsuario);
}

string construirRutaPartida(const string &nombreUsuario, ModoJuego modo) {
    string nombreArchivo = obtenerNombreArchivoModo(modo);

    if (nombreArchivo.empty()) {
        return "";
    }

    return construirRutaCarpetaUsuario(nombreUsuario) + "/" + nombreArchivo;
}

bool ArchivoPersistencia::guardarUsuarios(const SistemaUsuarios &sistemaUsuarios, const string &rutaArchivo) {

    if (!crearCarpeta("data")) {
        return false;
    }

    ofstream archivo(rutaArchivo, ios::binary | ios::trunc);

    if (!archivo.is_open()) {
        return false;
    }

    uint32_t total = static_cast<uint32_t>(sistemaUsuarios.totalUsuarios);

    if (!escribirValor(archivo, total)) {
        return false;
    }

    for (int i = 0; i < sistemaUsuarios.totalUsuarios; i++) {
        const Usuario* usuario = sistemaUsuarios.usuarios[i];

        if (!crearCarpeta(construirRutaCarpetaUsuario(usuario->nombreUsuario))) {
            return false;
        }

        if (!escribirCadena(archivo, usuario->nombreUsuario) ||
            !escribirCadena(archivo, usuario->contrasena) ||
            !escribirValor(archivo, usuario->puntajeTotal) ||
            !escribirValor(archivo, usuario->siguienteNivelProgresivo) ||
            !escribirCadena(archivo, usuario->avatar)) {

            return false;
        }

        uint8_t musica = usuario->musicaActiva ? 1 : 0;

        if (!escribirValor(archivo, musica)) {
            return false;
        }

        for (int nivel = 0; nivel < CANTIDAD_NIVELES; nivel++) {
            uint8_t completado = usuario->nivelesCompletados[nivel] ? 1 : 0;

            if (!escribirValor(archivo, completado)) {
                return false;
            }
        }

        for (int logro = 0; logro < CANTIDAD_LOGROS; logro++) {
            uint8_t obtenido = usuario->logros[logro] ? 1 : 0;

            if (!escribirValor(archivo, obtenido)) {
                return false;
            }
        }
    }

    return archivo.good();
}

bool ArchivoPersistencia::cargarUsuarios(SistemaUsuarios &sistemaUsuarios, const string &rutaArchivo) {
    ifstream archivo(rutaArchivo, ios::binary);

    // Primera ejecucion: no tener archivo todavia no es un error.
    if (!archivo.is_open()) {
        return true;
    }

    uint32_t total = 0;

    if (!leerValor(archivo, total)) {
        return false;
    }

    sistemaUsuarios.liberarUsuarios();

    sistemaUsuarios.capacidadUsuarios = total < 10 ? 10 : static_cast<int>(total);
    sistemaUsuarios.usuarios = new Usuario*[sistemaUsuarios.capacidadUsuarios];

    for (int i = 0; i < sistemaUsuarios.capacidadUsuarios; i++) {
        sistemaUsuarios.usuarios[i] = nullptr;
    }

    sistemaUsuarios.totalUsuarios = 0;

    for (uint32_t i = 0; i < total; i++) {
        Usuario* usuario = new Usuario();

        if (!leerCadena(archivo, usuario->nombreUsuario) ||
            !leerCadena(archivo, usuario->contrasena) ||
            !leerValor(archivo, usuario->puntajeTotal) ||
            !leerValor(archivo, usuario->siguienteNivelProgresivo) ||
            !leerCadena(archivo, usuario->avatar)) {
            delete usuario;
            return false;
        }

        uint8_t musica = 0;

        if (!leerValor(archivo, musica)) {
            delete usuario;
            return false;
        }

        usuario->musicaActiva = musica != 0;

        for (int nivel = 0; nivel < CANTIDAD_NIVELES; nivel++) {
            uint8_t completado = 0;

            if (!leerValor(archivo, completado)) {
                delete usuario;
                return false;
            }

            usuario->nivelesCompletados[nivel] = completado != 0;
        }

        for (int logro = 0; logro < CANTIDAD_LOGROS; logro++) {
            uint8_t obtenido = 0;

            if (!leerValor(archivo, obtenido)) {
                delete usuario;
                return false;
            }

            usuario->logros[logro] = obtenido != 0;
        }

        sistemaUsuarios.usuarios[sistemaUsuarios.totalUsuarios] = usuario;
        sistemaUsuarios.totalUsuarios++;
    }

    return true;
}

bool ArchivoPersistencia::guardarPartida(const string &nombreUsuario, const Partida &partida) {
    if (partida.tablero == nullptr || partida.estaFinalizada()) {
        return false;
    }

    ModoJuego modoPartida = partida.configuracion.obtenerModo();
    string carpetaUsuario = construirRutaCarpetaUsuario(nombreUsuario);

    if (!crearCarpeta(carpetaUsuario)) {
        return false;
    }

    string ruta = construirRutaPartida(nombreUsuario, modoPartida);

    if (ruta.empty()) {
        return false;
    }

    ofstream archivo(ruta, ios::binary | ios::trunc);

    if (!archivo.is_open()) {
        return false;
    }

    int modo = static_cast<int>(partida.configuracion.obtenerModo());
    int dificultad = static_cast<int>(partida.configuracion.obtenerDificultad());
    int estado = static_cast<int>(partida.estado);

    if (!escribirValor(archivo, partida.configuracion.obtenerFilas()) ||
        !escribirValor(archivo, partida.configuracion.obtenerColumnas()) ||
        !escribirValor(archivo, partida.configuracion.obtenerMinas()) ||
        !escribirValor(archivo, partida.configuracion.obtenerTiempoLimiteSegundos()) ||
        !escribirValor(archivo, partida.configuracion.obtenerNumeroNivel()) ||
        !escribirValor(archivo, dificultad) ||
        !escribirValor(archivo, modo) ||
        !escribirValor(archivo, estado) ||
        !escribirValor(archivo, partida.tiempoRestanteSegundos)) {

        return false;
    }

    uint8_t usoBanderas = partida.utilizoBanderas ? 1 : 0;
    uint8_t minasSembradas = partida.tablero->minasSembradas ? 1 : 0;

    if (!escribirValor(archivo, usoBanderas) || !escribirValor(archivo, minasSembradas)) {
        return false;
    }

    for (int fila = 0; fila < partida.tablero->filas; fila++) {
        for (int columna = 0; columna < partida.tablero->columnas; columna++) {
            const Celda &celda = partida.tablero->celdas[fila][columna];

            uint8_t contieneMina = celda.contieneMina ? 1 : 0;
            uint8_t revelada = celda.revelada ? 1 : 0;
            uint8_t marcada = celda.marcada ? 1 : 0;

            if (!escribirValor(archivo, contieneMina) || !escribirValor(archivo, revelada) ||
                !escribirValor(archivo, marcada) || !escribirValor(archivo, celda.minasVecinas)) {

                return false;
            }
        }
    }

    return archivo.good();
}

bool ArchivoPersistencia::cargarPartida(const string &nombreUsuario, ModoJuego modo, Partida &partida) {

    string ruta = construirRutaPartida(nombreUsuario, modo);

    ifstream archivo(ruta, ios::binary);

    if (!archivo.is_open()) {
        return false;
    }

    int filas = 0;
    int columnas = 0;
    int minas = 0;
    int tiempoLimite = 0;
    int numeroNivel = 0;
    int dificultadEntera = 0;
    int modoEntero = 0;
    int estadoEntero = 0;
    int tiempoRestante = 0;

    if (!leerValor(archivo, filas) || !leerValor(archivo, columnas) || !leerValor(archivo, minas) ||
        !leerValor(archivo, tiempoLimite) || !leerValor(archivo, numeroNivel) || !leerValor(archivo, dificultadEntera) ||
        !leerValor(archivo, modoEntero) || !leerValor(archivo, estadoEntero) || !leerValor(archivo, tiempoRestante)) {

        return false;
    }

    if (filas <= 0 || columnas <= 0 || minas <= 0 || minas >= filas * columnas || tiempoLimite <= 0) {
        return false;
    }

    Dificultad dificultad = static_cast<Dificultad>(dificultadEntera);
    ModoJuego modoCargado = static_cast<ModoJuego>(modoEntero);

    ConfiguracionPartida configuracion(filas, columnas, minas, tiempoLimite, numeroNivel, dificultad, modoCargado);

    if (!partida.iniciar(configuracion)) {
        return false;
    }

    uint8_t usoBanderas = 0;
    uint8_t minasSembradas = 0;

    if (!leerValor(archivo, usoBanderas) || !leerValor(archivo, minasSembradas)) {
        return false;
    }

    partida.estado = static_cast<EstadoPartida>(estadoEntero);
    partida.tiempoRestanteSegundos = tiempoRestante;
    partida.utilizoBanderas = usoBanderas != 0;
    partida.resultadoRegistrado = false;
    partida.tablero->minasSembradas = minasSembradas != 0;

    for (int fila = 0; fila < filas; fila++) {
        for (int columna = 0; columna < columnas; columna++) {
            Celda &celda = partida.tablero->celdas[fila][columna];

            uint8_t contieneMina = 0;
            uint8_t revelada = 0;
            uint8_t marcada = 0;
            int minasVecinas = 0;

            if (!leerValor(archivo, contieneMina) || !leerValor(archivo, revelada) ||
                !leerValor(archivo, marcada) || !leerValor(archivo, minasVecinas)) {

                return false;
            }

            celda.contieneMina = contieneMina != 0;
            celda.revelada = revelada != 0;
            celda.marcada = marcada != 0;
            celda.minasVecinas = minasVecinas;
        }
    }

    return true;
}

bool ArchivoPersistencia::existePartidaGuardada(const string &nombreUsuario, ModoJuego modo) {

    string ruta = construirRutaPartida(nombreUsuario, modo);

    ifstream archivo(ruta, ios::binary);

    return archivo.is_open();
}

bool ArchivoPersistencia::eliminarPartidaGuardada(const string &nombreUsuario, ModoJuego modo) {
    string ruta = construirRutaPartida(nombreUsuario, modo);

    if (ruta.empty()) {
        return false;
    }

    return remove(ruta.c_str()) == 0;
}