#ifndef INPUT_H
#define INPUT_H

/** \brief Funkcja odczytujaca dane wprowadzone przez uzytownika.
 *         Funkcja odczytuje od uzytownika dane. Funckja zwraca
           jakakolwiek wartosc tylko w przypadku poprwanie wprowadzonych
           danych. Wartoscia zwracan przez funkcje jest tablica odczytanych danych.
           Dla poszczegolnych komend wyglada ona nastepujaca:

           AND oraz OR: komorka 0 - wezel1, komorka 1 - wezel2, komorka 2 - wezel3
                        np. 1=2and3 -----> [1][2][3]

           NOT:         komorka 0 - wezel1, komorka 1 - wezel2, komorka 2 - pusta
                        np. 1=not6 ------> [1][6][N]

           SHOW, END:   NULL

           TEST:        kombinacja bitow np:
                        010110 ---------> [0][1][0][1][1][0]

          Rozmiar tablicy przekazywany jest poprzez wsaznik size, zas kod
          komendy poprzez wskaznik command.
 * \param size - wskaznik na rozmiar zwracananej tablicy.
 * \param command - wskaznik na kodf odczytanej komendy.

 * \return Wskaznik na tablice z danymi.
 */
unsigned int* read_input(int* size, int* command);

#endif // INPUT_H
