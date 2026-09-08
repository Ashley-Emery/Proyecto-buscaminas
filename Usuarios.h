#ifndef USUARIOS_H
#define USUARIOS_H

#include <string>
#include "Partida.h"

using namespace std;

const char ARCHIVO_USUARIOS[] = "data/users.dat";

// -----------------------------------------------------------------------------
// CLASE 7: USUARIO
// Guarda la información persistente de cada cuenta.
// -----------------------------------------------------------------------------
class Usuario {
    private:
        string nombreUsuario;
        string contrasena;
        int puntajeTotal;
        int siguienteNivelProgresivo;
        bool nivelesCompletados[CANTIDAD_NIVELES];
        bool logros[CANTIDAD_LOGROS];
        string avatar;
        bool musicaActiva;

    public:
        Usuario();

        const string &obtenerNombreUsuario() const;
        int obtenerPuntajeTotal() const;
        int obtenerSiguienteNivelProgresivo() const;
        const string &obtenerAvatar() const;
        bool tieneMusicaActiva() const;

        bool completoNivel(int numeroNivel) const;
        bool tieneLogro(TipoLogro logro) const;
        int contarLogros() const;

        void agregarPuntaje(int puntos);
        void marcarNivelCompletado(int numeroNivel);
        void avanzarNivelProgresivoSiCorresponde(int numeroNivel);
        void otorgarLogro(TipoLogro logro);

        void establecerAvatar(const string &nuevoAvatar);
        void establecerMusicaActiva(bool activa);

        bool cambiarContrasena(const string &contrasenaAnterior, const string &contrasenaNueva, string &mensajeError);

        friend class SistemaUsuarios;
        friend class ArchivoPersistencia;
};

// -----------------------------------------------------------------------------
// CLASE 8: SISTEMA USUARIOS
// Administra cuentas en memoria mediante arreglo dinámico de punteros.
// -----------------------------------------------------------------------------
class SistemaUsuarios {
    private:
        Usuario** usuarios;
        int totalUsuarios;
        int capacidadUsuarios;
        string rutaArchivoUsuarios;

        void liberarUsuarios();
        bool ampliarCapacidad();

    public:
        explicit SistemaUsuarios(const string &rutaArchivo = ARCHIVO_USUARIOS);
        ~SistemaUsuarios();

        SistemaUsuarios(const SistemaUsuarios&) = delete;
        SistemaUsuarios &operator=(const SistemaUsuarios&) = delete;

        bool cargar();
        bool guardar() const;

        bool registrarUsuario(const string &nombreUsuario, const string &contrasena, string &mensajeError);

        Usuario* iniciarSesion(const string &nombreUsuario, const string &contrasena) const;
        Usuario* buscarUsuario(const string &nombreUsuario) const;

        int obtenerTotalUsuarios() const;

        // Llena un arreglo de hasta 5 punteros con los mejores puntajes.
        int obtenerRanking(Usuario* ranking[CANTIDAD_RANKING]) const;

        friend class ArchivoPersistencia;
};

// -----------------------------------------------------------------------------
// CLASE 9: ARCHIVO PERSISTENCIA
// Maneja archivos binarios de usuarios y partidas guardadas.
// -----------------------------------------------------------------------------
class ArchivoPersistencia {
    public:
        static bool guardarUsuarios(const SistemaUsuarios &sistemaUsuarios, const string &rutaArchivo);

        static bool cargarUsuarios(SistemaUsuarios &sistemaUsuarios, const string &rutaArchivo);

        static bool guardarPartida(const string &nombreUsuario, const Partida &partida);

        static bool cargarPartida(const string &nombreUsuario, ModoJuego modo, Partida &partida);

        static bool existePartidaGuardada(const string &nombreUsuario, ModoJuego modo);

        static bool eliminarPartidaGuardada(const string &nombreUsuario, ModoJuego modo);
};


// -----------------------------------------------------------------------------
// PONER AQUI PROTOTIPOS - FUNCIONES RELACIONADAS CON USUARIOS Y CONTRASENAS
// -----------------------------------------------------------------------------

bool validarContrasenaFuerte(const string &contrasena, string &mensajeError);
string normalizarNombreUsuario(const string &nombreUsuario);

#endif