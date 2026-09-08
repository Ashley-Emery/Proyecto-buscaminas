#include "Usuarios.h"

#include <algorithm>
#include <cctype>

using namespace std;


// FUNCIONES GENERALES DE USUARIO / CONTRASEÑA
string normalizarNombreUsuario(const string &nombreUsuario) {

    string resultado = nombreUsuario;

    transform( resultado.begin(), resultado.end(), resultado.begin(), [](unsigned char caracter) {
            return static_cast<char>(tolower(caracter));
        }
    );

    return resultado;
}

bool validarContrasenaFuerte( const string &contrasena, string &mensajeError ) {
    
    if (contrasena.length() < 6) {
        mensajeError = "La contrasena debe tener una longitud minima de 6 caracteres.";
        return false;
    }

    bool tieneMayuscula = false;
    bool tieneMinuscula = false;
    bool tieneNumero = false;
    bool tieneEspecial = false;

    for (unsigned char caracter : contrasena) {
        if (isupper(caracter)) {
            tieneMayuscula = true;
        } else if (islower(caracter)) {
            tieneMinuscula = true;
        } else if (isdigit(caracter)) {
            tieneNumero = true;
        } else if (ispunct(caracter)) {
            tieneEspecial = true;
        }
    }

    if (!tieneMayuscula) {
        mensajeError = "La contrasena debe contener al menos una mayuscula.";
        return false;
    }

    if (!tieneMinuscula) {
        mensajeError = "La contrasena debe contener al menos una minuscula.";
        return false;
    }

    if (!tieneNumero) {
        mensajeError = "La contrasena debe contener al menos un numero.";
        return false;
    }

    if (!tieneEspecial) {
        mensajeError = "La contrasena debe contener al menos un caracter especial.";
        return false;
    }

    mensajeError.clear();
    return true;
}


// USUARIO
Usuario::Usuario() : puntajeTotal(0), siguienteNivelProgresivo(1), avatar("avatar_default"), musicaActiva(true) {
    for (int i = 0; i < CANTIDAD_NIVELES; i++) {
        nivelesCompletados[i] = false;
    }

    for (int i = 0; i < CANTIDAD_LOGROS; i++) {
        logros[i] = false;
    }
}

const string &Usuario::obtenerNombreUsuario() const {
    return nombreUsuario;
}

int Usuario::obtenerPuntajeTotal() const {
    return puntajeTotal;
}

int Usuario::obtenerSiguienteNivelProgresivo() const {
    return siguienteNivelProgresivo;
}

const string &Usuario::obtenerAvatar() const {
    return avatar;
}

bool Usuario::tieneMusicaActiva() const {
    return musicaActiva;
}

bool Usuario::completoNivel(int numeroNivel) const {

    if (numeroNivel < 1 || numeroNivel > CANTIDAD_NIVELES) {
        return false;
    }

    return nivelesCompletados[numeroNivel - 1];
}

bool Usuario::tieneLogro(TipoLogro logro) const {

    int indice = static_cast<int>(logro);

    if (indice < 0 || indice >= CANTIDAD_LOGROS) {
        return false;
    }

    return logros[indice];
}

int Usuario::contarLogros() const {

    int total = 0;

    for (int i = 0; i < CANTIDAD_LOGROS; i++) {
        if (logros[i]) {
            total++;
        }
    }

    return total;
}

void Usuario::agregarPuntaje(int puntos) {

    if (puntos > 0) {
        puntajeTotal += puntos;
    }
}

void Usuario::marcarNivelCompletado(int numeroNivel) {

    if (numeroNivel >= 1 && numeroNivel <= CANTIDAD_NIVELES) {
        nivelesCompletados[numeroNivel - 1] = true;
    }
}

void Usuario::avanzarNivelProgresivoSiCorresponde(int numeroNivel) {

    if (numeroNivel == siguienteNivelProgresivo && siguienteNivelProgresivo <= CANTIDAD_NIVELES) {
        siguienteNivelProgresivo++;
    }
}

void Usuario::otorgarLogro(TipoLogro logro) {

    int indice = static_cast<int>(logro);

    if (indice >= 0 && indice < CANTIDAD_LOGROS) {
        logros[indice] = true;
    }
}

void Usuario::establecerAvatar(const string &nuevoAvatar) {

    if (!nuevoAvatar.empty()) {
        avatar = nuevoAvatar;
    }
}

void Usuario::establecerMusicaActiva(bool activa) {

    musicaActiva = activa;
}

bool Usuario::cambiarContrasena( const string &contrasenaAnterior, const string &contrasenaNueva, string &mensajeError) {

    if (contrasenaAnterior != contrasena) {
        mensajeError = "La contrasena actual no coincide.";
        return false;
    }

    if (!validarContrasenaFuerte(contrasenaNueva, mensajeError)) {
        return false;
    }

    if (contrasenaNueva == contrasena) {
        mensajeError = "La nueva contrasena debe ser diferente a la actual.";
        return false;
    }

    contrasena = contrasenaNueva;
    mensajeError.clear();
    return true;
}


// SISTEMA USUARIOS
SistemaUsuarios::SistemaUsuarios(const string &rutaArchivo) : usuarios(nullptr), totalUsuarios(0), capacidadUsuarios(10), rutaArchivoUsuarios(rutaArchivo) {

    usuarios = new Usuario*[capacidadUsuarios];

    for (int i = 0; i < capacidadUsuarios; i++) {
        usuarios[i] = nullptr;
    }
}

SistemaUsuarios::~SistemaUsuarios() {
    liberarUsuarios();
}

void SistemaUsuarios::liberarUsuarios() {

    if (usuarios == nullptr) {
        return;
    }

    for (int i = 0; i < totalUsuarios; i++) {
        delete usuarios[i];
        usuarios[i] = nullptr;
    }

    delete[] usuarios;
    usuarios = nullptr;

    totalUsuarios = 0;
    capacidadUsuarios = 0;
}

bool SistemaUsuarios::ampliarCapacidad() {

    int nuevaCapacidad = capacidadUsuarios <= 0 ? 10 : capacidadUsuarios * 2;

    Usuario** nuevosUsuarios = new Usuario*[nuevaCapacidad];

    for (int i = 0; i < nuevaCapacidad; i++) {
        nuevosUsuarios[i] = nullptr;
    }

    for (int i = 0; i < totalUsuarios; i++) {
        nuevosUsuarios[i] = usuarios[i];
    }

    delete[] usuarios;
    usuarios = nuevosUsuarios;
    capacidadUsuarios = nuevaCapacidad;

    return true;
}

bool SistemaUsuarios::cargar() {
    return ArchivoPersistencia::cargarUsuarios( *this, rutaArchivoUsuarios);
}

bool SistemaUsuarios::guardar() const {
    return ArchivoPersistencia::guardarUsuarios( *this, rutaArchivoUsuarios);
}

Usuario* SistemaUsuarios::buscarUsuario( const string &nombreUsuario) const {

    string buscado = normalizarNombreUsuario(nombreUsuario);

    for (int i = 0; i < totalUsuarios; i++) {
        if (normalizarNombreUsuario(usuarios[i]->nombreUsuario) == buscado) {
            return usuarios[i];
        }
    }

    return nullptr;
}

bool SistemaUsuarios::registrarUsuario( const string &nombreUsuario, const string &contrasena, string &mensajeError) {

    if (nombreUsuario.empty()) {
        mensajeError = "El nombre de usuario no puede estar vacio.";
        return false;
    }

    if (buscarUsuario(nombreUsuario) != nullptr) {
        mensajeError = "Ese nombre de usuario ya existe.";
        return false;
    }

    if (!validarContrasenaFuerte(contrasena, mensajeError)) {
        return false;
    }

    if (totalUsuarios >= capacidadUsuarios) {
        if (!ampliarCapacidad()) {
            mensajeError = "No fue posible ampliar la memoria para usuarios.";
            return false;
        }
    }

    Usuario* nuevoUsuario = new Usuario();
    nuevoUsuario->nombreUsuario = nombreUsuario;
    nuevoUsuario->contrasena = contrasena;

    usuarios[totalUsuarios] = nuevoUsuario;
    totalUsuarios++;

    if (!guardar()) {
        totalUsuarios--;
        usuarios[totalUsuarios] = nullptr;
        delete nuevoUsuario;

        mensajeError = "No fue posible guardar la nueva cuenta en disco.";
        return false;
    }

    mensajeError.clear();
    return true;
}

Usuario* SistemaUsuarios::iniciarSesion( const string &nombreUsuario, const string &contrasena) const {

    Usuario* usuario = buscarUsuario(nombreUsuario);

    if (usuario == nullptr) {
        return nullptr;
    }

    if (usuario->contrasena != contrasena) {
        return nullptr;
    }

    return usuario;
}

int SistemaUsuarios::obtenerTotalUsuarios() const {
    return totalUsuarios;
}

int SistemaUsuarios::obtenerRanking(Usuario* ranking[CANTIDAD_RANKING]) const {

    for (int i = 0; i < CANTIDAD_RANKING; i++) {
        ranking[i] = nullptr;
    }

    int cantidadRanking = 0;

    // Mantiene unicamente los cinco mejores. Evita ordenar/copiar toda la colección de usuarios.
    for (int i = 0; i < totalUsuarios; i++) {
        Usuario* candidato = usuarios[i];

        if (cantidadRanking < CANTIDAD_RANKING) {
            int posicion = cantidadRanking;

            while (posicion > 0 && candidato->obtenerPuntajeTotal() > ranking[posicion - 1]->obtenerPuntajeTotal()) {

                ranking[posicion] = ranking[posicion - 1];
                posicion--;
            }

            ranking[posicion] = candidato;
            cantidadRanking++;
            continue;
        }

        if (candidato->obtenerPuntajeTotal() <= ranking[CANTIDAD_RANKING - 1]->obtenerPuntajeTotal()) {
            continue;
        }

        int posicion = CANTIDAD_RANKING - 1;

        while (posicion > 0 && candidato->obtenerPuntajeTotal() > ranking[posicion - 1]->obtenerPuntajeTotal()) {

            ranking[posicion] = ranking[posicion - 1];
            posicion--;
        }

        ranking[posicion] = candidato;
    }

    return cantidadRanking;
}
