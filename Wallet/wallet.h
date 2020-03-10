#ifndef WALLET_WALLET_H
#define WALLET_WALLET_H


#include <iostream>
#include <string>
#include <chrono>
#include <list>
#include <cstring>

/** @brief Klasa reprezentująca operację wykonaną na portfelu.
    Klasa Operation posiada trzy prywatne atrybuty. Są to:

    std::chrono::high_resolution_clock::time_point date - Punkt w czasie reprezetujący
                       moment utworzenia operacji. Data z tego obiektu otrzymywana
                       jest dzięki metodzie getDate(). Zwracana data jest w formacie
                       YYYY-MM-DD.
    unsigned long long accountStatus - Liczba monet znajdujących się w
                       w portefelu w momencie zakończenia operacji.
    unsigned long long created - czas (przechowywany w milisekundach)
                       wykonania operacji i tym samym utworzenia obiektu
                       klasy Operation.

    Klasa udostępnia wszystkie wyspecyfikowane w treści zadania operatory
    porównujące oraz wypisujące na standardowe wyjście diagnostyczne informacje
    o danej operacji. W szczególności klasa Operation nie udostępnia żadnej
    metody modyfikującej.
*/
class Operation {

private:
    std::chrono::high_resolution_clock::time_point date;
    unsigned long long accountStatus;
    unsigned long long created;
    std::string getDate() const;

public:
    explicit Operation(unsigned long long accountStatus);
    const unsigned long long& getUnits() const;
    bool operator== (const Operation &o) const;
    bool operator!= (const Operation &o) const;
    bool operator<  (const Operation &o) const;
    bool operator<= (const Operation &o) const;
    bool operator>  (const Operation &o) const;
    bool operator>= (const Operation &o) const;
    friend std::ostream& operator<<(std::ostream& os, const Operation &o);
};



/** @brief Klasa reprezentująca portfel
    Klasa Wallet reprezentuje portfel przechowywujący Bajtkomonety. Posiada tzy
    prywatne atrybuty. Są to:

    unsigned long long Value - liczba reprezentująca liczbę Bajtkomonet aktualnie
                        znajdujących się w portfelu. W szczególności informacja
                        przechowywana jest w następujący sposób: 1B = 100000000
                        jednostek zapisanych pod polem Value.
    std::list< Opertion > operationsHistory - lista operacji wykonanych na portfelu
                        posortowana względem czasu wykonania operacji (względem
                        milisekundy wykonania operacji).

    Klasa wallet udostępnia wszystkie operatory i operacje opisane w treści zadania.
*/
class Wallet {

private:
    unsigned long long Value;
    std::list< Operation > operationsHistory;
    explicit Wallet(unsigned long long Value);

public:
    Wallet(int Value);
    Wallet();
    Wallet(Wallet&& w);
    Wallet(Wallet&& w1, Wallet&& w2);
    explicit Wallet(std::string& str);
    explicit Wallet(const char* _str);
    static Wallet fromBinary(std::string& str);
    static Wallet fromBinary(const char* _str);


    template <class T>
    Wallet(T) = delete;

    const unsigned long long int & getUnits() const;
    unsigned long long opSize() const;
    Operation operator[] (int k) const;

    Wallet& operator= (Wallet&& w);

    Wallet& operator+= (Wallet& w);
    Wallet& operator+= (Wallet&& w);

    Wallet& operator-= (Wallet& w);
    Wallet& operator-= (Wallet&& w);

    Wallet& operator*= (int n);

    friend Wallet operator+ (Wallet&& w1, Wallet&& w2);
    friend Wallet operator+ (Wallet&& w1, Wallet& w2);

    friend Wallet operator- (Wallet&& w1, Wallet&& w2);
    friend Wallet operator- (Wallet&& w1, Wallet& w2);

    Wallet operator* (int n);
    friend Wallet operator* (int n, Wallet&& w);
    friend Wallet operator* (int n, Wallet& w);

    friend std::ostream& operator<<(std::ostream& os, const Wallet &w);
};

/** Zbiór operatorów porównujących obiekty klasy Wallet */
bool operator== (const Wallet& w1, const Wallet& w2);
bool operator!= (const Wallet& w1, const Wallet& w2);
bool operator< (const Wallet& w1, const Wallet& w2);
bool operator<= (const Wallet& w1, const Wallet& w2);
bool operator> (const Wallet& w1, const Wallet& w2);
bool operator>= (const Wallet& w1, const Wallet& w2);

/** Funkcja wyznaczająca pusty portfel */
const Wallet& Empty();

#endif //WALLET_WALLET_H