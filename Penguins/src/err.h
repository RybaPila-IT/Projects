#ifndef PINGWINY_ERR_H
#define PINGWINY_ERR_H


/** Blad w momencie proby otwarcia pliku */
void file_load_err();


/**Blad w stanie gry zawartym w pliku */
void file_status_err(unsigned int col_counter, unsigned int line_counter);


void file_dimensions_err();


void file_player_err(int line);


void input_args_err();

#endif //PINGWINY_ERR_H
