#include <stdio.h>
#include <stdlib.h>
#include "board_saver.h"


static void save_dimensions(BOARD* board, FILE* file) {

  fprintf(file, "%d %d\n", board->height, board->width);

}

static void save_field(FIELD field, FILE* file) {

  fprintf(file, "%d%d ", field.fish, field.player);

}

static void save_fields(BOARD* board, FILE* file) {

  int i,j;

  for(i = 0; i < board->height; i++) {

    for(j = 0; j < board->width; j++)
      save_field(board->fields[i][j], file);

    fprintf(file, "\n");

  }

}

static void save_player(GAMER gamer, FILE* file) {

  fprintf(file, "%s %d %d\n", gamer.name, gamer.id, gamer.points);

}


static void save_players(BOARD* board, FILE* file) {

  int i;

  for(i = 0; i < board->plyr_size; i++)
    save_player(board->players[i], file);

}

void save_board(BOARD* board, FILE* file) {

  save_dimensions(board, file);
  save_fields(board, file);
  save_players(board, file);
  fclose(file);
}
