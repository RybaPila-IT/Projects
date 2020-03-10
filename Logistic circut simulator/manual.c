#include <stdio.h>
#include <stdlib.h>
#include "manual.h"

const char* const manual[] =  {"********* SYMULATOR UKLADU LOGICZNEGO *********",
                                "Program symuluje dzialanie ukladu logicznego.",
                                "Uklad logiczny obsluguje trzy rodzaje bramek",
                                "logicznych. Sa to bramki: AND, OR i NOT. Dodanie",
                                "bramki do ukladu wyglada w nastepujacy sposob:",
                                "wezel1=wezel2AND|ORwezel3 lub wezel1=notwezel2.",
                                "Wykonanie operacji dodania wezla spowoduje powsta",
                                "nie nowych wejsc (w przykladzie wejsciami zostanal",
                                "wezel2 oraz wezel3). Uklad logiczny ma jedno wyjscie.",
                                "Na ukladzie logicznym mozna przeprowadzac symulacje",
                                "dzialania dla zadanych bitow. Bity przydzielane sa do",
                                "wejsc zgodnie ze starszenstwem identyfikatorow wezlow tzn.",
                                "bit pierwszy trafi do wejscia o najmniejszym numerze, bit",
                                "drugi do kolejnego itd. W celu przeprowadzenia symulacji",
                                "nalezy uzyc polecenia test [kombinacja bitow]. Po symu",
                                "lacji na ekranie wyswietli sie wynik dzialania ukladu.",
                                "W celu wyswietlenia aktualnej topologi ukladu nalezy uzyc",
                                "polecenia show. Program jest odporny na bledy skladniowe",
                                "polecen oraz na tworzenie niedozwolonych polaczen (sprzeze",
                                "nia zwrotne) czy nieodpowiedni test bitow. Ponizej znajduja",
                                "sie przyklady poprawnych polecen dla programu:",
                                "wezel1=wezel2AND|ORwezel3 wezel1,2,3 - liczby naturalne, brak spacji w skladni",
                                "show<enter>",
                                "test [kombinacja bitow] po formule test wystepuje jedna spacja",
                                "Wszystkie polecenia, z wyjatkiem test, moga konczyc sie dowolnym bialym znakiem.",
                                "Test musi byc zakonczony <enterem>. Aby zakonczyc dzialanaie programu nalezy",
                                "wpisac polecenie end"
                                };

#define MANUAL_SIZE 27

//Funkcja "czyszczaca" linie
static void clean_line() {

    int c;

    do {
        c = getchar();
    } while(c != '\n');

}


void usage() {

    printf("W celu wyswietlenia opisu programu wpisz m\n");

    char c = getchar();

    if(c == 'm') {
        for(int i = 0; i < MANUAL_SIZE; i++)
            printf("%s\n", manual[i]);

        clean_line();
        c = getchar();
    }

#ifdef WIN32
    system("cls");
#else
    system("clear");
#endif


    if(c != '\n')
        clean_line();
}
