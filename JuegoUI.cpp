#include "Partida.h"
#include "Usuarios.h"
#include "Assets.h"

#include <iostream>

#include <QWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>

#include <QTimer>
#include <QPixmap>
#include <QIcon>
#include <QFont>
#include <QFontDatabase>
#include <QFileInfo>
#include <QUrl>
#include <QMessageBox>
#include <QResizeEvent>
#include <QPainter>

#include <functional>

#include <QCloseEvent>

using namespace std;


// ---- CONSTANTES VISUALES
const int ANCHO_DISENO_JUEGO = 2080;
const int ALTO_DISENO_JUEGO = 1100;

const int ANCHO_VENTANA_INICIAL = 1280;
const int ALTO_VENTANA_INICIAL = 720;

const int ANCHO_MINIMO_JUEGO = 1280;
const int ALTO_MINIMO_JUEGO = 720;

// ---- Zona utilizada por el tablero dentro del diseño 2080 x 1100.
const int TABLERO_X = 95;
const int TABLERO_Y = 360;
const int TABLERO_ANCHO = 1890;
const int TABLERO_ALTO = 520;


// ---- Las celdas de Basico e Intermedio llegan aproximadamente a 60 px en el mockup.
const int TAMANO_MAXIMO_CELDA = 60;
const int SEPARACION_CELDAS = 0;

// ---- CELDA GRAFICA - Detecta el clic sobre una celda del QGraphicsScene
class CeldaGrafica : public QGraphicsPixmapItem {

    private:

        int fila;
        int columna;

        function<void(int, int, Qt::MouseButton)> accionClic;

    public:
        CeldaGrafica(int fila, int columna, function<void(int, int, Qt::MouseButton)> accionClic)
            : fila(fila), columna(columna), accionClic(accionClic) {

            setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
        }

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* evento) override {

            if (accionClic) {
                accionClic(fila, columna, evento->button());
            }

            evento->accept();
        }
};

// ---- JUEGO UI
class JuegoUI : public QWidget {

    private:
        Usuario* usuarioActual;
        SistemaUsuarios* sistemaUsuarios;

        ModoJuego modoActual;
        int numeroNivelActual;

        ConfiguracionPartida configuracionActual;
        Partida partida;

        QString nombreFuenteAlice;
        QString nombreFuenteLobster;

        // STACK DE JUEGO
        QStackedWidget* paginas;

        QWidget* paginaBoss;
        QWidget* paginaPartida;
        QWidget* paginaJulius;
        QWidget* paginaDerrota;
        QWidget* paginaSalida;
        QWidget* paginaAyuda;
        QWidget* paginaVictoria;
        QWidget* paginaCelebracionFinal;
        QWidget* paginaMisionCumplida;

        // INTRO DEL BOSS
        QVideoWidget* videoBoss;
        QMediaPlayer* reproductorBoss;

        bool introBossFinalizada;
        bool introBossIniciada;

        // ANIMACION DE DERROTA
        QVideoWidget* videoJulius;
        QMediaPlayer* reproductorJulius;
        bool animacionJuliusFinalizada;
        QTimer* timerRespaldoJulius;

        // PANTALLA LEVEL FAILED
        QWidget* contenedorDerrota;
        QLabel* fondoDerrota;
        QPushButton* botonRetryDerrota;
        QPushButton* botonQuitDerrota;

        // PANTALLA LEVEL WON
        QWidget* contenedorVictoria;
        QLabel* fondoVictoria;
        QPushButton* botonNextQuestVictoria;
        QPushButton* botonQuitVictoria;

        // CELEBRACION FINAL NIVEL 9
        QVideoWidget* videoCelebracionFinal;
        QMediaPlayer* reproductorCelebracionFinal;
        bool celebracionFinalFinalizada;

        // MISION CUMPLIDA
        QWidget* contenedorMisionCumplida;
        QLabel* fondoMisionCumplida;
        QPushButton* botonCustomMayhemFinal;
        QPushButton* botonClaimRewardFinal;
        QLabel* bubbleFinal;

        // PANTALLA DE PARTIDA
        QWidget* contenedorDiseno;
        QLabel* fondo;
        QVideoWidget* videoBanner;
        QMediaPlayer* reproductorBanner;
        QMediaPlaylist* playlistBanner;
        QLabel* labelTiempo;
        QTimer* timerPartida;

        // TABLERO
        QGraphicsView* vistaTablero;
        QGraphicsScene* escenaTablero;
        CeldaGrafica*** celdasGraficas;
        int filasGraficas;
        int columnasGraficas;
        int tamanoCeldaBase;

        // Mina especifica donde ocurrio el clic de derrota
        int filaBombaExplotada;
        int columnaBombaExplotada;

        // CONTROLES
        QPushButton* botonHome;
        QPushButton* botonRetry;
        QPushButton* botonHelp;
        QPushButton* botonMusic;

        // INFO DEL NIVEL
        QPushButton* botonFlagNivel;
        QLabel* bannerNivel;
        bool bannerNivelVisible;

        // MUSICA
        QMediaPlayer* reproductorMusica;
        QMediaPlaylist* playlistMusica;

        // OTROS
        bool resultadoProcesado;

        // PANTALLA OPCIONES DE SALIDA
        QWidget* contenedorSalida;
        QLabel* fondoSalida;
        QLabel* textoSalida;
        QPushButton* botonBackSalida;
        QPushButton* botonGuardarSalir;
        QPushButton* botonSalirSinGuardar;

        // PANTALLA AYUDA
        QWidget* contenedorAyuda;
        QLabel* fondoAyuda;
        QPushButton* botonBackAyuda;


    public:
        JuegoUI(Usuario* usuarioActual, SistemaUsuarios* sistemaUsuarios, ModoJuego modo, int numeroNivel, bool configurarAutomaticamente = true, QWidget* parent = nullptr)
            : QWidget(parent),
                usuarioActual(usuarioActual),
                sistemaUsuarios(sistemaUsuarios),
                modoActual(modo),
                numeroNivelActual(numeroNivel),
                paginas(nullptr),
                paginaBoss(nullptr),
                paginaPartida(nullptr),
                paginaJulius(nullptr),
                paginaDerrota(nullptr),
                paginaVictoria(nullptr),
                paginaCelebracionFinal(nullptr),
                paginaMisionCumplida(nullptr),
                videoJulius(nullptr),
                reproductorJulius(nullptr),
                animacionJuliusFinalizada(false),
                timerRespaldoJulius(nullptr),
                contenedorDerrota(nullptr),
                fondoDerrota(nullptr),
                botonRetryDerrota(nullptr),
                botonQuitDerrota(nullptr),
                contenedorVictoria(nullptr),
                fondoVictoria(nullptr),
                botonNextQuestVictoria(nullptr),
                botonQuitVictoria(nullptr),
                videoCelebracionFinal(nullptr),
                reproductorCelebracionFinal(nullptr),
                celebracionFinalFinalizada(false),
                contenedorMisionCumplida(nullptr),
                fondoMisionCumplida(nullptr),
                botonCustomMayhemFinal(nullptr),
                botonClaimRewardFinal(nullptr),
                bubbleFinal(nullptr),
                videoBoss(nullptr),
                reproductorBoss(nullptr),
                introBossFinalizada(false),
                introBossIniciada(false),
                contenedorDiseno(nullptr),
                fondo(nullptr),
                nombreFuenteAlice(""),
                nombreFuenteLobster(""),
                videoBanner(nullptr),
                reproductorBanner(nullptr),
                playlistBanner(nullptr),
                labelTiempo(nullptr),
                timerPartida(nullptr),
                vistaTablero(nullptr),
                escenaTablero(nullptr),
                celdasGraficas(nullptr),
                filasGraficas(0),
                columnasGraficas(0),
                tamanoCeldaBase(0),
                filaBombaExplotada(-1),
                columnaBombaExplotada(-1),
                botonHome(nullptr),
                botonRetry(nullptr),
                botonHelp(nullptr),
                botonMusic(nullptr),
                botonFlagNivel(nullptr),
                bannerNivel(nullptr),
                bannerNivelVisible(false),
                reproductorMusica(nullptr),
                playlistMusica(nullptr),
                paginaSalida(nullptr),
                paginaAyuda(nullptr),
                contenedorSalida(nullptr),
                fondoSalida(nullptr),
                textoSalida(nullptr),
                botonBackSalida(nullptr),
                botonGuardarSalir(nullptr),
                botonSalirSinGuardar(nullptr),
                contenedorAyuda(nullptr),
                fondoAyuda(nullptr),
                botonBackAyuda(nullptr),
                resultadoProcesado(false) {

            setWindowTitle("Minefield Mayhem - The Oppenheimer Incident");
            setProperty("destinoAlCerrar", "menu");

            resize(ANCHO_VENTANA_INICIAL, ALTO_VENTANA_INICIAL);
            setMinimumSize(ANCHO_MINIMO_JUEGO, ALTO_MINIMO_JUEGO);

            crearInterfaz();

            if (configurarAutomaticamente) {
                configurarNivel(modo, numeroNivel);
            }

            QTimer::singleShot(0, this, [this]() {

                if (paginas != nullptr) {
                    paginas->setGeometry(rect());
                }

                ajustarInterfaz();

                if (paginaBoss != nullptr && videoBoss != nullptr) {

                    videoBoss->setGeometry(
                        0,
                        0,
                        paginaBoss->width(),
                        paginaBoss->height()
                    );
                }

                if (paginaJulius != nullptr && videoJulius != nullptr) {

                    videoJulius->setGeometry(
                        0,
                        0,
                        paginaJulius->width(),
                        paginaJulius->height()
                    );
                }
            });
        }

        ~JuegoUI() {
            liberarCeldasGraficas();
        }

        bool cargarPartidaGuardadaDesdeDisco() {

            if (usuarioActual == nullptr) {
                return false;
            }

            string nombreUsuario = usuarioActual->obtenerNombreUsuario();

            if (!ArchivoPersistencia::cargarPartida(nombreUsuario, modoActual, partida)) {
                return false;
            }

            configuracionActual = partida.obtenerConfiguracion();

            modoActual = configuracionActual.obtenerModo();
            numeroNivelActual = configuracionActual.obtenerNumeroNivel();

            resultadoProcesado = false;

            filaBombaExplotada = -1;
            columnaBombaExplotada = -1;

            // La partida ya esta completamente cargada en memoria.
            // Consumimos el save para evitar dejar una copia vieja en cola.
            if (!ArchivoPersistencia::eliminarPartidaGuardada(nombreUsuario, modoActual)) {
                return false;
            }

            configurarFondo();
            crearTableroGrafico();
            actualizarTiempo();
            configurarMusica();
            configurarInfoNivel();

            paginas->setCurrentWidget(paginaPartida);

            ajustarInterfaz();

            reproductorBanner->play();
            iniciarMusica();

            timerPartida->start();

            return true;
        }

        bool configurarPartidaPersonalizada(int filas, int columnas) {

            ConfiguracionPartida nuevaConfiguracion =
                ConfiguracionPartida::crearPersonalizada(filas, columnas);

            if (!nuevaConfiguracion.esValida()) {
                return false;
            }

            modoActual = ModoJuego::PERSONALIZADO;
            numeroNivelActual = 0;

            introBossIniciada = false;
            introBossFinalizada = false;

            configuracionActual = nuevaConfiguracion;

            if (!partida.iniciar(configuracionActual)) {
                return false;
            }

            resultadoProcesado = false;

            filaBombaExplotada = -1;
            columnaBombaExplotada = -1;

            configurarFondo();
            crearTableroGrafico();
            actualizarTiempo();
            configurarMusica();
            configurarInfoNivel();

            reproducirIntroBoss();

            return true;
        }

    private:

        // CREAR INTERFAZ PRINCIPAL
        void crearInterfaz() {

            int idFuenteAlice = QFontDatabase::addApplicationFont(QString::fromStdString(FONT_ALICE));

            if (idFuenteAlice != -1) {
                QStringList familias = QFontDatabase::applicationFontFamilies(idFuenteAlice);

                if (!familias.isEmpty()) {
                    nombreFuenteAlice = familias.first();
                }
            }

            int idFuenteLobster = QFontDatabase::addApplicationFont(QString::fromStdString(FONT_LOBSTER));

            if (idFuenteLobster != -1) {

                QStringList familias = QFontDatabase::applicationFontFamilies(idFuenteLobster);

                if (!familias.isEmpty()) {
                    nombreFuenteLobster = familias.first();
                }
            }

            paginas = new QStackedWidget(this);

            crearPaginaBoss();
            crearPaginaPartida();
            crearPaginaJulius();
            crearPaginaDerrota();
            crearPaginaVictoria();
            crearPaginaCelebracionFinal();
            crearPaginaMisionCumplida();
            crearPaginaSalida();
            crearPaginaAyuda();

            paginas->setGeometry(rect());

            paginaBoss->setGeometry(paginas->rect());
            paginaPartida->setGeometry(paginas->rect());
            paginaJulius->setGeometry(paginas->rect());
            paginaDerrota->setGeometry(paginas->rect());
            paginaVictoria->setGeometry(paginas->rect());
            paginaCelebracionFinal->setGeometry(paginas->rect());
            paginaMisionCumplida->setGeometry(paginas->rect());
            paginaSalida->setGeometry(paginas->rect());
            paginaAyuda->setGeometry(paginas->rect());

            ajustarInterfaz();

            timerPartida = new QTimer(this);
            timerPartida->setInterval(1000);

            connect(timerPartida, &QTimer::timeout, this, [this]() {

                partida.actualizarCronometro();
                actualizarTiempo();

                if (partida.estaFinalizada()) {
                    verificarFinPartida();
                }
            });

            // TIMER DE RESPALDO PARA LA ANIMACION DE JULIUS
            timerRespaldoJulius = new QTimer(this);
            timerRespaldoJulius->setSingleShot(true);

            connect(timerRespaldoJulius, &QTimer::timeout, this, [this]() {
                finalizarAnimacionJulius();
            });

        }

        // PAGINA INTRO BOSS
        void crearPaginaBoss() {

            paginaBoss = new QWidget();
            paginaBoss->setStyleSheet("background-color: black;");

            videoBoss = new QVideoWidget(paginaBoss);
            videoBoss->setAspectRatioMode(Qt::IgnoreAspectRatio);

            reproductorBoss = new QMediaPlayer(this);
            reproductorBoss->setVideoOutput(videoBoss);

            // La musica del nivel sera el audio principal
            reproductorBoss->setVolume(0);

            connect(reproductorBoss, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus estado) {

                if (estado == QMediaPlayer::EndOfMedia) {
                    finalizarIntroBoss();
                }
            });

            paginas->addWidget(paginaBoss);
        }

        // PAGINA PARTIDA
        void crearPaginaPartida() {

            paginaPartida = new QWidget();

            contenedorDiseno = new QWidget(paginaPartida);

            fondo = new QLabel(contenedorDiseno);
            fondo->setScaledContents(true);

            // BANNER
            videoBanner = new QVideoWidget(contenedorDiseno);
            videoBanner->setAspectRatioMode(Qt::IgnoreAspectRatio);
            reproductorBanner = new QMediaPlayer(this);
            playlistBanner = new QMediaPlaylist(this);

            playlistBanner->addMedia(crearUrlArchivo(ANIMACION_TO_THE_RESCUE));
            playlistBanner->setCurrentIndex(0);
            playlistBanner->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

            reproductorBanner->setPlaylist(playlistBanner);
            reproductorBanner->setVideoOutput(videoBanner);
            reproductorBanner->setVolume(0);

            // TIMER VISUAL
            labelTiempo = new QLabel(videoBanner);

            labelTiempo->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

            QFont fuenteTimer;

            if (!nombreFuenteAlice.isEmpty()) {
                fuenteTimer.setFamily(nombreFuenteAlice);
            }

            fuenteTimer.setBold(false);
            labelTiempo->setFont(fuenteTimer);

            labelTiempo->setStyleSheet(
                "QLabel {"
                "background-color: #62a0bc;"
                "border: none;"
                "color: black;"
                "}"
            );

            labelTiempo->raise();

            // TABLERO
            escenaTablero = new QGraphicsScene(this);

            vistaTablero = new QGraphicsView(escenaTablero, contenedorDiseno);

            vistaTablero->setFrameShape(QFrame::NoFrame);
            vistaTablero->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            vistaTablero->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            vistaTablero->setStyleSheet(
                "background: transparent;"
                "border: none;"
            );

            vistaTablero->setRenderHint(QPainter::SmoothPixmapTransform, true);

            // BOTONES
            botonHome = crearBotonImagen(obtenerRutaBoton("home"));
            botonRetry = crearBotonImagen(obtenerRutaBoton("retry"));
            botonHelp = crearBotonImagen(obtenerRutaBoton("help"));
            botonMusic = crearBotonImagen(obtenerRutaBoton("music"));

            // BANDERA DE INFORMACION DEL NIVEL
            botonFlagNivel = new QPushButton(contenedorDiseno);
            botonFlagNivel->setStyleSheet(
                "QPushButton {"
                "background: transparent;"
                "border: none;"
                "}"
            );
            botonFlagNivel->setCursor(Qt::PointingHandCursor);
            botonFlagNivel->setFocusPolicy(Qt::NoFocus);

            // BANNER CON NOMBRE DEL NIVEL Y BOSS
            bannerNivel = new QLabel(contenedorDiseno);
            bannerNivel->setScaledContents(true);
            bannerNivel->setStyleSheet("background: transparent;");
            bannerNivel->hide();

            bannerNivelVisible = false;

            botonHome->setParent(contenedorDiseno);
            botonRetry->setParent(contenedorDiseno);
            botonHelp->setParent(contenedorDiseno);
            botonMusic->setParent(contenedorDiseno);

            connect(botonRetry, &QPushButton::clicked, this, [this]() {
                reiniciarPartida();
            });

            connect(botonMusic, &QPushButton::clicked, this, [this]() {
                alternarMusica();
            });

            connect(botonHelp, &QPushButton::clicked, this, [this]() {
                mostrarAyuda();
            });

            connect(botonHome, &QPushButton::clicked, this, [this]() {
                mostrarOpcionesSalida();
            });

            connect(botonFlagNivel, &QPushButton::clicked, this, [this]() {
                alternarBannerNivel();
            });

            // MUSICA
            reproductorMusica = new QMediaPlayer(this);

            playlistMusica = new QMediaPlaylist(this);
            playlistMusica->setPlaybackMode(QMediaPlaylist::Loop);

            reproductorMusica->setPlaylist(playlistMusica);

            paginas->addWidget(paginaPartida);
        }

        int obtenerNumeroVisualPersonalizado() const {

            BossPersonalizado boss = obtenerBossPersonalizado(
                configuracionActual.obtenerFilas(),
                configuracionActual.obtenerColumnas()
            );

            switch (boss) {

                case BossPersonalizado::HILDA_BERG:
                    return 1;

                case BossPersonalizado::WERNER_WERMAN:
                    return 2;

                case BossPersonalizado::BARONESS_VON_BON_BON:
                    return 3;

                case BossPersonalizado::DJIMMI_THE_GREAT:
                    return 4;

                case BossPersonalizado::KING_DICE:
                    return 5;
            }

            return 1;
        }

        void configurarInfoNivel() {

            if (botonFlagNivel == nullptr || bannerNivel == nullptr) {
                return;
            }

            int numeroFlag = numeroNivelActual;
            string rutaBanner;

            if (modoActual == ModoJuego::PERSONALIZADO) {

                numeroFlag = obtenerNumeroVisualPersonalizado();

                BossPersonalizado boss = obtenerBossPersonalizado(
                    configuracionActual.obtenerFilas(),
                    configuracionActual.obtenerColumnas()
                );

                rutaBanner = obtenerRutaBanner(boss);

            } else {

                BossNivel boss = obtenerBossNivel(numeroNivelActual);

                rutaBanner = obtenerRutaBanner(boss);
            }

            string rutaFlag = obtenerRutaFlag(numeroFlag);

            botonFlagNivel->setIcon(
                QIcon(QString::fromStdString(rutaFlag))
            );

            bannerNivel->setPixmap(
                QPixmap(QString::fromStdString(rutaBanner))
            );

            bannerNivelVisible = false;
            bannerNivel->hide();
        }

        void alternarBannerNivel() {

            bannerNivelVisible = !bannerNivelVisible;

            if (bannerNivelVisible) {
                bannerNivel->show();
                bannerNivel->raise();
                botonFlagNivel->raise();
            } else {
                bannerNivel->hide();
            }
        }

        // ---- PAGINA JULIUS
        void crearPaginaJulius() {

            paginaJulius = new QWidget();
            paginaJulius->setStyleSheet("background-color: black;");

            videoJulius = new QVideoWidget(paginaJulius);
            videoJulius->setAspectRatioMode(Qt::IgnoreAspectRatio);

            reproductorJulius = new QMediaPlayer(this);
            reproductorJulius->setVideoOutput(videoJulius);

            connect(reproductorJulius, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus estado) {

                if (estado == QMediaPlayer::EndOfMedia) {
                    finalizarAnimacionJulius();
                }
            });

            paginas->addWidget(paginaJulius);
        }


        // ---- PAGINA LEVEL FAILED
        void crearPaginaDerrota() {

            paginaDerrota = new QWidget();

            contenedorDerrota = new QWidget(paginaDerrota);

            fondoDerrota = new QLabel(contenedorDerrota);
            fondoDerrota->setScaledContents(true);

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_LEVEL_FAILED));
            fondoDerrota->setPixmap(imagenFondo);

            botonRetryDerrota = new QPushButton("RETRY", contenedorDerrota);
            botonQuitDerrota = new QPushButton("QUIT GAME", contenedorDerrota);

            botonRetryDerrota->setCursor(Qt::PointingHandCursor);
            botonQuitDerrota->setCursor(Qt::PointingHandCursor);

            QString estiloBoton =
                "QPushButton {"
                "background: transparent;"
                "border: none;"
                "color: black;"
                "font-weight: bold;"
                "}"
                "QPushButton:hover {"
                "color: #de461b;"
                "}";

            botonRetryDerrota->setStyleSheet(estiloBoton);
            botonQuitDerrota->setStyleSheet(estiloBoton);

            connect(botonRetryDerrota, &QPushButton::clicked, this, [this]() {
                reiniciarDesdeDerrota();
            });

            connect(botonQuitDerrota, &QPushButton::clicked, this, [this]() {
                volverMenuPrincipal();
            });

            paginas->addWidget(paginaDerrota);
        }

        // ---- PAGINA LEVEL WON
        void crearPaginaVictoria() {

            paginaVictoria = new QWidget();

            contenedorVictoria = new QWidget(paginaVictoria);

            // FONDO
            fondoVictoria = new QLabel(contenedorVictoria);
            fondoVictoria->setScaledContents(true);

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_LEVEL_WON));
            fondoVictoria->setPixmap(imagenFondo);

            // BOTONES
            botonNextQuestVictoria = crearBotonTexto("NEXT QUEST");
            botonQuitVictoria = crearBotonTexto("QUIT GAME");

            botonNextQuestVictoria->setParent(contenedorVictoria);
            botonQuitVictoria->setParent(contenedorVictoria);

            // ACCIONES
            connect(botonNextQuestVictoria, &QPushButton::clicked, this, [this]() {
                continuarDespuesDeVictoria();
            });

            connect(botonQuitVictoria, &QPushButton::clicked, this, [this]() {
                salirDespuesDeVictoria();
            });

            paginas->addWidget(paginaVictoria);
        }

        // ---- PAGINA CELEBRACION FINAL
        void crearPaginaCelebracionFinal() {

            paginaCelebracionFinal = new QWidget();
            paginaCelebracionFinal->setStyleSheet("background-color: black;");

            videoCelebracionFinal = new QVideoWidget(paginaCelebracionFinal);
            videoCelebracionFinal->setAspectRatioMode(Qt::IgnoreAspectRatio);

            reproductorCelebracionFinal = new QMediaPlayer(this);
            reproductorCelebracionFinal->setVideoOutput(videoCelebracionFinal);

            connect(reproductorCelebracionFinal, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus estado) {

                if (estado == QMediaPlayer::EndOfMedia) {
                    finalizarCelebracionFinal();
                }
            });

            paginas->addWidget(paginaCelebracionFinal);
        }

        // ---- PAGINA MISION CUMPLIDA
        void crearPaginaMisionCumplida() {

            paginaMisionCumplida = new QWidget();

            contenedorMisionCumplida = new QWidget(paginaMisionCumplida);

            // FONDO
            fondoMisionCumplida = new QLabel(contenedorMisionCumplida);
            fondoMisionCumplida->setScaledContents(true);

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_FINISH_LINE));
            fondoMisionCumplida->setPixmap(imagenFondo);

            // BOTON CUSTOM MAYHEM
            botonCustomMayhemFinal =
                crearBotonImagen(obtenerRutaMenu("custom-mayhem"));

            botonCustomMayhemFinal->setParent(contenedorMisionCumplida);

            // BOTON CLAIM REWARD
            botonClaimRewardFinal =
                crearBotonImagen(obtenerRutaMenu("claim-reward"));

            botonClaimRewardFinal->setParent(contenedorMisionCumplida);

            // BUBBLE
            bubbleFinal = new QLabel(contenedorMisionCumplida);

            QPixmap imagenBubble(
                QString::fromStdString(
                    obtenerRutaMenu("bubble")
                )
            );

            bubbleFinal->setPixmap(imagenBubble);
            bubbleFinal->setScaledContents(true);
            bubbleFinal->hide();

            connect(botonCustomMayhemFinal, &QPushButton::clicked, this, [this]() {
                intentarIrCustomMayhemFinal();
            });

            connect(botonClaimRewardFinal, &QPushButton::clicked, this, [this]() {
                reclamarRewardFinal();
            });

            paginas->addWidget(paginaMisionCumplida);
        }

        // ---- REPRODUCIR CELEBRACION FINAL
        void reproducirCelebracionFinal() {

            celebracionFinalFinalizada = false;

            reproductorBanner->stop();
            reproductorMusica->stop();
            timerPartida->stop();

            reproductorCelebracionFinal->setMedia(
                crearUrlArchivo(
                    ANIMACION_JULIUS_CELEBRATES
                )
            );

            paginas->setCurrentWidget(
                paginaCelebracionFinal
            );

            videoCelebracionFinal->setGeometry(
                0,
                0,
                paginaCelebracionFinal->width(),
                paginaCelebracionFinal->height()
            );

            videoCelebracionFinal->show();
            videoCelebracionFinal->raise();

            reproductorCelebracionFinal->play();

            // El MP4 dura 4 segundos.
            // Este timer solo es respaldo por si EndOfMedia no llega.
            QTimer::singleShot(5000, this, [this]() {
                finalizarCelebracionFinal();
            });
        }

        void finalizarCelebracionFinal() {

            if (celebracionFinalFinalizada) {
                return;
            }

            celebracionFinalFinalizada = true;

            reproductorCelebracionFinal->stop();

            prepararPantallaMisionCumplida();

            paginas->setCurrentWidget(
                paginaMisionCumplida
            );

            ajustarInterfaz();
        }

        void prepararPantallaMisionCumplida() {

            if (usuarioActual == nullptr) {
                return;
            }

            bool rewardReclamado =
                usuarioActual->reclamoRecompensaFinalProgresiva();

            bubbleFinal->hide();

            // CUSTOM MAYHEM siempre empieza visible y normal.
            botonCustomMayhemFinal->setEnabled(true);
            botonCustomMayhemFinal->setCursor(Qt::PointingHandCursor);
            botonCustomMayhemFinal->setGraphicsEffect(nullptr);

            if (rewardReclamado) {

                // CLAIM REWARD queda congelado.
                botonClaimRewardFinal->setEnabled(false);
                botonClaimRewardFinal->setCursor(Qt::ArrowCursor);

            } else {

                botonClaimRewardFinal->setEnabled(true);
                botonClaimRewardFinal->setCursor(Qt::PointingHandCursor);
            }
        }

        void intentarIrCustomMayhemFinal() {

            if (usuarioActual == nullptr) {
                return;
            }

            if (!usuarioActual->reclamoRecompensaFinalProgresiva()) {

                // Se bloquea visualmente igual que CLAIM REWARD.
                botonCustomMayhemFinal->setGraphicsEffect(nullptr);
                botonCustomMayhemFinal->setEnabled(false);
                botonCustomMayhemFinal->setCursor(Qt::ArrowCursor);

                bubbleFinal->show();
                bubbleFinal->raise();

                return;
            }

            setProperty(
                "destinoAlCerrar",
                "personalizado"
            );

            close();
        }

        void reclamarRewardFinal() {

            if (
                usuarioActual == nullptr ||
                sistemaUsuarios == nullptr
            ) {
                return;
            }

            bool reclamado =
                SistemaPuntajes::reclamarRecompensaFinalProgresiva(
                    *usuarioActual,
                    *sistemaUsuarios
                );

            if (
                reclamado ||
                usuarioActual->reclamoRecompensaFinalProgresiva()
            ) {

                bubbleFinal->hide();

                // CLAIM REWARD queda congelado.
                botonClaimRewardFinal->setEnabled(false);
                botonClaimRewardFinal->setCursor(Qt::ArrowCursor);

                // CUSTOM MAYHEM vuelve a estar disponible.
                botonCustomMayhemFinal->setGraphicsEffect(nullptr);
                botonCustomMayhemFinal->setEnabled(true);
                botonCustomMayhemFinal->setCursor(Qt::PointingHandCursor);
            }
        }

        // ---- PAGINA OPCIONES DE SALIDA
        void crearPaginaSalida() {

            paginaSalida = new QWidget();

            contenedorSalida = new QWidget(paginaSalida);

            // FONDO
            fondoSalida = new QLabel(contenedorSalida);
            fondoSalida->setScaledContents(true);

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_FELIX));
            fondoSalida->setPixmap(imagenFondo);

            // TEXTO
            textoSalida = new QLabel(
                "“Heading out already, kid? The minefield’ll be waitin’!”",
                contenedorSalida
            );

            textoSalida->setAlignment(Qt::AlignCenter);

            textoSalida->setStyleSheet(
                "QLabel {"
                "background: transparent;"
                "color: black;"
                "}"
            );

            // BOTONES
            botonBackSalida = crearBotonImagen(obtenerRutaBoton("back"));
            botonGuardarSalir = crearBotonTexto("SAVE GAME AND EXIT");
            botonSalirSinGuardar = crearBotonTexto("EXIT");

            botonBackSalida->setParent(contenedorSalida);
            botonGuardarSalir->setParent(contenedorSalida);
            botonSalirSinGuardar->setParent(contenedorSalida);

            // ACCIONES
            connect(botonBackSalida, &QPushButton::clicked, this, [this]() {
                volverAPartida();
            });

            connect(botonGuardarSalir, &QPushButton::clicked, this, [this]() {
                guardarPartidaYSalir();
            });

            connect(botonSalirSinGuardar, &QPushButton::clicked, this, [this]() {
                salirSinGuardar();
            });

            paginas->addWidget(paginaSalida);
        }

        // ---- PAGINA AYUDA
        void crearPaginaAyuda() {

            paginaAyuda = new QWidget();

            contenedorAyuda = new QWidget(paginaAyuda);

            // FONDO
            fondoAyuda = new QLabel(contenedorAyuda);
            fondoAyuda->setScaledContents(true);

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_HOW_TO_PLAY_3));
            fondoAyuda->setPixmap(imagenFondo);

            // BACK
            botonBackAyuda = crearBotonImagen(obtenerRutaBoton("back"));
            botonBackAyuda->setParent(contenedorAyuda);

            connect(botonBackAyuda, &QPushButton::clicked, this, [this]() {
                volverAPartida();
            });

            paginas->addWidget(paginaAyuda);
        }

        // ---- CONFIGURAR NIVEL
        bool configurarNivel(ModoJuego modo, int numeroNivel) {

            if (modo != ModoJuego::PROGRESIVO && modo != ModoJuego::MAPA) {
                return false;
            }

            ConfiguracionPartida nuevaConfiguracion = ConfiguracionPartida::crearNivel(numeroNivel, modo);

            if (!nuevaConfiguracion.esValida()) {
                return false;
            }

            modoActual = modo;
            numeroNivelActual = numeroNivel;

            introBossIniciada = false;
            introBossFinalizada = false;

            configuracionActual = nuevaConfiguracion;

            if (!partida.iniciar(configuracionActual)) {
                return false;
            }

            resultadoProcesado = false;

            filaBombaExplotada = -1;
            columnaBombaExplotada = -1;

            configurarFondo();
            crearTableroGrafico();
            actualizarTiempo();
            configurarMusica();
            configurarInfoNivel();

            // Primero se presenta el boss
            reproducirIntroBoss();

            return true;
        }

        // ---- FONDO
        void configurarFondo() {

            QPixmap imagenFondo(QString::fromStdString(TEMPLATE_CLEAN_1));
            fondo->setPixmap(imagenFondo);
        }

        // ---- INTRO DEL BOSS
        void reproducirIntroBoss() {

            if (introBossIniciada) {
                return;
            }

            introBossIniciada = true;
            introBossFinalizada = false;

            string rutaVideo;

            if (modoActual == ModoJuego::PERSONALIZADO) {

                BossPersonalizado boss = obtenerBossPersonalizado(
                    configuracionActual.obtenerFilas(),
                    configuracionActual.obtenerColumnas()
                );

                rutaVideo = obtenerRutaVideoBoss(boss);

            } else {

                BossNivel boss = obtenerBossNivel(numeroNivelActual);

                rutaVideo = obtenerRutaVideoBoss(boss);
            }

            reproductorBoss->stop();
            reproductorBoss->setMedia(crearUrlArchivo(rutaVideo));

            // La musica comienza desde el video del boss.
            iniciarMusica();

            paginas->setCurrentWidget(paginaBoss);

            videoBoss->setGeometry(0, 0, paginaBoss->width(), paginaBoss->height());
            videoBoss->show();
            videoBoss->raise();

            reproductorBoss->play();

            // Respaldo por si EndOfMedia no se dispara correctamente.
            QTimer::singleShot(4500, this, [this]() {

                if (!introBossFinalizada) {
                    finalizarIntroBoss();
                }
            });
        }


        void finalizarIntroBoss() {

            if (introBossFinalizada) {
                return;
            }

            introBossFinalizada = true;

            reproductorBoss->stop();

            paginas->setCurrentWidget(paginaPartida);

            // IMPORTANTE:
            // paginaPartida ya tiene aqui sus dimensiones reales dentro del QStackedWidget.
            ajustarInterfaz();

            reproductorBanner->play();

            // El timer comienza JUSTO cuando aparece el tablero
            timerPartida->start();
            actualizarTiempo();
        }

        // ---- MUSICA
        void configurarMusica() {

            playlistMusica->clear();

            string rutaMusica;

            Dificultad dificultad = configuracionActual.obtenerDificultad();

            if (dificultad == Dificultad::BASICO) {
                rutaMusica = MUSICA_THE_MINOR_DRAGS;
            } else if (dificultad == Dificultad::INTERMEDIO) {
                rutaMusica = MUSICA_IT_AINT_RIGHT;
            } else {
                rutaMusica = MUSICA_CHICKEN_FEET;
            }

            playlistMusica->addMedia(crearUrlArchivo(rutaMusica));
            playlistMusica->setPlaybackMode(QMediaPlaylist::Loop);
        }


        void iniciarMusica() {

            bool musicaActiva = true;

            if (usuarioActual != nullptr) {
                musicaActiva = usuarioActual->tieneMusicaActiva();
            }

            reproductorMusica->setVolume(musicaActiva ? 70 : 0);
            reproductorMusica->play();
        }


        void alternarMusica() {

            if (usuarioActual == nullptr) {

                if (reproductorMusica->volume() > 0) {
                    reproductorMusica->setVolume(0);
                } else {
                    reproductorMusica->setVolume(70);
                }

                return;
            }

            bool nuevaConfiguracion = !usuarioActual->tieneMusicaActiva();

            usuarioActual->establecerMusicaActiva(nuevaConfiguracion);

            reproductorMusica->setVolume(nuevaConfiguracion ? 70 : 0);

            if (sistemaUsuarios != nullptr) {
                sistemaUsuarios->guardar();
            }
        }

        // TIMER
        void actualizarTiempo() {

            int segundos = partida.obtenerTiempoRestante();

            int minutos = segundos / 60;
            int segundosRestantes = segundos % 60;

            QString texto = QString("%1:%2")
                .arg(minutos, 2, 10, QChar('0'))
                .arg(segundosRestantes, 2, 10, QChar('0'));

            labelTiempo->setText(texto);
        }

        // CREAR TABLERO GRAFICO
        void crearTableroGrafico() {

            liberarCeldasGraficas();
            escenaTablero->clear();

            const Tablero* tablero = partida.obtenerTablero();

            if (tablero == nullptr) {
                return;
            }

            filasGraficas = tablero->obtenerFilas();
            columnasGraficas = tablero->obtenerColumnas();

            calcularTamanoCelda();

            celdasGraficas = new CeldaGrafica**[filasGraficas];

            for (int fila = 0; fila < filasGraficas; fila++) {

                celdasGraficas[fila] = new CeldaGrafica*[columnasGraficas];

                for (int columna = 0; columna < columnasGraficas; columna++) {

                    CeldaGrafica* item = new CeldaGrafica(fila, columna, [this](int filaClic, int columnaClic, Qt::MouseButton boton) { procesarClicCelda(filaClic, columnaClic, boton);});

                    celdasGraficas[fila][columna] = item;

                    int x = columna * (tamanoCeldaBase + SEPARACION_CELDAS);
                    int y = fila * (tamanoCeldaBase + SEPARACION_CELDAS);

                    item->setPos(x, y);

                    escenaTablero->addItem(item);
                }
            }

            actualizarTableroGrafico();

            int anchoTablero = columnasGraficas * tamanoCeldaBase + (columnasGraficas - 1) * SEPARACION_CELDAS;
            int altoTablero = filasGraficas * tamanoCeldaBase + (filasGraficas - 1) * SEPARACION_CELDAS;

            escenaTablero->setSceneRect(0, 0, anchoTablero, altoTablero);

            ajustarInterfaz();
        }

        void calcularTamanoCelda() {

            int espacioHorizontal = (TABLERO_ANCHO - (columnasGraficas - 1) * SEPARACION_CELDAS) / columnasGraficas;
            int espacioVertical = (TABLERO_ALTO - (filasGraficas - 1) * SEPARACION_CELDAS) / filasGraficas;

            tamanoCeldaBase = qMin(TAMANO_MAXIMO_CELDA, qMin(espacioHorizontal, espacioVertical));

            if (tamanoCeldaBase < 10) {
                tamanoCeldaBase = 10;
            }
        }

        // ---- ACTUALIZAR TABLERO
        void actualizarTableroGrafico() {

            const Tablero* tablero = partida.obtenerTablero();

            if (tablero == nullptr || celdasGraficas == nullptr) {
                return;
            }

            for (int fila = 0; fila < filasGraficas; fila++) {

                for (int columna = 0; columna < columnasGraficas; columna++) {

                    const Celda* celda = tablero->obtenerCelda(fila, columna);

                    if (celda == nullptr) {
                        continue;
                    }

                    string rutaImagen;

                    if (fila == filaBombaExplotada && columna == columnaBombaExplotada) {
                        rutaImagen = obtenerRutaBombaExplotada();
                    } else {
                        rutaImagen = obtenerRutaCelda(*celda);
                    }

                    QPixmap pixmap(QString::fromStdString(rutaImagen));

                    pixmap = pixmap.scaled(
                        tamanoCeldaBase,
                        tamanoCeldaBase,
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation
                    );

                    CeldaGrafica* item = celdasGraficas[fila][columna];

                    item->setPixmap(pixmap);
                }
            }
        }

        //COMENTAR ESTE BLOQUE LUEGO
        // void imprimirTableroDebug() {

        //     const Tablero* tablero = partida.obtenerTablero();

        //     if (tablero == nullptr || !tablero->estanSembradasLasMinas()) {
        //         return;
        //     }

        //     cout << "\n========== SOLUCION DEL TABLERO ==========\n";

        //     for (int fila = 0; fila < tablero->obtenerFilas(); fila++) {

        //         for (int columna = 0; columna < tablero->obtenerColumnas(); columna++) {

        //             const Celda* celda = tablero->obtenerCelda(fila, columna);

        //             if (celda != nullptr && celda->tieneMina()) {
        //                 cout << "B ";
        //             } else {
        //                 cout << "* ";
        //             }
        //         }

        //         cout << '\n';
        //     }

        //     cout << "==========================================\n\n";
        // }

        // ---- CLIC EN CELDA
        void procesarClicCelda(int fila, int columna, Qt::MouseButton boton) {

            if (partida.estaFinalizada()) {
                return;
            }

            bool cambio = false;

            if (boton == Qt::LeftButton) {

                //COMENTAR ESTE BOOL LUEGO
                //bool minasYaSembradas = partida.obtenerTablero()->estanSembradasLasMinas();

                cambio = partida.procesarClicIzquierdo(fila, columna);

                //COMENTAR ESTE IF LUEGO
                // if (!minasYaSembradas && partida.obtenerTablero()->estanSembradasLasMinas()) {

                //     imprimirTableroDebug();
                // }

                const Celda* celda = partida.obtenerTablero()->obtenerCelda(fila, columna);

                if (cambio && partida.perdio() && celda != nullptr && celda->tieneMina()) {
                    filaBombaExplotada = fila;
                    columnaBombaExplotada = columna;
                }
            } else if (boton == Qt::RightButton) {
                cambio = partida.procesarClicDerecho(fila, columna);
            }

            if (!cambio) {
                return;
            }

            actualizarTableroGrafico();

            if (partida.estaFinalizada()) {
                verificarFinPartida();
            }
        }

        void establecerControlesPartidaHabilitados(bool habilitados) {

            botonHome->setEnabled(habilitados);
            botonRetry->setEnabled(habilitados);
            botonHelp->setEnabled(habilitados);
            botonMusic->setEnabled(habilitados);
            botonFlagNivel->setEnabled(habilitados);
        }

        // ---- RETRY
        void reiniciarPartida() {

            timerPartida->stop();

            filaBombaExplotada = -1;
            columnaBombaExplotada = -1;

            resultadoProcesado = false;

            if (!partida.iniciar(configuracionActual)) {
                return;
            }

            crearTableroGrafico();
            actualizarTiempo();

            // La musica continua desde donde iba.
            timerPartida->start();
        }

        // ---- FIN DE PARTIDA
        void verificarFinPartida() {

            if (!partida.estaFinalizada()) {
                return;
            }

            timerPartida->stop();

            if (partida.gano()) {

                actualizarTableroGrafico();

                if (!resultadoProcesado) {

                    resultadoProcesado = true;

                    if (usuarioActual != nullptr && sistemaUsuarios != nullptr) {
                        SistemaPuntajes::procesarResultado(*usuarioActual, partida, *sistemaUsuarios);
                    }
                }

                reproductorBanner->stop();
                reproductorMusica->stop();

                if (modoActual == ModoJuego::PROGRESIVO && numeroNivelActual == CANTIDAD_NIVELES) {

                    reproducirCelebracionFinal();
                    return;
                }

                paginas->setCurrentWidget(paginaVictoria);

                ajustarInterfaz();

                return;
            }


            // A partir de este momento el flujo de derrota no debe poder interrumpirse.
            establecerControlesPartidaHabilitados(false);

            // Si hubo derrota, el esqueleto revela todas las minas.
            partida.revelarMinasAlFinalizar();

            // JuegoUI solamente vuelve a dibujar el tablero.
            actualizarTableroGrafico();

            reproductorMusica->stop();

            // Dejamos visible el tablero durante 2 segundos
            // para que el usuario pueda ver todas las minas.
            QTimer::singleShot(2000, this, [this]() {
                reproducirAnimacionJulius();
            });
        }

        // ---- ANIMACION JULIUS
        void reproducirAnimacionJulius() {

            animacionJuliusFinalizada = false;

            reproductorBanner->stop();

            reproductorJulius->setMedia(crearUrlArchivo(ANIMACION_JULIUS));

            paginas->setCurrentWidget(paginaJulius);

            videoJulius->setGeometry(0, 0, paginaJulius->width(), paginaJulius->height());
            videoJulius->show();
            videoJulius->raise();

            reproductorJulius->play();

            // Julius dura 5 segundos.
            // El respaldo solo se usa si Qt no dispara EndOfMedia.
            timerRespaldoJulius->start(11000);
        }


        void finalizarAnimacionJulius() {

            if (animacionJuliusFinalizada) {
                return;
            }

            animacionJuliusFinalizada = true;

            if (timerRespaldoJulius != nullptr) {
                timerRespaldoJulius->stop();
            }

            reproductorJulius->stop();

            paginas->setCurrentWidget(paginaDerrota);

            ajustarInterfaz();
        }

        // ---- RETRY DESDE LEVEL FAILED
        void reiniciarDesdeDerrota() {

            if (timerRespaldoJulius != nullptr) {
                timerRespaldoJulius->stop();
            }

            reproductorJulius->stop();

            animacionJuliusFinalizada = true;

            filaBombaExplotada = -1;
            columnaBombaExplotada = -1;

            resultadoProcesado = false;

            if (!partida.iniciar(configuracionActual)) {
                return;
            }

            crearTableroGrafico();
            actualizarTiempo();
            configurarMusica();
            configurarInfoNivel();

            // Esta ya es una partida nueva.
            establecerControlesPartidaHabilitados(true);

            paginas->setCurrentWidget(paginaPartida);

            ajustarInterfaz();

            reproductorBanner->play();
            iniciarMusica();

            timerPartida->start();
        }

        // ---- VOLVER AL MENU PRINCIPAL
        void volverMenuPrincipal() {

            reproductorJulius->stop();
            reproductorBanner->stop();
            reproductorMusica->stop();
            timerPartida->stop();

            setProperty("destinoAlCerrar", "menu");
            close();
        }

        // ---- CONTINUAR DESPUES DE VICTORIA
        void continuarDespuesDeVictoria() {

            reproductorBanner->stop();
            reproductorMusica->stop();
            timerPartida->stop();

            // PROGRESIVO
            if (modoActual == ModoJuego::PROGRESIVO) {

                int siguienteNivel = numeroNivelActual + 1;

                if (siguienteNivel <= CANTIDAD_NIVELES) {

                    configurarNivel(
                        ModoJuego::PROGRESIVO,
                        siguienteNivel
                    );

                    return;
                }

                // No existe otro nivel despues del nivel 9.
                setProperty("destinoAlCerrar", "menu");
                close();

                return;
            }

            // MAPA
            if (modoActual == ModoJuego::MAPA) {

                setProperty(
                    "destinoAlCerrar",
                    "mapa"
                );

                close();

                return;
            }

            // PERSONALIZADO
            if (modoActual == ModoJuego::PERSONALIZADO) {

                setProperty(
                    "destinoAlCerrar",
                    "personalizado"
                );

                close();

                return;
            }
        }

        // ---- SALIR DESPUES DE VICTORIA
        void salirDespuesDeVictoria() {

            reproductorBanner->stop();
            reproductorMusica->stop();
            timerPartida->stop();

            setProperty(
                "destinoAlCerrar",
                "menu"
            );

            close();
        }

        // ---- HELP
        void mostrarAyuda() {

            if (partida.estaFinalizada()) {
                return;
            }

            // La partida queda pausada mientras el usuario consulta la ayuda.
            timerPartida->stop();

            reproductorBanner->pause();
            reproductorMusica->pause();

            paginas->setCurrentWidget(paginaAyuda);

            ajustarInterfaz();
        }

        // ---- VOLVER A LA PARTIDA
        void volverAPartida() {

            if (partida.estaFinalizada()) {
                return;
            }

            paginas->setCurrentWidget(paginaPartida);

            ajustarInterfaz();

            reproductorBanner->play();
            reproductorMusica->play();

            timerPartida->start();
        }

        // ---- GUARDAR PARTIDA Y SALIR
        void guardarPartidaYSalir() {

            if (usuarioActual == nullptr) {
                return;
            }

            timerPartida->stop();

            string nombreUsuario = usuarioActual->obtenerNombreUsuario();

            bool guardada = ArchivoPersistencia::guardarPartida(
                nombreUsuario,
                partida
            );

            if (!guardada) {

                QMessageBox::warning(
                    this,
                    "Save Game",
                    "The game could not be saved."
                );

                return;
            }

            reproductorBanner->stop();
            reproductorMusica->stop();

            setProperty("destinoAlCerrar", "menu");

            close();
        }

        // ---- SALIR SIN GUARDAR
        void salirSinGuardar() {

            if (usuarioActual == nullptr) {
                return;
            }

            timerPartida->stop();

            string nombreUsuario = usuarioActual->obtenerNombreUsuario();

            if (
                ArchivoPersistencia::existePartidaGuardada(
                    nombreUsuario,
                    modoActual
                )
            ) {

                bool eliminada =
                    ArchivoPersistencia::eliminarPartidaGuardada(
                        nombreUsuario,
                        modoActual
                    );

                if (!eliminada) {

                    QMessageBox::warning(
                        this,
                        "Exit Game",
                        "The saved game could not be removed."
                    );

                    return;
                }
            }

            reproductorBanner->stop();
            reproductorMusica->stop();

            setProperty("destinoAlCerrar", "menu");

            close();
        }

        // ---- HOME
        void mostrarOpcionesSalida() {

            if (partida.estaFinalizada()) {
                return;
            }

            // Pausamos la partida mientras el usuario decide.
            timerPartida->stop();

            reproductorBanner->pause();
            reproductorMusica->pause();

            paginas->setCurrentWidget(paginaSalida);

            ajustarInterfaz();
        }

        // ---- BOTON CON IMAGEN
        QPushButton* crearBotonImagen(const string &ruta) {
            QPushButton* boton = new QPushButton();

            boton->setCursor(Qt::PointingHandCursor);
            boton->setFlat(true);

            boton->setStyleSheet(
                "QPushButton {"
                "border: none;"
                "background: transparent;"
                "}"
            );

            boton->setIcon(QIcon(QString::fromStdString(ruta)));
            boton->setIconSize(QSize(100, 100));

            return boton;
        }

        // ---- BOTON DE TEXTO
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
            );

            return boton;
        }

        // ---- LIBERAR MATRIZ GRAFICA
        void liberarCeldasGraficas() {

            if (celdasGraficas == nullptr) {
                return;
            }

            // Los QGraphicsItems son destruidos por escenaTablero->clear()
            // Aqui solamente liberamos la matriz que contiene los punteros
            for (int fila = 0; fila < filasGraficas; fila++) {

                delete[] celdasGraficas[fila];
                celdasGraficas[fila] = nullptr;
            }

            delete[] celdasGraficas;
            celdasGraficas = nullptr;

            filasGraficas = 0;
            columnasGraficas = 0;
        }

        // ---- AJUSTAR INTERFAZ
        void ajustarInterfaz() {

            if (paginaPartida == nullptr || contenedorDiseno == nullptr) {
                return;
            }

            double escalaX = static_cast<double>(paginaPartida->width()) / ANCHO_DISENO_JUEGO;
            double escalaY = static_cast<double>(paginaPartida->height()) / ALTO_DISENO_JUEGO;

            double escalaFuente = qMin(escalaX, escalaY);


            // -------------------------------------------------------------------------
            // PAGINA PARTIDA
            // -------------------------------------------------------------------------

            // CONTENEDOR
            contenedorDiseno->setGeometry(
                0,
                0,
                paginaPartida->width(),
                paginaPartida->height()
            );

            // TEMPLATE
            fondo->setGeometry(
                0,
                0,
                contenedorDiseno->width(),
                contenedorDiseno->height()
            );

            // -------------------------------------------------------------------------
            // BANNER SUPERIOR
            // -------------------------------------------------------------------------

            videoBanner->setGeometry(
                static_cast<int>(95 * escalaX),
                static_cast<int>(145 * escalaY),
                static_cast<int>(1890 * escalaX),
                static_cast<int>(175 * escalaY)
            );

            // -------------------------------------------------------------------------
            // TIMER
            // El timer es hijo del videoBanner, por eso estas coordenadas son locales
            // al banner.
            // -------------------------------------------------------------------------

            labelTiempo->setGeometry(
                0,
                0,
                static_cast<int>(350 * escalaX),
                videoBanner->height()
            );

            labelTiempo->setContentsMargins(
                static_cast<int>(30 * escalaX),
                0,
                0,
                0
            );

            QFont fuenteTimer = labelTiempo->font();

            fuenteTimer.setPixelSize(static_cast<int>(58 * escalaFuente));

            labelTiempo->setFont(fuenteTimer);
            labelTiempo->raise();

            // -------------------------------------------------------------------------
            // TABLERO
            // -------------------------------------------------------------------------

            if (filasGraficas > 0 && columnasGraficas > 0 && !escenaTablero->sceneRect().isEmpty()) {

                int anchoTableroBase = columnasGraficas * tamanoCeldaBase + (columnasGraficas - 1) * SEPARACION_CELDAS;

                int altoTableroBase = filasGraficas * tamanoCeldaBase + (filasGraficas - 1) * SEPARACION_CELDAS;

                int xTableroBase = TABLERO_X + (TABLERO_ANCHO - anchoTableroBase) / 2;

                int yTableroBase = TABLERO_Y + (TABLERO_ALTO - altoTableroBase) / 2;

                // La POSICION sigue la escala completa del template.
                int centroTableroX = static_cast<int>(
                    (xTableroBase + anchoTableroBase / 2.0) * escalaX
                );

                int centroTableroY = static_cast<int>(
                    (yTableroBase + altoTableroBase / 2.0) * escalaY
                );

                // Las CELDAS deben continuar siendo cuadradas.
                double escalaTablero = qMin(escalaX, escalaY);

                int anchoTableroVisible = static_cast<int>(
                    anchoTableroBase * escalaTablero
                );

                int altoTableroVisible = static_cast<int>(
                    altoTableroBase * escalaTablero
                );

                vistaTablero->setGeometry(
                    centroTableroX - anchoTableroVisible / 2,
                    centroTableroY - altoTableroVisible / 2,
                    anchoTableroVisible,
                    altoTableroVisible
                );

                vistaTablero->resetTransform();

                vistaTablero->fitInView(
                    escenaTablero->sceneRect(),
                    Qt::KeepAspectRatio
                );

                vistaTablero->centerOn(
                    escenaTablero->sceneRect().center()
                );
            }

            // -------------------------------------------------------------------------
            // BOTONES INFERIORES
            // -------------------------------------------------------------------------

            int tamanoBotonBase = 110;
            int separacionBase = 25;

            int anchoGrupoBase =
                tamanoBotonBase * 4 +
                separacionBase * 3;

            int xGrupoBase = (ANCHO_DISENO_JUEGO - anchoGrupoBase) / 2;

            int yGrupoBase = 895;

            int tamanoBotonVisible = static_cast<int>(
                tamanoBotonBase * escalaFuente
            );

            int separacionVisible = static_cast<int>(
                separacionBase * escalaFuente
            );

            int centroGrupoX = static_cast<int>(
                (xGrupoBase + anchoGrupoBase / 2.0) * escalaX
            );

            int anchoGrupoVisible = tamanoBotonVisible * 4 + separacionVisible * 3;

            int xGrupoVisible = centroGrupoX - anchoGrupoVisible / 2;

            int yGrupoVisible = static_cast<int>(yGrupoBase * escalaY);


            botonHome->setGeometry(
                xGrupoVisible,
                yGrupoVisible,
                tamanoBotonVisible,
                tamanoBotonVisible
            );

            botonRetry->setGeometry(
                xGrupoVisible + tamanoBotonVisible + separacionVisible,
                yGrupoVisible,
                tamanoBotonVisible,
                tamanoBotonVisible
            );

            botonHelp->setGeometry(
                xGrupoVisible + (tamanoBotonVisible + separacionVisible) * 2,
                yGrupoVisible,
                tamanoBotonVisible,
                tamanoBotonVisible
            );

            botonMusic->setGeometry(
                xGrupoVisible + (tamanoBotonVisible + separacionVisible) * 3,
                yGrupoVisible,
                tamanoBotonVisible,
                tamanoBotonVisible
            );

            QSize iconoEscalado(
                static_cast<int>(100 * escalaFuente),
                static_cast<int>(100 * escalaFuente)
            );

            botonHome->setIconSize(iconoEscalado);
            botonRetry->setIconSize(iconoEscalado);
            botonHelp->setIconSize(iconoEscalado);
            botonMusic->setIconSize(iconoEscalado);

            fondo->lower();

            videoBanner->raise();
            vistaTablero->raise();

            botonHome->raise();
            botonRetry->raise();
            botonHelp->raise();
            botonMusic->raise();

            if (bannerNivelVisible) {
                bannerNivel->raise();
            }

            botonFlagNivel->raise();

            // -------------------------------------------------------------------------
            // FLAG DE NIVEL
            // -------------------------------------------------------------------------

            botonFlagNivel->setGeometry(
                static_cast<int>(1535 * escalaX),
                static_cast<int>(890 * escalaY),
                static_cast<int>(125 * escalaFuente),
                static_cast<int>(125 * escalaFuente)
            );

            botonFlagNivel->setIconSize(
                QSize(
                    static_cast<int>(120 * escalaFuente),
                    static_cast<int>(120 * escalaFuente)
                )
            );


            // -------------------------------------------------------------------------
            // BANNER INFORMATIVO DEL NIVEL
            // -------------------------------------------------------------------------

            bannerNivel->setGeometry(
                static_cast<int>(300 * escalaX),
                static_cast<int>(355 * escalaY),
                static_cast<int>(1480 * escalaX),
                static_cast<int>(455 * escalaY)
            );

            // -------------------------------------------------------------------------
            // PAGINA LEVEL FAILED
            // -------------------------------------------------------------------------

            if (paginaDerrota != nullptr && contenedorDerrota != nullptr) {

                double escalaDerrotaX = static_cast<double>(paginaDerrota->width()) / ANCHO_DISENO_JUEGO;

                double escalaDerrotaY = static_cast<double>(paginaDerrota->height()) / ALTO_DISENO_JUEGO;

                double escalaDerrotaFuente = qMin(escalaDerrotaX, escalaDerrotaY);

                contenedorDerrota->setGeometry(
                    0,
                    0,
                    paginaDerrota->width(),
                    paginaDerrota->height()
                );

                fondoDerrota->setGeometry(
                    0,
                    0,
                    contenedorDerrota->width(),
                    contenedorDerrota->height()
                );

                botonRetryDerrota->setGeometry(
                    static_cast<int>(790 * escalaDerrotaX),
                    static_cast<int>(710 * escalaDerrotaY),
                    static_cast<int>(500 * escalaDerrotaX),
                    static_cast<int>(90 * escalaDerrotaY)
                );

                botonQuitDerrota->setGeometry(
                    static_cast<int>(740 * escalaDerrotaX),
                    static_cast<int>(825 * escalaDerrotaY),
                    static_cast<int>(600 * escalaDerrotaX),
                    static_cast<int>(90 * escalaDerrotaY)
                );

                QFont fuenteDerrota;

                if (!nombreFuenteAlice.isEmpty()) {
                    fuenteDerrota.setFamily(nombreFuenteAlice);
                }

                fuenteDerrota.setBold(true);

                fuenteDerrota.setPixelSize(
                    static_cast<int>(52 * escalaDerrotaFuente)
                );

                botonRetryDerrota->setFont(fuenteDerrota);
                botonQuitDerrota->setFont(fuenteDerrota);

                fondoDerrota->lower();

                botonRetryDerrota->raise();
                botonQuitDerrota->raise();
            }

            // -------------------------------------------------------------------------
            // PAGINA OPCIONES DE SALIDA
            // -------------------------------------------------------------------------

            if (paginaSalida != nullptr && contenedorSalida != nullptr) {

                double escalaSalidaX = static_cast<double>(paginaSalida->width()) / ANCHO_DISENO_JUEGO;

                double escalaSalidaY = static_cast<double>(paginaSalida->height()) / ALTO_DISENO_JUEGO;

                double escalaSalidaFuente = qMin(escalaSalidaX, escalaSalidaY);

                // CONTENEDOR
                contenedorSalida->setGeometry(
                    0,
                    0,
                    paginaSalida->width(),
                    paginaSalida->height()
                );

                // TEMPLATE
                fondoSalida->setGeometry(
                    0,
                    0,
                    contenedorSalida->width(),
                    contenedorSalida->height()
                );

                // TEXTO FELIX
                textoSalida->setGeometry(
                    static_cast<int>(250 * escalaSalidaX),
                    static_cast<int>(500 * escalaSalidaY),
                    static_cast<int>(1580 * escalaSalidaX),
                    static_cast<int>(110 * escalaSalidaY)
                );

                QFont fuenteSalida;

                if (!nombreFuenteLobster.isEmpty()) {
                    fuenteSalida.setFamily(nombreFuenteLobster);
                }

                fuenteSalida.setPixelSize(
                    static_cast<int>(58 * escalaSalidaFuente)
                );

                textoSalida->setFont(fuenteSalida);

                // BACK
                botonBackSalida->setGeometry(
                    static_cast<int>(120 * escalaSalidaX),
                    static_cast<int>(115 * escalaSalidaY),
                    static_cast<int>(145 * escalaSalidaX),
                    static_cast<int>(145 * escalaSalidaY)
                );

                botonBackSalida->setIconSize(
                    QSize(
                        static_cast<int>(135 * escalaSalidaFuente),
                        static_cast<int>(135 * escalaSalidaFuente)
                    )
                );

                // SAVE GAME AND EXIT
                botonGuardarSalir->setGeometry(
                    static_cast<int>(610 * escalaSalidaX),
                    static_cast<int>(700 * escalaSalidaY),
                    static_cast<int>(860 * escalaSalidaX),
                    static_cast<int>(90 * escalaSalidaY)
                );

                // EXIT
                botonSalirSinGuardar->setGeometry(
                    static_cast<int>(790 * escalaSalidaX),
                    static_cast<int>(815 * escalaSalidaY),
                    static_cast<int>(500 * escalaSalidaX),
                    static_cast<int>(90 * escalaSalidaY)
                );

                QFont fuenteBotonesSalida;

                if (!nombreFuenteAlice.isEmpty()) {
                    fuenteBotonesSalida.setFamily(nombreFuenteAlice);
                }

                fuenteBotonesSalida.setBold(true);

                fuenteBotonesSalida.setPixelSize(
                    static_cast<int>(48 * escalaSalidaFuente)
                );

                botonGuardarSalir->setFont(fuenteBotonesSalida);
                botonSalirSinGuardar->setFont(fuenteBotonesSalida);

                fondoSalida->lower();

                textoSalida->raise();

                botonBackSalida->raise();
                botonGuardarSalir->raise();
                botonSalirSinGuardar->raise();
            }

            // -------------------------------------------------------------------------
            // PAGINA AYUDA
            // -------------------------------------------------------------------------

            if (paginaAyuda != nullptr && contenedorAyuda != nullptr) {

                double escalaAyudaX = static_cast<double>(paginaAyuda->width()) / ANCHO_DISENO_JUEGO;

                double escalaAyudaY = static_cast<double>(paginaAyuda->height()) / ALTO_DISENO_JUEGO;

                double escalaAyuda = qMin(escalaAyudaX, escalaAyudaY);

                // CONTENEDOR
                contenedorAyuda->setGeometry(
                    0,
                    0,
                    paginaAyuda->width(),
                    paginaAyuda->height()
                );

                // TEMPLATE
                fondoAyuda->setGeometry(
                    0,
                    0,
                    contenedorAyuda->width(),
                    contenedorAyuda->height()
                );

                // BACK
                botonBackAyuda->setGeometry(
                    static_cast<int>(95 * escalaAyudaX),
                    static_cast<int>(95 * escalaAyudaY),
                    static_cast<int>(145 * escalaAyudaX),
                    static_cast<int>(145 * escalaAyudaY)
                );

                botonBackAyuda->setIconSize(
                    QSize(
                        static_cast<int>(135 * escalaAyuda),
                        static_cast<int>(135 * escalaAyuda)
                    )
                );

                fondoAyuda->lower();
                botonBackAyuda->raise();
            }

            // -------------------------------------------------------------------------
            // PAGINA LEVEL WON
            // -------------------------------------------------------------------------

            if (paginaVictoria != nullptr && contenedorVictoria != nullptr) {

                double escalaVictoriaX =
                    static_cast<double>(paginaVictoria->width()) /
                    ANCHO_DISENO_JUEGO;

                double escalaVictoriaY =
                    static_cast<double>(paginaVictoria->height()) /
                    ALTO_DISENO_JUEGO;

                double escalaVictoriaFuente =
                    qMin(
                        escalaVictoriaX,
                        escalaVictoriaY
                    );


                // CONTENEDOR
                contenedorVictoria->setGeometry(
                    0,
                    0,
                    paginaVictoria->width(),
                    paginaVictoria->height()
                );


                // TEMPLATE COMPLETO 2080 x 1100
                fondoVictoria->setGeometry(
                    0,
                    0,
                    contenedorVictoria->width(),
                    contenedorVictoria->height()
                );


                // NEXT QUEST
                botonNextQuestVictoria->setGeometry(
                    static_cast<int>(690 * escalaVictoriaX),
                    static_cast<int>(710 * escalaVictoriaY),
                    static_cast<int>(700 * escalaVictoriaX),
                    static_cast<int>(90 * escalaVictoriaY)
                );


                // QUIT GAME
                botonQuitVictoria->setGeometry(
                    static_cast<int>(740 * escalaVictoriaX),
                    static_cast<int>(825 * escalaVictoriaY),
                    static_cast<int>(600 * escalaVictoriaX),
                    static_cast<int>(90 * escalaVictoriaY)
                );


                QFont fuenteVictoria;

                if (!nombreFuenteAlice.isEmpty()) {
                    fuenteVictoria.setFamily(nombreFuenteAlice);
                }

                fuenteVictoria.setBold(true);

                fuenteVictoria.setPixelSize(
                    static_cast<int>(
                        52 * escalaVictoriaFuente
                    )
                );

                botonNextQuestVictoria->setFont(
                    fuenteVictoria
                );

                botonQuitVictoria->setFont(
                    fuenteVictoria
                );


                fondoVictoria->lower();

                botonNextQuestVictoria->raise();
                botonQuitVictoria->raise();
            }

            // -------------------------------------------------------------------------
            // PAGINA MISION CUMPLIDA
            // -------------------------------------------------------------------------

            if (paginaMisionCumplida != nullptr && contenedorMisionCumplida != nullptr) {

                double escalaFinalX =
                    static_cast<double>(
                        paginaMisionCumplida->width()
                    ) / ANCHO_DISENO_JUEGO;

                double escalaFinalY =
                    static_cast<double>(
                        paginaMisionCumplida->height()
                    ) / ALTO_DISENO_JUEGO;

                double escalaFinal =
                    qMin(
                        escalaFinalX,
                        escalaFinalY
                    );


                // CONTENEDOR
                contenedorMisionCumplida->setGeometry(
                    0,
                    0,
                    paginaMisionCumplida->width(),
                    paginaMisionCumplida->height()
                );


                // TEMPLATE COMPLETO
                fondoMisionCumplida->setGeometry(
                    0,
                    0,
                    contenedorMisionCumplida->width(),
                    contenedorMisionCumplida->height()
                );


                // GO TO CUSTOM MAYHEM
                botonCustomMayhemFinal->setGeometry(
                    static_cast<int>(1130 * escalaFinalX),
                    static_cast<int>(635 * escalaFinalY),
                    static_cast<int>(470 * escalaFinalX),
                    static_cast<int>(120 * escalaFinalY)
                );

                botonCustomMayhemFinal->setIconSize(
                    QSize(
                        static_cast<int>(460 * escalaFinal),
                        static_cast<int>(110 * escalaFinal)
                    )
                );


                // CLAIM REWARD
                botonClaimRewardFinal->setGeometry(
                    static_cast<int>(315 * escalaFinalX),
                    static_cast<int>(880 * escalaFinalY),
                    static_cast<int>(385 * escalaFinalX),
                    static_cast<int>(125 * escalaFinalY)
                );

                botonClaimRewardFinal->setIconSize(
                    QSize(
                        static_cast<int>(375 * escalaFinal),
                        static_cast<int>(115 * escalaFinal)
                    )
                );


                // BUBBLE
                bubbleFinal->setGeometry(
                    static_cast<int>(515 * escalaFinalX),
                    static_cast<int>(85 * escalaFinalY),
                    static_cast<int>(820 * escalaFinalX),
                    static_cast<int>(245 * escalaFinalY)
                );


                fondoMisionCumplida->lower();

                botonCustomMayhemFinal->raise();
                botonClaimRewardFinal->raise();

                if (bubbleFinal->isVisible()) {
                    bubbleFinal->raise();
                }
            }

        }

        // ---- URL PARA ARCHIVO LOCAL
        QUrl crearUrlArchivo(const string &ruta) const {

            QFileInfo archivo(QString::fromStdString(ruta));

            return QUrl::fromLocalFile(archivo.absoluteFilePath());
        }

    protected:

        // ---- RESIZE
        void resizeEvent(QResizeEvent* evento) override {

            QWidget::resizeEvent(evento);

            if (paginas != nullptr) {

                paginas->setGeometry(
                    0,
                    0,
                    width(),
                    height()
                );
            }

            if (paginaBoss != nullptr && videoBoss != nullptr) {

                videoBoss->setGeometry(
                    0,
                    0,
                    paginaBoss->width(),
                    paginaBoss->height()
                );
            }

            if (paginaJulius != nullptr && videoJulius != nullptr) {

                videoJulius->setGeometry(
                    0,
                    0,
                    paginaJulius->width(),
                    paginaJulius->height()
                );
            }

            if (paginaCelebracionFinal != nullptr && videoCelebracionFinal != nullptr) {

                videoCelebracionFinal->setGeometry(
                    0,
                    0,
                    paginaCelebracionFinal->width(),
                    paginaCelebracionFinal->height()
                );
            }

            ajustarInterfaz();
        }

        void closeEvent(QCloseEvent* evento) override {

            setProperty("geometriaAlCerrar", geometry());
            setProperty("estabaMaximizada", isMaximized());
            setProperty("estabaFullScreen", isFullScreen());

            QWidget::closeEvent(evento);
        }
};


// -----------------------------------------------------------------------------
// FUNCION PARA CREAR UN JUEGO
// Luego MenusUI.cpp podra llamar esta funcion.
// -----------------------------------------------------------------------------

QWidget* crearJuegoUI(Usuario* usuarioActual, SistemaUsuarios* sistemaUsuarios, ModoJuego modo, int numeroNivel) {

    JuegoUI* juego = new JuegoUI(
        usuarioActual,
        sistemaUsuarios,
        modo,
        numeroNivel
    );

    return juego;
}

QWidget* crearJuegoUICargado(Usuario* usuarioActual, SistemaUsuarios* sistemaUsuarios, ModoJuego modo) {

    JuegoUI* juego = new JuegoUI(
        usuarioActual,
        sistemaUsuarios,
        modo,
        0,
        false
    );

    if (!juego->cargarPartidaGuardadaDesdeDisco()) {
        delete juego;
        return nullptr;
    }

    return juego;
}

QWidget* crearJuegoUIPersonalizado(
    Usuario* usuarioActual,
    SistemaUsuarios* sistemaUsuarios,
    int filas,
    int columnas
) {

    JuegoUI* juego = new JuegoUI(
        usuarioActual,
        sistemaUsuarios,
        ModoJuego::PERSONALIZADO,
        0,
        false
    );

    if (!juego->configurarPartidaPersonalizada(filas, columnas)) {
        delete juego;
        return nullptr;
    }

    return juego;
}
