#ifndef _AI_MOVE_H_
#define _AI_MOVE_H_

#include "file_manager.h"

/**Struktura trzymająca informacje potrzebne do stawiania pingwina*/
typedef struct best_place_to_place {
    int x; //kolumna
    int y; //wiersz
    int place_value; //wartosciowanie pola
}BEST_PLACE;

/**Struktura trzymająca pozycję pingwina i wartość najlepszego dla niego ruchu */
typedef struct BEST_MOVE {
    int x1; // aktualna pozycja
    int y1;
    int move_value;
    int x2; //najlepszy ruch dla tego pingwina
    int y2;
}BEST_MOVE;


/**Funkcje gry autonomicznej*/
void place_penguins(BOARD *board);
void move_penguins(BOARD *board);

/** Funkcje odpowiedzialne za sprawdzanie konca fazy */
int check_game_status(PList *list, BOARD *my_board);
int check_placement_status(BOARD *board, int penguin);




#endif /** _AI_MOVE_H_ */
