#ifndef STRSET_H
#define STRSET_H

#ifdef __cplusplus

    #include <cstddef>
    #include <iostream>

namespace jnp1 {

    extern "C" {
        
#elif __STDC__
        
    #include <stddef.h>
        
#endif // _cplusplus and _STDC_

    /** @brief Tworzy nowy zbiór i zwraca jego identyfikator.
       @return Identyfiaktor ww. zbioru.
     */
    unsigned long strset_new();

    /** @brief Usuwa zbiór o podanym identyfikatorze.
        Funkcja sprawdza, czy istnieje zbiór o podanym indetyfikatorze @param id.
        Jeżeli istnieje, to zostaje on usunięty, w przeciwnym przypadku nic nie robi.
        @param[in] id - Identyfikator zbioru do usunięcia.
     */
    void strset_delete(unsigned long id);

    /** @brief Funkcja podająca liczbę elementów zbioru.
        Funkcja sprawdza, czy podany zbiór o @param id istnieje. Jeżeli taki
        zbiór istnieje, to wynikiem działania funkcji jest liczba jego elementów.
        W przeciwnym przypadku wynikiem działania funkcji jest 0.
        @param[in] id - Identyfikator zbioru;
        @return Ww. liczba.
     */
    size_t strset_size(unsigned long id);

    /** @brief Funkcja dodąjąca element do zbioru.
        Funkcja sprawdza, czy podany element @param value nie należy do zbioru
        @param id. Jeśli zbiór @param id nie posiada elementu @param value, to zostaje
        on do niego dodany. Jeśli zbiór posiada już @param value lub zbiór nie istnieje, to
        nic się nie dzieje.
        @param[in] id - Identyfiaktor zbioru;
        @param[value] - Element do dodania.
     */
    void strset_insert(unsigned long id, const char *value);

    /** @brief Funkcja usuwająca element ze zbioru.
        Funkcja usuwa element @param value ze zbioru @param id. Jeżeli w zbiorze nie
        ma wartości @param value lub zbiór @param id nie istnieje, to nic się nie dzieje.
        @param[in|out] id - Identyfikator zbioru;
        @param[in] value - Element do usunięcia.
     */
    void strset_remove(unsigned long id, const char *value);

    /** @brief Funkcja sprawdzająca, czy element należy do zbioru.
        Funkcja sprawdza, czy podany element @param value należy do zbioru @param id.
        Jeśli tak, to funkcja zwraca wartość 1. W przeciwnym przypadku zwraca 0.
        @param[in] id - Identyfikator zbioru;
        @param[in] value - Element do sprawdzenia.
        @return Ww. liczba.
     */
    int strset_test(unsigned long id, const char *value);

    /** @brief Funkcja usuwająca podany zbiór.
        Funkcja usuwa zbiór @param id. Jeśli podany zbiór nie istnieje, to nic się nie dzieje.
        @param[in] id - Identyfiaktor zbioru.
     */
    void strset_clear(unsigned long id);

    /** @brief Funkcja porównująca zbiory.
        Funkcja porównuje ze sobą zbiory @param id1 i @param id2. Zbiory porównywane są
        poprzez porównywanie ich elementów posortowanych w porządku leksykograficznym.
        Jeśli któryś z podanych zbiorów nie istnieje, to jest traktowany jako zbiór pusty.
        Niech operacja sorted(id) oznacza posortowanie elementów zbioru id w porządku
        leksykograficznym.
        @param[in] id1, id2 - Identyfikatory zbiorów.
        @return -1, gdy sorted(id1) < sorted(id2),
                0, gdy sorted(id1) = sorted(id2),
                1, gdy sorted(id1) > sorted(id2).
     */
    int strset_comp(unsigned long id1, unsigned long id2);

#ifdef __cplusplus
    };

}
#endif // _cplusplus

#endif // STRSET_H