#ifndef PINGWINY_FILE_MANAGER_H
#define PINGWINY_FILE_MANAGER_H


#include <stdio.h>
#include <stdlib.h>


/** Struktura reprezentująca pojedyńcze pole na planszy */
typedef struct _field_ {

    int fish;
    int player;

}FIELD;


/**Struktura reprezentująca gracza wraz z jego danymi */
typedef struct _gamer_ {

    const char* name;
    int id;
    int points;

}GAMER;

/** Lista odpowiedzialna za przechowywanie pozycji i stanu naszych pingwinow */
typedef struct Penguin_List {

    int x; //wspolrzedna kolumnowa
    int y; //wsporzedna wierszowa
    struct Penguin_List *next;

}PList;


/** Struktura reprezentująca planszę do gry */
typedef struct _board_ {
    //Wymiary oraz id z ktorym bedziemy grali
    int width, height, our_id;
    //Właściwa plansza, pierwsza wartość to wiersz czyli height (wiersz)
    //druga to kolumna czyli width, tablica tablic wskaźników
    FIELD** fields;

    int plyr_size;
    GAMER* players;

    PList *list;

}BOARD;


/** Funkcja odpowiedzialna za załadowanie planszy ze stanem gry */
BOARD* load_board(FILE* status, const char* program);


/** Funckja zwalniająca pamięć struktury BOARD */
void deallocate_board(BOARD* board);


#ifdef PLAYER
void display_board(BOARD* board);
#endif



#endif //PINGWINY_FILE_MANAGER_H
