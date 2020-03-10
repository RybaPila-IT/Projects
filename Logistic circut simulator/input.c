#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include "input.h"

#define ERR  -1
#define AND   1
#define OR    2
#define NOT   3
#define SHOW  4
#define TEST  5
#define END   6
#define TRUE  1
#define FALSE 0

//Stale odpowiedzialne za rozpoznawanie komend
const char test[] = {'t','e','s','t','\0'};
const char show[] = {'s','h','o','w','\0'};
const char not[] = {'n','o','t','\0'};
const char and[] = {'a','n','d','\0'};
const char or[] = {'o','r','\0'};
const char end[] = {'e','n','d','\0'};

//Tablica komend wraz z komunikatami bledu skladni
const char* commands[] = {and, or, not, show, test, end};
const char* command_err = "Nieprawidlowa skladnia polecenia";
const char* bytes_err = "Nieprawidlowa kombinacja bitow";
const char* int_err = "Uwaga: doszlo do przepelnienia zakresu wartosci typu int";


//Funkcja wypisujaca informacje o bledzie.
static void give_error_message(const char* mess) {
    fprintf(stderr, "%s\n", mess);
}


/** \brief Funkcja porownujaca napisy.
 *         Funkcja porownuje napis zczytany z wejscia z odpowiednia
           komenda w tablicy commands. Jesli napisy sa takie same to
           funkcja zwraca wrtosc TRUE w p.p. FALSE.
 * \param command - instrukcja w tablicy commands z ktora ma byc porownany
                    odczyt z wejscia
 * \return TRUE jesli napisy sa takie same. False w przeciwnym przypadku.
 *
 */
static int string_check(int command) {

    int c;

    for(int i = 1; commands[command][i] != '\0'; i++) {

        c = getchar();

        if(commands[command][i] != c){
            ungetc(c, stdin);
            return FALSE;
        }
    }

    return TRUE;
}


/** \brief Funkcja odczytujaca z wejscia liczbe naturalna.
           Funkcja odczytuje z wejscia znaki reprezentujace
           cyfry. Wynikem dzialania funckji jest odczytana
           liczba naturalna w systemie dziesietnym. Jezeli
           nie odczyta ona zadnej cyfry zwraca wartosc -1.
 *
 * \return Odczytana liczba natauralna lub -1 w przypadku bledu.
 *
 */
static int get_integer() {

    int warning = FALSE;
    int sum = -1;
    int c = getchar();

    while(isdigit(c)) {

        if(sum == -1)
            sum = 0;

        if(sum + c - '0' >= INT_MAX/10 && !warning) {
            give_error_message(int_err);
            warning = TRUE;
        }

        sum *= 10;
        sum += c - '0';
        c = getchar();
    }

    ungetc(c, stdin);
    return sum;
}

/** \brief Funkcja sprawdzajaca czy podana komenda to NOT.
 *         Funkcja sprawdza czy uzytkownik wywolal dodanie
           wezla typu not do ukladu logicznego. Jezeli tak,
           to uzupelnia ona odpowiednie komorki w tablicy data
           oraz zwraca wartosc TRUE. W przeciwnym przypadku
           zwracana jest wartosc FALSE.

 * \param  data - wskaznik na tablice do ktorej maja byc zapisane dane.
 * \return TRUE jesli odczytana komenda to NOT. W przeciwnym przypadku
           wartosc FALSE.
 *
 */
static int read_NOT(unsigned int* data){

    if(!string_check(2))
        return ERR;

    int node2 = get_integer();
    int c = getchar();

    if(node2 == -1 || (c != '\n' && c != ' ' && c != '\t')) {
        ungetc(c, stdin);
        return ERR;
    }

    data[1] = node2;
    return NOT;
}


/** \brief Funkcja odczytujaca polecenie AND
 *         Funkcja sprawdza, czy uzytownik podal komende
           dodania wezla and do ukladu logicznego. Korzysta z
           funkcji string_check oraz get_integer.
 * \param data - wszanik na tablice do ktorej maja byc zapisane dane
 * \return stala AND jesli odczyt danych byl poprawny
           stala ERR jesli odczytane dane byly bledne.
 */
static int read_AND(unsigned int* data) {
    if(!string_check(0))
        return ERR;

    int node3 = get_integer();
    int c = getchar();

    if(node3 == -1 || (c !='\n' && c != ' ' && c != '\t')) {
        ungetc(c, stdin);
        return ERR;
    }

    data[2] = node3;

    return AND;
}



/** \brief Funkcja odczytujaca dodanie ezla OR.
 *         Funkcja odczytuje ze standardowego wejscia
           czy uzytkownik dodal do ukladu logicznego wezel OR.
           Funkcja korzysta z funkcji pomocniczych string_check
           oraz get_integer.
 * \param data - wskzanik na tablice do ktroej maja byc zapisane dane
 * \return OR jesli odczytna komenda bla poprawana.
 *         ERR jesli odczytana komenda zawierala blad skladni.
 */
static int read_OR(unsigned int* data) {

    if(!string_check(1))
        return ERR;

    int node3 = get_integer();
    int c = getchar();

    if(node3 == -1 || (c != '\n' && c != ' ' && c != '\t')){
        ungetc(c, stdin);
        return ERR;
    }

    data[2] = node3;

    return OR;
}


/** \brief Funkcja rozrozniajaca odczyt dodania welza AND od OR.
 *         Funkcja sprawdza, ktory wezle ma byc wprowadzony do
           ukladu logicznego. Na podstawie odczytu danych od
           uzytownika wywoluje odpowiednie podfunkcje odpowiedzialne za
           odczyt odpowiedniego wezla. Sa to read_AND oraz read_OR.
 * \param data - wskaznik na tablice do ktorej maja byc zapisane dane.
 * \return wynik dzialania podfunkcji (tj. AND lub OR jesli odczyt byl poprawny)
           ERR jesli w danych wejsciowych znajdowal sie blad.
 *
 */
static int read_AND_OR(unsigned int* data) {

    int node2 = get_integer();

    if(node2 == -1)
        return ERR;

    data[1] = node2;
    int c = getchar();

    if(c == 'a')
        return read_AND(data);
    else if(c == 'o')
        return read_OR(data);

    ungetc(c, stdin);
    return ERR;
}


/** \brief Funkcja rozrozniajaca odcczyt odpowiedniego typu wezla.
 *         Funckja na podstawie odczytu danych wprowadzonych przez
           uzytkownika decyduej ktora podfunkcje wywolac w celu
           odczytu odpowiedniego typu wezla. Wywolanymi podfunkcjami
           moga byc read_NOT lub read_AND_OR. Jest to najwyzszy poziom
           rozdzielenia sterowania w odczycie wezla.

 * \param  data - wskaznik na tablice do ktrorej maja byc zapisane odczytane dane.
 * \return wynik dzialania podunckji (NOT, AND, OR) jesli dane byly popranwe.
           ERR jesli wprowadzone dane zawieraly blad.
 *
 */
static int read_nodes(unsigned int* data) {

    int node1 = get_integer();

    if(node1 == -1)
        return ERR;

    int c = getchar();

    if(c != '=')
        return ERR;

    data[0] = node1;
    c = getchar();

    if(c == 'n')
        return read_NOT(data);

    ungetc(c, stdin);

    return read_AND_OR(data);
}

/** \brief Funckja sprawdzajaca czy podano komende show.
 *         Funkcja sprawdza czy podana komenda to komenda show.
           Funckja korzysta z podfunkcji string_check.
 * \return SHOW jesli podana komenda jest prawidlowa.
           ERR jesli wystapil blad skladni.
 *
 */
static int read_SHOW() {

    if(!string_check(3))
        return ERR;

    int c = getchar();

    if(c != '\n' && c != ' ' && c != '\t')
        return ERR;

    return SHOW;
}

/** \brief Funcja sprawdzajaca czy podano komende test.
           Funckja sprawdza czy uzytkownik wykonal na ukaldzie logicznym
           operacje test. Korzysta z podfunkcji string_check.

 * \return TEST jesli podana komenda jest prawidlowa.
           ERR w przypadku wystapienia bledu.
 *
 */
static int read_TEST() {

    if(!string_check(4))
        return ERR;

    return TEST;
}

/** \brief Funckja sprawdzajaca czy uztkownik wykonal operacje end.
 *         Funckja sprawdza czy uzytkownik wykonal na ukladzie logicznym
           operacje end. Korzysta z podfunkcji string_check.

 * \return END jesli podana komenda jest poprawna.
           ERR w przypadku wystapienia bledu.
 *
 */
static int read_END() {

    if(!string_check(5))
        return ERR;

    int c = getchar();

    if(c != '\n' && c != ' ' && c != '\t')
        return ERR;

    return END;
}


/** \brief Funkcja decydujaca ktora komende odczytac.
           Funkcja decyduje ktora podfunkcje wywolac w celu oodczytania
           komendy podanej przez uzytownika. Jest na najwyzszym poziomie
           rozdzielania sterowania w odczycie danych. Korzysta z podfunkcji
           read_nodes, read_SHOW, read_TEST oraz read_END.
 *
 * \param data - wskaznik na tablice do ktorej maja byc zapisane dane.
 * \return Wartosc wywolania odpowiedniej podfunkcji lub ERR w momencie
           nierozpoznania polecenia.
 *
 */
static int check_input(unsigned int* data) {

    int c = getchar();

    if(isdigit(c)) {
        ungetc(c, stdin);
        return read_nodes(data);

    }
    else if(c == 's')
        return read_SHOW();
    else if(c == 't')
        return read_TEST();
    else if(c == 'e')
        return read_END();

    return ERR;
}

//Funkcja usuwajaca znaki do momentu pojawienia sie znaku nowej linii.
static void clean_line() {

    int c = getchar();

    while(c != '\n')
        c = getchar();

}

//Funkcja omijajaca biale znaki.
static void omit_blank() {

    int c = getchar();

    while(isspace(c))
        c = getchar();

    ungetc(c, stdin);
}

/** \brief Funkcja okalajaca check_input.
 *         Funkcja sprawdza czy wynikiem wywolania podfunkcji
           check_input jest wartosc ERR. Jesli tak, wywoluje
           ona podfunkcje clean_line. W przeciwnym przypadku
           nic nie robi. Przekazuje wynik wywolania podfunkcji
           check_input wyzej.

 * \param  data - wskaznik na tablice do ktorej maja byc zapisane dane.
 * \return Wynik dzialania podfunkcji check_input.
 *
 */
static int user_command(unsigned int* data) {

    int result = check_input(data);

    if(result == ERR)
        clean_line();

    return result;
}

/** \brief Funkcja zczytujaca kombinacje bitow z wejscia.
 *         Funkcja odczytuje z wejsci kombinacje bitow wprowadzona przez
           uzytownika. Dynamicznie, w miare potrzeby realokuje tablice
           data do ktorej zapisywane sa kolejne odczytane bity. W przypadku
           bleu skaldni polecenia lub niedozwolonej wartosci bitu informuje
           uzytkownika o bledzie.
 * \param  data - wskaznik na wskaznik na tablice do ktorej maja byc zapisane dane.
 * \param  data_size - wskaznik na miejsce gdzie zapisany bedzie wynikowy rozmiar
                       tablicy data.
 * \return TEST jesli odczyt bitow sie udal.
           ERR jesli podczas odczytu wystapil blad.
 *
 */
static int read_bytes(unsigned int** data, int* data_size) {

    int index = 0;
    int c = getchar();

    if(c != ' ') {
        ungetc(c, stdin);
        clean_line();
        return ERR;
    }

    c = getchar();

    while(c == '1' || c == '0') {

        (*data)[index] = c - '0';
        index++;

        if(index == (*data_size)){
            (*data_size)++;
            (*data) = realloc((*data), (*data_size)*sizeof( unsigned int));
        }

        c = getchar();
    }

    if(c != '\n' || index == 0) {
        ungetc(c,stdin);
        clean_line();
        return ERR;
    }

    (*data_size) = index;

    return TEST;
}


unsigned int* read_input(int* size, int* command) {

    unsigned int* data = malloc(3*sizeof(unsigned int));
    int result;
    int data_size = 3;

    do {

        omit_blank();

        result = user_command(data);

        if(result == ERR)
            give_error_message(command_err);

        else if(result == SHOW || result == END){
            free(data);
            data = NULL;
        }
        else if(result == TEST) {
            result = read_bytes(&data, &data_size);

            if(result == ERR)
                give_error_message(bytes_err);

        }

    } while(result == -1);

    (*size) = data_size;
    (*command) = result;

    return data;
}



