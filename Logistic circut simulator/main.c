#include <stdio.h>
#include <stdlib.h>
#include "logicboard.h"
#include "input.h"
#include "manual.h"

#define ERR  -1
#define AND   1
#define OR    2
#define NOT   3
#define SHOW  4
#define TEST  5
#define END   6
#define TRUE  1
#define FALSE 0

const char* forbidden_conn = "Niedozwlone polaczenie wezlow";
const char* byte_test_err =  "Nieodpowiednia liczba bitow do przperowadzenia operacji";


static void give_error_message(const char* mes) {
    fprintf(stderr, "%s\n", mes);
}





int main()
{
    int command, size, result;
    unsigned int* data;

    command = 0;

    usage();

    while(command != END) {

        data = read_input(&size, &command);

        if(command == NOT || command == AND || command == OR) {

            result = add_node(data, command);

            if(result == ERR)
                give_error_message(forbidden_conn);

        }

        else if(command == SHOW)
            show_board();

        else if(command == TEST) {

           result = test_board(data, size);

           if(result == ERR)
                give_error_message(byte_test_err);
            else
                printf("Wynik: %d\n", result);
        }

        if(data != NULL)
            free(data);
    }

    end_existing_board();

    return 0;
}
