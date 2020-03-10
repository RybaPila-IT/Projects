#ifndef LOGICBOARD_H
#define LOGICBOARD_H


/** \brief Funkcja dodajaca do ukladu wezel odpowiedniego typu.
 *         Funkcja dodaje do ukladu wezel odpowiedniego typu.
           Sprawdza czy dodwanay wezel spelnia warunki popranosci
           (brak sprzezenia zwrotnego, duplikacji nazwy itd.).
 * \param  to_add - wskaznik na tablice z danymi wezla do dodania.
 * \param  type - typ wezla ktory ma byc dodany do ukladu.
 * \return TRUE jesli dodanie bylo mozliwe.
 *         ERR w przypadku wykrycia bledu.
 */
int add_node(unsigned int* to_add, int type);


/** \brief Funkcja wyswietlajaca uklad logiczny.
 *         Funkcja wyswietla uklad logiczny wedlug formayu podanego
           w tresci zadania. Wezly prezentowane sa w kolejnosci ich numerow,
           nastepnie przedstawiane sa wejscia rowniez posortowane rosnaca
           na koniec wyjscie ukladu logicznego.
 */
void show_board();


/** \brief Funkcja wykonujaca test bitowy na ukladzie logicznym.
           Funkcja wykonuje na ukladzie logicznym test bitow zadany
           przez uzytownika. Jezeli test jest niemozliwy (za duzo lub za malo
           podanych bitow) funkcja informuje o bledzie uzytownika. W przypadku
           poprawnych danych zwraca wynik przeprowadzenia testu bitow.
 *
 * \param bytes - wskaznik na tablice z bitami do testu.
 * \param bytes_size - rozmiar tablicv bytes.
 * \return ERR jesli test byl niemozliwy.
           Wynik przeprowadzenia testu w przeciwnym razie.
 *
 */
int test_board(unsigned int* bytes, int bytes_size);


/** \brief Funkcja dealokujaca pamiec zajmowana przez uklad logiczny.
 *         Funkcja dealokuje pamiec zajeta przez uklad logiczny.
           Jest ona wykonywana na koniec dzialania programu.
 */
void end_existing_board();


#endif // LOGICBOARD_H
