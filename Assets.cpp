#include "Assets.h"
#include "Partida.h"
#include "Tablero.h"


// -----------------------------------------------------------------------------
// AVATARES
// -----------------------------------------------------------------------------

string obtenerRutaAvatar(int numeroAvatar) {

    switch (numeroAvatar) {

        case 1:
            return AVATAR_1;

        case 2:
            return AVATAR_2;

        case 3:
            return AVATAR_3;

        case 4:
            return AVATAR_4;

        case 5:
            return AVATAR_5;

        case 6:
            return AVATAR_6;

        case 7:
            return AVATAR_7;

        case 8:
            return AVATAR_8;

        case 9:
            return AVATAR_9;

        case 10:
            return AVATAR_10;
    }

    return AVATAR_1;
}


string obtenerRutaAvatar(const string &nombreAvatar) {

    if (nombreAvatar == "avatar_1.png") {
        return AVATAR_1;
    }

    if (nombreAvatar == "avatar_2.png") {
        return AVATAR_2;
    }

    if (nombreAvatar == "avatar_3.png") {
        return AVATAR_3;
    }

    if (nombreAvatar == "avatar_4.png") {
        return AVATAR_4;
    }

    if (nombreAvatar == "avatar_5.png") {
        return AVATAR_5;
    }

    if (nombreAvatar == "avatar_6.png") {
        return AVATAR_6;
    }

    if (nombreAvatar == "avatar_7.png") {
        return AVATAR_7;
    }

    if (nombreAvatar == "avatar_8.png") {
        return AVATAR_8;
    }

    if (nombreAvatar == "avatar_9.png") {
        return AVATAR_9;
    }

    if (nombreAvatar == "avatar_10.png") {
        return AVATAR_10;
    }

    return AVATAR_1;
}


bool avatarValido(const string &nombreAvatar) {

    return nombreAvatar == "avatar_1.png" ||
           nombreAvatar == "avatar_2.png" ||
           nombreAvatar == "avatar_3.png" ||
           nombreAvatar == "avatar_4.png" ||
           nombreAvatar == "avatar_5.png" ||
           nombreAvatar == "avatar_6.png" ||
           nombreAvatar == "avatar_7.png" ||
           nombreAvatar == "avatar_8.png" ||
           nombreAvatar == "avatar_9.png" ||
           nombreAvatar == "avatar_10.png";
}


// -----------------------------------------------------------------------------
// BADGES
// -----------------------------------------------------------------------------

string obtenerRutaBadge(TipoLogro logro) {

    switch (logro) {

        case TipoLogro::FIRST_RESCUE:
            return BADGE_FIRST_RESCUE;

        case TipoLogro::MINE_TAMER:
            return BADGE_MINE_TAMER;

        case TipoLogro::QUICK_FUSE:
            return BADGE_QUICK_FUSE;

        case TipoLogro::NINE_LIVES:
            return BADGE_NINE_LIVES;

        case TipoLogro::BOMB_DISPOSAL_EXPERT:
            return BADGE_BOMB_DISPOSAL_EXPERT;

        case TipoLogro::CLEAN_SWEEP:
            return BADGE_CLEAN_SWEEP;
    }

    return "";
}


// -----------------------------------------------------------------------------
// BOTONES DE ACCION
// -----------------------------------------------------------------------------

string obtenerRutaBoton(const string &accion) {

    if (accion == "accept") {
        return BOTON_ACCEPT;
    }

    if (accion == "back") {
        return BOTON_BACK;
    }

    if (accion == "cancel") {
        return BOTON_CANCEL;
    }

    if (accion == "config") {
        return BOTON_CONFIG;
    }

    if (accion == "forward") {
        return BOTON_FORWARD;
    }

    if (accion == "help") {
        return BOTON_HELP;
    }

    if (accion == "high-scores") {
        return BOTON_HIGH_SCORES;
    }

    if (accion == "home") {
        return BOTON_HOME;
    }

    if (accion == "info") {
        return BOTON_INFO;
    }

    if (accion == "music") {
        return BOTON_MUSIC;
    }

    if (accion == "retry") {
        return BOTON_RETRY;
    }

    if (accion == "rewards") {
        return BOTON_REWARDS;
    }

    if (accion == "save") {
        return BOTON_SAVE;
    }

    return "";
}


// -----------------------------------------------------------------------------
// TABLERO DE JUEGO
// -----------------------------------------------------------------------------

string obtenerRutaNumeroCelda(int numeroMinasVecinas) {

    switch (numeroMinasVecinas) {

        case 1:
            return CELDA_1;

        case 2:
            return CELDA_2;

        case 3:
            return CELDA_3;

        case 4:
            return CELDA_4;

        case 5:
            return CELDA_5;
    }

    return CELDA_LIBRE;
}


string obtenerRutaCelda(const Celda &celda) {

    if (!celda.estaRevelada()) {

        if (celda.estaMarcada()) {
            return CELDA_MARCADA;
        }

        return CELDA_OCULTA;
    }

    if (celda.tieneMina()) {
        return CELDA_BOMBA;
    }

    int minasVecinas = celda.obtenerMinasVecinas();

    if (minasVecinas == 0) {
        return CELDA_LIBRE;
    }

    return obtenerRutaNumeroCelda(minasVecinas);
}


string obtenerRutaBombaExplotada() {

    return CELDA_BOMBA_EXPLOTADA;
}


// -----------------------------------------------------------------------------
// FUENTES
// -----------------------------------------------------------------------------

string obtenerRutaFuente(const string &nombreFuente) {

    if (nombreFuente == "Alice") {
        return FONT_ALICE;
    }

    if (nombreFuente == "Antonio") {
        return FONT_ANTONIO;
    }

    if (nombreFuente == "Lobster") {
        return FONT_LOBSTER;
    }

    return "";
}


// -----------------------------------------------------------------------------
// MENU PRINCIPAL
// -----------------------------------------------------------------------------

string obtenerRutaMenu(const string &opcion) {

    if (opcion == "claim-reward") {
        return MENU_CLAIM_REWARD;
    }

    if (opcion == "exit") {
        return MENU_EXIT;
    }

    if (opcion == "high-scores") {
        return MENU_HIGH_SCORES;
    }

    if (opcion == "how-to-play") {
        return MENU_HOW_TO_PLAY;
    }

    if (opcion == "my-profile") {
        return MENU_MY_PROFILE;
    }

    if (opcion == "play") {
        return MENU_PLAY;
    }

    return "";
}


// -----------------------------------------------------------------------------
// MUSICA
// -----------------------------------------------------------------------------

string obtenerRutaMusica(const string &nombreMusica) {

    if (nombreMusica == "Chicken Feet") {
        return MUSICA_CHICKEN_FEET;
    }

    if (nombreMusica == "It Aint Right") {
        return MUSICA_IT_AINT_RIGHT;
    }

    if (nombreMusica == "The Minor Drags") {
        return MUSICA_THE_MINOR_DRAGS;
    }

    return "";
}


// -----------------------------------------------------------------------------
// TEMPLATES DE UI
// -----------------------------------------------------------------------------

string obtenerRutaTemplate(const string &nombreTemplate) {

    if (nombreTemplate == "badges-info") {
        return TEMPLATE_BADGES_INFO;
    }

    if (nombreTemplate == "blackpowder-bay-map") {
        return TEMPLATE_BLACKPOWDER_BAY_MAP;
    }

    if (nombreTemplate == "box") {
        return TEMPLATE_BOX;
    }

    if (nombreTemplate == "calamity-cays-map") {
        return TEMPLATE_CALAMITY_CAYS_MAP;
    }

    if (nombreTemplate == "clean-1") {
        return TEMPLATE_CLEAN_1;
    }

    if (nombreTemplate == "clean-2") {
        return TEMPLATE_CLEAN_2;
    }

    if (nombreTemplate == "clean-3") {
        return TEMPLATE_CLEAN_3;
    }

    if (nombreTemplate == "felix") {
        return TEMPLATE_FELIX;
    }

    if (nombreTemplate == "how-to-play-1") {
        return TEMPLATE_HOW_TO_PLAY_1;
    }

    if (nombreTemplate == "how-to-play-2") {
        return TEMPLATE_HOW_TO_PLAY_2;
    }

    if (nombreTemplate == "how-to-play-3") {
        return TEMPLATE_HOW_TO_PLAY_3;
    }

    if (nombreTemplate == "level-failed") {
        return TEMPLATE_LEVEL_FAILED;
    }

    if (nombreTemplate == "level-won") {
        return TEMPLATE_LEVEL_WON;
    }

    if (nombreTemplate == "menu-principal") {
        return TEMPLATE_MENU_PRINCIPAL;
    }

    return "";
}


// -----------------------------------------------------------------------------
// VIDEOS DE BOSSES - NIVELES NORMALES
// -----------------------------------------------------------------------------

string obtenerRutaVideoBoss(BossNivel boss) {

    switch (boss) {

        case BossNivel::CAPTAIN_BRINEYBEARD:
            return VIDEO_CAPTAIN_BRINEYBEARD;

        case BossNivel::BEPPI_THE_CLOWN:
            return VIDEO_BEPPI_THE_CLOWN;

        case BossNivel::GRIM_MATCHSTICK:
            return VIDEO_GRIM_MATCHSTICK;

        case BossNivel::RIBBY_AND_CROAKS:
            return VIDEO_RIBBY_AND_CROAKS;

        case BossNivel::CALA_MARIA:
            return VIDEO_CALA_MARIA;

        case BossNivel::DR_KAHL_ROBOT:
            return VIDEO_DR_KAHL_ROBOT;

        case BossNivel::WALLY_WARBLES:
            return VIDEO_WALLY_WARBLES;

        case BossNivel::CHEF_SALTBAKER:
            return VIDEO_CHEF_SALTBAKER;

        case BossNivel::THE_DEVIL:
            return VIDEO_THE_DEVIL;
    }

    return "";
}


// -----------------------------------------------------------------------------
// VIDEOS DE BOSSES - PERSONALIZADO
// -----------------------------------------------------------------------------

string obtenerRutaVideoBoss(BossPersonalizado boss) {

    switch (boss) {

        case BossPersonalizado::HILDA_BERG:
            return VIDEO_HILDA_BERG;

        case BossPersonalizado::WERNER_WERMAN:
            return VIDEO_WERNER_WERMAN;

        case BossPersonalizado::BARONESS_VON_BON_BON:
            return VIDEO_BARONESS_VON_BON_BON;

        case BossPersonalizado::DJIMMI_THE_GREAT:
            return VIDEO_DJIMMI_THE_GREAT;

        case BossPersonalizado::KING_DICE:
            return VIDEO_KING_DICE;
    }

    return "";
}


// -----------------------------------------------------------------------------
// ANIMACIONES GENERALES
// -----------------------------------------------------------------------------

string obtenerRutaAnimacion(const string &nombreAnimacion) {

    if (nombreAnimacion == "intro") {
        return ANIMACION_INTRO;
    }

    if (nombreAnimacion == "julius") {
        return ANIMACION_JULIUS;
    }

    if (nombreAnimacion == "to-the-rescue") {
        return ANIMACION_TO_THE_RESCUE;
    }

    return "";
}