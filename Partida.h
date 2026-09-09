#ifndef PARTIDA_H
#define PARTIDA_H

#include <string>
#include "Tablero.h"

using namespace std;

class Usuario;
class SistemaUsuarios;
class ArchivoPersistencia;


// -----------------------------------------------------------------------------
// CONSTANTES GENERALES
// -----------------------------------------------------------------------------
const int CANTIDAD_NIVELES = 9;
const int CANTIDAD_LOGROS = 6;
const int CANTIDAD_RANKING = 5;

const int FILAS_MINIMAS = 8;
const int FILAS_MAXIMAS = 12;
const int COLUMNAS_MINIMAS = 8;
const int COLUMNAS_MAXIMAS = 40;


// -----------------------------------------------------------------------------
// ENUMS
// -----------------------------------------------------------------------------
enum class EstadoPartida {
    NO_INICIADA,
    EN_CURSO,
    GANADA,
    PERDIDA,
    TIEMPO_AGOTADO
};

enum class ModoJuego {
    PROGRESIVO,
    MAPA,
    PERSONALIZADO
};

enum class Dificultad {
    BASICO,
    INTERMEDIO,
    EXPERTO,
    PERSONALIZADO
};

enum class TipoLogro {
    FIRST_RESCUE = 0,
    MINE_TAMER = 1,
    QUICK_FUSE = 2,
    NINE_LIVES = 3,
    BOMB_DISPOSAL_EXPERT = 4,
    CLEAN_SWEEP = 5
};

enum class BossPersonalizado {
    HILDA_BERG,
    WERNER_WERMAN,
    BARONESS_VON_BON_BON,
    DJIMMI_THE_GREAT,
    KING_DICE
};

enum class BossNivel {
    CAPTAIN_BRINEYBEARD,
    BEPPI_THE_CLOWN,
    GRIM_MATCHSTICK,
    RIBBY_AND_CROAKS,
    CALA_MARIA,
    DR_KAHL_ROBOT,
    WALLY_WARBLES,
    CHEF_SALTBAKER,
    THE_DEVIL
};


// -----------------------------------------------------------------------------
// CLASE 3: CONFIGURACION PARTIDA
// Almacena dimensiones, minas, tiempo, nivel, dificultad y modo.
// -----------------------------------------------------------------------------
class ConfiguracionPartida {
    private:
        int filas;
        int columnas;
        int minas;
        int tiempoLimiteSegundos;
        int numeroNivel;
        Dificultad dificultad;
        ModoJuego modo;

    public:
        ConfiguracionPartida();
        ConfiguracionPartida(int filas, int columnas, int minas, int tiempoLimiteSegundos, int numeroNivel, Dificultad dificultad, ModoJuego modo);

        int obtenerFilas() const;
        int obtenerColumnas() const;
        int obtenerMinas() const;
        int obtenerTiempoLimiteSegundos() const;
        int obtenerNumeroNivel() const;
        Dificultad obtenerDificultad() const;
        ModoJuego obtenerModo() const;

        bool esValida() const;

        static ConfiguracionPartida crearNivel(int numeroNivel, ModoJuego modo);
        static ConfiguracionPartida crearPersonalizada(int filas, int columnas);
};


// -----------------------------------------------------------------------------
// CLASE 4: PARTIDA
// Mantiene el estado completo de una partida y coordina acciones sobre Tablero.
// -----------------------------------------------------------------------------
class Partida {
    
    private:
        ConfiguracionPartida configuracion;
        Tablero* tablero;
        EstadoPartida estado;
        int tiempoRestanteSegundos;
        bool utilizoBanderas;
        bool resultadoRegistrado;

    public:
        Partida();
        explicit Partida(const ConfiguracionPartida& configuracion);
        ~Partida();

        Partida(const Partida&) = delete;
        Partida& operator=(const Partida&) = delete;

        bool iniciar(const ConfiguracionPartida& nuevaConfiguracion);
        bool procesarClicIzquierdo(int fila, int columna);
        bool procesarClicDerecho(int fila, int columna);
        void actualizarCronometro();

        void revelarMinasAlFinalizar();

        bool estaFinalizada() const;
        bool gano() const;
        bool perdio() const;
        bool seAgotoElTiempo() const;

        int obtenerTiempoRestante() const;
        int obtenerMinasRestantes() const;
        bool usoBanderas() const;
        bool yaRegistroResultado() const;
        void marcarResultadoRegistrado();

        EstadoPartida obtenerEstado() const;
        const ConfiguracionPartida& obtenerConfiguracion() const;
        const Tablero* obtenerTablero() const;
        Tablero* obtenerTablero();

        friend class ArchivoPersistencia;
};



// -----------------------------------------------------------------------------
// CLASE 5: SISTEMA PUNTAJES
// Centraliza las reglas para calcular y aplicar puntajes.
// -----------------------------------------------------------------------------
class SistemaPuntajes {
    public:
        static int calcularPuntaje(const Partida& partida);
        static int calcularPuntajePersonalizado(int filas, int columnas);

        // Aplica puntaje, progreso, niveles y logros una sola vez.
        static int procesarResultado(Usuario& usuario, Partida& partida, SistemaUsuarios& sistemaUsuarios);
};


// -----------------------------------------------------------------------------
// CLASE 6: SISTEMA LOGROS
// Evalúa y concede las seis badges/logros.
// -----------------------------------------------------------------------------
class SistemaLogros {
    public:
        static void evaluarLogros(Usuario& usuario, const Partida &partida);
        static string obtenerNombre(TipoLogro logro);
        static string obtenerDescripcion(TipoLogro logro);
};


// -----------------------------------------------------------------------------
// PONER AQUI PROTOTIPOS - FUNCIONES GENERALES RELACIONADAS CON NIVELES Y CONFIGURACION
// -----------------------------------------------------------------------------

bool validarConfiguracionPersonalizada(int filas, int columnas, string &mensajeError);
int calcularMinasPersonalizadas(int filas, int columnas);

BossPersonalizado obtenerBossPersonalizado(int filas, int columnas);
string obtenerNombreBossPersonalizado(BossPersonalizado boss);
string obtenerNombreNivelPersonalizado(BossPersonalizado boss);

BossNivel obtenerBossNivel(int numeroNivel);
string obtenerNombreBossNivel(BossNivel boss);
string obtenerNombreNivel(BossNivel boss);

string obtenerTextoDificultad(Dificultad dificultad);
string obtenerTextoModo(ModoJuego modo);

#endif