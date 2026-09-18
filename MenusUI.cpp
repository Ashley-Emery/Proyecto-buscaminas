#include "Usuarios.h"
#include "Assets.h"

#include <QWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QLineEdit>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QTimer>
#include <QPixmap>
#include <QFont>
#include <QFontDatabase>
#include <QFileInfo>
#include <QUrl>
#include <QResizeEvent>
#include <QApplication>
#include <QImage>
#include <QComboBox>

using namespace std;

QWidget* crearJuegoUI(Usuario* usuarioActual, SistemaUsuarios* sistemaUsuarios, ModoJuego modo, int numeroNivel);
QWidget* crearJuegoUICargado(Usuario* usuarioActual, SistemaUsuarios* sistemaUsuarios, ModoJuego modo);
QWidget* crearJuegoUIPersonalizado(Usuario* usuarioActual, SistemaUsuarios* sistemaUsuarios, int filas, int columnas);


// CONSTANTES VISUALES
const int ANCHO_DISENO_MENU = 2080;
const int ALTO_DISENO_MENU = 1100;

const int ANCHO_VENTANA_INICIAL_MENU = 1280;
const int ALTO_VENTANA_INICIAL_MENU = 720;

const int ANCHO_MINIMO_MENU = 1280;
const int ALTO_MINIMO_MENU = 720;

// MENUS UI
class MenusUI : public QWidget {

    private:

        // FUENTES
        QString nombreFuenteAlice;
        QString nombreFuenteLobster;

        // STACK DE MENUS
        QStackedWidget* paginas;

        QWidget* paginaIntro;
        QWidget* paginaInicio; 
        QWidget* paginaAutenticacion;
        QWidget* paginaMenuPrincipal;
        QWidget* paginaMapa;
        QWidget* paginaHuevoPascua;
        QWidget* paginaComoJugar;
        QWidget* paginaPerfil;
        QWidget* paginaCambioPassword;
        QWidget* paginaHighScores;
        QWidget* paginaPlay;
        QWidget* paginaRewards;
        QWidget* paginaBadgesInfo;
        QWidget* paginaPartidaGuardada;
        QWidget* paginaCustomMayhem;

        // INTRO
        QVideoWidget* videoIntro;
        QMediaPlayer* reproductorIntro;

        bool introIniciada;
        bool introFinalizada;

        // MENU INICIAL
        QWidget* contenedorInicio;

        QLabel* fondoInicio;
        QLabel* textoFelix;

        QPushButton* botonLogin;
        QPushButton* botonSignin;
        QPushButton* botonExit;

        // AUTENTICACION
        SistemaUsuarios sistemaUsuarios;
        Usuario* usuarioActual;

        bool creandoCuenta;

        QWidget* contenedorAutenticacion;

        QLabel* fondoAutenticacion;
        QLabel* textoAutenticacion;

        QLabel* labelUsername;
        QLabel* labelPassword;

        QLineEdit* campoUsername;
        QLineEdit* campoPassword;

        QPushButton* botonCancelarAutenticacion;
        QPushButton* botonAceptarAutenticacion;

        // MENU PRINCIPAL AUTENTICADO
        QWidget* contenedorMenuPrincipal;
        QLabel* fondoMenuPrincipal;
        QLabel* textoBienvenida;
        QPushButton* botonMiPerfil;
        QPushButton* botonComoJugar;
        QPushButton* botonSalirSesion;
        QPushButton* botonHighScores;
        QPushButton* botonPlay;
        QPushButton* botonMapa;
        QPushButton* botonHuevoPascua;

        // MAPA BLACKPOWDER BAY
        QWidget* contenedorMapa;
        QLabel* fondoMapa;
        QPushButton* botonHomeMapa;
        QPushButton* botonesNivelMapa[CANTIDAD_NIVELES];

        // HUEVO DE PASCUA
        QWidget* contenedorHuevoPascua;
        QLabel* fondoHuevoPascua;
        QLabel* textoHuevoPascua;
        QPushButton* botonClaimReward;
        QPushButton* botonHomeHuevoPascua;

        // HOW TO PLAY
        QWidget* contenedorComoJugar;
        QLabel* fondoComoJugar;
        QPushButton* botonAdelanteComoJugar;
        QPushButton* botonAtrasComoJugar;
        QPushButton* botonHomeComoJugar;
        int paginaComoJugarActual;

        // MY PROFILE
        QWidget* contenedorPerfil;
        QLabel* fondoPerfil;
        QLabel* imagenAvatarPerfil;
        QLabel* textoUsernamePerfil;
        QPushButton* botonAvatarAnterior;
        QPushButton* botonAvatarSiguiente;
        QPushButton* botonAceptarPerfil;
        QPushButton* botonCancelarPerfil;
        QPushButton* botonCambiarPassword;
        int avatarPerfilSeleccionado;

        // CHANGE PASSWORD
        QWidget* contenedorCambioPassword;
        QLabel* fondoCambioPassword;
        QLabel* imagenAvatarCambioPassword;
        QLabel* textoOldPassword;
        QLabel* textoNewPassword;
        QLineEdit* campoOldPassword;
        QLineEdit* campoNewPassword;
        QPushButton* botonAceptarCambioPassword;
        QPushButton* botonCancelarCambioPassword;

        // HIGH SCORES
        QWidget* contenedorHighScores;
        QLabel* fondoHighScores;
        QLabel* imagenRanking;
        QLabel* textoTituloMiScore;
        QLabel* textoMiScore;
        QLabel* textoTituloMedallas;
        QLabel* textosRankingUsuario[CANTIDAD_RANKING];
        QLabel* textosRankingPuntaje[CANTIDAD_RANKING];
        QLabel* imagenesBadges[CANTIDAD_LOGROS];
        QPushButton* botonHomeHighScores;

        // PLAY
        QWidget* contenedorPlay;
        QLabel* fondoPlay;
        QLabel* textoPlay;
        QPushButton* botonHomePlay;
        QPushButton* botonRewardsPlay;
        QPushButton* botonHighScoresPlay;
        QPushButton* botonNextQuest;
        QPushButton* botonCustomMayhem;

        // REWARDS
        QWidget* contenedorRewards;
        QLabel* fondoRewards;
        QLabel* textoRewards;
        QLabel* imagenesBadgesRewards[CANTIDAD_LOGROS];
        QPushButton* botonBackRewards;
        QPushButton* botonInfoRewards;

        // BADGES INFO
        QWidget* contenedorBadgesInfo;
        QLabel* fondoBadgesInfo;
        QPushButton* botonBackBadgesInfo;

        // PARTIDA GUARDADA
        QWidget* contenedorPartidaGuardada;
        QLabel* fondoPartidaGuardada;
        QLabel* textoPartidaGuardada;
        QPushButton* botonBackPartidaGuardada;
        QPushButton* botonHomePartidaGuardada;
        QPushButton* botonLoadQuest;
        QPushButton* botonResetQuest;

        // PARTIDA MAPA GUARDADA
        QWidget* paginaMapaGuardada;
        QWidget* contenedorMapaGuardada;
        QLabel* fondoMapaGuardada;
        QLabel* textoMapaGuardada;
        QPushButton* botonBackMapaGuardada;
        QPushButton* botonLoadMapaGuardada;
        QPushButton* botonNewQuestMapa;

        // PARTIDA PERSONALIZADA GUARDADA
        QWidget* paginaPersonalizadaGuardada;
        QWidget* contenedorPersonalizadaGuardada;
        QLabel* fondoPersonalizadaGuardada;
        QLabel* textoPersonalizadaGuardada;
        QPushButton* botonBackPersonalizadaGuardada;
        QPushButton* botonLoadPersonalizadaGuardada;
        QPushButton* botonNewQuestPersonalizada;

        // CUSTOM MAYHEM
        QWidget* contenedorCustomMayhem;
        QLabel* fondoCustomMayhem;
        QLabel* textoCustomMayhem;
        QLabel* textoInstruccionCustomMayhem;
        QLabel* textoXCustomMayhem;
        QComboBox* comboFilasCustomMayhem;
        QComboBox* comboColumnasCustomMayhem;
        QPushButton* botonBackCustomMayhem;
        QPushButton* botonPlayGameCustomMayhem;
        QPushButton* botonMapaCustomMayhem;

        // MAPA CALAMITY CAYS
        QWidget* paginaMapaCustomMayhem;
        QWidget* contenedorMapaCustomMayhem;
        QLabel* fondoMapaCustomMayhem;
        QPushButton* botonBackMapaCustomMayhem;


    public:

        MenusUI(QWidget* parent = nullptr)
            : QWidget(parent),
                nombreFuenteAlice(""),
                nombreFuenteLobster(""),
                paginas(nullptr),
                paginaIntro(nullptr),
                paginaInicio(nullptr),
                paginaMenuPrincipal(nullptr),
                contenedorMenuPrincipal(nullptr),
                fondoMenuPrincipal(nullptr),
                textoBienvenida(nullptr),
                botonMiPerfil(nullptr),
                botonComoJugar(nullptr),
                botonSalirSesion(nullptr),
                botonHighScores(nullptr),
                botonPlay(nullptr),
                botonMapa(nullptr),
                botonHuevoPascua(nullptr),
                videoIntro(nullptr),
                reproductorIntro(nullptr),
                introIniciada(false),
                introFinalizada(false),
                contenedorInicio(nullptr),
                fondoInicio(nullptr),
                textoFelix(nullptr),
                botonLogin(nullptr),
                botonSignin(nullptr),
                paginaAutenticacion(nullptr),
                sistemaUsuarios(),
                usuarioActual(nullptr),
                creandoCuenta(false),
                contenedorAutenticacion(nullptr),
                fondoAutenticacion(nullptr),
                textoAutenticacion(nullptr),
                labelUsername(nullptr),
                labelPassword(nullptr),
                campoUsername(nullptr),
                campoPassword(nullptr),
                botonCancelarAutenticacion(nullptr),
                botonAceptarAutenticacion(nullptr),
                paginaMapa(nullptr),
                contenedorMapa(nullptr),
                fondoMapa(nullptr),
                botonHomeMapa(nullptr),
                paginaHuevoPascua(nullptr),
                contenedorHuevoPascua(nullptr),
                fondoHuevoPascua(nullptr),
                textoHuevoPascua(nullptr),
                botonClaimReward(nullptr),
                botonHomeHuevoPascua(nullptr),
                paginaComoJugar(nullptr),
                contenedorComoJugar(nullptr),
                fondoComoJugar(nullptr),
                botonAdelanteComoJugar(nullptr),
                botonAtrasComoJugar(nullptr),
                botonHomeComoJugar(nullptr),
                paginaComoJugarActual(1),
                paginaPerfil(nullptr),
                contenedorPerfil(nullptr),
                fondoPerfil(nullptr),
                imagenAvatarPerfil(nullptr),
                textoUsernamePerfil(nullptr),
                botonAvatarAnterior(nullptr),
                botonAvatarSiguiente(nullptr),
                botonAceptarPerfil(nullptr),
                botonCancelarPerfil(nullptr),
                botonCambiarPassword(nullptr),
                avatarPerfilSeleccionado(1),
                paginaCambioPassword(nullptr),
                contenedorCambioPassword(nullptr),
                fondoCambioPassword(nullptr),
                imagenAvatarCambioPassword(nullptr),
                textoOldPassword(nullptr),
                textoNewPassword(nullptr),
                campoOldPassword(nullptr),
                campoNewPassword(nullptr),
                botonAceptarCambioPassword(nullptr),
                botonCancelarCambioPassword(nullptr),
                paginaHighScores(nullptr),
                contenedorHighScores(nullptr),
                fondoHighScores(nullptr),
                imagenRanking(nullptr),
                textoTituloMiScore(nullptr),
                textoMiScore(nullptr),
                textoTituloMedallas(nullptr),
                botonHomeHighScores(nullptr),
                paginaPlay(nullptr),
                contenedorPlay(nullptr),
                fondoPlay(nullptr),
                textoPlay(nullptr),
                botonHomePlay(nullptr),
                botonRewardsPlay(nullptr),
                botonHighScoresPlay(nullptr),
                botonNextQuest(nullptr),
                botonCustomMayhem(nullptr),
                paginaRewards(nullptr),
                contenedorRewards(nullptr),
                fondoRewards(nullptr),
                textoRewards(nullptr),
                botonBackRewards(nullptr),
                botonInfoRewards(nullptr),
                paginaBadgesInfo(nullptr),
                contenedorBadgesInfo(nullptr),
                fondoBadgesInfo(nullptr),
                botonBackBadgesInfo(nullptr),
                paginaPartidaGuardada(nullptr),
                contenedorPartidaGuardada(nullptr),
                fondoPartidaGuardada(nullptr),
                textoPartidaGuardada(nullptr),
                botonBackPartidaGuardada(nullptr),
                botonHomePartidaGuardada(nullptr),
                botonLoadQuest(nullptr),
                botonResetQuest(nullptr),
                paginaMapaGuardada(nullptr),
                contenedorMapaGuardada(nullptr),
                fondoMapaGuardada(nullptr),
                textoMapaGuardada(nullptr),
                botonBackMapaGuardada(nullptr),
                botonLoadMapaGuardada(nullptr),
                botonNewQuestMapa(nullptr),
                paginaPersonalizadaGuardada(nullptr),
                contenedorPersonalizadaGuardada(nullptr),
                fondoPersonalizadaGuardada(nullptr),
                textoPersonalizadaGuardada(nullptr),
                botonBackPersonalizadaGuardada(nullptr),
                botonLoadPersonalizadaGuardada(nullptr),
                botonNewQuestPersonalizada(nullptr),
                botonMapaCustomMayhem(nullptr),
                paginaMapaCustomMayhem(nullptr),
                contenedorMapaCustomMayhem(nullptr),
                fondoMapaCustomMayhem(nullptr),
                botonBackMapaCustomMayhem(nullptr),
                botonExit(nullptr) {

            for (int i = 0; i < CANTIDAD_NIVELES; i++) {
                botonesNivelMapa[i] = nullptr;
            }

            for (int i = 0; i < CANTIDAD_RANKING; i++) {
                textosRankingUsuario[i] = nullptr;
                textosRankingPuntaje[i] = nullptr;
            }

            for (int i = 0; i < CANTIDAD_LOGROS; i++) {
                //imagenesBadges[i] = nullptr;
                imagenesBadgesRewards[i] = nullptr;
            }

            setWindowTitle("Minefield Mayhem - The Oppenheimer Incident");

            resize(ANCHO_VENTANA_INICIAL_MENU, ALTO_VENTANA_INICIAL_MENU);
            setMinimumSize(ANCHO_MINIMO_MENU, ALTO_MINIMO_MENU);

            crearInterfaz();

            QTimer::singleShot(0, this, [this]() {
                reproducirIntro();
            });
        }


    private:

        // CREAR INTERFAZ
        void crearInterfaz() {

            cargarFuentes();

            paginas = new QStackedWidget(this);

            crearPaginaIntro();
            crearPaginaInicio();
            crearPaginaAutenticacion();
            crearPaginaMenuPrincipal();
            crearPaginaMapa();
            crearPaginaHuevoPascua();
            crearPaginaComoJugar();
            crearPaginaPerfil();
            crearPaginaCambioPassword();
            crearPaginaHighScores();
            crearPaginaPlay();
            crearPaginaRewards();
            crearPaginaBadgesInfo();
            crearPaginaPartidaGuardada();
            crearPaginaMapaGuardada();
            crearPaginaPersonalizadaGuardada();
            crearPaginaCustomMayhem();
            crearPaginaMapaCustomMayhem();

            if (!sistemaUsuarios.cargar()) {

                QMessageBox::warning(
                    this,
                    "Users",
                    "The user database could not be loaded."
                );
            }

            paginas->setCurrentWidget(paginaIntro);
        }

        void conectarRetornoJuego(QWidget* ventanaJuego) {

            connect(
                ventanaJuego,
                &QObject::destroyed,
                this,
                [this](QObject* objeto) {

                    QString destino =
                        objeto->property("destinoAlCerrar").toString();

                    show();

                    if (destino == "mapa") {

                        mostrarMapaNuevo();

                        return;
                    }

                    if (destino == "personalizado") {

                        mostrarCustomMayhemNuevo();

                        return;
                    }

                    mostrarMenuPrincipal();
                }
            );
        }

        // CARGAR FUENTES
        void cargarFuentes() {

            int idFuenteAlice = QFontDatabase::addApplicationFont(QString::fromStdString(FONT_ALICE));

            if (idFuenteAlice != -1) {

                QStringList familiasAlice = QFontDatabase::applicationFontFamilies(idFuenteAlice);

                if (!familiasAlice.isEmpty()) {
                    nombreFuenteAlice = familiasAlice.first();
                }
            }


            int idFuenteLobster = QFontDatabase::addApplicationFont(QString::fromStdString(FONT_LOBSTER));

            if (idFuenteLobster != -1) {

                QStringList familiasLobster = QFontDatabase::applicationFontFamilies(idFuenteLobster);

                if (!familiasLobster.isEmpty()) {
                    nombreFuenteLobster = familiasLobster.first();
                }
            }
        }

        // PAGINA INTRO
        void crearPaginaIntro() {

            paginaIntro = new QWidget();
            paginaIntro->setStyleSheet("background-color: black;");

            videoIntro = new QVideoWidget(paginaIntro);
            videoIntro->setAspectRatioMode(Qt::IgnoreAspectRatio);

            reproductorIntro = new QMediaPlayer(this);
            reproductorIntro->setVideoOutput(videoIntro);

            connect(reproductorIntro, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus estado) {

                if (estado == QMediaPlayer::EndOfMedia) {
                    finalizarIntro();
                }
            });

            paginas->addWidget(paginaIntro);
        }

        // PAGINA MENU INICIAL
        void crearPaginaInicio() {

            paginaInicio = new QWidget();

            contenedorInicio = new QWidget(paginaInicio);

            // FONDO
            fondoInicio = new QLabel(contenedorInicio);
            fondoInicio->setScaledContents(true);

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_FELIX));
            fondoInicio->setPixmap(imagenFondo);

            // TEXTO FELIX
            textoFelix = new QLabel(
                "“There you are, kid! Took you long enough!”",
                contenedorInicio
            );

            textoFelix->setAlignment(Qt::AlignCenter);
            textoFelix->setStyleSheet(
                "QLabel {"
                "background: transparent;"
                "color: black;"
                "}"
            );

            // BOTONES
            botonLogin = crearBotonTexto("LOG IN");
            botonSignin = crearBotonTexto("SIGN IN");
            botonExit = crearBotonTexto("EXIT");

            botonLogin->setParent(contenedorInicio);
            botonSignin->setParent(contenedorInicio);
            botonExit->setParent(contenedorInicio);


            connect(botonLogin, &QPushButton::clicked, this, [this]() {
                mostrarLogin();
            });

            connect(botonSignin, &QPushButton::clicked, this, [this]() {
                mostrarSignin();
            });

            connect(botonExit, &QPushButton::clicked, this, []() {
                QApplication::quit();
            });


            paginas->addWidget(paginaInicio);
        }

        // PAGINA AUTENTICACION
        void crearPaginaAutenticacion() {

            paginaAutenticacion = new QWidget();

            contenedorAutenticacion = new QWidget(paginaAutenticacion);


            // FONDO
            fondoAutenticacion = new QLabel(contenedorAutenticacion);
            fondoAutenticacion->setScaledContents(true);

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_FELIX));
            fondoAutenticacion->setPixmap(imagenFondo);


            // TEXTO FELIX
            textoAutenticacion = new QLabel("“Adventure’s just ahead!”", contenedorAutenticacion);
            textoAutenticacion->setAlignment(Qt::AlignCenter);

            textoAutenticacion->setStyleSheet(
                "QLabel {"
                "background: transparent;"
                "color: black;"
                "}"
            );


            // LABELS
            labelUsername = new QLabel("Username", contenedorAutenticacion);
            labelPassword = new QLabel("Password", contenedorAutenticacion);

            labelUsername->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            labelPassword->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

            labelUsername->setStyleSheet("background: transparent; color: black;");
            labelPassword->setStyleSheet("background: transparent; color: black;");


            // CAMPOS
            campoUsername = new QLineEdit(contenedorAutenticacion);
            campoPassword = new QLineEdit(contenedorAutenticacion);

            campoPassword->setEchoMode(QLineEdit::Password);

            QString estiloCampo =
                "QLineEdit {"
                "background-color: #fff5e6;"
                "border: none;"
                "padding-left: 12px;"
                "color: black;"
                "}";

            campoUsername->setStyleSheet(estiloCampo);
            campoPassword->setStyleSheet(estiloCampo);


            // BOTONES
            botonCancelarAutenticacion = crearBotonImagen(obtenerRutaBoton("cancel"));
            botonAceptarAutenticacion = crearBotonImagen(obtenerRutaBoton("accept"));

            botonCancelarAutenticacion->setParent(contenedorAutenticacion);
            botonAceptarAutenticacion->setParent(contenedorAutenticacion);


            connect(botonCancelarAutenticacion, &QPushButton::clicked, this, [this]() {
                cancelarAutenticacion();
            });

            connect(botonAceptarAutenticacion, &QPushButton::clicked, this, [this]() {
                aceptarAutenticacion();
            });


            // Permite presionar Enter desde cualquiera de los campos.
            connect(campoUsername, &QLineEdit::returnPressed, this, [this]() {
                aceptarAutenticacion();
            });

            connect(campoPassword, &QLineEdit::returnPressed, this, [this]() {
                aceptarAutenticacion();
            });


            paginas->addWidget(paginaAutenticacion);
        }

        // PAGINA MENU PRINCIPAL AUTENTICADO
        void crearPaginaMenuPrincipal() {

            paginaMenuPrincipal = new QWidget();

            contenedorMenuPrincipal = new QWidget(paginaMenuPrincipal);


            // FONDO
            fondoMenuPrincipal = new QLabel(contenedorMenuPrincipal);
            fondoMenuPrincipal->setScaledContents(true);

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_MENU_PRINCIPAL));
            fondoMenuPrincipal->setPixmap(imagenFondo);


            // TEXTO DE BIENVENIDA
            textoBienvenida = new QLabel(contenedorMenuPrincipal);
            textoBienvenida->setAlignment(Qt::AlignCenter);

            textoBienvenida->setStyleSheet(
                "QLabel {"
                "background: transparent;"
                "color: white;"
                "}"
            );


            // BOTONES CON IMAGEN
            botonMiPerfil = crearBotonImagen(obtenerRutaMenu("my-profile"));
            botonComoJugar = crearBotonImagen(obtenerRutaMenu("how-to-play"));
            botonSalirSesion = crearBotonImagen(obtenerRutaMenu("exit"));
            botonHighScores = crearBotonImagen(obtenerRutaMenu("high-scores"));
            botonPlay = crearBotonImagen(obtenerRutaMenu("play"));

            botonMiPerfil->setParent(contenedorMenuPrincipal);
            botonComoJugar->setParent(contenedorMenuPrincipal);
            botonSalirSesion->setParent(contenedorMenuPrincipal);
            botonHighScores->setParent(contenedorMenuPrincipal);
            botonPlay->setParent(contenedorMenuPrincipal);


            // BOTONES TRANSPARENTES
            botonMapa = crearBotonTransparente();
            botonHuevoPascua = crearBotonTransparente();

            botonMapa->setParent(contenedorMenuPrincipal);
            botonHuevoPascua->setParent(contenedorMenuPrincipal);


            // ACCIONES
            connect(botonPlay, &QPushButton::clicked, this, [this]() {
                mostrarPlay();
            });

            connect(botonSalirSesion, &QPushButton::clicked, this, [this]() {
                cerrarSesion();
            });

            connect(botonMapa, &QPushButton::clicked, this, [this]() {
                mostrarModoMapa();
            });

            connect(botonHuevoPascua, &QPushButton::clicked, this, [this]() {
                mostrarHuevoPascua();
            });

            connect(botonMiPerfil, &QPushButton::clicked, this, [this]() {
                mostrarMiPerfil();
            });

            connect(botonComoJugar, &QPushButton::clicked, this, [this]() {
                mostrarComoJugar();
            });

            connect(botonHighScores, &QPushButton::clicked, this, [this]() {
                mostrarHighScores();
            });


            paginas->addWidget(paginaMenuPrincipal);
        }

        // PAGINA MAPA BLACKPOWDER BAY
        void crearPaginaMapa() {

            paginaMapa = new QWidget();

            contenedorMapa = new QWidget(paginaMapa);


            // FONDO
            fondoMapa = new QLabel(contenedorMapa);
            fondoMapa->setScaledContents(true);

            QPixmap imagenMapa(QString::fromStdString(TEMPLATE_BLACKPOWDER_BAY_MAP));
            fondoMapa->setPixmap(imagenMapa);


            // HOME
            botonHomeMapa = crearBotonImagen(obtenerRutaBoton("home"));
            botonHomeMapa->setParent(contenedorMapa);


            connect(botonHomeMapa, &QPushButton::clicked, this, [this]() {
                mostrarMenuPrincipal();
            });


            // ZONAS TRANSPARENTES DE LOS 9 NIVELES
            for (int i = 0; i < CANTIDAD_NIVELES; i++) {

                botonesNivelMapa[i] = crearBotonTransparente();
                botonesNivelMapa[i]->setParent(contenedorMapa);

                int numeroNivel = i + 1;

                connect(botonesNivelMapa[i], &QPushButton::clicked, this, [this, numeroNivel]() {
                    iniciarNivelMapa(numeroNivel);
                });
            }


            paginas->addWidget(paginaMapa);
        }

        // PAGINA HUEVO DE PASCUA
        void crearPaginaHuevoPascua() {

            paginaHuevoPascua = new QWidget();
            contenedorHuevoPascua = new QWidget(paginaHuevoPascua);

            // FONDO
            fondoHuevoPascua = new QLabel(contenedorHuevoPascua);
            fondoHuevoPascua->setScaledContents(true);

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_CLEAN_3));
            fondoHuevoPascua->setPixmap(imagenFondo);

            // TEXTO
            textoHuevoPascua = new QLabel(contenedorHuevoPascua);
            textoHuevoPascua->setAlignment(Qt::AlignCenter);
            textoHuevoPascua->setTextFormat(Qt::RichText);

            textoHuevoPascua->setText(
                "<div style='text-align:center;'>"
                "<b>Developed by</b><br>"
                "Galatea Monserrat<br>"
                "Ashley Emery"
                "<br><br><br>"
                "GMAE Inc., Havoc"
                "<br><br>"
                "Well, well... look who found us!<br>"
                "Thanks for playing, kid. We hope you had as much fun causing mayhem as we had making it.<br>"
                "Here’s <b>500 bonus points</b> for finding our little secret!"
                "<br><br><br>"
                "— Galatea M. &amp; Ashley E."
                "</div>"
            );

            textoHuevoPascua->setStyleSheet(
                "QLabel {"
                "background: transparent;"
                "color: black;"
                "}"
            );

            // CLAIM REWARD
            botonClaimReward = crearBotonImagen(obtenerRutaMenu("claim-reward"));
            botonClaimReward->setParent(contenedorHuevoPascua);

            // HOME
            botonHomeHuevoPascua = crearBotonImagen(obtenerRutaBoton("home"));
            botonHomeHuevoPascua->setParent(contenedorHuevoPascua);

            connect(botonClaimReward, &QPushButton::clicked, this, [this]() {
                reclamarRecompensaSecreta();
            });

            connect(botonHomeHuevoPascua, &QPushButton::clicked, this, [this]() {
                mostrarMenuPrincipal();
            });

            paginas->addWidget(paginaHuevoPascua);
        }

        // PAGINA HOW TO PLAY
        void crearPaginaComoJugar() {

            paginaComoJugar = new QWidget();
            contenedorComoJugar = new QWidget(paginaComoJugar);


            // FONDO
            fondoComoJugar = new QLabel(contenedorComoJugar);
            fondoComoJugar->setScaledContents(true);


            // BOTONES
            botonAdelanteComoJugar = crearBotonImagen(obtenerRutaBoton("forward"));
            botonAtrasComoJugar = crearBotonImagen(obtenerRutaBoton("back"));
            botonHomeComoJugar = crearBotonImagen(obtenerRutaBoton("home"));

            botonAdelanteComoJugar->setParent(contenedorComoJugar);
            botonAtrasComoJugar->setParent(contenedorComoJugar);
            botonHomeComoJugar->setParent(contenedorComoJugar);


            // ACCIONES
            connect(botonAdelanteComoJugar, &QPushButton::clicked, this, [this]() {
                avanzarComoJugar();
            });

            connect(botonAtrasComoJugar, &QPushButton::clicked, this, [this]() {
                retrocederComoJugar();
            });

            connect(botonHomeComoJugar, &QPushButton::clicked, this, [this]() {
                mostrarMenuPrincipal();
            });


            paginas->addWidget(paginaComoJugar);
        }

        // PAGINA MY PROFILE
        void crearPaginaPerfil() {

            paginaPerfil = new QWidget();
            contenedorPerfil = new QWidget(paginaPerfil);


            // FONDO
            fondoPerfil = new QLabel(contenedorPerfil);
            fondoPerfil->setScaledContents(true);

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_BOX));
            fondoPerfil->setPixmap(imagenFondo);


            // AVATAR
            imagenAvatarPerfil = new QLabel(contenedorPerfil);
            imagenAvatarPerfil->setAlignment(Qt::AlignCenter);
            imagenAvatarPerfil->setScaledContents(true);
            imagenAvatarPerfil->setStyleSheet("background: transparent;");


            // USERNAME
            textoUsernamePerfil = new QLabel(contenedorPerfil);
            textoUsernamePerfil->setAlignment(Qt::AlignCenter);
            textoUsernamePerfil->setStyleSheet(
                "QLabel {"
                "background: transparent;"
                "color: black;"
                "font-weight: bold;"
                "}"
            );

            // FLECHAS DE AVATAR
            botonAvatarAnterior = crearBotonImagen(obtenerRutaBoton("back"));
            botonAvatarSiguiente = crearBotonImagen(obtenerRutaBoton("forward"));

            botonAvatarAnterior->setParent(contenedorPerfil);
            botonAvatarSiguiente->setParent(contenedorPerfil);

            // ACCEPT / CANCEL
            botonAceptarPerfil = crearBotonImagen(obtenerRutaBoton("accept"));
            botonCancelarPerfil = crearBotonImagen(obtenerRutaBoton("cancel"));

            botonAceptarPerfil->setParent(contenedorPerfil);
            botonCancelarPerfil->setParent(contenedorPerfil);

            // CHANGE PASSWORD
            botonCambiarPassword = crearBotonTexto("CHANGE PASSWORD");
            botonCambiarPassword->setParent(contenedorPerfil);

            // ACCIONES
            connect(botonAvatarAnterior, &QPushButton::clicked, this, [this]() {
                avatarAnteriorPerfil();
            });

            connect(botonAvatarSiguiente, &QPushButton::clicked, this, [this]() {
                avatarSiguientePerfil();
            });

            connect(botonAceptarPerfil, &QPushButton::clicked, this, [this]() {
                guardarCambiosPerfil();
            });

            connect(botonCancelarPerfil, &QPushButton::clicked, this, [this]() {
                cancelarCambiosPerfil();
            });

            connect(botonCambiarPassword, &QPushButton::clicked, this, [this]() {
                mostrarCambioPassword();
            });


            paginas->addWidget(paginaPerfil);
        }

        // PAGINA CHANGE PASSWORD
        void crearPaginaCambioPassword() {

            paginaCambioPassword = new QWidget();
            contenedorCambioPassword = new QWidget(paginaCambioPassword);

            // FONDO
            fondoCambioPassword = new QLabel(contenedorCambioPassword);
            fondoCambioPassword->setScaledContents(true);

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_BOX));
            fondoCambioPassword->setPixmap(imagenFondo);

            // AVATAR
            imagenAvatarCambioPassword = new QLabel(contenedorCambioPassword);
            imagenAvatarCambioPassword->setAlignment(Qt::AlignCenter);
            imagenAvatarCambioPassword->setScaledContents(true);
            imagenAvatarCambioPassword->setStyleSheet("background: transparent;");

            // TEXTOS
            textoOldPassword = new QLabel("Old Password", contenedorCambioPassword);
            textoNewPassword = new QLabel("New Password", contenedorCambioPassword);

            textoOldPassword->setAlignment(Qt::AlignCenter);
            textoNewPassword->setAlignment(Qt::AlignCenter);

            textoOldPassword->setStyleSheet("background: transparent; color: black;");
            textoNewPassword->setStyleSheet("background: transparent; color: black;");

            // CAMPOS
            campoOldPassword = new QLineEdit(contenedorCambioPassword);
            campoNewPassword = new QLineEdit(contenedorCambioPassword);

            campoOldPassword->setEchoMode(QLineEdit::Password);
            campoNewPassword->setEchoMode(QLineEdit::Password);

            QString estiloCampoPassword =
                "QLineEdit {"
                "background-color: #fff5e6;"
                "border: none;"
                "padding-left: 12px;"
                "color: black;"
                "}";

            campoOldPassword->setStyleSheet(estiloCampoPassword);
            campoNewPassword->setStyleSheet(estiloCampoPassword);

            // BOTONES
            botonAceptarCambioPassword = crearBotonImagen(obtenerRutaBoton("accept"));
            botonCancelarCambioPassword = crearBotonImagen(obtenerRutaBoton("cancel"));

            botonAceptarCambioPassword->setParent(contenedorCambioPassword);
            botonCancelarCambioPassword->setParent(contenedorCambioPassword);

            // ACCIONES
            connect(botonAceptarCambioPassword, &QPushButton::clicked, this, [this]() {
                guardarCambioPassword();
            });

            connect(botonCancelarCambioPassword, &QPushButton::clicked, this, [this]() {
                cancelarCambioPassword();
            });

            connect(campoOldPassword, &QLineEdit::returnPressed, this, [this]() {
                guardarCambioPassword();
            });

            connect(campoNewPassword, &QLineEdit::returnPressed, this, [this]() {
                guardarCambioPassword();
            });

            paginas->addWidget(paginaCambioPassword);
        }

        // PAGINA HIGH SCORES
        void crearPaginaHighScores() {

            paginaHighScores = new QWidget();
            contenedorHighScores = new QWidget(paginaHighScores);


            // FONDO
            fondoHighScores = new QLabel(contenedorHighScores);
            fondoHighScores->setScaledContents(true);

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_CLEAN_2));
            fondoHighScores->setPixmap(imagenFondo);


            // IMAGEN DEL RANKING
            imagenRanking = new QLabel(contenedorHighScores);
            imagenRanking->setAlignment(Qt::AlignCenter);
            imagenRanking->setScaledContents(true);
            imagenRanking->setStyleSheet("background: transparent;");

            QPixmap ranking(QString::fromStdString(obtenerRutaRanking("bomb-squad")));
            imagenRanking->setPixmap(ranking);


            // TITULOS
            textoTituloMiScore = new QLabel("My Score", contenedorHighScores);
            textoMiScore = new QLabel(contenedorHighScores);
            textoTituloMedallas = new QLabel("MY MAYHEM MEDALS", contenedorHighScores);

            textoTituloMiScore->setAlignment(Qt::AlignCenter);
            textoMiScore->setAlignment(Qt::AlignCenter);
            textoTituloMedallas->setAlignment(Qt::AlignCenter);

            textoTituloMiScore->setStyleSheet("background: transparent; color: black;");
            textoMiScore->setStyleSheet("background: transparent; color: #215960;");
            textoTituloMedallas->setStyleSheet("background: transparent; color: black;");


            // TOP 5
            for (int i = 0; i < CANTIDAD_RANKING; i++) {

                textosRankingUsuario[i] = new QLabel(contenedorHighScores);
                textosRankingPuntaje[i] = new QLabel(contenedorHighScores);

                textosRankingUsuario[i]->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
                textosRankingPuntaje[i]->setAlignment(Qt::AlignCenter);

                textosRankingUsuario[i]->setStyleSheet("background: transparent; color: #6d5b39;");
                textosRankingPuntaje[i]->setStyleSheet("background: transparent; color: #6d5b39;");
            }


            // BADGES
            for (int i = 0; i < CANTIDAD_LOGROS; i++) {

                imagenesBadges[i] = new QLabel(contenedorHighScores);

                imagenesBadges[i]->setAlignment(Qt::AlignCenter);
                imagenesBadges[i]->setScaledContents(true);
                imagenesBadges[i]->setStyleSheet("background: transparent;");
            }


            // HOME
            botonHomeHighScores = crearBotonImagen(obtenerRutaBoton("home"));
            botonHomeHighScores->setParent(contenedorHighScores);

            connect(botonHomeHighScores, &QPushButton::clicked, this, [this]() {
                mostrarMenuPrincipal();
            });


            paginas->addWidget(paginaHighScores);
        }

        // PAGINA PLAY
        void crearPaginaPlay() {

            paginaPlay = new QWidget();
            contenedorPlay = new QWidget(paginaPlay);


            // FONDO
            fondoPlay = new QLabel(contenedorPlay);
            fondoPlay->setScaledContents(true);

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_FELIX));
            fondoPlay->setPixmap(imagenFondo);


            // TEXTO FELIX
            textoPlay = new QLabel("“Adventure’s just ahead!”", contenedorPlay);

            textoPlay->setAlignment(Qt::AlignCenter);

            textoPlay->setStyleSheet(
                "QLabel {"
                "background: transparent;"
                "color: black;"
                "}"
            );


            // BOTONES DE TEXTO
            botonNextQuest = crearBotonTexto("NEXT QUEST");
            botonCustomMayhem = crearBotonTexto("CUSTOM MAYHEM");

            botonNextQuest->setParent(contenedorPlay);
            botonCustomMayhem->setParent(contenedorPlay);


            // BOTONES DE IMAGEN
            botonHomePlay = crearBotonImagen(obtenerRutaBoton("home"));
            botonRewardsPlay = crearBotonImagen(obtenerRutaBoton("rewards"));
            botonHighScoresPlay = crearBotonImagen(obtenerRutaBoton("high-scores"));

            botonHomePlay->setParent(contenedorPlay);
            botonRewardsPlay->setParent(contenedorPlay);
            botonHighScoresPlay->setParent(contenedorPlay);


            // ACCIONES
            connect(botonHomePlay, &QPushButton::clicked, this, [this]() {
                mostrarMenuPrincipal();
            });

            connect(botonHighScoresPlay, &QPushButton::clicked, this, [this]() {
                mostrarHighScores();
            });

            connect(botonRewardsPlay, &QPushButton::clicked, this, [this]() {
                mostrarRewards();
            });

            connect(botonNextQuest, &QPushButton::clicked, this, [this]() {
                seleccionarSiguienteQuest();
            });

            connect(botonCustomMayhem, &QPushButton::clicked, this, [this]() {
                mostrarCustomMayhem();
            });


            paginas->addWidget(paginaPlay);
        }

        // PAGINA REWARDS
        void crearPaginaRewards() {

            paginaRewards = new QWidget();
            contenedorRewards = new QWidget(paginaRewards);


            // FONDO
            fondoRewards = new QLabel(contenedorRewards);
            fondoRewards->setScaledContents(true);

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_FELIX));
            fondoRewards->setPixmap(imagenFondo);


            // TEXTO
            textoRewards = new QLabel(
                "“Earn ’em. Pin ’em. Brag about ’em!”",
                contenedorRewards
            );

            textoRewards->setAlignment(Qt::AlignCenter);

            textoRewards->setStyleSheet(
                "QLabel {"
                "background: transparent;"
                "color: black;"
                "}"
            );


            // BADGES
            for (int i = 0; i < CANTIDAD_LOGROS; i++) {

                imagenesBadgesRewards[i] = new QLabel(contenedorRewards);

                imagenesBadgesRewards[i]->setAlignment(Qt::AlignCenter);
                imagenesBadgesRewards[i]->setScaledContents(true);
                imagenesBadgesRewards[i]->setStyleSheet("background: transparent;");
            }


            // BOTONES
            botonBackRewards = crearBotonImagen(obtenerRutaBoton("back"));
            botonInfoRewards = crearBotonImagen(obtenerRutaBoton("info"));

            botonBackRewards->setParent(contenedorRewards);
            botonInfoRewards->setParent(contenedorRewards);


            // ACCIONES
            connect(botonBackRewards, &QPushButton::clicked, this, [this]() {
                mostrarPlay();
            });

            connect(botonInfoRewards, &QPushButton::clicked, this, [this]() {
                mostrarBadgesInfo();
            });


            paginas->addWidget(paginaRewards);
        }

        // PAGINA BADGES INFO
        void crearPaginaBadgesInfo() {

            paginaBadgesInfo = new QWidget();
            contenedorBadgesInfo = new QWidget(paginaBadgesInfo);

            // FONDO
            fondoBadgesInfo = new QLabel(contenedorBadgesInfo);
            fondoBadgesInfo->setScaledContents(true);

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_BADGES_INFO));
            fondoBadgesInfo->setPixmap(imagenFondo);

            // BACK
            botonBackBadgesInfo = crearBotonImagen(obtenerRutaBoton("back"));
            botonBackBadgesInfo->setParent(contenedorBadgesInfo);

            // ACCION
            connect(botonBackBadgesInfo, &QPushButton::clicked, this, [this]() {
                mostrarRewards();
            });

            paginas->addWidget(paginaBadgesInfo);
        }

        // PAGINA PARTIDA GUARDADA
        void crearPaginaPartidaGuardada() {

            paginaPartidaGuardada = new QWidget();
            contenedorPartidaGuardada = new QWidget(paginaPartidaGuardada);

            // FONDO
            fondoPartidaGuardada = new QLabel(contenedorPartidaGuardada);
            fondoPartidaGuardada->setScaledContents(true);

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_FELIX));
            fondoPartidaGuardada->setPixmap(imagenFondo);

            // TEXTO
            textoPartidaGuardada = new QLabel("“Your quest, your call—carry on or start again!”", contenedorPartidaGuardada);
            textoPartidaGuardada->setAlignment(Qt::AlignCenter);
            textoPartidaGuardada->setStyleSheet("QLabel { background: transparent; color: black; }");

            // BOTONES DE TEXTO
            botonLoadQuest = crearBotonTexto("LOAD GAME");
            botonResetQuest = crearBotonTexto("RESET QUEST");

            botonLoadQuest->setParent(contenedorPartidaGuardada);
            botonResetQuest->setParent(contenedorPartidaGuardada);

            // BOTONES DE IMAGEN
            botonBackPartidaGuardada = crearBotonImagen(obtenerRutaBoton("back"));
            botonHomePartidaGuardada = crearBotonImagen(obtenerRutaBoton("home"));

            botonBackPartidaGuardada->setParent(contenedorPartidaGuardada);
            botonHomePartidaGuardada->setParent(contenedorPartidaGuardada);

            // ACCIONES
            connect(botonBackPartidaGuardada, &QPushButton::clicked, this, [this]() {
                mostrarPlay();
            });

            connect(botonHomePartidaGuardada, &QPushButton::clicked, this, [this]() {
                mostrarMenuPrincipal();
            });

            connect(botonLoadQuest, &QPushButton::clicked, this, [this]() {
                cargarQuestGuardada();
            });

            connect(botonResetQuest, &QPushButton::clicked, this, [this]() {
                resetearQuestGuardada();
            });

            paginas->addWidget(paginaPartidaGuardada);
        }

        // PAGINA PARTIDA MAPA GUARDADA
        void crearPaginaMapaGuardada() {

            paginaMapaGuardada = new QWidget();
            contenedorMapaGuardada = new QWidget(paginaMapaGuardada);

            // FONDO
            fondoMapaGuardada = new QLabel(contenedorMapaGuardada);
            fondoMapaGuardada->setScaledContents(true);

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_FELIX));
            fondoMapaGuardada->setPixmap(imagenFondo);

            // TEXTO
            textoMapaGuardada = new QLabel(
                "“Your quest, your call—carry on or start again!”",
                contenedorMapaGuardada
            );

            textoMapaGuardada->setAlignment(Qt::AlignCenter);

            textoMapaGuardada->setStyleSheet(
                "QLabel {"
                "background: transparent;"
                "color: black;"
                "}"
            );

            // BOTON BACK
            botonBackMapaGuardada = crearBotonImagen(obtenerRutaBoton("back"));
            botonBackMapaGuardada->setParent(contenedorMapaGuardada);

            // BOTONES DE TEXTO
            botonLoadMapaGuardada = crearBotonTexto("LOAD GAME");
            botonNewQuestMapa = crearBotonTexto("NEW QUEST");

            botonLoadMapaGuardada->setParent(contenedorMapaGuardada);
            botonNewQuestMapa->setParent(contenedorMapaGuardada);

            // ACCIONES
            connect(botonBackMapaGuardada, &QPushButton::clicked, this, [this]() {
                mostrarMenuPrincipal();
            });

            connect(botonLoadMapaGuardada, &QPushButton::clicked, this, [this]() {
                cargarMapaGuardado();
            });

            connect(botonNewQuestMapa, &QPushButton::clicked, this, [this]() {
                mostrarMapaNuevo();
            });

            paginas->addWidget(paginaMapaGuardada);
        }

        // PAGINA PARTIDA PERSONALIZADA GUARDADA
        void crearPaginaPersonalizadaGuardada() {

            paginaPersonalizadaGuardada = new QWidget();
            contenedorPersonalizadaGuardada = new QWidget(paginaPersonalizadaGuardada);

            // FONDO
            fondoPersonalizadaGuardada = new QLabel(contenedorPersonalizadaGuardada);
            fondoPersonalizadaGuardada->setScaledContents(true);

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_FELIX));
            fondoPersonalizadaGuardada->setPixmap(imagenFondo);

            // TEXTO
            textoPersonalizadaGuardada = new QLabel(
                "“Your quest, your call—carry on or start again!”",
                contenedorPersonalizadaGuardada
            );

            textoPersonalizadaGuardada->setAlignment(Qt::AlignCenter);

            textoPersonalizadaGuardada->setStyleSheet(
                "QLabel {"
                "background: transparent;"
                "color: black;"
                "}"
            );

            // BOTON BACK
            botonBackPersonalizadaGuardada =
                crearBotonImagen(obtenerRutaBoton("back"));

            botonBackPersonalizadaGuardada->setParent(
                contenedorPersonalizadaGuardada
            );

            // BOTONES DE TEXTO
            botonLoadPersonalizadaGuardada = crearBotonTexto("LOAD GAME");
            botonNewQuestPersonalizada = crearBotonTexto("NEW QUEST");

            botonLoadPersonalizadaGuardada->setParent(
                contenedorPersonalizadaGuardada
            );

            botonNewQuestPersonalizada->setParent(
                contenedorPersonalizadaGuardada
            );

            // ACCIONES
            connect(
                botonBackPersonalizadaGuardada,
                &QPushButton::clicked,
                this,
                [this]() {
                    mostrarPlay();
                }
            );

            connect(
                botonLoadPersonalizadaGuardada,
                &QPushButton::clicked,
                this,
                [this]() {
                    cargarPersonalizadaGuardada();
                }
            );

            connect(
                botonNewQuestPersonalizada,
                &QPushButton::clicked,
                this,
                [this]() {
                    mostrarCustomMayhemNuevo();
                }
            );

            paginas->addWidget(paginaPersonalizadaGuardada);
        }

        // PAGINA CUSTOM MAYHEM
        void crearPaginaCustomMayhem() {

            paginaCustomMayhem = new QWidget();
            contenedorCustomMayhem = new QWidget(paginaCustomMayhem);

            // FONDO
            fondoCustomMayhem = new QLabel(contenedorCustomMayhem);
            fondoCustomMayhem->setScaledContents(true);

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_FELIX));
            fondoCustomMayhem->setPixmap(imagenFondo);

            // TEXTO FELIX
            textoCustomMayhem = new QLabel(
                "“Make it your own, kid! Pick the board size and set the challenge!”",
                contenedorCustomMayhem
            );

            textoCustomMayhem->setAlignment(Qt::AlignCenter);
            textoCustomMayhem->setStyleSheet(
                "QLabel {"
                "background: transparent;"
                "color: black;"
                "}"
            );

            // INSTRUCCION
            textoInstruccionCustomMayhem = new QLabel(
                "Pick anywhere from 8 to 40 columns and 8 to 12 rows.",
                contenedorCustomMayhem
            );

            textoInstruccionCustomMayhem->setAlignment(Qt::AlignCenter);
            textoInstruccionCustomMayhem->setStyleSheet(
                "QLabel {"
                "background: transparent;"
                "color: black;"
                "}"
            );

            // X ENTRE FILAS Y COLUMNAS
            textoXCustomMayhem = new QLabel("x", contenedorCustomMayhem);
            textoXCustomMayhem->setAlignment(Qt::AlignCenter);
            textoXCustomMayhem->setStyleSheet(
                "QLabel {"
                "background: transparent;"
                "color: black;"
                "}"
            );

            // DROPDOWN FILAS
            comboFilasCustomMayhem = new QComboBox(contenedorCustomMayhem);

            for (int fila = FILAS_MINIMAS; fila <= FILAS_MAXIMAS; fila++) {
                comboFilasCustomMayhem->addItem(QString::number(fila));
            }

            comboFilasCustomMayhem->setCurrentText("8");
            comboFilasCustomMayhem->setCursor(Qt::PointingHandCursor);
            comboFilasCustomMayhem->setFocusPolicy(Qt::NoFocus);

            // DROPDOWN COLUMNAS
            comboColumnasCustomMayhem = new QComboBox(contenedorCustomMayhem);

            for (int columna = COLUMNAS_MINIMAS; columna <= COLUMNAS_MAXIMAS; columna++) {
                comboColumnasCustomMayhem->addItem(QString::number(columna));
            }

            comboColumnasCustomMayhem->setCurrentText("20");
            comboColumnasCustomMayhem->setCursor(Qt::PointingHandCursor);
            comboColumnasCustomMayhem->setFocusPolicy(Qt::NoFocus);

            // ESTILO DE LOS DROPDOWNS
            QString rutaDropdown = QString::fromStdString(BOTON_DROPDOWN);

            QString estiloDropdown =
                "QComboBox {"
                "border: none;"
                "background: transparent;"
                "border-image: url(" + rutaDropdown + ") 0 0 0 0 stretch stretch;"
                "color: black;"
                "padding-left: 20px;"
                "padding-right: 8px;"
                "}"
                "QComboBox::drop-down {"
                "border: none;"
                "background: transparent;"
                "width: 45px;"
                "}"
                "QComboBox::down-arrow {"
                "image: none;"
                "}"
                "QComboBox QAbstractItemView {"
                "background-color: #fff5e6;"
                "color: black;"
                "border: 2px solid black;"
                "selection-background-color: #de461b;"
                "selection-color: white;"
                "}";

            comboFilasCustomMayhem->setStyleSheet(estiloDropdown);
            comboColumnasCustomMayhem->setStyleSheet(estiloDropdown);

            // BACK
            botonBackCustomMayhem = crearBotonImagen(obtenerRutaBoton("back"));
            botonBackCustomMayhem->setParent(contenedorCustomMayhem);

            // MAPA CALAMITY CAYS
            botonMapaCustomMayhem = crearBotonImagen(obtenerRutaBoton("map"));
            botonMapaCustomMayhem->setParent(contenedorCustomMayhem);

            // PLAY GAME
            botonPlayGameCustomMayhem = crearBotonTexto("PLAY GAME");
            botonPlayGameCustomMayhem->setParent(contenedorCustomMayhem);

            // ACCIONES
            connect(botonBackCustomMayhem, &QPushButton::clicked, this, [this]() {
                mostrarPlay();
            });

            connect(botonPlayGameCustomMayhem, &QPushButton::clicked, this, [this]() {
                iniciarPartidaPersonalizada();
            });

            connect(botonMapaCustomMayhem, &QPushButton::clicked, this, [this]() {
                mostrarMapaCustomMayhem();
            });

            paginas->addWidget(paginaCustomMayhem);
        }

        // PAGINA MAPA CALAMITY CAYS
        void crearPaginaMapaCustomMayhem() {

            paginaMapaCustomMayhem = new QWidget();
            contenedorMapaCustomMayhem = new QWidget(paginaMapaCustomMayhem);

            // FONDO
            fondoMapaCustomMayhem = new QLabel(contenedorMapaCustomMayhem);
            fondoMapaCustomMayhem->setScaledContents(true);

            QPixmap imagenMapa(
                QString::fromStdString(TEMPLATE_CALAMITY_CAYS_MAP)
            );

            fondoMapaCustomMayhem->setPixmap(imagenMapa);

            // BACK
            botonBackMapaCustomMayhem =
                crearBotonImagen(obtenerRutaBoton("back"));

            botonBackMapaCustomMayhem->setParent(
                contenedorMapaCustomMayhem
            );

            // ACCION
            connect(
                botonBackMapaCustomMayhem,
                &QPushButton::clicked,
                this,
                [this]() {
                    mostrarCustomMayhemDesdeMapa();
                }
            );

            paginas->addWidget(paginaMapaCustomMayhem);
        }

        void mostrarMapaCustomMayhem() {

            paginas->setCurrentWidget(paginaMapaCustomMayhem);

            ajustarInterfaz();
        }


        void mostrarCustomMayhemDesdeMapa() {

            paginas->setCurrentWidget(paginaCustomMayhem);

            ajustarInterfaz();
        }

        // CREAR BOTON DE TEXTO
        QPushButton* crearBotonTexto(const QString &texto) {

            QPushButton* boton = new QPushButton(texto);

            boton->setCursor(Qt::PointingHandCursor);
            boton->setFlat(true);
            boton->setFocusPolicy(Qt::NoFocus);

            boton->setStyleSheet(
                "QPushButton {"
                "background: transparent;"
                "border: none;"
                "outline: none;"
                "color: black;"
                "font-weight: bold;"
                "}"
                "QPushButton:hover {"
                "color: #de461b;"
                "}"
                "QPushButton:focus {"
                "border: none;"
                "outline: none;"
                "}"
            );

            return boton;
        }

        // CREAR BOTON CON IMAGEN
        QPushButton* crearBotonImagen(const string &ruta) {

            QPushButton* boton = new QPushButton();

            boton->setCursor(Qt::PointingHandCursor);
            boton->setFlat(true);
            boton->setFocusPolicy(Qt::NoFocus);

            boton->setStyleSheet(
                "QPushButton {"
                "background: transparent;"
                "border: none;"
                "outline: none;"
                "}"
                "QPushButton:focus {"
                "border: none;"
                "outline: none;"
                "}"
            );

            boton->setIcon(QIcon(QString::fromStdString(ruta)));
            boton->setIconSize(QSize(120, 120));

            return boton;
        }

        QPushButton* crearBotonTransparente() {

            QPushButton* boton = new QPushButton();

            boton->setCursor(Qt::PointingHandCursor);
            boton->setFlat(true);
            boton->setFocusPolicy(Qt::NoFocus);

            boton->setStyleSheet(
                "QPushButton {"
                "background: transparent;"
                "border: none;"
                "outline: none;"
                "}"
            );

            return boton;
        }

        QPixmap convertirAGris(const QPixmap &pixmapOriginal) {

            QImage imagen = pixmapOriginal.toImage().convertToFormat(QImage::Format_ARGB32);

            for (int y = 0; y < imagen.height(); y++) {

                for (int x = 0; x < imagen.width(); x++) {

                    QRgb pixel = imagen.pixel(x, y);

                    int gris = qGray(pixel);

                    imagen.setPixel(
                        x,
                        y,
                        qRgba(
                            gris,
                            gris,
                            gris,
                            qAlpha(pixel)
                        )
                    );
                }
            }

            return QPixmap::fromImage(imagen);
        }

        // REPRODUCIR INTRO
        void reproducirIntro() {

            if (introIniciada) {
                return;
            }

            introIniciada = true;
            introFinalizada = false;

            reproductorIntro->stop();
            reproductorIntro->setMedia(crearUrlArchivo(ANIMACION_INTRO));

            paginas->setCurrentWidget(paginaIntro);

            videoIntro->setGeometry(
                0,
                0,
                paginaIntro->width(),
                paginaIntro->height()
            );

            videoIntro->show();
            videoIntro->raise();

            reproductorIntro->play();

            // intro.mp4 dura 4 segundos.
            // Este timer solamente funciona como respaldo si Qt no genera EndOfMedia.
            QTimer::singleShot(4500, this, [this]() {

                if (!introFinalizada) {
                    finalizarIntro();
                }
            });
        }

        // FINALIZAR INTRO
        void finalizarIntro() {

            if (introFinalizada) {
                return;
            }

            introFinalizada = true;

            reproductorIntro->stop();

            paginas->setCurrentWidget(paginaInicio);

            ajustarInterfaz();
        }

        void mostrarMenuPrincipal() {

            if (usuarioActual == nullptr) {
                return;
            }

            QString nombreUsuario = QString::fromStdString(usuarioActual->obtenerNombreUsuario());

            textoBienvenida->setText(
                "Welcome back, " + nombreUsuario + "!\n"
                "Ready for another scrape, kid?"
            );

            paginas->setCurrentWidget(paginaMenuPrincipal);

            ajustarInterfaz();
        }

        void cerrarSesion() {

            usuarioActual = nullptr;

            campoUsername->clear();
            campoPassword->clear();

            paginas->setCurrentWidget(paginaInicio);

            ajustarInterfaz();
        }

        void mostrarPlay() {

            if (usuarioActual == nullptr) {
                return;
            }

            paginas->setCurrentWidget(paginaPlay);

            ajustarInterfaz();
        }

        void seleccionarSiguienteQuest() {

            if (usuarioActual == nullptr) {
                return;
            }

            int numeroNivel = usuarioActual->obtenerSiguienteNivelProgresivo();

            if (numeroNivel > CANTIDAD_NIVELES) {

                QMessageBox::information(
                    this,
                    "Progressive Mode",
                    "You have completed all 9 quests!"
                );

                return;
            }

            bool tienePartidaGuardada = ArchivoPersistencia::existePartidaGuardada(
                usuarioActual->obtenerNombreUsuario(),
                ModoJuego::PROGRESIVO
            );

            if (tienePartidaGuardada) {

                mostrarPartidaGuardada();

                return;
            }

            iniciarModoProgresivo();
        }

        void iniciarModoProgresivo() {

            if (usuarioActual == nullptr) {
                return;
            }

            int numeroNivel = usuarioActual->obtenerSiguienteNivelProgresivo();

            if (numeroNivel < 1 || numeroNivel > CANTIDAD_NIVELES) {
                return;
            }

            QWidget* ventanaJuego = crearJuegoUI(
                usuarioActual,
                &sistemaUsuarios,
                ModoJuego::PROGRESIVO,
                numeroNivel
            );

            ventanaJuego->setAttribute(Qt::WA_DeleteOnClose);
            ventanaJuego->setAttribute(Qt::WA_QuitOnClose, false);

            conectarRetornoJuego(ventanaJuego);
            ventanaJuego->show();

            hide();
        }

        void mostrarPartidaGuardada() {

            if (usuarioActual == nullptr) {
                return;
            }

            if (!ArchivoPersistencia::existePartidaGuardada(usuarioActual->obtenerNombreUsuario(), ModoJuego::PROGRESIVO)) {
                iniciarModoProgresivo();
                return;
            }

            paginas->setCurrentWidget(paginaPartidaGuardada);

            ajustarInterfaz();
        }

        void cargarQuestGuardada() {

            if (usuarioActual == nullptr) {
                return;
            }

            QWidget* ventanaJuego = crearJuegoUICargado(
                usuarioActual,
                &sistemaUsuarios,
                ModoJuego::PROGRESIVO
            );

            if (ventanaJuego == nullptr) {

                QMessageBox::warning(
                    this,
                    "Load Quest",
                    "The saved quest could not be loaded."
                );

                return;
            }

            ventanaJuego->setAttribute(Qt::WA_DeleteOnClose);
            ventanaJuego->setAttribute(Qt::WA_QuitOnClose, false);

            conectarRetornoJuego(ventanaJuego);

            ventanaJuego->show();

            hide();
        }

        void resetearQuestGuardada() {

            if (usuarioActual == nullptr) {
                return;
            }

            string nombreUsuario = usuarioActual->obtenerNombreUsuario();

            if (ArchivoPersistencia::existePartidaGuardada(nombreUsuario, ModoJuego::PROGRESIVO)) {

                if (!ArchivoPersistencia::eliminarPartidaGuardada(nombreUsuario, ModoJuego::PROGRESIVO)) {

                    QMessageBox::warning(
                        this,
                        "Reset Quest",
                        "The saved quest could not be deleted."
                    );

                    return;
                }
            }

            iniciarModoProgresivo();
        }

        void mostrarRewards() {

            if (usuarioActual == nullptr) {
                return;
            }

            actualizarRewards();

            paginas->setCurrentWidget(paginaRewards);

            ajustarInterfaz();
        }

        void actualizarRewards() {

            if (usuarioActual == nullptr) {
                return;
            }

            for (int i = 0; i < CANTIDAD_LOGROS; i++) {

                TipoLogro logro = static_cast<TipoLogro>(i);

                string rutaBadge = obtenerRutaBadge(logro);

                QPixmap badgeOriginal(
                    QString::fromStdString(rutaBadge)
                );


                if (usuarioActual->tieneLogro(logro)) {

                    imagenesBadgesRewards[i]->setPixmap(
                        badgeOriginal
                    );

                } else {

                    imagenesBadgesRewards[i]->setPixmap(
                        convertirAGris(badgeOriginal)
                    );
                }


                imagenesBadgesRewards[i]->show();
            }
        }

        void mostrarBadgesInfo() {

            paginas->setCurrentWidget(paginaBadgesInfo);

            ajustarInterfaz();
        }

        void mostrarCustomMayhem() {

            if (usuarioActual == nullptr) {
                return;
            }

            bool tienePartidaGuardada =
                ArchivoPersistencia::existePartidaGuardada(
                    usuarioActual->obtenerNombreUsuario(),
                    ModoJuego::PERSONALIZADO
                );

            if (tienePartidaGuardada) {

                paginas->setCurrentWidget(paginaPersonalizadaGuardada);

                ajustarInterfaz();

                return;
            }

            mostrarCustomMayhemNuevo();
        }

        void mostrarCustomMayhemNuevo() {

            if (usuarioActual == nullptr) {
                return;
            }

            comboFilasCustomMayhem->setCurrentText("8");
            comboColumnasCustomMayhem->setCurrentText("20");

            paginas->setCurrentWidget(paginaCustomMayhem);

            ajustarInterfaz();
        }

        void cargarPersonalizadaGuardada() {

            if (usuarioActual == nullptr) {
                return;
            }

            string nombreUsuario =
                usuarioActual->obtenerNombreUsuario();

            if (!ArchivoPersistencia::existePartidaGuardada(
                    nombreUsuario,
                    ModoJuego::PERSONALIZADO
                )) {

                mostrarCustomMayhemNuevo();

                return;
            }

            QWidget* ventanaJuego = crearJuegoUICargado(
                usuarioActual,
                &sistemaUsuarios,
                ModoJuego::PERSONALIZADO
            );

            if (ventanaJuego == nullptr) {

                QMessageBox::warning(
                    this,
                    "Load Game",
                    "The saved custom game could not be loaded."
                );

                return;
            }

            ventanaJuego->setAttribute(Qt::WA_DeleteOnClose);
            ventanaJuego->setAttribute(Qt::WA_QuitOnClose, false);

            conectarRetornoJuego(ventanaJuego);

            ventanaJuego->show();

            hide();
        }

        void iniciarPartidaPersonalizada() {

            if (usuarioActual == nullptr) {
                return;
            }

            int filas = comboFilasCustomMayhem->currentText().toInt();
            int columnas = comboColumnasCustomMayhem->currentText().toInt();

            string mensajeError;

            if (!validarConfiguracionPersonalizada(filas, columnas, mensajeError)) {

                QMessageBox::warning(
                    this,
                    "Custom Mayhem",
                    QString::fromStdString(mensajeError)
                );

                return;
            }

            QWidget* ventanaJuego = crearJuegoUIPersonalizado(
                usuarioActual,
                &sistemaUsuarios,
                filas,
                columnas
            );

            if (ventanaJuego == nullptr) {

                QMessageBox::warning(
                    this,
                    "Custom Mayhem",
                    "The custom game could not be created."
                );

                return;
            }

            ventanaJuego->setAttribute(Qt::WA_DeleteOnClose);
            ventanaJuego->setAttribute(Qt::WA_QuitOnClose, false);

            conectarRetornoJuego(ventanaJuego);

            ventanaJuego->show();

            hide();
        }

        void mostrarMiPerfil() {

            if (usuarioActual == nullptr) {
                return;
            }

            string avatarActual = usuarioActual->obtenerAvatar();

            avatarPerfilSeleccionado = 1;

            for (int i = 1; i <= 10; i++) {

                string nombreAvatar = "avatar_" + to_string(i) + ".png";

                if (avatarActual == nombreAvatar) {
                    avatarPerfilSeleccionado = i;
                    break;
                }
            }

            textoUsernamePerfil->setText(QString::fromStdString(usuarioActual->obtenerNombreUsuario()));

            actualizarAvatarPerfil();

            paginas->setCurrentWidget(paginaPerfil);

            ajustarInterfaz();
        }

        void actualizarAvatarPerfil() {

            string rutaAvatar = obtenerRutaAvatar(avatarPerfilSeleccionado);

            QPixmap avatar(QString::fromStdString(rutaAvatar));

            imagenAvatarPerfil->setPixmap(avatar);
        }

        void avatarAnteriorPerfil() {

            avatarPerfilSeleccionado--;

            if (avatarPerfilSeleccionado < 1) {
                avatarPerfilSeleccionado = 10;
            }

            actualizarAvatarPerfil();
        }

        void avatarSiguientePerfil() {

            avatarPerfilSeleccionado++;

            if (avatarPerfilSeleccionado > 10) {
                avatarPerfilSeleccionado = 1;
            }

            actualizarAvatarPerfil();
        }

        void guardarCambiosPerfil() {

            if (usuarioActual == nullptr) {
                return;
            }

            string avatarAnterior = usuarioActual->obtenerAvatar();
            string nuevoAvatar = "avatar_" + to_string(avatarPerfilSeleccionado) + ".png";

            if (!avatarValido(nuevoAvatar)) {
                return;
            }

            usuarioActual->establecerAvatar(nuevoAvatar);

            if (!sistemaUsuarios.guardar()) {

                usuarioActual->establecerAvatar(avatarAnterior);

                QMessageBox::warning(
                    this,
                    "My Profile",
                    "The avatar could not be saved."
                );

                return;
            }

            mostrarMenuPrincipal();
        }

        void cancelarCambiosPerfil() {

            if (usuarioActual == nullptr) {
                return;
            }

            mostrarMenuPrincipal();
        }

        void mostrarCambioPassword() {

            if (usuarioActual == nullptr) {
                return;
            }

            campoOldPassword->clear();
            campoNewPassword->clear();

            string rutaAvatar = obtenerRutaAvatar(usuarioActual->obtenerAvatar());
            imagenAvatarCambioPassword->setPixmap(QPixmap(QString::fromStdString(rutaAvatar)));

            paginas->setCurrentWidget(paginaCambioPassword);

            campoOldPassword->setFocus();

            ajustarInterfaz();
        }

        void cancelarCambioPassword() {

            campoOldPassword->clear();
            campoNewPassword->clear();

            mostrarMiPerfil();
        }

        void guardarCambioPassword() {

            if (usuarioActual == nullptr) {
                return;
            }

            string contrasenaAnterior = campoOldPassword->text().toStdString();
            string contrasenaNueva = campoNewPassword->text().toStdString();


            if (contrasenaAnterior.empty() || contrasenaNueva.empty()) {

                QMessageBox::warning(
                    this,
                    "Change Password",
                    "Old password and new password are required."
                );

                return;
            }


            string mensajeError;

            if (!sistemaUsuarios.cambiarContrasenaUsuario(*usuarioActual, contrasenaAnterior, contrasenaNueva, mensajeError)) {

                QMessageBox::warning(
                    this,
                    "Change Password",
                    QString::fromStdString(mensajeError)
                );

                campoNewPassword->clear();

                return;
            }


            QMessageBox::information(
                this,
                "Change Password",
                "Password changed successfully."
            );


            campoOldPassword->clear();
            campoNewPassword->clear();

            mostrarMiPerfil();
        }

        void mostrarComoJugar() {

            paginaComoJugarActual = 1;

            actualizarComoJugar();

            paginas->setCurrentWidget(paginaComoJugar);

            ajustarInterfaz();
        }

        void avanzarComoJugar() {

            if (paginaComoJugarActual < 3) {
                paginaComoJugarActual++;
            }

            actualizarComoJugar();
        }


        void retrocederComoJugar() {

            if (paginaComoJugarActual > 1) {
                paginaComoJugarActual--;
            }

            actualizarComoJugar();
        }

        void actualizarComoJugar() {

            switch (paginaComoJugarActual) {

                case 1: {
                    fondoComoJugar->setPixmap(QPixmap(QString::fromStdString(TEMPLATE_HOW_TO_PLAY_1)));

                    botonAtrasComoJugar->hide();
                    botonHomeComoJugar->hide();
                    botonAdelanteComoJugar->show();

                    break;
                }

                case 2: {
                    fondoComoJugar->setPixmap(QPixmap(QString::fromStdString(TEMPLATE_HOW_TO_PLAY_2)));

                    botonAtrasComoJugar->show();
                    botonHomeComoJugar->hide();
                    botonAdelanteComoJugar->show();

                    break;
                }

                case 3: {
                    fondoComoJugar->setPixmap(QPixmap(QString::fromStdString(TEMPLATE_HOW_TO_PLAY_3)));

                    botonAtrasComoJugar->show();
                    botonHomeComoJugar->show();
                    botonAdelanteComoJugar->hide();

                    break;
                }
            }
        }

        void mostrarHighScores() {

            if (usuarioActual == nullptr) {
                return;
            }

            actualizarHighScores();

            paginas->setCurrentWidget(paginaHighScores);

            ajustarInterfaz();
        }

        void actualizarHighScores() {

            if (usuarioActual == nullptr) {
                return;
            }


            // -------------------------------------------------------------------------
            // SCORE DEL USUARIO ACTUAL
            // -------------------------------------------------------------------------

            textoMiScore->setText(
                QString("%1").arg(
                    usuarioActual->obtenerPuntajeTotal(),
                    5,
                    10,
                    QChar('0')
                )
            );


            // -------------------------------------------------------------------------
            // RANKING TOP 5
            // -------------------------------------------------------------------------

            Usuario* ranking[CANTIDAD_RANKING];

            int cantidadRanking = sistemaUsuarios.obtenerRanking(ranking);


            for (int i = 0; i < CANTIDAD_RANKING; i++) {

                if (i < cantidadRanking && ranking[i] != nullptr) {

                    textosRankingUsuario[i]->setText(
                        QString::fromStdString(ranking[i]->obtenerNombreUsuario())
                    );

                    textosRankingPuntaje[i]->setText(
                        QString::number(ranking[i]->obtenerPuntajeTotal())
                    );

                    textosRankingUsuario[i]->show();
                    textosRankingPuntaje[i]->show();

                } else {

                    textosRankingUsuario[i]->clear();
                    textosRankingPuntaje[i]->clear();

                    textosRankingUsuario[i]->hide();
                    textosRankingPuntaje[i]->hide();
                }
            }


            // -------------------------------------------------------------------------
            // BADGES
            // -------------------------------------------------------------------------

            for (int i = 0; i < CANTIDAD_LOGROS; i++) {

                TipoLogro logro = static_cast<TipoLogro>(i);

                if (usuarioActual->tieneLogro(logro)) {

                    string rutaBadge = obtenerRutaBadge(logro);

                    imagenesBadges[i]->setPixmap(
                        QPixmap(QString::fromStdString(rutaBadge))
                    );

                    imagenesBadges[i]->show();

                } else {

                    imagenesBadges[i]->clear();
                    imagenesBadges[i]->hide();
                }
            }
        }

        void mostrarModoMapa() {

            if (usuarioActual == nullptr) {
                return;
            }

            bool tienePartidaGuardada = ArchivoPersistencia::existePartidaGuardada(
                usuarioActual->obtenerNombreUsuario(),
                ModoJuego::MAPA
            );

            if (tienePartidaGuardada) {

                paginas->setCurrentWidget(paginaMapaGuardada);

                ajustarInterfaz();

                return;
            }

            mostrarMapaNuevo();
        }

        void mostrarMapaNuevo() {

            if (usuarioActual == nullptr) {
                return;
            }

            paginas->setCurrentWidget(paginaMapa);

            ajustarInterfaz();
        }

        void cargarMapaGuardado() {

            if (usuarioActual == nullptr) {
                return;
            }

            string nombreUsuario = usuarioActual->obtenerNombreUsuario();

            if (!ArchivoPersistencia::existePartidaGuardada(nombreUsuario, ModoJuego::MAPA)) {

                mostrarMapaNuevo();

                return;
            }

            QWidget* ventanaJuego = crearJuegoUICargado(
                usuarioActual,
                &sistemaUsuarios,
                ModoJuego::MAPA
            );

            if (ventanaJuego == nullptr) {

                QMessageBox::warning(
                    this,
                    "Load Game",
                    "The saved map game could not be loaded."
                );

                return;
            }

            ventanaJuego->setAttribute(Qt::WA_DeleteOnClose);
            ventanaJuego->setAttribute(Qt::WA_QuitOnClose, false);

            conectarRetornoJuego(ventanaJuego);

            ventanaJuego->show();

            hide();
        }

        void iniciarNivelMapa(int numeroNivel) {

            if (usuarioActual == nullptr) {
                return;
            }

            if (numeroNivel < 1 || numeroNivel > CANTIDAD_NIVELES) {
                return;
            }

            QWidget* ventanaJuego = crearJuegoUI(
                usuarioActual,
                &sistemaUsuarios,
                ModoJuego::MAPA,
                numeroNivel
            );

            ventanaJuego->setAttribute(Qt::WA_DeleteOnClose);
            ventanaJuego->setAttribute(Qt::WA_QuitOnClose, false);

            conectarRetornoJuego(ventanaJuego);;

            ventanaJuego->show();

            hide();
        }

        void mostrarHuevoPascua() {

            if (usuarioActual == nullptr) {
                return;
            }

            bool recompensaReclamada = usuarioActual->reclamoRecompensaSecreta();

            botonClaimReward->setEnabled(!recompensaReclamada);

            if (recompensaReclamada) {

                botonClaimReward->setCursor(Qt::ArrowCursor);

                botonClaimReward->setStyleSheet(
                    "QPushButton {"
                    "background: transparent;"
                    "border: none;"
                    "outline: none;"
                    "}"
                );

            } else {
                botonClaimReward->setCursor(Qt::PointingHandCursor);
            }

            paginas->setCurrentWidget(paginaHuevoPascua);

            ajustarInterfaz();
        }

        void reclamarRecompensaSecreta() {

            if (usuarioActual == nullptr) {
                return;
            }

            if (usuarioActual->reclamoRecompensaSecreta()) {

                QMessageBox::information(
                    this,
                    "Secret Reward",
                    "You already claimed this reward."
                );

                return;
            }

            if (!SistemaPuntajes::reclamarRecompensaSecreta(*usuarioActual, sistemaUsuarios)) {

                QMessageBox::warning(
                    this,
                    "Secret Reward",
                    "The reward could not be claimed."
                );

                return;
            }

            QMessageBox::information(
                this,
                "Secret Reward",
                "500 bonus points added to your score!"
            );

            mostrarMenuPrincipal();
        }

        // LOGIN
        void mostrarLogin() {

            creandoCuenta = false;

            campoUsername->clear();
            campoPassword->clear();

            paginas->setCurrentWidget(paginaAutenticacion);

            campoUsername->setFocus();

            ajustarInterfaz();
        }


        // SIGN IN
        void mostrarSignin() {

            creandoCuenta = true;

            campoUsername->clear();
            campoPassword->clear();

            paginas->setCurrentWidget(paginaAutenticacion);

            campoUsername->setFocus();

            ajustarInterfaz();
        }

        // CANCELAR AUTENTICACION
        void cancelarAutenticacion() {

            campoUsername->clear();
            campoPassword->clear();

            paginas->setCurrentWidget(paginaInicio);

            ajustarInterfaz();
        }

        // ACEPTAR AUTENTICACION
        void aceptarAutenticacion() {

            string username = campoUsername->text().trimmed().toStdString();
            string password = campoPassword->text().toStdString();


            if (username.empty() || password.empty()) {

                QMessageBox::warning(
                    this,
                    "Authentication",
                    "Username and password are required."
                );

                return;
            }


            // -------------------------------------------------------------------------
            // SIGN IN - CREAR CUENTA
            // -------------------------------------------------------------------------

            if (creandoCuenta) {

                string mensajeError;

                if (!sistemaUsuarios.registrarUsuario(username, password, mensajeError)) {

                    QString mensaje = QString::fromStdString(mensajeError);

                    if (mensajeError == "Ese nombre de usuario ya existe.") {
                        mensaje = "This user already exists.";
                    }

                    QMessageBox::warning(
                        this,
                        "Sign In",
                        mensaje
                    );

                    return;
                }

                usuarioActual = sistemaUsuarios.iniciarSesion(username, password);

                if (usuarioActual == nullptr) {

                    QMessageBox::warning(
                        this,
                        "Sign In",
                        "The account was created, but the session could not be started."
                    );

                    return;
                }

                mostrarMenuPrincipal();

                return;
            }

            // LOG IN - AUTENTICAR
            usuarioActual = sistemaUsuarios.iniciarSesion(username, password);

            if (usuarioActual == nullptr) {

                QMessageBox::warning(
                    this,
                    "Log In",
                    "Incorrect username or password."
                );

                campoPassword->clear();
                campoPassword->setFocus();

                return;
            }

            mostrarMenuPrincipal();
        }

        // AJUSTAR INTERFAZ
        void ajustarInterfaz() {

            if (paginaInicio == nullptr || contenedorInicio == nullptr) {
                return;
            }

            double escalaX = static_cast<double>(paginaInicio->width()) / ANCHO_DISENO_MENU;
            double escalaY = static_cast<double>(paginaInicio->height()) / ALTO_DISENO_MENU;

            double escalaFuente = qMin(escalaX, escalaY);

            // CONTENEDOR Y TEMPLATE
            contenedorInicio->setGeometry(
                0,
                0,
                paginaInicio->width(),
                paginaInicio->height()
            );

            fondoInicio->setGeometry(
                0,
                0,
                contenedorInicio->width(),
                contenedorInicio->height()
            );

            // FRASE DE FELIX
            textoFelix->setGeometry(
                static_cast<int>(380 * escalaX),
                static_cast<int>(500 * escalaY),
                static_cast<int>(1320 * escalaX),
                static_cast<int>(120 * escalaY)
            );

            QFont fuenteFelix;

            if (!nombreFuenteLobster.isEmpty()) {
                fuenteFelix.setFamily(nombreFuenteLobster);
            }

            fuenteFelix.setPixelSize(static_cast<int>(58 * escalaFuente));
            fuenteFelix.setBold(false);

            textoFelix->setFont(fuenteFelix);

            // BOTONES
            int xBotones = static_cast<int>(760 * escalaX);
            int anchoBotones = static_cast<int>(560 * escalaX);
            int altoBotones = static_cast<int>(90 * escalaY);

            botonLogin->setGeometry(
                xBotones,
                static_cast<int>(655 * escalaY),
                anchoBotones,
                altoBotones
            );

            botonSignin->setGeometry(
                xBotones,
                static_cast<int>(755 * escalaY),
                anchoBotones,
                altoBotones
            );

            botonExit->setGeometry(
                xBotones,
                static_cast<int>(855 * escalaY),
                anchoBotones,
                altoBotones
            );


            QFont fuenteBotones;

            if (!nombreFuenteAlice.isEmpty()) {
                fuenteBotones.setFamily(nombreFuenteAlice);
            }

            fuenteBotones.setBold(true);
            fuenteBotones.setPixelSize(static_cast<int>(48 * escalaFuente));

            botonLogin->setFont(fuenteBotones);
            botonSignin->setFont(fuenteBotones);
            botonExit->setFont(fuenteBotones);


            fondoInicio->lower();

            textoFelix->raise();
            botonLogin->raise();
            botonSignin->raise();
            botonExit->raise();

            // PAGINA AUTENTICACION
            if (paginaAutenticacion != nullptr && contenedorAutenticacion != nullptr) {

                double escalaAuthX = static_cast<double>(paginaAutenticacion->width()) / ANCHO_DISENO_MENU;
                double escalaAuthY = static_cast<double>(paginaAutenticacion->height()) / ALTO_DISENO_MENU;

                double escalaAuthFuente = qMin(escalaAuthX, escalaAuthY);

                contenedorAutenticacion->setGeometry(
                    0,
                    0,
                    paginaAutenticacion->width(),
                    paginaAutenticacion->height()
                );


                fondoAutenticacion->setGeometry(
                    0,
                    0,
                    contenedorAutenticacion->width(),
                    contenedorAutenticacion->height()
                );

                // TEXTO FELIX
                textoAutenticacion->setGeometry(
                    static_cast<int>(600 * escalaAuthX),
                    static_cast<int>(500 * escalaAuthY),
                    static_cast<int>(880 * escalaAuthX),
                    static_cast<int>(110 * escalaAuthY)
                );

                QFont fuenteTextoAuth;

                if (!nombreFuenteLobster.isEmpty()) {
                    fuenteTextoAuth.setFamily(nombreFuenteLobster);
                }

                fuenteTextoAuth.setPixelSize(static_cast<int>(58 * escalaAuthFuente));

                textoAutenticacion->setFont(fuenteTextoAuth);

                // LABELS
                labelUsername->setGeometry(
                    static_cast<int>(435 * escalaAuthX),
                    static_cast<int>(685 * escalaAuthY),
                    static_cast<int>(220 * escalaAuthX),
                    static_cast<int>(70 * escalaAuthY)
                );

                labelPassword->setGeometry(
                    static_cast<int>(435 * escalaAuthX),
                    static_cast<int>(810 * escalaAuthY),
                    static_cast<int>(220 * escalaAuthX),
                    static_cast<int>(70 * escalaAuthY)
                );


                // CAMPOS
                campoUsername->setGeometry(
                    static_cast<int>(695 * escalaAuthX),
                    static_cast<int>(690 * escalaAuthY),
                    static_cast<int>(650 * escalaAuthX),
                    static_cast<int>(75 * escalaAuthY)
                );

                campoPassword->setGeometry(
                    static_cast<int>(695 * escalaAuthX),
                    static_cast<int>(815 * escalaAuthY),
                    static_cast<int>(650 * escalaAuthX),
                    static_cast<int>(75 * escalaAuthY)
                );


                QFont fuenteCampos;

                if (!nombreFuenteAlice.isEmpty()) {
                    fuenteCampos.setFamily(nombreFuenteAlice);
                }

                fuenteCampos.setPixelSize(static_cast<int>(34 * escalaAuthFuente));

                labelUsername->setFont(fuenteCampos);
                labelPassword->setFont(fuenteCampos);

                campoUsername->setFont(fuenteCampos);
                campoPassword->setFont(fuenteCampos);


                // BOTONES
                int tamanoBotonAuth = static_cast<int>(135 * escalaAuthFuente);

                botonCancelarAutenticacion->setGeometry(
                    static_cast<int>(1460 * escalaAuthX),
                    static_cast<int>(725 * escalaAuthY),
                    tamanoBotonAuth,
                    tamanoBotonAuth
                );

                botonAceptarAutenticacion->setGeometry(
                    static_cast<int>(1615 * escalaAuthX),
                    static_cast<int>(725 * escalaAuthY),
                    tamanoBotonAuth,
                    tamanoBotonAuth
                );


                QSize iconoAuth(
                    static_cast<int>(125 * escalaAuthFuente),
                    static_cast<int>(125 * escalaAuthFuente)
                );

                botonCancelarAutenticacion->setIconSize(iconoAuth);
                botonAceptarAutenticacion->setIconSize(iconoAuth);


                fondoAutenticacion->lower();

                textoAutenticacion->raise();
                labelUsername->raise();
                labelPassword->raise();
                campoUsername->raise();
                campoPassword->raise();
                botonCancelarAutenticacion->raise();
                botonAceptarAutenticacion->raise();
            }

            // PAGINA MENU PRINCIPAL AUTENTICADO
            if (paginaMenuPrincipal != nullptr && contenedorMenuPrincipal != nullptr) {

                double escalaMenuX = static_cast<double>(paginaMenuPrincipal->width()) / ANCHO_DISENO_MENU;
                double escalaMenuY = static_cast<double>(paginaMenuPrincipal->height()) / ALTO_DISENO_MENU;

                double escalaMenuFuente = qMin(escalaMenuX, escalaMenuY);

                contenedorMenuPrincipal->setGeometry(
                    0,
                    0,
                    paginaMenuPrincipal->width(),
                    paginaMenuPrincipal->height()
                );

                fondoMenuPrincipal->setGeometry(
                    0,
                    0,
                    contenedorMenuPrincipal->width(),
                    contenedorMenuPrincipal->height()
                );

                // MY PROFILE
                botonMiPerfil->setGeometry(
                    static_cast<int>(330 * escalaMenuX),
                    static_cast<int>(115 * escalaMenuY),
                    static_cast<int>(370 * escalaMenuX),
                    static_cast<int>(145 * escalaMenuY)
                );

                // HOW TO PLAY
                botonComoJugar->setGeometry(
                    static_cast<int>(1400 * escalaMenuX),
                    static_cast<int>(115 * escalaMenuY),
                    static_cast<int>(370 * escalaMenuX),
                    static_cast<int>(145 * escalaMenuY)
                );

                // EXIT
                botonSalirSesion->setGeometry(
                    static_cast<int>(330 * escalaMenuX),
                    static_cast<int>(545 * escalaMenuY),
                    static_cast<int>(370 * escalaMenuX),
                    static_cast<int>(145 * escalaMenuY)
                );

                // HIGH SCORES
                botonHighScores->setGeometry(
                    static_cast<int>(1400 * escalaMenuX),
                    static_cast<int>(545 * escalaMenuY),
                    static_cast<int>(370 * escalaMenuX),
                    static_cast<int>(145 * escalaMenuY)
                );

                // PLAY
                botonPlay->setGeometry(
                    static_cast<int>(1540 * escalaMenuX),
                    static_cast<int>(920 * escalaMenuY),
                    static_cast<int>(360 * escalaMenuX),
                    static_cast<int>(130 * escalaMenuY)
                );

                // BOTON INVISIBLE SOBRE EL MAPA
                botonMapa->setGeometry(
                    static_cast<int>(1000 * escalaMenuX),
                    static_cast<int>(645 * escalaMenuY),
                    static_cast<int>(340 * escalaMenuX),
                    static_cast<int>(145 * escalaMenuY)
                );

                // BOTON INVISIBLE SOBRE LOS CUBOS ROSADOS
                botonHuevoPascua->setGeometry(
                    static_cast<int>(80 * escalaMenuX),
                    static_cast<int>(600 * escalaMenuY),
                    static_cast<int>(190 * escalaMenuX),
                    static_cast<int>(165 * escalaMenuY)
                );

                // TEXTO DE BIENVENIDA
                textoBienvenida->setGeometry(
                    static_cast<int>(445 * escalaMenuX),
                    static_cast<int>(905 * escalaMenuY),
                    static_cast<int>(850 * escalaMenuX),
                    static_cast<int>(145 * escalaMenuY)
                );

                QFont fuenteBienvenida;

                if (!nombreFuenteAlice.isEmpty()) {
                    fuenteBienvenida.setFamily(nombreFuenteAlice);
                }

                fuenteBienvenida.setPixelSize(static_cast<int>(38 * escalaMenuFuente));
                textoBienvenida->setFont(fuenteBienvenida);

                // ICONOS
                botonMiPerfil->setIconSize(
                    QSize(
                        static_cast<int>(360 * escalaMenuFuente),
                        static_cast<int>(135 * escalaMenuFuente)
                    )
                );

                botonComoJugar->setIconSize(
                    QSize(
                        static_cast<int>(360 * escalaMenuFuente),
                        static_cast<int>(135 * escalaMenuFuente)
                    )
                );

                botonSalirSesion->setIconSize(
                    QSize(
                        static_cast<int>(360 * escalaMenuFuente),
                        static_cast<int>(135 * escalaMenuFuente)
                    )
                );

                botonHighScores->setIconSize(
                    QSize(
                        static_cast<int>(360 * escalaMenuFuente),
                        static_cast<int>(135 * escalaMenuFuente)
                    )
                );

                botonPlay->setIconSize(
                    QSize(
                        static_cast<int>(350 * escalaMenuFuente),
                        static_cast<int>(120 * escalaMenuFuente)
                    )
                );

                fondoMenuPrincipal->lower();

                textoBienvenida->raise();

                botonMiPerfil->raise();
                botonComoJugar->raise();
                botonSalirSesion->raise();
                botonHighScores->raise();
                botonPlay->raise();

                botonMapa->raise();
                botonHuevoPascua->raise();
            }

            // PAGINA MAPA BLACKPOWDER BAY
            if (paginaMapa != nullptr && contenedorMapa != nullptr) {

                double escalaMapaX = static_cast<double>(paginaMapa->width()) / ANCHO_DISENO_MENU;
                double escalaMapaY = static_cast<double>(paginaMapa->height()) / ALTO_DISENO_MENU;

                double escalaMapa = qMin(escalaMapaX, escalaMapaY);


                // CONTENEDOR
                contenedorMapa->setGeometry(
                    0,
                    0,
                    paginaMapa->width(),
                    paginaMapa->height()
                );


                // TEMPLATE
                fondoMapa->setGeometry(
                    0,
                    0,
                    contenedorMapa->width(),
                    contenedorMapa->height()
                );


                // HOME
                botonHomeMapa->setGeometry(
                    static_cast<int>(5 * escalaMapaX),
                    static_cast<int>(475 * escalaMapaY),
                    static_cast<int>(150 * escalaMapaX),
                    static_cast<int>(150 * escalaMapaY)
                );

                botonHomeMapa->setIconSize(
                    QSize(
                        static_cast<int>(140 * escalaMapa),
                        static_cast<int>(140 * escalaMapa)
                    )
                );


                // NIVEL 1 - PIRATE MINEFIELD
                botonesNivelMapa[0]->setGeometry(
                    static_cast<int>(322 * escalaMapaX),
                    static_cast<int>(572 * escalaMapaY),
                    static_cast<int>(303 * escalaMapaX),
                    static_cast<int>(228 * escalaMapaY)
                );


                // NIVEL 2 - CARNIVAL MINEFIELD
                botonesNivelMapa[1]->setGeometry(
                    static_cast<int>(518 * escalaMapaX),
                    static_cast<int>(301 * escalaMapaY),
                    static_cast<int>(303 * escalaMapaX),
                    static_cast<int>(228 * escalaMapaY)
                );


                // NIVEL 3 - DRAGON MINEFIELD
                botonesNivelMapa[2]->setGeometry(
                    static_cast<int>(872 * escalaMapaX),
                    static_cast<int>(111 * escalaMapaY),
                    static_cast<int>(303 * escalaMapaX),
                    static_cast<int>(228 * escalaMapaY)
                );


                // NIVEL 4 - RUMBLE MINEFIELD
                botonesNivelMapa[3]->setGeometry(
                    static_cast<int>(1241 * escalaMapaX),
                    static_cast<int>(144 * escalaMapaY),
                    static_cast<int>(304 * escalaMapaX),
                    static_cast<int>(229 * escalaMapaY)
                );


                // NIVEL 5 - HIGH SEAS MINEFIELD
                botonesNivelMapa[4]->setGeometry(
                    static_cast<int>(1571 * escalaMapaX),
                    static_cast<int>(322 * escalaMapaY),
                    static_cast<int>(304 * escalaMapaX),
                    static_cast<int>(228 * escalaMapaY)
                );


                // NIVEL 6 - MECHANICAL MINEFIELD
                botonesNivelMapa[5]->setGeometry(
                    static_cast<int>(1075 * escalaMapaX),
                    static_cast<int>(415 * escalaMapaY),
                    static_cast<int>(303 * escalaMapaX),
                    static_cast<int>(229 * escalaMapaY)
                );


                // NIVEL 7 - AERIAL MINEFIELD
                botonesNivelMapa[6]->setGeometry(
                    static_cast<int>(684 * escalaMapaX),
                    static_cast<int>(657 * escalaMapaY),
                    static_cast<int>(303 * escalaMapaX),
                    static_cast<int>(229 * escalaMapaY)
                );


                // NIVEL 8 - BAKER'S MINEFIELD
                botonesNivelMapa[7]->setGeometry(
                    static_cast<int>(1059 * escalaMapaX),
                    static_cast<int>(673 * escalaMapaY),
                    static_cast<int>(304 * escalaMapaX),
                    static_cast<int>(229 * escalaMapaY)
                );


                // NIVEL 9 - ONE HELL OF A MINEFIELD
                botonesNivelMapa[8]->setGeometry(
                    static_cast<int>(1457 * escalaMapaX),
                    static_cast<int>(687 * escalaMapaY),
                    static_cast<int>(303 * escalaMapaX),
                    static_cast<int>(228 * escalaMapaY)
                );


                fondoMapa->lower();

                botonHomeMapa->raise();

                for (int i = 0; i < CANTIDAD_NIVELES; i++) {
                    botonesNivelMapa[i]->raise();
                }
            }

            // PAGINA HUEVO DE PASCUA
            if (paginaHuevoPascua != nullptr && contenedorHuevoPascua != nullptr) {

                double escalaHuevoX = static_cast<double>(paginaHuevoPascua->width()) / ANCHO_DISENO_MENU;
                double escalaHuevoY = static_cast<double>(paginaHuevoPascua->height()) / ALTO_DISENO_MENU;

                double escalaHuevoFuente = qMin(escalaHuevoX, escalaHuevoY);


                // CONTENEDOR
                contenedorHuevoPascua->setGeometry(
                    0,
                    0,
                    paginaHuevoPascua->width(),
                    paginaHuevoPascua->height()
                );


                // FONDO
                fondoHuevoPascua->setGeometry(
                    0,
                    0,
                    contenedorHuevoPascua->width(),
                    contenedorHuevoPascua->height()
                );


                // TEXTO
                textoHuevoPascua->setGeometry(
                    static_cast<int>(170 * escalaHuevoX),
                    static_cast<int>(145 * escalaHuevoY),
                    static_cast<int>(1740 * escalaHuevoX),
                    static_cast<int>(670 * escalaHuevoY)
                );


                QFont fuenteHuevo;

                if (!nombreFuenteAlice.isEmpty()) {
                    fuenteHuevo.setFamily(nombreFuenteAlice);
                }

                fuenteHuevo.setPixelSize(static_cast<int>(36 * escalaHuevoFuente));

                textoHuevoPascua->setFont(fuenteHuevo);


                // CLAIM REWARD
                botonClaimReward->setGeometry(
                    static_cast<int>(855 * escalaHuevoX),
                    static_cast<int>(835 * escalaHuevoY),
                    static_cast<int>(370 * escalaHuevoX),
                    static_cast<int>(145 * escalaHuevoY)
                );

                // HOME
                botonHomeHuevoPascua->setGeometry(
                    static_cast<int>(100 * escalaHuevoX),
                    static_cast<int>(110 * escalaHuevoY),
                    static_cast<int>(140 * escalaHuevoX),
                    static_cast<int>(140 * escalaHuevoY)
                );

                botonHomeHuevoPascua->setIconSize(
                    QSize(
                        static_cast<int>(130 * escalaHuevoFuente),
                        static_cast<int>(130 * escalaHuevoFuente)
                    )
                );

                botonClaimReward->setIconSize(
                    QSize(
                        static_cast<int>(360 * escalaHuevoFuente),
                        static_cast<int>(135 * escalaHuevoFuente)
                    )
                );


                fondoHuevoPascua->lower();

                textoHuevoPascua->raise();
                botonClaimReward->raise();
                botonHomeHuevoPascua->raise();
            }

            // PAGINA HOW TO PLAY
            if (paginaComoJugar != nullptr && contenedorComoJugar != nullptr) {

                double escalaComoJugarX = static_cast<double>(paginaComoJugar->width()) / ANCHO_DISENO_MENU;
                double escalaComoJugarY = static_cast<double>(paginaComoJugar->height()) / ALTO_DISENO_MENU;

                double escalaComoJugar = qMin(escalaComoJugarX, escalaComoJugarY);


                // CONTENEDOR
                contenedorComoJugar->setGeometry(0, 0, paginaComoJugar->width(), paginaComoJugar->height());


                // TEMPLATE
                fondoComoJugar->setGeometry(0, 0, contenedorComoJugar->width(), contenedorComoJugar->height());


                // BACK - LADO IZQUIERDO
                botonAtrasComoJugar->setGeometry(
                    static_cast<int>(20 * escalaComoJugarX),
                    static_cast<int>(480 * escalaComoJugarY),
                    static_cast<int>(145 * escalaComoJugarX),
                    static_cast<int>(145 * escalaComoJugarY)
                );

                botonAtrasComoJugar->setIconSize(
                    QSize(
                        static_cast<int>(135 * escalaComoJugar),
                        static_cast<int>(135 * escalaComoJugar)
                    )
                );


                // FORWARD - LADO DERECHO
                botonAdelanteComoJugar->setGeometry(
                    static_cast<int>(1915 * escalaComoJugarX),
                    static_cast<int>(480 * escalaComoJugarY),
                    static_cast<int>(145 * escalaComoJugarX),
                    static_cast<int>(145 * escalaComoJugarY)
                );

                botonAdelanteComoJugar->setIconSize(
                    QSize(
                        static_cast<int>(135 * escalaComoJugar),
                        static_cast<int>(135 * escalaComoJugar)
                    )
                );


                // HOME - LADO DERECHO EN LA ULTIMA PAGINA
                botonHomeComoJugar->setGeometry(
                    static_cast<int>(1915 * escalaComoJugarX),
                    static_cast<int>(480 * escalaComoJugarY),
                    static_cast<int>(145 * escalaComoJugarX),
                    static_cast<int>(145 * escalaComoJugarY)
                );

                botonHomeComoJugar->setIconSize(
                    QSize(
                        static_cast<int>(135 * escalaComoJugar),
                        static_cast<int>(135 * escalaComoJugar)
                    )
                );


                fondoComoJugar->lower();

                botonAtrasComoJugar->raise();
                botonAdelanteComoJugar->raise();
                botonHomeComoJugar->raise();
            }

            // PAGINA MY PROFILE
            if (paginaPerfil != nullptr && contenedorPerfil != nullptr) {

                double escalaPerfilX = static_cast<double>(paginaPerfil->width()) / ANCHO_DISENO_MENU;
                double escalaPerfilY = static_cast<double>(paginaPerfil->height()) / ALTO_DISENO_MENU;

                double escalaPerfil = qMin(escalaPerfilX, escalaPerfilY);


                // CONTENEDOR
                contenedorPerfil->setGeometry(0, 0, paginaPerfil->width(), paginaPerfil->height());


                // TEMPLATE
                fondoPerfil->setGeometry(0, 0, contenedorPerfil->width(), contenedorPerfil->height());


                // AVATAR
                imagenAvatarPerfil->setGeometry(
                    static_cast<int>(875 * escalaPerfilX),
                    static_cast<int>(145 * escalaPerfilY),
                    static_cast<int>(330 * escalaPerfilX),
                    static_cast<int>(330 * escalaPerfilY)
                );


                // BACK AVATAR
                botonAvatarAnterior->setGeometry(
                    static_cast<int>(620 * escalaPerfilX),
                    static_cast<int>(255 * escalaPerfilY),
                    static_cast<int>(150 * escalaPerfilX),
                    static_cast<int>(150 * escalaPerfilY)
                );

                botonAvatarAnterior->setIconSize(
                    QSize(
                        static_cast<int>(140 * escalaPerfil),
                        static_cast<int>(140 * escalaPerfil)
                    )
                );


                // FORWARD AVATAR
                botonAvatarSiguiente->setGeometry(
                    static_cast<int>(1310 * escalaPerfilX),
                    static_cast<int>(255 * escalaPerfilY),
                    static_cast<int>(150 * escalaPerfilX),
                    static_cast<int>(150 * escalaPerfilY)
                );

                botonAvatarSiguiente->setIconSize(
                    QSize(
                        static_cast<int>(140 * escalaPerfil),
                        static_cast<int>(140 * escalaPerfil)
                    )
                );


                // USERNAME
                textoUsernamePerfil->setGeometry(
                    static_cast<int>(690 * escalaPerfilX),
                    static_cast<int>(525 * escalaPerfilY),
                    static_cast<int>(700 * escalaPerfilX),
                    static_cast<int>(90 * escalaPerfilY)
                );

                QFont fuentePerfil;

                if (!nombreFuenteAlice.isEmpty()) {
                    fuentePerfil.setFamily(nombreFuenteAlice);
                }

                fuentePerfil.setBold(true);
                fuentePerfil.setPixelSize(static_cast<int>(48 * escalaPerfil));

                textoUsernamePerfil->setFont(fuentePerfil);


                // CHANGE PASSWORD
                botonCambiarPassword->setGeometry(
                    static_cast<int>(650 * escalaPerfilX),
                    static_cast<int>(700 * escalaPerfilY),
                    static_cast<int>(780 * escalaPerfilX),
                    static_cast<int>(90 * escalaPerfilY)
                );

                botonCambiarPassword->setFont(fuentePerfil);


                // CANCEL
                botonCancelarPerfil->setGeometry(
                    static_cast<int>(935 * escalaPerfilX),
                    static_cast<int>(825 * escalaPerfilY),
                    static_cast<int>(115 * escalaPerfilX),
                    static_cast<int>(115 * escalaPerfilY)
                );

                botonCancelarPerfil->setIconSize(
                    QSize(
                        static_cast<int>(105 * escalaPerfil),
                        static_cast<int>(105 * escalaPerfil)
                    )
                );


                // ACCEPT
                botonAceptarPerfil->setGeometry(
                    static_cast<int>(1060 * escalaPerfilX),
                    static_cast<int>(825 * escalaPerfilY),
                    static_cast<int>(115 * escalaPerfilX),
                    static_cast<int>(115 * escalaPerfilY)
                );

                botonAceptarPerfil->setIconSize(
                    QSize(
                        static_cast<int>(105 * escalaPerfil),
                        static_cast<int>(105 * escalaPerfil)
                    )
                );

                fondoPerfil->lower();

                imagenAvatarPerfil->raise();
                textoUsernamePerfil->raise();

                botonAvatarAnterior->raise();
                botonAvatarSiguiente->raise();

                botonCambiarPassword->raise();

                botonCancelarPerfil->raise();
                botonAceptarPerfil->raise();
            }

            // PAGINA CHANGE PASSWORD
            if (paginaCambioPassword != nullptr && contenedorCambioPassword != nullptr) {

                double escalaPasswordX = static_cast<double>(paginaCambioPassword->width()) / ANCHO_DISENO_MENU;
                double escalaPasswordY = static_cast<double>(paginaCambioPassword->height()) / ALTO_DISENO_MENU;

                double escalaPassword = qMin(escalaPasswordX, escalaPasswordY);

                // CONTENEDOR
                contenedorCambioPassword->setGeometry(0, 0, paginaCambioPassword->width(), paginaCambioPassword->height());

                // TEMPLATE
                fondoCambioPassword->setGeometry(0, 0, contenedorCambioPassword->width(), contenedorCambioPassword->height());

                // AVATAR
                imagenAvatarCambioPassword->setGeometry(
                    static_cast<int>(875 * escalaPasswordX),
                    static_cast<int>(145 * escalaPasswordY),
                    static_cast<int>(330 * escalaPasswordX),
                    static_cast<int>(330 * escalaPasswordY)
                );

                // CANCEL
                botonCancelarCambioPassword->setGeometry(
                    static_cast<int>(935 * escalaPasswordX),
                    static_cast<int>(510 * escalaPasswordY),
                    static_cast<int>(115 * escalaPasswordX),
                    static_cast<int>(115 * escalaPasswordY)
                );

                botonCancelarCambioPassword->setIconSize(
                    QSize(
                        static_cast<int>(105 * escalaPassword),
                        static_cast<int>(105 * escalaPassword)
                    )
                );

                // ACCEPT
                botonAceptarCambioPassword->setGeometry(
                    static_cast<int>(1060 * escalaPasswordX),
                    static_cast<int>(510 * escalaPasswordY),
                    static_cast<int>(115 * escalaPasswordX),
                    static_cast<int>(115 * escalaPasswordY)
                );

                botonAceptarCambioPassword->setIconSize(
                    QSize(
                        static_cast<int>(105 * escalaPassword),
                        static_cast<int>(105 * escalaPassword)
                    )
                );

                QFont fuentePassword;

                if (!nombreFuenteAlice.isEmpty()) {
                    fuentePassword.setFamily(nombreFuenteAlice);
                }

                fuentePassword.setPixelSize(static_cast<int>(38 * escalaPassword));

                // OLD PASSWORD
                textoOldPassword->setGeometry(
                    static_cast<int>(650 * escalaPasswordX),
                    static_cast<int>(665 * escalaPasswordY),
                    static_cast<int>(780 * escalaPasswordX),
                    static_cast<int>(55 * escalaPasswordY)
                );

                textoOldPassword->setFont(fuentePassword);

                campoOldPassword->setGeometry(
                    static_cast<int>(600 * escalaPasswordX),
                    static_cast<int>(725 * escalaPasswordY),
                    static_cast<int>(880 * escalaPasswordX),
                    static_cast<int>(75 * escalaPasswordY)
                );

                campoOldPassword->setFont(fuentePassword);

                // NEW PASSWORD
                textoNewPassword->setGeometry(
                    static_cast<int>(650 * escalaPasswordX),
                    static_cast<int>(815 * escalaPasswordY),
                    static_cast<int>(780 * escalaPasswordX),
                    static_cast<int>(55 * escalaPasswordY)
                );

                textoNewPassword->setFont(fuentePassword);

                campoNewPassword->setGeometry(
                    static_cast<int>(600 * escalaPasswordX),
                    static_cast<int>(875 * escalaPasswordY),
                    static_cast<int>(880 * escalaPasswordX),
                    static_cast<int>(75 * escalaPasswordY)
                );

                campoNewPassword->setFont(fuentePassword);

                fondoCambioPassword->lower();

                imagenAvatarCambioPassword->raise();

                textoOldPassword->raise();
                textoNewPassword->raise();

                campoOldPassword->raise();
                campoNewPassword->raise();

                botonCancelarCambioPassword->raise();
                botonAceptarCambioPassword->raise();
            }

            // PAGINA HIGH SCORES
            if (paginaHighScores != nullptr && contenedorHighScores != nullptr) {

                double escalaScoresX = static_cast<double>(paginaHighScores->width()) / ANCHO_DISENO_MENU;
                double escalaScoresY = static_cast<double>(paginaHighScores->height()) / ALTO_DISENO_MENU;

                double escalaScores = qMin(escalaScoresX, escalaScoresY);

                // CONTENEDOR
                contenedorHighScores->setGeometry(0, 0, paginaHighScores->width(), paginaHighScores->height());

                // TEMPLATE
                fondoHighScores->setGeometry(0, 0, contenedorHighScores->width(), contenedorHighScores->height());

                // BOMB SQUAD
                imagenRanking->setGeometry(
                    static_cast<int>(145 * escalaScoresX),
                    static_cast<int>(55 * escalaScoresY),
                    static_cast<int>(1000 * escalaScoresX),
                    static_cast<int>(895 * escalaScoresY)
                );

                // TITULO MY SCORE
                textoTituloMiScore->setGeometry(
                    static_cast<int>(1380 * escalaScoresX),
                    static_cast<int>(100 * escalaScoresY),
                    static_cast<int>(430 * escalaScoresX),
                    static_cast<int>(70 * escalaScoresY)
                );

                // SCORE ACTUAL
                textoMiScore->setGeometry(
                    static_cast<int>(1360 * escalaScoresX),
                    static_cast<int>(195 * escalaScoresY),
                    static_cast<int>(470 * escalaScoresX),
                    static_cast<int>(110 * escalaScoresY)
                );

                // MY MAYHEM MEDALS
                textoTituloMedallas->setGeometry(
                    static_cast<int>(1330 * escalaScoresX),
                    static_cast<int>(340 * escalaScoresY),
                    static_cast<int>(550 * escalaScoresX),
                    static_cast<int>(70 * escalaScoresY)
                );

                QFont fuenteTituloScores;

                if (!nombreFuenteAlice.isEmpty()) {
                    fuenteTituloScores.setFamily(nombreFuenteAlice);
                }

                fuenteTituloScores.setBold(true);
                fuenteTituloScores.setPixelSize(static_cast<int>(38 * escalaScores));

                textoTituloMiScore->setFont(fuenteTituloScores);
                textoTituloMedallas->setFont(fuenteTituloScores);

                QFont fuenteMiScore;

                if (!nombreFuenteAlice.isEmpty()) {
                    fuenteMiScore.setFamily(nombreFuenteAlice);
                }

                fuenteMiScore.setBold(true);
                fuenteMiScore.setPixelSize(static_cast<int>(72 * escalaScores));

                textoMiScore->setFont(fuenteMiScore);

                // -------------------------------------------------------------------------
                // TOP 5 - POSICIONES ALINEADAS CON BOMB-SQUAD.PNG
                // -------------------------------------------------------------------------

                int posicionesYRanking[CANTIDAD_RANKING] = {
                    472,
                    570,
                    668,
                    766,
                    864
                };

                QFont fuenteRanking;

                if (!nombreFuenteAlice.isEmpty()) {
                    fuenteRanking.setFamily(nombreFuenteAlice);
                }

                fuenteRanking.setPixelSize(static_cast<int>(34 * escalaScores));

                for (int i = 0; i < CANTIDAD_RANKING; i++) {

                    textosRankingUsuario[i]->setGeometry(
                        static_cast<int>(565 * escalaScoresX),
                        static_cast<int>((posicionesYRanking[i] - 32) * escalaScoresY),
                        static_cast<int>(280 * escalaScoresX),
                        static_cast<int>(64 * escalaScoresY)
                    );

                    textosRankingPuntaje[i]->setGeometry(
                        static_cast<int>(845 * escalaScoresX),
                        static_cast<int>((posicionesYRanking[i] - 32) * escalaScoresY),
                        static_cast<int>(180 * escalaScoresX),
                        static_cast<int>(64 * escalaScoresY)
                    );

                    textosRankingUsuario[i]->setFont(fuenteRanking);
                    textosRankingPuntaje[i]->setFont(fuenteRanking);
                }

                // -------------------------------------------------------------------------
                // BADGES
                // -------------------------------------------------------------------------

                int badgeX[CANTIDAD_LOGROS] = {
                    1320,
                    1500,
                    1680,
                    1320,
                    1500,
                    1680
                };

                int badgeY[CANTIDAD_LOGROS] = {
                    430,
                    430,
                    430,
                    620,
                    620,
                    620
                };

                for (int i = 0; i < CANTIDAD_LOGROS; i++) {

                    imagenesBadges[i]->setGeometry(
                        static_cast<int>(badgeX[i] * escalaScoresX),
                        static_cast<int>(badgeY[i] * escalaScoresY),
                        static_cast<int>(150 * escalaScoresX),
                        static_cast<int>(150 * escalaScoresY)
                    );
                }

                // HOME
                botonHomeHighScores->setGeometry(
                    static_cast<int>(1515 * escalaScoresX),
                    static_cast<int>(855 * escalaScoresY),
                    static_cast<int>(145 * escalaScoresX),
                    static_cast<int>(145 * escalaScoresY)
                );

                botonHomeHighScores->setIconSize(
                    QSize(
                        static_cast<int>(135 * escalaScores),
                        static_cast<int>(135 * escalaScores)
                    )
                );

                fondoHighScores->lower();

                imagenRanking->raise();

                textoTituloMiScore->raise();
                textoMiScore->raise();
                textoTituloMedallas->raise();

                for (int i = 0; i < CANTIDAD_RANKING; i++) {
                    textosRankingUsuario[i]->raise();
                    textosRankingPuntaje[i]->raise();
                }

                for (int i = 0; i < CANTIDAD_LOGROS; i++) {
                    imagenesBadges[i]->raise();
                }

                botonHomeHighScores->raise();
            }

            // PAGINA PLAY
            if (paginaPlay != nullptr && contenedorPlay != nullptr) {

                double escalaPlayX = static_cast<double>(paginaPlay->width()) / ANCHO_DISENO_MENU;
                double escalaPlayY = static_cast<double>(paginaPlay->height()) / ALTO_DISENO_MENU;

                double escalaPlay = qMin(escalaPlayX, escalaPlayY);

                // CONTENEDOR
                contenedorPlay->setGeometry(
                    0,
                    0,
                    paginaPlay->width(),
                    paginaPlay->height()
                );

                // FONDO
                fondoPlay->setGeometry(
                    0,
                    0,
                    contenedorPlay->width(),
                    contenedorPlay->height()
                );

                // TEXTO "ADVENTURE'S JUST AHEAD!"
                textoPlay->setGeometry(
                    static_cast<int>(520 * escalaPlayX),
                    static_cast<int>(490 * escalaPlayY),
                    static_cast<int>(1040 * escalaPlayX),
                    static_cast<int>(110 * escalaPlayY)
                );

                QFont fuenteTextoPlay;

                if (!nombreFuenteLobster.isEmpty()) {
                    fuenteTextoPlay.setFamily(nombreFuenteLobster);
                }

                fuenteTextoPlay.setPixelSize(static_cast<int>(58 * escalaPlay));

                textoPlay->setFont(fuenteTextoPlay);

                // NEXT QUEST
                botonNextQuest->setGeometry(
                    static_cast<int>(760 * escalaPlayX),
                    static_cast<int>(670 * escalaPlayY),
                    static_cast<int>(560 * escalaPlayX),
                    static_cast<int>(75 * escalaPlayY)
                );

                // CUSTOM MAYHEM
                botonCustomMayhem->setGeometry(
                    static_cast<int>(690 * escalaPlayX),
                    static_cast<int>(755 * escalaPlayY),
                    static_cast<int>(700 * escalaPlayX),
                    static_cast<int>(75 * escalaPlayY)
                );

                QFont fuenteBotonesPlay;

                if (!nombreFuenteAlice.isEmpty()) {
                    fuenteBotonesPlay.setFamily(nombreFuenteAlice);
                }

                fuenteBotonesPlay.setBold(true);
                fuenteBotonesPlay.setPixelSize(static_cast<int>(44 * escalaPlay));

                botonNextQuest->setFont(fuenteBotonesPlay);
                botonCustomMayhem->setFont(fuenteBotonesPlay);

                // HOME
                botonHomePlay->setGeometry(
                    static_cast<int>(815 * escalaPlayX),
                    static_cast<int>(840 * escalaPlayY),
                    static_cast<int>(140 * escalaPlayX),
                    static_cast<int>(140 * escalaPlayY)
                );

                // REWARDS
                botonRewardsPlay->setGeometry(
                    static_cast<int>(970 * escalaPlayX),
                    static_cast<int>(840 * escalaPlayY),
                    static_cast<int>(140 * escalaPlayX),
                    static_cast<int>(140 * escalaPlayY)
                );

                // HIGH SCORES
                botonHighScoresPlay->setGeometry(
                    static_cast<int>(1125 * escalaPlayX),
                    static_cast<int>(840 * escalaPlayY),
                    static_cast<int>(140 * escalaPlayX),
                    static_cast<int>(140 * escalaPlayY)
                );

                botonHomePlay->setIconSize(
                    QSize(
                        static_cast<int>(130 * escalaPlay),
                        static_cast<int>(130 * escalaPlay)
                    )
                );

                botonRewardsPlay->setIconSize(
                    QSize(
                        static_cast<int>(130 * escalaPlay),
                        static_cast<int>(130 * escalaPlay)
                    )
                );

                botonHighScoresPlay->setIconSize(
                    QSize(
                        static_cast<int>(130 * escalaPlay),
                        static_cast<int>(130 * escalaPlay)
                    )
                );

                fondoPlay->lower();

                textoPlay->raise();

                botonNextQuest->raise();
                botonCustomMayhem->raise();

                botonHomePlay->raise();
                botonRewardsPlay->raise();
                botonHighScoresPlay->raise();
            }

            // PAGINA REWARDS
            if (paginaRewards != nullptr && contenedorRewards != nullptr) {

                double escalaRewardsX = static_cast<double>(paginaRewards->width()) / ANCHO_DISENO_MENU;
                double escalaRewardsY = static_cast<double>(paginaRewards->height()) / ALTO_DISENO_MENU;

                double escalaRewards = qMin(escalaRewardsX, escalaRewardsY);

                // CONTENEDOR
                contenedorRewards->setGeometry(
                    0,
                    0,
                    paginaRewards->width(),
                    paginaRewards->height()
                );

                // FONDO
                fondoRewards->setGeometry(
                    0,
                    0,
                    contenedorRewards->width(),
                    contenedorRewards->height()
                );

                // TEXTO
                textoRewards->setGeometry(
                    static_cast<int>(460 * escalaRewardsX),
                    static_cast<int>(500 * escalaRewardsY),
                    static_cast<int>(1160 * escalaRewardsX),
                    static_cast<int>(110 * escalaRewardsY)
                );

                QFont fuenteRewards;

                if (!nombreFuenteLobster.isEmpty()) {
                    fuenteRewards.setFamily(nombreFuenteLobster);
                }

                fuenteRewards.setPixelSize(static_cast<int>(58 * escalaRewards));

                textoRewards->setFont(fuenteRewards);

                // BACK
                botonBackRewards->setGeometry(
                    static_cast<int>(150 * escalaRewardsX),
                    static_cast<int>(485 * escalaRewardsY),
                    static_cast<int>(145 * escalaRewardsX),
                    static_cast<int>(145 * escalaRewardsY)
                );

                botonBackRewards->setIconSize(
                    QSize(
                        static_cast<int>(135 * escalaRewards),
                        static_cast<int>(135 * escalaRewards)
                    )
                );

                // INFO
                botonInfoRewards->setGeometry(
                    static_cast<int>(1785 * escalaRewardsX),
                    static_cast<int>(485 * escalaRewardsY),
                    static_cast<int>(145 * escalaRewardsX),
                    static_cast<int>(145 * escalaRewardsY)
                );

                botonInfoRewards->setIconSize(
                    QSize(
                        static_cast<int>(135 * escalaRewards),
                        static_cast<int>(135 * escalaRewards)
                    )
                );

                // BADGES
                int posicionesBadgeX[CANTIDAD_LOGROS] = {
                    355,
                    585,
                    815,
                    1045,
                    1275,
                    1505
                };

                for (int i = 0; i < CANTIDAD_LOGROS; i++) {

                    imagenesBadgesRewards[i]->setGeometry(
                        static_cast<int>(posicionesBadgeX[i] * escalaRewardsX),
                        static_cast<int>(690 * escalaRewardsY),
                        static_cast<int>(205 * escalaRewardsX),
                        static_cast<int>(205 * escalaRewardsY)
                    );
                }

                fondoRewards->lower();

                textoRewards->raise();

                for (int i = 0; i < CANTIDAD_LOGROS; i++) {
                    imagenesBadgesRewards[i]->raise();
                }

                botonBackRewards->raise();
                botonInfoRewards->raise();
            }

            // PAGINA BADGES INFO
            if (paginaBadgesInfo != nullptr && contenedorBadgesInfo != nullptr) {

                double escalaBadgesInfoX = static_cast<double>(paginaBadgesInfo->width()) / ANCHO_DISENO_MENU;
                double escalaBadgesInfoY = static_cast<double>(paginaBadgesInfo->height()) / ALTO_DISENO_MENU;

                double escalaBadgesInfo = qMin(escalaBadgesInfoX, escalaBadgesInfoY);

                // CONTENEDOR
                contenedorBadgesInfo->setGeometry(
                    0,
                    0,
                    paginaBadgesInfo->width(),
                    paginaBadgesInfo->height()
                );

                // TEMPLATE
                fondoBadgesInfo->setGeometry(
                    0,
                    0,
                    contenedorBadgesInfo->width(),
                    contenedorBadgesInfo->height()
                );

                // BACK
                botonBackBadgesInfo->setGeometry(
                    static_cast<int>(95 * escalaBadgesInfoX),
                    static_cast<int>(95 * escalaBadgesInfoY),
                    static_cast<int>(145 * escalaBadgesInfoX),
                    static_cast<int>(145 * escalaBadgesInfoY)
                );

                botonBackBadgesInfo->setIconSize(
                    QSize(
                        static_cast<int>(135 * escalaBadgesInfo),
                        static_cast<int>(135 * escalaBadgesInfo)
                    )
                );

                fondoBadgesInfo->lower();
                botonBackBadgesInfo->raise();
            }

            // PAGINA PARTIDA GUARDADA
            if (paginaPartidaGuardada != nullptr && contenedorPartidaGuardada != nullptr) {

                double escalaGuardadaX = static_cast<double>(paginaPartidaGuardada->width()) / ANCHO_DISENO_MENU;
                double escalaGuardadaY = static_cast<double>(paginaPartidaGuardada->height()) / ALTO_DISENO_MENU;

                double escalaGuardada = qMin(escalaGuardadaX, escalaGuardadaY);

                // CONTENEDOR
                contenedorPartidaGuardada->setGeometry(0, 0, paginaPartidaGuardada->width(), paginaPartidaGuardada->height());

                // FONDO
                fondoPartidaGuardada->setGeometry(0, 0, contenedorPartidaGuardada->width(), contenedorPartidaGuardada->height());

                // TEXTO
                textoPartidaGuardada->setGeometry(
                    static_cast<int>(400 * escalaGuardadaX),
                    static_cast<int>(500 * escalaGuardadaY),
                    static_cast<int>(1280 * escalaGuardadaX),
                    static_cast<int>(105 * escalaGuardadaY)
                );

                QFont fuenteTextoGuardada;

                if (!nombreFuenteLobster.isEmpty()) {
                    fuenteTextoGuardada.setFamily(nombreFuenteLobster);
                }

                fuenteTextoGuardada.setPixelSize(static_cast<int>(55 * escalaGuardada));

                textoPartidaGuardada->setFont(fuenteTextoGuardada);

                // BACK
                botonBackPartidaGuardada->setGeometry(
                    static_cast<int>(145 * escalaGuardadaX),
                    static_cast<int>(485 * escalaGuardadaY),
                    static_cast<int>(145 * escalaGuardadaX),
                    static_cast<int>(145 * escalaGuardadaY)
                );

                botonBackPartidaGuardada->setIconSize(
                    QSize(
                        static_cast<int>(135 * escalaGuardada),
                        static_cast<int>(135 * escalaGuardada)
                    )
                );

                // HOME
                botonHomePartidaGuardada->setGeometry(
                    static_cast<int>(1790 * escalaGuardadaX),
                    static_cast<int>(485 * escalaGuardadaY),
                    static_cast<int>(145 * escalaGuardadaX),
                    static_cast<int>(145 * escalaGuardadaY)
                );

                botonHomePartidaGuardada->setIconSize(
                    QSize(
                        static_cast<int>(135 * escalaGuardada),
                        static_cast<int>(135 * escalaGuardada)
                    )
                );

                // LOAD QUEST
                botonLoadQuest->setGeometry(
                    static_cast<int>(720 * escalaGuardadaX),
                    static_cast<int>(700 * escalaGuardadaY),
                    static_cast<int>(640 * escalaGuardadaX),
                    static_cast<int>(85 * escalaGuardadaY)
                );

                // RESET QUEST
                botonResetQuest->setGeometry(
                    static_cast<int>(680 * escalaGuardadaX),
                    static_cast<int>(815 * escalaGuardadaY),
                    static_cast<int>(720 * escalaGuardadaX),
                    static_cast<int>(85 * escalaGuardadaY)
                );

                QFont fuenteBotonesGuardada;

                if (!nombreFuenteAlice.isEmpty()) {
                    fuenteBotonesGuardada.setFamily(nombreFuenteAlice);
                }

                fuenteBotonesGuardada.setBold(true);
                fuenteBotonesGuardada.setPixelSize(static_cast<int>(46 * escalaGuardada));

                botonLoadQuest->setFont(fuenteBotonesGuardada);
                botonResetQuest->setFont(fuenteBotonesGuardada);

                fondoPartidaGuardada->lower();

                textoPartidaGuardada->raise();

                botonBackPartidaGuardada->raise();
                botonHomePartidaGuardada->raise();
                botonLoadQuest->raise();
                botonResetQuest->raise();
            }

            // PAGINA PARTIDA MAPA GUARDADA
            if (paginaMapaGuardada != nullptr && contenedorMapaGuardada != nullptr) {

                double escalaMapaGuardadaX =
                    static_cast<double>(paginaMapaGuardada->width()) /
                    ANCHO_DISENO_MENU;

                double escalaMapaGuardadaY =
                    static_cast<double>(paginaMapaGuardada->height()) /
                    ALTO_DISENO_MENU;

                double escalaMapaGuardada =
                    qMin(escalaMapaGuardadaX, escalaMapaGuardadaY);


                // CONTENEDOR
                contenedorMapaGuardada->setGeometry(
                    0,
                    0,
                    paginaMapaGuardada->width(),
                    paginaMapaGuardada->height()
                );


                // FONDO
                fondoMapaGuardada->setGeometry(
                    0,
                    0,
                    contenedorMapaGuardada->width(),
                    contenedorMapaGuardada->height()
                );


                // TEXTO
                textoMapaGuardada->setGeometry(
                    static_cast<int>(400 * escalaMapaGuardadaX),
                    static_cast<int>(500 * escalaMapaGuardadaY),
                    static_cast<int>(1280 * escalaMapaGuardadaX),
                    static_cast<int>(105 * escalaMapaGuardadaY)
                );


                QFont fuenteTextoMapaGuardada;

                if (!nombreFuenteLobster.isEmpty()) {
                    fuenteTextoMapaGuardada.setFamily(nombreFuenteLobster);
                }

                fuenteTextoMapaGuardada.setPixelSize(
                    static_cast<int>(55 * escalaMapaGuardada)
                );

                textoMapaGuardada->setFont(fuenteTextoMapaGuardada);


                // BACK
                botonBackMapaGuardada->setGeometry(
                    static_cast<int>(145 * escalaMapaGuardadaX),
                    static_cast<int>(485 * escalaMapaGuardadaY),
                    static_cast<int>(145 * escalaMapaGuardadaX),
                    static_cast<int>(145 * escalaMapaGuardadaY)
                );

                botonBackMapaGuardada->setIconSize(
                    QSize(
                        static_cast<int>(135 * escalaMapaGuardada),
                        static_cast<int>(135 * escalaMapaGuardada)
                    )
                );


                // LOAD GAME
                botonLoadMapaGuardada->setGeometry(
                    static_cast<int>(720 * escalaMapaGuardadaX),
                    static_cast<int>(700 * escalaMapaGuardadaY),
                    static_cast<int>(640 * escalaMapaGuardadaX),
                    static_cast<int>(85 * escalaMapaGuardadaY)
                );


                // NEW QUEST
                botonNewQuestMapa->setGeometry(
                    static_cast<int>(680 * escalaMapaGuardadaX),
                    static_cast<int>(815 * escalaMapaGuardadaY),
                    static_cast<int>(720 * escalaMapaGuardadaX),
                    static_cast<int>(85 * escalaMapaGuardadaY)
                );


                QFont fuenteBotonesMapaGuardada;

                if (!nombreFuenteAlice.isEmpty()) {
                    fuenteBotonesMapaGuardada.setFamily(nombreFuenteAlice);
                }

                fuenteBotonesMapaGuardada.setBold(true);

                fuenteBotonesMapaGuardada.setPixelSize(
                    static_cast<int>(46 * escalaMapaGuardada)
                );

                botonLoadMapaGuardada->setFont(fuenteBotonesMapaGuardada);
                botonNewQuestMapa->setFont(fuenteBotonesMapaGuardada);


                fondoMapaGuardada->lower();

                textoMapaGuardada->raise();

                botonBackMapaGuardada->raise();
                botonLoadMapaGuardada->raise();
                botonNewQuestMapa->raise();
            }

            // PAGINA PARTIDA PERSONALIZADA GUARDADA
            if (paginaPersonalizadaGuardada != nullptr && contenedorPersonalizadaGuardada != nullptr) {

                double escalaPersonalizadaGuardadaX =
                    static_cast<double>(
                        paginaPersonalizadaGuardada->width()
                    ) / ANCHO_DISENO_MENU;

                double escalaPersonalizadaGuardadaY =
                    static_cast<double>(
                        paginaPersonalizadaGuardada->height()
                    ) / ALTO_DISENO_MENU;

                double escalaPersonalizadaGuardada =
                    qMin(
                        escalaPersonalizadaGuardadaX,
                        escalaPersonalizadaGuardadaY
                    );

                // CONTENEDOR
                contenedorPersonalizadaGuardada->setGeometry(
                    0,
                    0,
                    paginaPersonalizadaGuardada->width(),
                    paginaPersonalizadaGuardada->height()
                );

                // FONDO
                fondoPersonalizadaGuardada->setGeometry(
                    0,
                    0,
                    contenedorPersonalizadaGuardada->width(),
                    contenedorPersonalizadaGuardada->height()
                );

                // TEXTO
                textoPersonalizadaGuardada->setGeometry(
                    static_cast<int>(
                        400 * escalaPersonalizadaGuardadaX
                    ),
                    static_cast<int>(
                        500 * escalaPersonalizadaGuardadaY
                    ),
                    static_cast<int>(
                        1280 * escalaPersonalizadaGuardadaX
                    ),
                    static_cast<int>(
                        105 * escalaPersonalizadaGuardadaY
                    )
                );

                QFont fuenteTextoPersonalizadaGuardada;

                if (!nombreFuenteLobster.isEmpty()) {
                    fuenteTextoPersonalizadaGuardada.setFamily(
                        nombreFuenteLobster
                    );
                }

                fuenteTextoPersonalizadaGuardada.setPixelSize(
                    static_cast<int>(
                        55 * escalaPersonalizadaGuardada
                    )
                );

                textoPersonalizadaGuardada->setFont(
                    fuenteTextoPersonalizadaGuardada
                );

                // BACK
                botonBackPersonalizadaGuardada->setGeometry(
                    static_cast<int>(
                        145 * escalaPersonalizadaGuardadaX
                    ),
                    static_cast<int>(
                        485 * escalaPersonalizadaGuardadaY
                    ),
                    static_cast<int>(
                        145 * escalaPersonalizadaGuardadaX
                    ),
                    static_cast<int>(
                        145 * escalaPersonalizadaGuardadaY
                    )
                );

                botonBackPersonalizadaGuardada->setIconSize(
                    QSize(
                        static_cast<int>(
                            135 * escalaPersonalizadaGuardada
                        ),
                        static_cast<int>(
                            135 * escalaPersonalizadaGuardada
                        )
                    )
                );

                // LOAD GAME
                botonLoadPersonalizadaGuardada->setGeometry(
                    static_cast<int>(
                        720 * escalaPersonalizadaGuardadaX
                    ),
                    static_cast<int>(
                        700 * escalaPersonalizadaGuardadaY
                    ),
                    static_cast<int>(
                        640 * escalaPersonalizadaGuardadaX
                    ),
                    static_cast<int>(
                        85 * escalaPersonalizadaGuardadaY
                    )
                );

                // NEW QUEST
                botonNewQuestPersonalizada->setGeometry(
                    static_cast<int>(
                        680 * escalaPersonalizadaGuardadaX
                    ),
                    static_cast<int>(
                        815 * escalaPersonalizadaGuardadaY
                    ),
                    static_cast<int>(
                        720 * escalaPersonalizadaGuardadaX
                    ),
                    static_cast<int>(
                        85 * escalaPersonalizadaGuardadaY
                    )
                );

                QFont fuenteBotonesPersonalizadaGuardada;

                if (!nombreFuenteAlice.isEmpty()) {
                    fuenteBotonesPersonalizadaGuardada.setFamily(
                        nombreFuenteAlice
                    );
                }

                fuenteBotonesPersonalizadaGuardada.setBold(true);

                fuenteBotonesPersonalizadaGuardada.setPixelSize(
                    static_cast<int>(
                        46 * escalaPersonalizadaGuardada
                    )
                );

                botonLoadPersonalizadaGuardada->setFont(
                    fuenteBotonesPersonalizadaGuardada
                );

                botonNewQuestPersonalizada->setFont(
                    fuenteBotonesPersonalizadaGuardada
                );

                fondoPersonalizadaGuardada->lower();

                textoPersonalizadaGuardada->raise();

                botonBackPersonalizadaGuardada->raise();
                botonLoadPersonalizadaGuardada->raise();
                botonNewQuestPersonalizada->raise();
            }

            // PAGINA CUSTOM MAYHEM
            if (paginaCustomMayhem != nullptr && contenedorCustomMayhem != nullptr) {

                double escalaCustomX =
                    static_cast<double>(paginaCustomMayhem->width()) / ANCHO_DISENO_MENU;

                double escalaCustomY =
                    static_cast<double>(paginaCustomMayhem->height()) / ALTO_DISENO_MENU;

                double escalaCustom = qMin(escalaCustomX, escalaCustomY);

                // CONTENEDOR
                contenedorCustomMayhem->setGeometry(
                    0,
                    0,
                    paginaCustomMayhem->width(),
                    paginaCustomMayhem->height()
                );

                // FONDO
                fondoCustomMayhem->setGeometry(
                    0,
                    0,
                    contenedorCustomMayhem->width(),
                    contenedorCustomMayhem->height()
                );

                // TEXTO FELIX
                textoCustomMayhem->setGeometry(
                    static_cast<int>(140 * escalaCustomX),
                    static_cast<int>(490 * escalaCustomY),
                    static_cast<int>(1800 * escalaCustomX),
                    static_cast<int>(110 * escalaCustomY)
                );

                QFont fuenteTextoCustom;

                if (!nombreFuenteLobster.isEmpty()) {
                    fuenteTextoCustom.setFamily(nombreFuenteLobster);
                }

                fuenteTextoCustom.setPixelSize(
                    static_cast<int>(55 * escalaCustom)
                );

                textoCustomMayhem->setFont(fuenteTextoCustom);

                // INSTRUCCION
                textoInstruccionCustomMayhem->setGeometry(
                    static_cast<int>(520 * escalaCustomX),
                    static_cast<int>(660 * escalaCustomY),
                    static_cast<int>(1040 * escalaCustomX),
                    static_cast<int>(65 * escalaCustomY)
                );

                QFont fuenteInstruccion;

                if (!nombreFuenteAlice.isEmpty()) {
                    fuenteInstruccion.setFamily(nombreFuenteAlice);
                }

                fuenteInstruccion.setBold(true);
                fuenteInstruccion.setPixelSize(
                    static_cast<int>(34 * escalaCustom)
                );

                textoInstruccionCustomMayhem->setFont(fuenteInstruccion);

                // DROPDOWN FILAS
                comboFilasCustomMayhem->setGeometry(
                    static_cast<int>(815 * escalaCustomX),
                    static_cast<int>(760 * escalaCustomY),
                    static_cast<int>(170 * escalaCustomX),
                    static_cast<int>(70 * escalaCustomY)
                );

                // X
                textoXCustomMayhem->setGeometry(
                    static_cast<int>(995 * escalaCustomX),
                    static_cast<int>(760 * escalaCustomY),
                    static_cast<int>(90 * escalaCustomX),
                    static_cast<int>(70 * escalaCustomY)
                );

                // DROPDOWN COLUMNAS
                comboColumnasCustomMayhem->setGeometry(
                    static_cast<int>(1095 * escalaCustomX),
                    static_cast<int>(760 * escalaCustomY),
                    static_cast<int>(170 * escalaCustomX),
                    static_cast<int>(70 * escalaCustomY)
                );

                QFont fuenteDropdown;

                if (!nombreFuenteAlice.isEmpty()) {
                    fuenteDropdown.setFamily(nombreFuenteAlice);
                }

                fuenteDropdown.setBold(true);
                fuenteDropdown.setPixelSize(
                    static_cast<int>(28 * escalaCustom)
                );

                comboFilasCustomMayhem->setFont(fuenteDropdown);
                comboColumnasCustomMayhem->setFont(fuenteDropdown);

                QFont fuenteX = fuenteDropdown;
                fuenteX.setPixelSize(static_cast<int>(38 * escalaCustom));
                textoXCustomMayhem->setFont(fuenteX);

                // PLAY GAME
                botonPlayGameCustomMayhem->setGeometry(
                    static_cast<int>(765 * escalaCustomX),
                    static_cast<int>(855 * escalaCustomY),
                    static_cast<int>(550 * escalaCustomX),
                    static_cast<int>(75 * escalaCustomY)
                );

                QFont fuentePlayCustom;

                if (!nombreFuenteAlice.isEmpty()) {
                    fuentePlayCustom.setFamily(nombreFuenteAlice);
                }

                fuentePlayCustom.setBold(true);
                fuentePlayCustom.setPixelSize(
                    static_cast<int>(45 * escalaCustom)
                );

                botonPlayGameCustomMayhem->setFont(fuentePlayCustom);

                // BACK
                botonBackCustomMayhem->setGeometry(
                    static_cast<int>(115 * escalaCustomX),
                    static_cast<int>(115 * escalaCustomY),
                    static_cast<int>(145 * escalaCustomX),
                    static_cast<int>(145 * escalaCustomY)
                );

                botonBackCustomMayhem->setIconSize(
                    QSize(
                        static_cast<int>(135 * escalaCustom),
                        static_cast<int>(135 * escalaCustom)
                    )
                );

                // MAPA CALAMITY CAYS
                botonMapaCustomMayhem->setGeometry(
                    static_cast<int>(1765 * escalaCustomX),
                    static_cast<int>(500 * escalaCustomY),
                    static_cast<int>(145 * escalaCustomX),
                    static_cast<int>(145 * escalaCustomY)
                );

                botonMapaCustomMayhem->setIconSize(
                    QSize(
                        static_cast<int>(135 * escalaCustom),
                        static_cast<int>(135 * escalaCustom)
                    )
                );

                fondoCustomMayhem->lower();

                textoCustomMayhem->raise();
                textoInstruccionCustomMayhem->raise();
                textoXCustomMayhem->raise();

                comboFilasCustomMayhem->raise();
                comboColumnasCustomMayhem->raise();

                botonBackCustomMayhem->raise();
                botonMapaCustomMayhem->raise();
                botonPlayGameCustomMayhem->raise();
            }

            // PAGINA MAPA CALAMITY CAYS
            if (paginaMapaCustomMayhem != nullptr &&
                contenedorMapaCustomMayhem != nullptr) {

                double escalaMapaCustomX =
                    static_cast<double>(paginaMapaCustomMayhem->width()) /
                    ANCHO_DISENO_MENU;

                double escalaMapaCustomY =
                    static_cast<double>(paginaMapaCustomMayhem->height()) /
                    ALTO_DISENO_MENU;

                double escalaMapaCustom =
                    qMin(escalaMapaCustomX, escalaMapaCustomY);

                // CONTENEDOR
                contenedorMapaCustomMayhem->setGeometry(
                    0,
                    0,
                    paginaMapaCustomMayhem->width(),
                    paginaMapaCustomMayhem->height()
                );

                // MAPA
                fondoMapaCustomMayhem->setGeometry(
                    0,
                    0,
                    contenedorMapaCustomMayhem->width(),
                    contenedorMapaCustomMayhem->height()
                );

                // BACK
                botonBackMapaCustomMayhem->setGeometry(
                    static_cast<int>(40 * escalaMapaCustomX),
                    static_cast<int>(475 * escalaMapaCustomY),
                    static_cast<int>(145 * escalaMapaCustomX),
                    static_cast<int>(145 * escalaMapaCustomY)
                );

                botonBackMapaCustomMayhem->setIconSize(
                    QSize(
                        static_cast<int>(135 * escalaMapaCustom),
                        static_cast<int>(135 * escalaMapaCustom)
                    )
                );

                fondoMapaCustomMayhem->lower();
                botonBackMapaCustomMayhem->raise();
            }

        }

        // URL PARA ARCHIVO LOCAL
        QUrl crearUrlArchivo(const string &ruta) const {

            QFileInfo archivo(QString::fromStdString(ruta));

            return QUrl::fromLocalFile(archivo.absoluteFilePath());
        }


    protected:
        // RESIZE
        void resizeEvent(QResizeEvent* evento) override {

            QWidget::resizeEvent(evento);

            if (paginas != nullptr) {
                paginas->setGeometry(rect());
            }

            if (paginaIntro != nullptr && videoIntro != nullptr) {

                videoIntro->setGeometry(
                    0,
                    0,
                    paginaIntro->width(),
                    paginaIntro->height()
                );
            }

            ajustarInterfaz();
        }
};

// FUNCION PARA CREAR MENUS UI
QWidget* crearMenusUI() {

    MenusUI* menus = new MenusUI();

    return menus;
}