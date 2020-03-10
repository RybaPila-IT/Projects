#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "graphic_penguin.h"
#include <assert.h>
#include <unistd.h>
#include <string.h>

#define PREVIOUS        101
#define NEXT            100
#define ESC             27
#define ENTER           10
#define FISH_COLOR      99
#define FIRST_PLAYER    1
#define SECOND_PLAYER   2
#define THIRD_PLAYER    3
#define FOURTH_PLAYER   4
#define FIFTH_PLAYER    5
#define SIXTH_PLAYER    6
#define TRUE            1
#define FALSE           0




const int player_colours[] = {COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE};

typedef struct _graphic_board_ {

  BOARD* board;
  WINDOW* window;
  int fields_width, fields_height;
  int highlighted_width, highlighted_height;
  int moving_width, moving_height;
  int startx, starty;

}GRAPHIC_BOARD;


typedef struct _key_ {

  int player_size;
  GAMER* players;
  WINDOW* window;
  int startx, starty;
  int height, width;

}KEY;


static void set_center_xy(int *x, int *y, int box_width, int box_hight) {

	(*x) = (COLS - box_width)/2;
	(*y) = (LINES - box_hight)/2;

}


static WINDOW* get_new_window(int width, int height, int startx, int starty) {

  WINDOW* win = newwin(height, width, starty, startx);
  keypad(win, TRUE);

  return win;
}


static GRAPHIC_BOARD* make_empty_graphic_board(BOARD* board) {

  assert(board);
  GRAPHIC_BOARD* g_b = malloc(sizeof(GRAPHIC_BOARD));
  assert(g_b);
  g_b->board = board;
  g_b->window = NULL;
  g_b->highlighted_width = -1;
  g_b->highlighted_height = -1;
  g_b->moving_width = -1;
  g_b->moving_height = -1;
  g_b->fields_width = (board->width * 5) + 2;
  g_b->fields_height = (board->height * 4) + 2;

  set_center_xy(&(g_b->startx), &(g_b->starty), g_b->fields_width , g_b->fields_height);
  g_b->window = get_new_window(g_b->fields_width, g_b->fields_height, g_b->startx, g_b->starty);

  return g_b;
}


static int get_longest_name_size(BOARD* board) {

  GAMER* tmp = board->players;
  int max = 0;

  for(int i = 0; i < board->plyr_size; i++) {

    int len = strlen(tmp->name) + 9;

    if(len > max)
      max = len;

  }

  return max;

}



static KEY* make_new_key(BOARD* board) {

  assert(board);
  KEY* my_key = malloc(sizeof(KEY));
  assert(my_key);

  my_key->height = 8;
  my_key->players = board->players;
  my_key->player_size = board->plyr_size;
  my_key->width = get_longest_name_size(board);

  set_center_xy( &(my_key->startx), &(my_key->starty), my_key->width, my_key->height);

  my_key->window = get_new_window(my_key->width, my_key->height, my_key->startx, my_key->starty);

  return my_key;
}



static void print_empty_box(GRAPHIC_BOARD* g_b, int i, int j) {

  mvwprintw(g_b->window, (j*4) + 2, (i*5) + 2, "   ");
  mvwprintw(g_b->window, (j*4) + 3, (i*5) + 2, "   ");

}



static void print_fish(GRAPHIC_BOARD* g_b, int i , int j) {

  init_pair(FISH_COLOR, COLOR_BLUE, COLOR_BLACK);
  wattron(g_b->window, COLOR_PAIR(FISH_COLOR));

  if(g_b->board->fields[j][i].fish == 1){
    mvwprintw(g_b->window, (j*4)+2, (i*5)+2, "   ");
    mvwprintw(g_b->window, (j*4)+3, (i*5)+2, " * ");
  }
  else if(g_b->board->fields[j][i].fish == 2) {
    mvwprintw(g_b->window, (j*4)+2, (i*5)+2, "* *");
    mvwprintw(g_b->window, (j*4)+3, (i*5)+2, "   ");
  } else {
    mvwprintw(g_b->window, (j*4)+2, (i*5)+2, "* *");
    mvwprintw(g_b->window, (j*4)+3, (i*5)+2, " * ");

  }

  wattroff(g_b->window, COLOR_PAIR(FISH_COLOR));

}


static void print_penguin(GRAPHIC_BOARD* g_b, int i, int j) {

  mvwprintw(g_b->window, (j*4)+2, (i*5)+2, " -0");
  mvwprintw(g_b->window, (j*4)+3, (i*5)+2, " ()");

}


static void print_penguin_with_color(GRAPHIC_BOARD* g_b, int i, int j) {

  int player_id = g_b->board->fields[j][i].player;

  init_pair(player_id, player_colours[player_id-1], COLOR_BLACK);
  wattron(g_b->window, COLOR_PAIR(player_id));

  print_penguin(g_b, i, j);

  wattroff(g_b->window, COLOR_PAIR(player_id));
}


//Funkcja wyswietlajaca zawartosc pojedynczego okienka
//Wyswietla nic, 1,2 lub 3 ryby lub pingwina gracza
//j to numer wiersza a i to numer kolumny
static void print_field_content(GRAPHIC_BOARD* g_b, int i, int j) {

  if(g_b->board->fields[j][i].fish != 0)
    print_fish(g_b, i, j);
  else if(g_b->board->fields[j][i].player != 0)
    print_penguin_with_color(g_b, i, j);
  else
    print_empty_box(g_b, i, j);

}


static void print_field_border(GRAPHIC_BOARD* g_b, int i, int j) {

  mvwprintw(g_b->window, (j*4) + 1, (i*5) + 1, "-----");
  mvwprintw(g_b->window, (j*4) + 2, (i*5) + 1, "|");
  mvwprintw(g_b->window, (j*4) + 2, (i*5) + 5, "|");
  mvwprintw(g_b->window, (j*4) + 3, (i*5) + 1, "|");
  mvwprintw(g_b->window, (j*4) + 3, (i*5) + 5, "|");
  mvwprintw(g_b->window, (j*4) + 4, (i*5) + 1, "-----");
}

//i odpowiada za szerokosc a j za wysokosc
static void display_single_box(GRAPHIC_BOARD* g_b, int i, int j) {

  print_field_border(g_b, i, j);
  print_field_content(g_b, i, j);
}



static void print_window(GRAPHIC_BOARD* graphic_board) {

  box(graphic_board->window,0,0);

  //i iteruje po lini a j po kolumnie, przechodzenie wiersz--->kolumn
  for(int j = 0; j < graphic_board->board->height; j++)
    for(int i = 0; i < graphic_board->board->width; i++)
      display_single_box(graphic_board, i, j);


  wrefresh(graphic_board->window);
}



static PList* graphic_penguin(int j, int i) {

  PList* penguin = malloc(sizeof(PList));
  assert(penguin);

  penguin->next = NULL;
  penguin->x = i;
  penguin->y = j;

  return penguin;
}

static void add_to_board_list(BOARD* board, PList* penguin) {

  if(board->list == NULL)
    board->list = penguin;
  else {

    PList* tmp = board->list;

    while (tmp->next != NULL)
      tmp = tmp->next;

    tmp->next = penguin;

  }
}


static void get_graphic_penguins(BOARD* board) {

  for(int j = 0; j < board->height; j++)
    for(int i = 0; i < board->width; i++)
      if(board->fields[j][i].player == board->our_id) {
        PList* penguin = graphic_penguin(j, i);
        add_to_board_list(board, penguin);
      }
}


static void get_next_penguin(GRAPHIC_BOARD* g_b) {

  PList* tmp = g_b->board->list;

  while(tmp->x != g_b->highlighted_width || tmp->y != g_b->highlighted_height)
    tmp = tmp->next;

  if(tmp->next == NULL) {
    g_b->highlighted_width = g_b->board->list->x;
    g_b->highlighted_height = g_b->board->list->y;

  } else {
    g_b->highlighted_width = tmp->next->x;
    g_b->highlighted_height = tmp->next->y;

  }

}


static void get_previous_penguin(GRAPHIC_BOARD* g_b) {

  PList* tmp = g_b->board->list;
  //Przypadek gdy musimy znalezc koniec listy
  if(g_b->board->list->x == g_b->highlighted_width && g_b->board->list->y == g_b->highlighted_height) {

    while(tmp->next != NULL)
      tmp = tmp->next;

    g_b->highlighted_width = tmp->x;
    g_b->highlighted_height = tmp->y;
  } else {

    while(tmp->next->x != g_b->highlighted_width || tmp->next->y != g_b->highlighted_height)
      tmp = tmp->next;

    g_b->highlighted_width = tmp->x;
    g_b->highlighted_height = tmp->y;

  }
}


static void display_bold_penguin(GRAPHIC_BOARD* g_b) {

  wattron(g_b->window, A_BOLD);
  print_field_content(g_b, g_b->highlighted_width, g_b->highlighted_height);
  wattroff(g_b->window, A_BOLD);

}

static void highlight_penguin(GRAPHIC_BOARD* g_b, int type) {
  //Jak nie mamy pingwinow to nic nie robimy
  if(g_b->board->list == NULL)
    return;

  if( g_b->highlighted_width != -1 ) {
    //cofnięcie poprzedniego highlightu
    display_single_box(g_b, g_b->highlighted_width, g_b->highlighted_height);

    if(type == NEXT)
      get_next_penguin(g_b);
    else
      get_previous_penguin(g_b);

    display_bold_penguin(g_b);

  } else {

    g_b->highlighted_width = g_b->board->list->x;
    g_b->highlighted_height = g_b->board->list->y;

    display_bold_penguin(g_b);;
  }
}


static void display_highligthed_penguin(GRAPHIC_BOARD* g_b) {

  int our_id = g_b->board->our_id;

  init_pair(our_id, player_colours[our_id-1], COLOR_BLACK);
  wattron(g_b->window, COLOR_PAIR(our_id));
  wattron(g_b->window, A_STANDOUT);
  print_penguin(g_b, g_b->moving_width, g_b->moving_height);
  wattroff(g_b->window, A_STANDOUT);
  wattroff(g_b->window, COLOR_PAIR(our_id));

}


static void redisplay_field(GRAPHIC_BOARD* g_b) {

  if(g_b->moving_width == g_b->highlighted_width && g_b->moving_height == g_b->highlighted_height)
    print_empty_box(g_b, g_b->highlighted_width, g_b->highlighted_height);
  else
    display_single_box(g_b, g_b->moving_width, g_b->moving_height);

}




static int correct_move(GRAPHIC_BOARD* g_b) {

  int j = g_b->moving_height;
  int i = g_b->moving_width;

  if(g_b->moving_width != g_b->highlighted_width && g_b->moving_height != g_b->highlighted_height)
    return FALSE;

  if(g_b->board->fields[j][i].player == 0 && g_b->board->fields[j][i].fish == 0)
    return FALSE;

  if(g_b->board->fields[j][i].player != 0 && (i != g_b->highlighted_width || j != g_b->highlighted_height))
    return FALSE;

  return TRUE;

}



static void add_points(BOARD* board, int points) {

  GAMER* gamer = board->players;
  int i = 0;

  while(gamer[i].id != board->our_id)
    i++;

  gamer[i].points += points;
}



static void do_move(GRAPHIC_BOARD* g_b) {

  add_points(g_b->board, g_b->board->fields[g_b->moving_height][g_b->moving_width].fish);
  g_b->board->fields[g_b->moving_height][g_b->moving_width].fish = 0;
  g_b->board->fields[g_b->highlighted_height][g_b->highlighted_width].player = 0;
  g_b->board->fields[g_b->moving_height][g_b->moving_width].player = g_b->board->our_id;
}





static int move_penguin(GRAPHIC_BOARD* g_b) {

  g_b->moving_width = g_b->highlighted_width;
  g_b->moving_height = g_b->highlighted_height;

  display_highligthed_penguin(g_b);

  int ch;

  while( (ch = wgetch(g_b->window)) != ESC) {

    switch (ch) {
      case KEY_UP:
        redisplay_field(g_b);

        if(g_b->moving_height > 0) {
          g_b->moving_height--;

          if(!correct_move(g_b))
            g_b->moving_height++;

          }

        display_highligthed_penguin(g_b);
      break;

      case KEY_DOWN:
        redisplay_field(g_b);

        if(g_b->moving_height < g_b->board->height - 1) {
          g_b->moving_height++;

          if(!correct_move(g_b))
            g_b->moving_height--;

          }

        display_highligthed_penguin(g_b);
      break;

      case KEY_LEFT:
        redisplay_field(g_b);

        if(g_b->moving_width > 0) {
          g_b->moving_width--;

          if(!correct_move(g_b))
            g_b->moving_width++;

          }

        display_highligthed_penguin(g_b);
      break;

      case KEY_RIGHT:
        redisplay_field(g_b);

        if(g_b->moving_width < g_b->board->width - 1) {
          g_b->moving_width++;

          if(!correct_move(g_b))
            g_b->moving_width--;

          }

        display_highligthed_penguin(g_b);
      break;
      //Tutaj zatwierdzamy ruch
      case ENTER:
        do_move(g_b);
        return TRUE;
      break;
    }

    wrefresh(g_b->window);
  }

  redisplay_field(g_b);
  display_single_box(g_b, g_b->highlighted_width, g_b->highlighted_height);
  wrefresh(g_b->window);

  return FALSE;

}


static void del_window(WINDOW* win, int startx, int starty, int width, int height) {

  delwin(win);

  for(int i = starty; i <= starty+height; i++)
    mvhline(i, startx, ' ', width);

}


static void print_players_at_key(KEY* my_key) {

  GAMER* tmp = my_key->players;

  for(int i = 0; i < my_key->player_size; i++) {

    wattron(my_key->window, COLOR_PAIR(tmp[i].id));
    mvwprintw(my_key->window, i+1, 1, "%s  %d", tmp[i].name, tmp[i].points);
    wattroff(my_key->window, COLOR_PAIR(tmp[i].id));

  }

}



static void display_key(GRAPHIC_BOARD* g_b) {

  del_window(g_b->window, g_b->startx, g_b->starty, g_b->fields_width, g_b->fields_height);

  refresh();

  KEY* key = make_new_key(g_b->board);

  box(key->window,0,0);
  print_players_at_key(key);
  wgetch(key->window);

  del_window(key->window, key->startx, key->starty, key->width, key->height);
  free(key);

  g_b->window = get_new_window(g_b->fields_width, g_b->fields_height, g_b->startx, g_b->starty);
  print_window(g_b);
}






static void listen_to_user(GRAPHIC_BOARD* g_b) {

  int ch;
  int move = 0;
  //Esc konczy petle lub wykonanie dobrego ruchu
  while( !move && (ch = wgetch(g_b->window)) != ESC) {

    switch (ch) {
      case KEY_UP:
        highlight_penguin(g_b, NEXT);
      break;

      case KEY_RIGHT:
        highlight_penguin(g_b, NEXT);
      break;

      case KEY_DOWN:
        highlight_penguin(g_b, PREVIOUS);
      break;

      case KEY_LEFT:
        highlight_penguin(g_b, PREVIOUS);
      break;

      case ENTER:
        move = move_penguin(g_b);
      break;

      case KEY_F(2):
        display_key(g_b);
      break;
    }

    wrefresh(g_b->window);

  }
}


static int fits_screen(GRAPHIC_BOARD* g_b) {

  if(g_b->fields_width > COLS || g_b->fields_height > LINES) {
    mvprintw(0, 0, "Screen size is to small for displaying board, press any key to continue");
    refresh();
    getch();
    return FALSE;
  }

  mvprintw(LINES-2, 0, "For exit press ESC");
  mvprintw(LINES-1, 0, "For key press F2");
  refresh();
  return TRUE;

}



void make_move(BOARD* board) {

  initscr();
  clear();
  noecho();
  cbreak();

  if(!has_colors())
    fprintf(stderr, "%s\n", "This terminal does not support color capabilities");
  else
    start_color();

  GRAPHIC_BOARD* graphic_board = make_empty_graphic_board(board);

  if(fits_screen(graphic_board)) {

    print_window(graphic_board);
    get_graphic_penguins(graphic_board->board);
    listen_to_user(graphic_board);

  }


  delwin(graphic_board->window);
  free(graphic_board);

  endwin();

}
