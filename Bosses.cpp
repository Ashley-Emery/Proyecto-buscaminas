#include "Partida.h"

BossPersonalizado obtenerBossPersonalizado(int filas, int columnas) {

    int cantidadCeldas = filas * columnas;

    if (cantidadCeldas <= 147) {
        return BossPersonalizado::HILDA_BERG;
    }

    if (cantidadCeldas <= 230) {
        return BossPersonalizado::WERNER_WERMAN;
    }

    if (cantidadCeldas <= 313) {
        return BossPersonalizado::BARONESS_VON_BON_BON;
    }

    if (cantidadCeldas <= 396) {
        return BossPersonalizado::DJIMMI_THE_GREAT;
    }

    return BossPersonalizado::KING_DICE;
}


string obtenerNombreBossPersonalizado(BossPersonalizado boss) {

    switch (boss) {

        case BossPersonalizado::HILDA_BERG:{
            return "Hilda Berg";
        }

        case BossPersonalizado::WERNER_WERMAN:{
            return "Werner Werman";
        }

        case BossPersonalizado::BARONESS_VON_BON_BON:{
            return "Baroness Von Bon Bon";
        }

        case BossPersonalizado::DJIMMI_THE_GREAT:{
            return "Djimmi the Great";
        }

        case BossPersonalizado::KING_DICE:{
            return "King Dice";
        }
    }

    return "";
}

string obtenerNombreNivelPersonalizado(BossPersonalizado boss) {

    switch (boss) {

        case BossPersonalizado::HILDA_BERG:{
            return "Zodiac Minefield";
        }

        case BossPersonalizado::WERNER_WERMAN:{
            return "Mouse Trap Minefield";
        }

        case BossPersonalizado::BARONESS_VON_BON_BON:{
            return "Candyland Minefield";
        }

        case BossPersonalizado::DJIMMI_THE_GREAT:{
            return "Mystical Minefield";
        }

        case BossPersonalizado::KING_DICE:{
            return "All Bets Are Mined";
        }
    }

    return "";
}

BossNivel obtenerBossNivel(int numeroNivel) {

    switch (numeroNivel) {

        case 1:
            return BossNivel::CAPTAIN_BRINEYBEARD;

        case 2:
            return BossNivel::BEPPI_THE_CLOWN;

        case 3:
            return BossNivel::GRIM_MATCHSTICK;

        case 4:
            return BossNivel::RIBBY_AND_CROAKS;

        case 5:
            return BossNivel::CALA_MARIA;

        case 6:
            return BossNivel::DR_KAHL_ROBOT;

        case 7:
            return BossNivel::WALLY_WARBLES;

        case 8:
            return BossNivel::CHEF_SALTBAKER;

        case 9:
            return BossNivel::THE_DEVIL;
    }

    return BossNivel::CAPTAIN_BRINEYBEARD;
}

string obtenerNombreBossNivel(BossNivel boss) {

    switch (boss) {

        case BossNivel::CAPTAIN_BRINEYBEARD:{
            return "Captain Brineybeard";
        }

        case BossNivel::BEPPI_THE_CLOWN:{
            return "Beppi the Clown";
        }

        case BossNivel::GRIM_MATCHSTICK:{
            return "Grim Matchstick";
        }

        case BossNivel::RIBBY_AND_CROAKS:{
            return "Ribby & Croaks";
        }

        case BossNivel::CALA_MARIA:{
            return "Cala Maria";
        }

        case BossNivel::DR_KAHL_ROBOT:{
            return "Dr. Kahl's Robot";
        }

        case BossNivel::WALLY_WARBLES:{
            return "Wally Warbles";
        }

        case BossNivel::CHEF_SALTBAKER:{
            return "Chef Saltbaker";
        }

        case BossNivel::THE_DEVIL:{
            return "The Devil";
        }
    }

    return "";
}

string obtenerNombreNivel(BossNivel boss) {

    switch (boss) {

        case BossNivel::CAPTAIN_BRINEYBEARD:{
            return "Pirate Minefield";
        }

        case BossNivel::BEPPI_THE_CLOWN:{
            return "Carnival Minefield";
        }

        case BossNivel::GRIM_MATCHSTICK:{
            return "Dragon Minefield";
        }

        case BossNivel::RIBBY_AND_CROAKS:{
            return "Rumble Minefield";
        }

        case BossNivel::CALA_MARIA:{
            return "High Seas Minefield";
        }

        case BossNivel::DR_KAHL_ROBOT:{
            return "Mechanical Minefield";
        }

        case BossNivel::WALLY_WARBLES:{
            return "Aerial Minefield";
        }

        case BossNivel::CHEF_SALTBAKER:{
            return "Baker's Minefield";
        }

        case BossNivel::THE_DEVIL:{
            return "One Hell of a Minefield";
        }
    }

    return "";
}