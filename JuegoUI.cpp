#include "Partida.h"
#include "Usuarios.h"
#include "Assets.h"

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
const int TABLERO_Y = 345;
const int TABLERO_ANCHO = 1890;
const int TABLERO_ALTO = 535;


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

        // STACK DE JUEGO
        QStackedWidget* paginas;

        QWidget* paginaBoss;
        QWidget* paginaPartida;

        QWidget* paginaJulius;
        QWidget* paginaDerrota;

        // INTRO DEL BOSS
        QVideoWidget* videoBoss;
        QMediaPlayer* reproductorBoss;

        bool introBossFinalizada;
        bool introBossIniciada;

        // ANIMACION DE DERROTA
        QVideoWidget* videoJulius;
        QMediaPlayer* reproductorJulius;
        bool animacionJuliusFinalizada;

        // PANTALLA LEVEL FAILED
        QWidget* contenedorDerrota;
        QLabel* fondoDerrota;

        QPushButton* botonRetryDerrota;
        QPushButton* botonQuitDerrota;

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

        // MUSICA
        QMediaPlayer* reproductorMusica;
        QMediaPlaylist* playlistMusica;

        // OTROS
        bool resultadoProcesado;


    public:
        JuegoUI(Usuario* usuarioActual, SistemaUsuarios* sistemaUsuarios, ModoJuego modo, int numeroNivel, QWidget* parent = nullptr)
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
                videoJulius(nullptr),
                reproductorJulius(nullptr),
                animacionJuliusFinalizada(false),
                contenedorDerrota(nullptr),
                fondoDerrota(nullptr),
                botonRetryDerrota(nullptr),
                botonQuitDerrota(nullptr),
                videoBoss(nullptr),
                reproductorBoss(nullptr),
                introBossFinalizada(false),
                introBossIniciada(false),
                contenedorDiseno(nullptr),
                fondo(nullptr),
                nombreFuenteAlice(""),
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
                reproductorMusica(nullptr),
                playlistMusica(nullptr),
                resultadoProcesado(false) {

            setWindowTitle("Minefield Mayhem - The Oppenheimer Incident");

            resize(ANCHO_VENTANA_INICIAL, ALTO_VENTANA_INICIAL);
            setMinimumSize(ANCHO_MINIMO_JUEGO, ALTO_MINIMO_JUEGO);

            crearInterfaz();
            configurarNivel(modo, numeroNivel);
        }

        ~JuegoUI() {
            liberarCeldasGraficas();
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

            paginas = new QStackedWidget(this);

            crearPaginaBoss();
            crearPaginaPartida();
            crearPaginaJulius();
            crearPaginaDerrota();

            timerPartida = new QTimer(this);
            timerPartida->setInterval(1000);

            connect(timerPartida, &QTimer::timeout, this, [this]() {

                partida.actualizarCronometro();
                actualizarTiempo();

                if (partida.estaFinalizada()) {
                    verificarFinPartida();
                }
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
            reproductorBanner = new QMediaPlayer(this);
            playlistBanner = new QMediaPlaylist(this);

            playlistBanner->addMedia(crearUrlArchivo(ANIMACION_TO_THE_RESCUE));
            playlistBanner->setPlaybackMode(QMediaPlaylist::Loop);

            reproductorBanner->setPlaylist(playlistBanner);
            reproductorBanner->setVideoOutput(videoBanner);
            reproductorBanner->setVolume(0);

            // TIMER VISUAL
            labelTiempo = new QLabel(videoBanner);
            labelTiempo->setAlignment(Qt::AlignCenter);

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

            // MUSICA
            reproductorMusica = new QMediaPlayer(this);

            playlistMusica = new QMediaPlaylist(this);
            playlistMusica->setPlaybackMode(QMediaPlaylist::Loop);

            reproductorMusica->setPlaylist(playlistMusica);

            paginas->addWidget(paginaPartida);
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

            BossNivel boss = obtenerBossNivel(numeroNivelActual);
            string rutaVideo = obtenerRutaVideoBoss(boss);

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

        // ---- CLIC EN CELDA
        void procesarClicCelda(int fila, int columna, Qt::MouseButton boton) {

            if (partida.estaFinalizada()) {
                return;
            }

            bool cambio = false;

            if (boton == Qt::LeftButton) {

                cambio = partida.procesarClicIzquierdo(fila, columna);

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

                // TEMPORAL:
                // Despues reemplazaremos esto con la pantalla LEVEL WON.
                QMessageBox::information(
                    this,
                    "Level Complete",
                    "Level completed successfully."
                );

                return;
            }


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
            QTimer::singleShot(11000, this, [this]() {
                finalizarAnimacionJulius();
            });
        }


        void finalizarAnimacionJulius() {

            if (animacionJuliusFinalizada) {
                return;
            }

            animacionJuliusFinalizada = true;

            reproductorJulius->stop();

            paginas->setCurrentWidget(paginaDerrota);

            ajustarInterfaz();
        }

        // ---- RETRY DESDE LEVEL FAILED
        void reiniciarDesdeDerrota() {

            animacionJuliusFinalizada = false;

            filaBombaExplotada = -1;
            columnaBombaExplotada = -1;

            resultadoProcesado = false;

            if (!partida.iniciar(configuracionActual)) {
                return;
            }

            crearTableroGrafico();
            actualizarTiempo();
            configurarMusica();

            paginas->setCurrentWidget(paginaPartida);

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

            // TEMPORAL:
            // Cuando conectemos MenusUI.cpp, este punto enviara al usuario
            // al template del menu principal.
            close();
        }

        // ---- HELP
        void mostrarAyuda() {

            // TEMPORAL:
            // Luego se sustituye por el template How To Play.
            QMessageBox::information(
                this,
                "How To Play",
                "The Help template will be integrated here."
            );
        }

        // ---- HOME
        void mostrarOpcionesSalida() {

            // TEMPORAL: Luego se sustituye por la pantalla que permitira: guardar, abandonar o cancelar.
            QMessageBox::information(
                this,
                "Game Options",
                "The Save / Leave template will be integrated here."
            );
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

        // ---- ESCALAR RECTANGULO
        QRect rectanguloEscalado(int x, int y, int ancho, int alto, double escala) const {
            return QRect(
                static_cast<int>(x * escala),
                static_cast<int>(y * escala),
                static_cast<int>(ancho * escala),
                static_cast<int>(alto * escala)
            );
        }

        // ---- AJUSTAR INTERFAZ
        void ajustarInterfaz() {

            if (contenedorDiseno == nullptr) {
                return;
            }

            double escalaHorizontal = static_cast<double>(paginaPartida->width()) / ANCHO_DISENO_JUEGO;
            double escalaVertical = static_cast<double>(paginaPartida->height()) / ALTO_DISENO_JUEGO;

            // Escalado uniforme para conservar la proporcion del mockup.
            double escala = qMin(escalaHorizontal, escalaVertical);

            int anchoContenedor = static_cast<int>(ANCHO_DISENO_JUEGO * escala);
            int altoContenedor = static_cast<int>(ALTO_DISENO_JUEGO * escala);

            int xContenedor = (paginaPartida->width() - anchoContenedor) / 2;
            int yContenedor = (paginaPartida->height() - altoContenedor) / 2;

            contenedorDiseno->setGeometry(xContenedor, yContenedor, anchoContenedor, altoContenedor);

            // ---- FONDO
            fondo->setGeometry(0, 0, anchoContenedor, altoContenedor);

            // ---- BANNER SUPERIOR
            videoBanner->setGeometry(rectanguloEscalado(110, 145, 1860, 170, escala));

            // ---- TIMER
            labelTiempo->setGeometry(static_cast<int>(15 * escala), static_cast<int>(30 * escala), static_cast<int>(280 * escala), static_cast<int>(100 * escala));

            QFont fuente = labelTiempo->font();

            fuente.setPixelSize( static_cast<int>(58 * escala ));

            labelTiempo->setFont(fuente);
            labelTiempo->raise();

            // ---- TABLERO
            int anchoTableroBase = columnasGraficas * tamanoCeldaBase + (columnasGraficas - 1) * SEPARACION_CELDAS;
            int altoTableroBase = filasGraficas * tamanoCeldaBase + (filasGraficas - 1) * SEPARACION_CELDAS;

            int xTableroBase = TABLERO_X + (TABLERO_ANCHO - anchoTableroBase) / 2;
            int yTableroBase = TABLERO_Y + (TABLERO_ALTO - altoTableroBase) / 2;

            vistaTablero->setGeometry(
                rectanguloEscalado(
                    xTableroBase,
                    yTableroBase,
                    anchoTableroBase,
                    altoTableroBase,
                    escala
                )
            );

            vistaTablero->fitInView(escenaTablero->sceneRect(), Qt::KeepAspectRatio);

            // ---- BOTONES
            int tamanoBoton = 110;
            int separacion = 25;

            int anchoGrupo = tamanoBoton * 4 + separacion * 3;
            int xBotones = (ANCHO_DISENO_JUEGO - anchoGrupo) / 2;
            int yBotones = 895;

            botonHome->setGeometry(
                rectanguloEscalado(
                    xBotones,
                    yBotones,
                    tamanoBoton,
                    tamanoBoton,
                    escala
                )
            );

            botonRetry->setGeometry(
                rectanguloEscalado(
                    xBotones + tamanoBoton + separacion,
                    yBotones,
                    tamanoBoton,
                    tamanoBoton,
                    escala
                )
            );

            botonHelp->setGeometry(
                rectanguloEscalado(
                    xBotones + (tamanoBoton + separacion) * 2,
                    yBotones,
                    tamanoBoton,
                    tamanoBoton,
                    escala
                )
            );

            botonMusic->setGeometry(
                rectanguloEscalado(
                    xBotones + (tamanoBoton + separacion) * 3,
                    yBotones,
                    tamanoBoton,
                    tamanoBoton,
                    escala
                )
            );

            QSize iconoEscalado(
                static_cast<int>(100 * escala),
                static_cast<int>(100 * escala)
            );

            botonHome->setIconSize(iconoEscalado);
            botonRetry->setIconSize(iconoEscalado);
            botonHelp->setIconSize(iconoEscalado);
            botonMusic->setIconSize(iconoEscalado);


            // ---- PANTALLA LEVEL FAILED
            if (paginaDerrota != nullptr && contenedorDerrota != nullptr) {

                double escalaDerrotaHorizontal = static_cast<double>(paginaDerrota->width()) / ANCHO_DISENO_JUEGO;
                double escalaDerrotaVertical = static_cast<double>(paginaDerrota->height()) / ALTO_DISENO_JUEGO;

                double escalaDerrota = qMin(escalaDerrotaHorizontal, escalaDerrotaVertical);

                int anchoDerrota = static_cast<int>(ANCHO_DISENO_JUEGO * escalaDerrota);
                int altoDerrota = static_cast<int>(ALTO_DISENO_JUEGO * escalaDerrota);

                int xDerrota = (paginaDerrota->width() - anchoDerrota) / 2;
                int yDerrota = (paginaDerrota->height() - altoDerrota) / 2;

                contenedorDerrota->setGeometry(xDerrota, yDerrota, anchoDerrota, altoDerrota);

                fondoDerrota->setGeometry(0, 0, anchoDerrota, altoDerrota);

                botonRetryDerrota->setGeometry(
                    static_cast<int>(790 * escalaDerrota),
                    static_cast<int>(710 * escalaDerrota),
                    static_cast<int>(500 * escalaDerrota),
                    static_cast<int>(90 * escalaDerrota)
                );

                botonQuitDerrota->setGeometry(
                    static_cast<int>(740 * escalaDerrota),
                    static_cast<int>(825 * escalaDerrota),
                    static_cast<int>(600 * escalaDerrota),
                    static_cast<int>(90 * escalaDerrota)
                );

                QFont fuenteDerrota;

                if (!nombreFuenteAlice.isEmpty()) {
                    fuenteDerrota.setFamily(nombreFuenteAlice);
                }

                fuenteDerrota.setBold(true);
                fuenteDerrota.setPixelSize(static_cast<int>(52 * escalaDerrota));

                botonRetryDerrota->setFont(fuenteDerrota);
                botonQuitDerrota->setFont(fuenteDerrota);
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
                paginas->setGeometry(rect());
            }

            if (paginaBoss != nullptr && videoBoss != nullptr) {
                videoBoss->setGeometry(0, 0, paginaBoss->width(), paginaBoss->height());
            }

            if (paginaJulius != nullptr && videoJulius != nullptr) {
                videoJulius->setGeometry(0, 0, paginaJulius->width(), paginaJulius->height());
            }

            ajustarInterfaz();
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