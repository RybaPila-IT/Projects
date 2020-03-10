#include <stdio.h>
#include <stdlib.h>
#include "err.h"



#define file_load_msg "Unable to open a file with board status"
#define file_stat_msg "Error occured while reading game status file"
#define file_dime_msg "Board has no dimension"
#define file_plyr_msg "Player name, separators or id is incorrect"



void file_load_err() {

    fprintf(stderr, "%s", file_load_msg);
    exit(3);
}


void file_status_err(unsigned int col_counter, unsigned int line_counter) {

    fprintf(stderr, "%s\n", file_stat_msg);
    fprintf(stderr, "line: %u ", line_counter);
    fprintf(stderr, "column: %u\n", col_counter);
    exit(2);
}


void file_dimensions_err() {

    fprintf(stderr, "%s\n", file_dime_msg);
    exit(2);
}



void file_player_err(int line) {

    fprintf(stderr, "At line: %d - ", line);
    fprintf(stderr, "%s\n", file_plyr_msg);
    exit(2);
}

void input_args_err() {

  fprintf(stderr, "Unacceptable arguments for running a programm\n");
  exit(-1);

}
