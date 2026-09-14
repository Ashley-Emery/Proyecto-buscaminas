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

using namespace std;

QWidget* crearJuegoUI(Usuario* usuarioActual, SistemaUsuarios* sistemaUsuarios, ModoJuego modo, int numeroNivel);


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
                botonExit(nullptr) {

            for (int i = 0; i < CANTIDAD_NIVELES; i++) {
                botonesNivelMapa[i] = nullptr;
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

            if (!sistemaUsuarios.cargar()) {

                QMessageBox::warning(
                    this,
                    "Users",
                    "The user database could not be loaded."
                );
            }

            paginas->setCurrentWidget(paginaIntro);
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
                iniciarModoProgresivo();
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


        void iniciarModoProgresivo() {

            if (usuarioActual == nullptr) {
                return;
            }

            int numeroNivel = usuarioActual->obtenerSiguienteNivelProgresivo();

            if (numeroNivel > CANTIDAD_NIVELES) {
                numeroNivel = CANTIDAD_NIVELES;
            }

            QWidget* ventanaJuego = crearJuegoUI(
                usuarioActual,
                &sistemaUsuarios,
                ModoJuego::PROGRESIVO,
                numeroNivel
            );

            ventanaJuego->setAttribute(Qt::WA_DeleteOnClose);
            ventanaJuego->show();

            hide();
        }

        void mostrarMiPerfil() {
            // Se implementara con el layout MY PROFILE.
        }

        void mostrarComoJugar() {
            // Se implementara con los layouts HOW TO PLAY.
        }

        void mostrarHighScores() {
            // Se implementara con el layout HIGH SCORES.
        }

        void mostrarModoMapa() {

            if (usuarioActual == nullptr) {
                return;
            }

            paginas->setCurrentWidget(paginaMapa);

            ajustarInterfaz();
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

            connect(ventanaJuego, &QObject::destroyed, this, [this]() {
                show();
                mostrarMenuPrincipal();
            });

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
                    static_cast<int>(252 * escalaMapaX),
                    static_cast<int>(600 * escalaMapaY),
                    static_cast<int>(303 * escalaMapaX),
                    static_cast<int>(229 * escalaMapaY)
                );


                // NIVEL 2 - CARNIVAL MINEFIELD
                botonesNivelMapa[1]->setGeometry(
                    static_cast<int>(486 * escalaMapaX),
                    static_cast<int>(323 * escalaMapaY),
                    static_cast<int>(303 * escalaMapaX),
                    static_cast<int>(227 * escalaMapaY)
                );


                // NIVEL 3 - DRAGON MINEFIELD
                botonesNivelMapa[2]->setGeometry(
                    static_cast<int>(769 * escalaMapaX),
                    static_cast<int>(44 * escalaMapaY),
                    static_cast<int>(303 * escalaMapaX),
                    static_cast<int>(228 * escalaMapaY)
                );


                // NIVEL 4 - RUMBLE MINEFIELD
                botonesNivelMapa[3]->setGeometry(
                    static_cast<int>(1225 * escalaMapaX),
                    static_cast<int>(158 * escalaMapaY),
                    static_cast<int>(303 * escalaMapaX),
                    static_cast<int>(228 * escalaMapaY)
                );


                // NIVEL 5 - HIGH SEAS MINEFIELD
                botonesNivelMapa[4]->setGeometry(
                    static_cast<int>(1596 * escalaMapaX),
                    static_cast<int>(344 * escalaMapaY),
                    static_cast<int>(303 * escalaMapaX),
                    static_cast<int>(228 * escalaMapaY)
                );


                // NIVEL 6 - MECHANICAL MINEFIELD
                botonesNivelMapa[5]->setGeometry(
                    static_cast<int>(1127 * escalaMapaX),
                    static_cast<int>(470 * escalaMapaY),
                    static_cast<int>(304 * escalaMapaX),
                    static_cast<int>(229 * escalaMapaY)
                );


                // NIVEL 7 - AERIAL MINEFIELD
                botonesNivelMapa[6]->setGeometry(
                    static_cast<int>(616 * escalaMapaX),
                    static_cast<int>(740 * escalaMapaY),
                    static_cast<int>(303 * escalaMapaX),
                    static_cast<int>(228 * escalaMapaY)
                );


                // NIVEL 8 - BAKER'S MINEFIELD
                botonesNivelMapa[7]->setGeometry(
                    static_cast<int>(1040 * escalaMapaX),
                    static_cast<int>(762 * escalaMapaY),
                    static_cast<int>(304 * escalaMapaX),
                    static_cast<int>(228 * escalaMapaY)
                );


                // NIVEL 9 - ONE HELL OF A MINEFIELD
                botonesNivelMapa[8]->setGeometry(
                    static_cast<int>(1492 * escalaMapaX),
                    static_cast<int>(740 * escalaMapaY),
                    static_cast<int>(302 * escalaMapaX),
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