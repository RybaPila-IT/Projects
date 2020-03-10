#include "file_manager.h"
#include "err.h"
#include <ctype.h>
#include <string.h>
#include <assert.h>


#define TRUE 1
#define FALSE 0
#define END -1



/** FUNKCJE ODPOWIEDZIALNE ZA ODCZYT KONKRETNYCH DANYCH Z PLIKU */

static int read_integer(FILE* status, unsigned int* line_counter);

static void read_field(FILE* status, FIELD* field);

static const char* read_player_name(FILE* status);


/** FUNKCJE ODPOWIEDZIALNE ZA POROWNYWANIE DANYCH LUB SPRAWDZANIE POPRAWNOSCI */

static int correct_field(int fish, int player);

static int equal_name(const char* name1, const char* name2);


/**FUNKCJE ODPOWIEDZIALNE ZA ZAPIS DANYCH DO STRUKTURY */

static void load_dimensions(BOARD* load, FILE* status);

static void load_fields(BOARD* board, FILE* status);

static void prepare_board(BOARD* board);

static GAMER* load_gamer(BOARD* board, FILE* status, unsigned int line);

static void add_player(BOARD* board, GAMER* player);

static void load_id(BOARD* board, FILE* status, const char* program);


/** FUNKCJE ODPOWIEDZIALNE ZA WYSWIETLANIE PLANSZY W TRYBIE GRAFICZNYM */

#ifdef PLAYER

static void display_players(GAMER* players, int length);

static void display_player(GAMER* player);

static void display_field(FIELD* field);

static void display_line(int width);

#endif


/** FUNKCJE ODPOWIEDZIALNE ZA ZARZADZANIE PAMIECIA */


static void free_player(GAMER* player);

static void free_players(BOARD* board);

static void free_fields(BOARD* board);



/** FUNKCJA ODPOWIEDZIALNA ZA AWARYJNE ZWALNIANIE PAMIECI ZAMKNIECIE PLIKU
 *  ORAZ WYOWLANIE ODPOWOWIEDNIEGO KOMUNIKATU BLEDU */

static void err_status_occured(BOARD* board, FILE* status, unsigned int col_cont, unsigned int ln_cont, int code);



/** Funkcja parsująca integer`a z pliku do int */
static int read_integer(FILE* status, unsigned int* line_counter) {

    int counter = 0;
    int value = 0;
    int c = fgetc(status);

    while( isdigit(c) ) {

        value *= 10;
        value += (c - '0');
        c = fgetc(status);
        (*line_counter)++;
        counter++;
    }

    if(counter == 0)
        return -1;

    if(c == ' ' || c == '\n' || isalpha(c) )
      ungetc(c, status);

    return value;
}



static int fine_end_line(FILE* status) {

  int c;

  if( (c = fgetc(status)) == EOF)
    return END;

  while (c == ' ') {

    if( (c = fgetc(status) ) == EOF)
      return END;

  }

  int result = c == '\n' ? TRUE : FALSE;

  return result;
}


/** Funkcja zwalniająca awaryjnie pamięć i wywołująca błąd statusu gry */
static void err_status_occured(BOARD* board, FILE* status, unsigned int col_cont, unsigned int ln_cont, int code) {

    if(board->fields != NULL)
        deallocate_board(board);
    else
        free(board);

    fclose(status);

    switch(code) {
        case 1: file_status_err(col_cont, ln_cont);
            break;
        case 2: file_dimensions_err();
            break;

        case 3: file_player_err(ln_cont);
            break;

        default:
            break;
    }
}


/** Funkcja odpwiedzialna za odczyt wymiarów planszy tzn pierwszego wiersza */
static void load_dimensions(BOARD* load, FILE* status) {

    unsigned int col_counter = 1;

    load->height = read_integer(status, &col_counter);

    int c = fgetc(status);

    if( c != ' ' )
        err_status_occured(load, status, col_counter, 1, 1);

    col_counter++;

    load->width = read_integer(status, &col_counter);

    c = fgetc(status);

    if( c != '\n' )
        err_status_occured(load, status, col_counter, 1, 1);

    if(load->width <= 0 || load->height <= 0 )
        err_status_occured(load, status, col_counter, 0, 2);
}



static int correct_field(int fish, int player) {

    //Przypadki gdy dane sa niepoprawne
    if(fish < 0  || fish > 3 || player < 0 || player > 9)
        return 0;

    //Przypadek gracz z ryba
    if( fish > 0 && player > 0)
        return 0;

    return 1;
}


/** Jeśli pole zawierało błąd zwracamy NULL w przeciwnym przypadku pole */
static void read_field(FILE* status, FIELD* field) {

    int fish = fgetc(status) - '0';
    int player = fgetc(status) - '0';

    if( !correct_field(fish, player) ) {
        field->fish = 0;
        field->player = player;
        return;
    }

    field->fish = fish;
    field->player = player;
}


/** Funkcja odpowiedzialna za odczyt kolejnych pól planszy */
static void load_fields(BOARD* board, FILE* status) {

    unsigned int line_counter = 2;
    unsigned int col_counter = 1;
    int i, j;

    for(i = 0; i < board->height; ++i) {
        for(j = 0; j < board->width; ++j) {

            read_field(status, &(board->fields[i][j]) );

            if(board->fields[i][j].fish == -1)
                err_status_occured(board, status, col_counter, line_counter, 1);

            col_counter += 2;

            int c = fgetc(status);

            if(c == -1)
              break;

            if( j == board->width-1 && c == '\n')
                ungetc(c, status);

            else if( c != ' ')
                err_status_occured(board, status, col_counter, line_counter, 1);

            col_counter++;

        }

        int result = fine_end_line(status);

        if(!result)
            err_status_occured(board, status, col_counter, line_counter, 1);

        col_counter = 1;
        line_counter++;
    }
}


/** Funkcja rezerwująca pamięć na pole do gry TODO błąd rezerwacji pamięci */
static void prepare_board(BOARD* board) {

    int i;
    board->fields = malloc( (board->height) * sizeof( FIELD* ) );

    for(i = 0; i < board->height; ++i)
        board->fields[i] = malloc(board->width * sizeof(FIELD));

    board->our_id = 0;
    board->players = NULL;
    board->plyr_size = 0;
}



static const char* read_player_name(FILE* status) {

    char* name = malloc(1 * sizeof(char));
    int size = 1;
    int c;
    c = fgetc(status);

    while (c != ' ') {

        name[size-1] = (char) c;
        size++;

        char* tmp = realloc(name, size * sizeof(char));
        name = tmp;
        c = fgetc(status);
    }

    ungetc(c, status);

    name[size-1] = '\0';

    return name;
}



static GAMER* load_gamer(BOARD* board, FILE* status, unsigned int line) {

    unsigned int dummy = 0;

    GAMER* player = malloc(sizeof(GAMER));

    player->name = read_player_name(status);

    int c = fgetc(status);

    if(c != ' ') {
        free_player(player);
        err_status_occured(board, status, 0, line, 3);
    }

    player->id = read_integer(status, &dummy);

    c = fgetc(status);

    if(c != ' ' || player->id < 1 || player->id > 9) {
        free_player(player);
        err_status_occured(board, status, 0, line, 3);
    }

    player->points = read_integer(status, &dummy);

    if(player->points == -1) {
        free_player(player);
        err_status_occured(board, status, 0, line, 3);
    }

    return player;
}



static void add_player(BOARD* board, GAMER* player) {

    GAMER* new_players;

    if(board->players == NULL)
        new_players = malloc(sizeof(GAMER) );
    else
        new_players = realloc(board->players, (board->plyr_size+1) * sizeof(GAMER) );

    new_players[board->plyr_size].id = player->id;
    new_players[board->plyr_size].name = player->name;
    new_players[board->plyr_size].points = player->points;
    board->plyr_size++;
    board->players = new_players;

    free(player);
}



static int equal_name(const char* name1, const char* name2) {

    int i = 0;

    while(name1[i] == name2[i] && name1[i] != '\0')
        ++i;

    return name1[i] == name2[i];
}


//TODO sprawdzanie duplikacji id oraz brak wolnych miejsc tzn. prev_id == 9 a nas nie ma w grze
static void load_id(BOARD* board, FILE* status, const char* program) {

    int i = 0;
    int prev_id = 0;
    int result, c;

    do {

        if( (c = fgetc(status)) == EOF)
          break;

        ungetc(c, status);

        GAMER* player = load_gamer(board, status, (unsigned int) (board->height + 2 + i));

        if(equal_name(player->name, program) )
            board->our_id = player->id;
        else if(prev_id >= player->id)
            err_status_occured(board, status, 3, board->height + i + 1, 3);
        else
          prev_id = player->id;

        add_player(board, player);
        ++i;

        result = fine_end_line(status);

        if(result == FALSE)
          err_status_occured(board, status, 3, board->height + i + 1, 3);

    } while(result);

    //Przypadek gdy nie napotkalismy swojego id
    if(board->our_id == 0) {

        GAMER* player = malloc(sizeof(GAMER));
        player->id = prev_id+1;
        player->points = 0;
        board->our_id = prev_id+1;

        char* name = malloc( (strlen(program)+1) *sizeof(char));

        player->name = strcpy(name, program);
        add_player(board, player);
    }
}


#ifdef PLAYER


//Drukuje linię w terminalu
static void display_line(int width) {

    int i;

    for(i = 0; i < width; ++i)
        printf("------");

    printf("\n");

}


//Drukuje jedną komórkę z pingwinem
static void display_field(FIELD* field) {

    printf("| %d%d |", field->fish, field->player);
}



static void display_player(GAMER* player) {

    printf("%s %d %d\n", player->name, player->id, player->points);

}



static void display_players(GAMER* players, int length) {

    int i;

    for(i = 0; i < length; ++i)
        display_player(&players[i]);
}


//TODO bardziej czytelna forma przedstawiania planszy
void display_board(BOARD* board) {

    int i,j;

    for(i = 0; i < board->height; ++i){

        display_line(board->width);

        for(j = 0; j < board->width; ++j){
            display_field( &(board->fields[i][j]) );
        }

        printf("\n");
    }

    display_line(board->width);
    display_players(board->players, board->plyr_size);
}

#endif


/**Funkcja ładująca całą planszę z pliku oraz zamyka plik*/
BOARD* load_board(FILE* status, const char* program) {

    BOARD* board = malloc(sizeof(BOARD));

    load_dimensions(board, status);
    prepare_board(board);
    load_fields(board, status);
    load_id(board, status, program);
    board->list = NULL;
    fclose(status);

    return board;
}


/** Funkcja zwalniająca pamięć pól*/
static void free_fields(BOARD* board) {

    int i;

    for(i = 0; i < board->height; ++i)
        if(board->fields[i] != NULL)
            free(board->fields[i]);


    if(board->fields != NULL)
        free(board->fields);
}


/** Funkcja zwalniająca pamięć jednego gracza */
static void free_player(GAMER* player) {

    if( player->name != NULL)
        free( (char *) player->name);
}


/** Funkcja zwalniająca pamięć graczy */
static void free_players(BOARD* board) {

    int i;

    for(i = 0; i < board->plyr_size; ++i)
            free_player(&board->players[i]);

    if(board->players != NULL)
        free(board->players);
}


/** Funkcja zwalniająca pamięć planszy */
void deallocate_board(BOARD* board) {

    free_fields(board);
    free_players(board);
    free(board);
}
