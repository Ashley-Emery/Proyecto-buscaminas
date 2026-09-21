#include "Assets.h"

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QFont>
#include <QFontDatabase>

#include <string>

using namespace std;


// -----------------------------------------------------------------------------
// FUNCIONES DEL CODIGO ESQUELETO INVESTIGATIVO
// -----------------------------------------------------------------------------

void generarMinasInvestigativas(bool minas[5][5]);
int contarMinasVecinasInvestigativas(const bool minas[5][5], int fila, int columna);
double calcularDistanciaPromedioInvestigativa(const bool minas[5][5]);
string clasificarNivelInvestigativo(double promedio);
void mostrarGridInvestigativo(const bool minas[5][5]);


// -----------------------------------------------------------------------------
// CONSTANTES DE LA INTERFAZ
// -----------------------------------------------------------------------------

const int FILAS_INVESTIGATIVO_UI = 5;
const int COLUMNAS_INVESTIGATIVO_UI = 5;

const int ANCHO_VENTANA_INVESTIGATIVO = 650;
const int ALTO_VENTANA_INVESTIGATIVO = 650;

const int TAMANO_CELDA_INVESTIGATIVO = 70;
const int SEPARACION_CELDA_INVESTIGATIVO = 3;

const int TABLERO_X_INVESTIGATIVO = 143;
const int TABLERO_Y_INVESTIGATIVO = 150;


// -----------------------------------------------------------------------------
// INTERFAZ DEL MODULO INVESTIGATIVO
// -----------------------------------------------------------------------------

class InvestigativoUI : public QWidget {

    private:

        bool minas[FILAS_INVESTIGATIVO_UI][COLUMNAS_INVESTIGATIVO_UI];
        bool descubiertas[FILAS_INVESTIGATIVO_UI][COLUMNAS_INVESTIGATIVO_UI];

        QPushButton* botonesCeldas[FILAS_INVESTIGATIVO_UI][COLUMNAS_INVESTIGATIVO_UI];

        QLabel* textoNivel;
        QLabel* textoPromedio;
        QLabel* textoMinas;

        QPushButton* botonHome;
        QPushButton* botonRetry;

        double promedioDistancias;

        bool tableroBloqueado;

        QString nombreFuenteAlice;


    public:

        InvestigativoUI(QWidget* parent = nullptr) : QWidget(parent), textoNivel(nullptr), textoPromedio(nullptr), textoMinas(nullptr), botonHome(nullptr), botonRetry(nullptr), promedioDistancias(0.0), tableroBloqueado(false) {

            setWindowTitle("Minefield Mayhem - Investigative Module");

            setFixedSize(ANCHO_VENTANA_INVESTIGATIVO, ALTO_VENTANA_INVESTIGATIVO);

            setStyleSheet("QWidget {" "background-color: #cfbfaf;" "}");

            inicializarInterfaz();
            cargarFuente();
            crearInterfaz();

            generarNuevoExperimento();
        }


    private:

        // ---------------------------------------------------------------------
        // INICIALIZAR MATRICES DE LA UI
        // ---------------------------------------------------------------------

        void inicializarInterfaz() {

            for (int fila = 0; fila < FILAS_INVESTIGATIVO_UI; fila++) {
                for (int columna = 0; columna < COLUMNAS_INVESTIGATIVO_UI; columna++) {

                    descubiertas[fila][columna] = false;
                    botonesCeldas[fila][columna] = nullptr;
                }
            }
        }


        // ---------------------------------------------------------------------
        // CARGAR FUENTE
        // ---------------------------------------------------------------------

        void cargarFuente() {

            string rutaFuente = obtenerRutaFuente("Alice");

            int idFuente = QFontDatabase::addApplicationFont(QString::fromStdString(rutaFuente));

            if (idFuente == -1) {
                return;
            }

            QStringList familias = QFontDatabase::applicationFontFamilies(idFuente);

            if (!familias.isEmpty()) {
                nombreFuenteAlice = familias.first();
            }
        }


        // ---------------------------------------------------------------------
        // CREAR INTERFAZ
        // ---------------------------------------------------------------------

        void crearInterfaz() {

            crearTextos();
            crearTablero();
            crearBotones();
        }


        // ---------------------------------------------------------------------
        // TEXTOS
        // ---------------------------------------------------------------------

        void crearTextos() {

            textoNivel = new QLabel(this);
            textoPromedio = new QLabel(this);
            textoMinas = new QLabel(this);

            textoNivel->setGeometry(65, 35, 300, 45);
            textoPromedio->setGeometry(65, 77, 300, 40);
            textoMinas->setGeometry(485, 35, 120, 45);

            QFont fuente;

            if (!nombreFuenteAlice.isEmpty()) {
                fuente.setFamily(nombreFuenteAlice);
            }

            fuente.setPixelSize(30);

            textoNivel->setFont(fuente);
            textoMinas->setFont(fuente);

            fuente.setPixelSize(26);

            textoPromedio->setFont(fuente);

            QString estiloTexto = "QLabel {" "background: transparent;" "color: black;" "}";

            textoNivel->setStyleSheet(estiloTexto);
            textoPromedio->setStyleSheet(estiloTexto);
            textoMinas->setStyleSheet(estiloTexto);

            textoMinas->setText("5 Minas");
        }


        // ---------------------------------------------------------------------
        // CREAR TABLERO 5 X 5
        // ---------------------------------------------------------------------

        void crearTablero() {

            for (int fila = 0; fila < FILAS_INVESTIGATIVO_UI; fila++) {
                for (int columna = 0; columna < COLUMNAS_INVESTIGATIVO_UI; columna++) {

                    QPushButton* boton = new QPushButton(this);

                    botonesCeldas[fila][columna] = boton;

                    int x = TABLERO_X_INVESTIGATIVO + columna * (TAMANO_CELDA_INVESTIGATIVO + SEPARACION_CELDA_INVESTIGATIVO);
                    int y = TABLERO_Y_INVESTIGATIVO + fila * (TAMANO_CELDA_INVESTIGATIVO + SEPARACION_CELDA_INVESTIGATIVO);

                    boton->setGeometry(x, y, TAMANO_CELDA_INVESTIGATIVO, TAMANO_CELDA_INVESTIGATIVO);

                    boton->setFlat(true);
                    boton->setFocusPolicy(Qt::NoFocus);
                    boton->setCursor(Qt::PointingHandCursor);

                    boton->setStyleSheet("QPushButton {" "border: none;" "background: transparent;" "}");

                    boton->setIconSize(QSize(TAMANO_CELDA_INVESTIGATIVO, TAMANO_CELDA_INVESTIGATIVO));

                    connect(boton, &QPushButton::clicked, this, [this, fila, columna]() {
                        procesarClicCelda(fila, columna);
                    });
                }
            }
        }


        // ---------------------------------------------------------------------
        // BOTONES HOME Y RETRY
        // ---------------------------------------------------------------------

        void crearBotones() {

            botonHome = new QPushButton(this);
            botonRetry = new QPushButton(this);

            botonHome->setGeometry(240, 550, 75, 75);
            botonRetry->setGeometry(340, 550, 75, 75);

            configurarBotonImagen(botonHome, obtenerRutaBoton("home"));
            configurarBotonImagen(botonRetry, obtenerRutaBoton("retry"));

            connect(botonHome, &QPushButton::clicked, this, [this]() {
                close();
            });

            connect(botonRetry, &QPushButton::clicked, this, [this]() {
                generarNuevoExperimento();
            });
        }


        void configurarBotonImagen(QPushButton* boton, const string& ruta) {

            boton->setFlat(true);
            boton->setFocusPolicy(Qt::NoFocus);
            boton->setCursor(Qt::PointingHandCursor);
            boton->setStyleSheet("QPushButton {" "border: none;" "background: transparent;" "}");
            boton->setIcon(QIcon(QString::fromStdString(ruta)));
            boton->setIconSize(boton->size());
        }


        // ---------------------------------------------------------------------
        // GENERAR NUEVO EXPERIMENTO
        // ---------------------------------------------------------------------

        void generarNuevoExperimento() {

            tableroBloqueado = false;

            for (int fila = 0; fila < FILAS_INVESTIGATIVO_UI; fila++) {
                for (int columna = 0; columna < COLUMNAS_INVESTIGATIVO_UI; columna++) {
                    descubiertas[fila][columna] = false;
                }
            }

            generarMinasInvestigativas(minas);
            
            mostrarGridInvestigativo(minas);

            promedioDistancias = calcularDistanciaPromedioInvestigativa(minas);

            actualizarTextos();
            actualizarTablero();
        }


        // ---------------------------------------------------------------------
        // ACTUALIZAR TEXTOS
        // ---------------------------------------------------------------------

        void actualizarTextos() {

            string nivel = clasificarNivelInvestigativo(promedioDistancias);

            textoNivel->setText("Nivel: " + QString::fromStdString(nivel));
            textoPromedio->setText(QString("Promedio: %1").arg(promedioDistancias, 0, 'f', 1));
        }


        // ---------------------------------------------------------------------
        // PROCESAR CLIC
        // ---------------------------------------------------------------------

        void procesarClicCelda(int fila, int columna) {

            if (tableroBloqueado) {
                return;
            }

            if (descubiertas[fila][columna]) {
                return;
            }

            descubiertas[fila][columna] = true;

            if (minas[fila][columna]) {

                tableroBloqueado = true;

                revelarMinas(fila, columna);

                return;
            }

            actualizarCelda(fila, columna);
        }


        // ---------------------------------------------------------------------
        // REVELAR LAS 5 MINAS
        // ---------------------------------------------------------------------

        void revelarMinas(int filaExplotada, int columnaExplotada) {

            for (int fila = 0; fila < FILAS_INVESTIGATIVO_UI; fila++) {
                for (int columna = 0; columna < COLUMNAS_INVESTIGATIVO_UI; columna++) {

                    if (!minas[fila][columna]) {
                        continue;
                    }

                    QPushButton* boton = botonesCeldas[fila][columna];

                    if (fila == filaExplotada && columna == columnaExplotada) {
                        boton->setIcon(QIcon(QString::fromStdString(obtenerRutaBombaExplotada())));

                    } else {
                        boton->setIcon(QIcon(QString::fromStdString(CELDA_BOMBA)));
                    }
                }
            }
        }


        // ---------------------------------------------------------------------
        // ACTUALIZAR CELDA DESCUBIERTA
        // ---------------------------------------------------------------------

        void actualizarCelda(int fila, int columna) {

            int minasVecinas = contarMinasVecinasInvestigativas(minas, fila, columna);
            string ruta = obtenerRutaNumeroCelda(minasVecinas);

            botonesCeldas[fila][columna]->setIcon(QIcon(QString::fromStdString(ruta)));
        }


        // ---------------------------------------------------------------------
        // ACTUALIZAR TABLERO
        // ---------------------------------------------------------------------

        void actualizarTablero() {

            for (int fila = 0; fila < FILAS_INVESTIGATIVO_UI; fila++) {
                for (int columna = 0; columna < COLUMNAS_INVESTIGATIVO_UI; columna++) {

                    QPushButton* boton = botonesCeldas[fila][columna];

                    boton->setEnabled(true);
                    boton->setIcon(QIcon(QString::fromStdString(CELDA_OCULTA)));
                }
            }
        }
};


// -----------------------------------------------------------------------------
// FUNCION DE CREACION
// -----------------------------------------------------------------------------

QWidget* crearInvestigativoUI(QWidget* parent) {

    return new InvestigativoUI(parent);
}