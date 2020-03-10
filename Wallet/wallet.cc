#include "wallet.h"
#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <string>
#include <iostream>
#include <unistd.h>
#include <regex>
#include <exception>


namespace {

    /**Statyczna inicjalizacja punktu w czasie odpowiedzialnego za tzw. rozpoczęcie
       działanie programu.
       */
    std::chrono::high_resolution_clock::time_point& programBeginning() {

        static std::chrono::high_resolution_clock::time_point programBeginning =
                std::chrono::high_resolution_clock::from_time_t(0);

        return programBeginning;
    }

    //Dane formatu monet
    const unsigned long long UNITS_IN_B =   100000000;
    //Licznik dostępnych monet
    unsigned long long B_AVAILABLE = 2100000000000000;

    //Wzory dostępnych napisów
    const std::string strPattern = R"(^(\s*)(\d+)(\s*)$)";
    const std::string strDelimPattern =   R"(^(\s*)(\d+)([.,])(\d){1,8}(\s*)$)";
    const std::string strFromBinaryPattern =   R"(^([01]+)$)";
    const std::string strFromBinaryDelimPattern =   R"(^([01]+)([.,])[01]{1,8}$)";

    //Regex dostępnych napisów
    const std::regex strRegex(strPattern);
    const std::regex strDelimRegex(strDelimPattern);
    const std::regex strFromBinaryRegex(strFromBinaryPattern);
    const std::regex strFromBinaryDelimRegex(strFromBinaryDelimPattern);


    //Aktualny czas w dzialaniu programu
    std::chrono::high_resolution_clock::time_point returnTimestamp() {

        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();

        return now;
    }

    //Zwraca date w odpowiednim formacie
    std::string getDateFromeTime_T(std::chrono::high_resolution_clock::time_point now) {

        auto date = std::chrono::high_resolution_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&date), "%Y-%m-%d");
        std::string currentDate = ss.str();

        return currentDate;
    }

    //Zwraca aktualne milisekundy jako wynik odejmowania poczatku dzialania programu
    // od momentu utworzenia obiektu operation
    unsigned long long getMiliseconds(std::chrono::high_resolution_clock::time_point now) {

        unsigned long long milliseconds = (unsigned long long)
                std::chrono::duration_cast<std::chrono::milliseconds>(now - programBeginning() ).count();

        return milliseconds;
    }

    //Funkcja dodająca operacje do historii operacji obiektu klasy Wallet
    void addToHistory(std::list<Operation>& adding, std::list<Operation>& inserting) {

        for (auto &IT : inserting) {
            adding.push_back(IT);
        }

    }

    std::string getBalanceString(unsigned long long accountStatus) {
        unsigned long long B = accountStatus / UNITS_IN_B;
        std::string Bstring = std::to_string(B);

        unsigned long long afterComma = accountStatus - (B * UNITS_IN_B);
        std::string afterString = std::to_string(afterComma);

        while (afterString[afterString.length() - 1] == '0') {
            afterString.erase(afterString.length() - 1, 1);
        }
        if (afterString.empty())
            return Bstring;
        else
            return (Bstring + ',' + afterString);
    }

    void removeWhitespaces(std::string& str) {
        while (isspace(str[0])) {
            str.erase(0, 1);
        }
        while (isspace(str[str.length() - 1])) {
            str.erase(str.length() - 1, 1);
        }
    }

    unsigned long long getUnitsFromStr(std::string& str, int base) {
        removeWhitespaces(str);
        return stoull(str, nullptr, base)*UNITS_IN_B;
    }

    int getFractionFromDecimal(std::string& str) {
        int fractionPart;
        size_t zerosToAdd = 8 - str.length();
        fractionPart = stoi(str, nullptr, 10);
        for (size_t i = 0; i < zerosToAdd; i++) {
            fractionPart *= 10;
        }
        return fractionPart;
    }

    int getFractionFromBinary(std::string& str) {
        int fractionPart = 0;
        size_t len = str.length();
        int toAdd = 50000000;
        for (size_t i = 0; i < len; i++) {
            if (str[i] == '1')
                fractionPart += toAdd;
            toAdd /= 2;
        }
        return fractionPart;
    }

    unsigned long long getUnitsFromStrWithDelim(std::string& str, int base) {
        removeWhitespaces(str);

        size_t delimIdx;
        int wholePart = stoi(str, &delimIdx, base);

        std::string fractionString = str.substr(delimIdx+1, std::string::npos);
        int fractionPart;
        if (base == 10) {
            fractionPart = getFractionFromDecimal(fractionString);
        } else { // base = 2
            fractionPart = getFractionFromBinary(fractionString);
        }
        return ((wholePart * UNITS_IN_B) + fractionPart);
    }

}


//Statyczne wywołanie konstruktora wzoru pustego Wallet`a
const Wallet& Empty() {

    static Wallet empty;

    return empty;
}




/** KLASA OPERATION I IMPEMENTACJA JEJ METOD */

std::string Operation::getDate() const {

    std::string currentDate = getDateFromeTime_T(date);

    return currentDate;
}


Operation::Operation(unsigned long long accountStatus): accountStatus(accountStatus) {
    created = getMiliseconds( std::chrono::high_resolution_clock::now() );
    date = returnTimestamp();
}

const unsigned long long& Operation::getUnits() const {
    return accountStatus;
}


bool Operation::operator== (const Operation &o) const {
    return (this->created == o.created);
}

bool Operation::operator!= (const Operation &o) const {
    return !((*this) == o);
}

bool Operation::operator< (const Operation &o) const {
    return (this->created < o.created);
}

bool Operation::operator<= (const Operation &o) const {
    return ((*this) < o || (*this) == o);
}

bool Operation::operator> (const Operation &o) const {
    return !((*this) <= o);
}

bool Operation::operator>= (const Operation &o) const {
    return !((*this) < o);
}

std::ostream& operator<<(std::ostream& os, const Operation &o) {
    os << "Wallet balance is " << getBalanceString(o.accountStatus)
       << " B after operation made at day " << o.getDate();
    return os;
}




/** KLASA WALLET I IMPELEMENTACJA JEJ METOD */


Wallet::Wallet(unsigned long long Value) {

    if (B_AVAILABLE < Value) {
        throw std::runtime_error("Not enough B in the world");
    }
    else {
        B_AVAILABLE -= Value;
        this->Value = Value;
        this->operationsHistory = std::list<Operation> (1, Operation(Value));
    }
}

//Konstruktor z wartości
Wallet::Wallet(int Value) {

    if (B_AVAILABLE < Value*UNITS_IN_B) {
        throw std::runtime_error("Not enough B in the world");
    }
    else if (Value < 0) {
        throw std::runtime_error("Wrong number of B");
    }
    else {
        B_AVAILABLE -= Value*UNITS_IN_B;
        this->Value = Value*UNITS_IN_B;
        this->operationsHistory = std::list<Operation> (1, Operation(Value*UNITS_IN_B));
    }
}

//Konstruktor domyślny tzn. pusty
Wallet::Wallet(): Value(0),
                  operationsHistory(std::list <Operation>(1, Operation(0))){}

//Konstruktor przenoszący
Wallet::Wallet(Wallet&& w): Value(w.Value), operationsHistory( std::move( w.operationsHistory)) {
    operationsHistory.emplace_back(Value);
}


//Konstruktor sumujący
Wallet::Wallet(Wallet&& w1, Wallet&& w2): Value(w1.Value + w2.Value){

    operationsHistory = std::list<Operation>();
    std::list<Operation> w1operations = std::move(w1.operationsHistory);
    std::list<Operation> w2operations = std::move(w2.operationsHistory);
    addToHistory(operationsHistory, w1operations);
    addToHistory(operationsHistory, w2operations);
    operationsHistory.emplace_back(Value);
    operationsHistory.sort();
    w1.Value = 0;
    w2.Value = 0;
}

//Konstruktor z napisu
Wallet::Wallet(std::string& str) {
    unsigned long long val;
    if (regex_match(str, strRegex)) {
        val = getUnitsFromStr(str, 10);
        if (B_AVAILABLE < val) {
            throw std::runtime_error("Not enough B in the world");
        }
        B_AVAILABLE -= val;
        this->Value = val;
        this->operationsHistory = std::list<Operation> (1, Operation(this->Value));

    } else if (regex_match(str, strDelimRegex)) {
        val = getUnitsFromStrWithDelim(str, 10);
        if (B_AVAILABLE < Value) {
            throw std::runtime_error("Not enough B in the world");
        }
        B_AVAILABLE -= val;
        this->Value = val;
        this->operationsHistory = std::list<Operation> (1, Operation(this->Value));

    } else {
        throw std::runtime_error("Wrong argument of Wallet constructor");
    }
}

//Konstruktor z napisu
Wallet::Wallet(const char* _str) {
    std::string str = _str;
    unsigned long long val;
    if (regex_match(str, strRegex)) {
        val = getUnitsFromStr(str, 10);
        if (B_AVAILABLE < val) {
            throw std::runtime_error("Not enough B in the world");
        }
        B_AVAILABLE -= val;
        this->Value = val;
        this->operationsHistory = std::list<Operation> (1, Operation(this->Value));

    } else if (regex_match(str, strDelimRegex)) {
        val = getUnitsFromStrWithDelim(str, 10);
        if (B_AVAILABLE < val) {
            throw std::runtime_error("Not enough B in the world");
        }
        B_AVAILABLE -= val;
        this->Value = val;
        this->operationsHistory = std::list<Operation> (1, Operation(this->Value));

    } else {
        throw std::runtime_error("Wrong argument of Wallet constructor");
    }
}

//Metoda klasowa fromBinary
Wallet Wallet::fromBinary(std::string& str) {
    unsigned long long val;
    if (regex_match(str, strFromBinaryRegex)) {
        val = getUnitsFromStr(str, 2);
        return Wallet(val);

    } else if (regex_match(str, strFromBinaryDelimRegex)) {
        val = getUnitsFromStrWithDelim(str, 2);
        return Wallet(val);
    } else {
        throw std::runtime_error("Wrong argument of Wallet constructor");
    }
}

//Metoda klasowa fromBinary
Wallet Wallet::fromBinary(const char *_str) {
    std::string str = _str;
    unsigned long long val;
    if (regex_match(str, strFromBinaryRegex)) {
        val = getUnitsFromStr(str, 2);
        return Wallet(val);

    } else if (regex_match(str, strFromBinaryDelimRegex)) {
        val = getUnitsFromStrWithDelim(str, 2);
        return Wallet(val);
    } else {
        throw std::runtime_error("Wrong argument of Wallet constructor");
    }
}




//Liczba monet w obiekcie klasy Wallet
const unsigned long long int & Wallet::getUnits() const {
    return Value;
}

//Liczba operacji wykonanych na obiekcie klasy Wallet
unsigned long long Wallet::opSize() const {
    return operationsHistory.size();
}


//Operator dostępu do k-tej operacji
Operation Wallet::operator[] (int k) const {

    if (k >= (int)operationsHistory.size()) {
        throw std::runtime_error("Wallet doesn't have that operation");
    }

    auto IT = operationsHistory.begin();

    for (int i = 0; i < k; i++)
        ++IT;

    return (*IT);
}

//Operator przypisania
Wallet& Wallet::operator= (Wallet&& w){

    if (this == &w)
        return (*this);

    Value = w.Value;
    operationsHistory = std::move(w.operationsHistory);
    operationsHistory.emplace_back(Value);

    return (*this);
}

//Operator transferu pieniędzy z prawego do lewego portfela
Wallet& Wallet::operator+= (Wallet& w){

    if (this == &w)
        return (*this);

    Value += w.Value;
    w.Value = 0;
    w.operationsHistory.emplace_back(Operation(0));
    operationsHistory.emplace_back(Operation(Value));

    return (*this);
}

Wallet& Wallet::operator+= (Wallet&& w){

    Value += w.Value;
    operationsHistory.emplace_back(Operation(Value));

    return (*this);
}


//Operator transferu monet z lewego portfela do prawego
Wallet& Wallet::operator-= (Wallet& w){

    if (this == &w)
        return (*this);

    if (Value < w.Value) {
        throw std::runtime_error("Not enough B in the first Wallet");
    }

    Value -= w.Value;
    w.Value += w.Value;
    w.operationsHistory.emplace_back(Operation(w.Value));
    operationsHistory.emplace_back(Operation(Value));

    return (*this);
}

Wallet& Wallet::operator-= (Wallet&& w){

    if (Value < w.Value) {
        throw std::runtime_error("Not enough B in the first Wallet");
    }

    B_AVAILABLE += 2*w.Value;
    Value -= w.Value;
    operationsHistory.emplace_back(Operation(Value));

    return (*this);
}

//Operator mnożenia
Wallet& Wallet::operator*= (int n){

    if ((n > 0) && ((Value * (n-1)) > B_AVAILABLE)) {
        throw std::runtime_error("Not enough B in the world to multiply Wallet");
    }

    B_AVAILABLE -= Value*(n-1);
    Value *= n;
    operationsHistory.emplace_back(Operation(Value));

    return (*this);
}

//Operator sumowania
Wallet operator+ (Wallet&& w1, Wallet&& w2) {
    return Wallet(std::move(Wallet(w1.Value + w2.Value)));
}

Wallet operator+ (Wallet&& w1, Wallet& w2) {
    return Wallet(std::move(w1+=w2));
}

//Operator odejmowania
Wallet operator- (Wallet&& w1, Wallet&& w2) {
    return Wallet(std::move(Wallet(w1.Value - w2.Value)));
}

Wallet operator- (Wallet&& w1, Wallet& w2) {
    return Wallet(std::move(w1-=w2));
}


Wallet Wallet::operator* (int n) {
    if ((Value * (n-1)) > B_AVAILABLE) {
        throw std::runtime_error("Not enough B in the world to multiply Wallet");
    }

    B_AVAILABLE -= Value*(n-1);
    return Wallet(this->Value * n);
}

Wallet operator*(int n, Wallet &&w) {
    if ((w.Value * (n-1)) > B_AVAILABLE) {
        throw std::runtime_error("Not enough B in the world to multiply Wallet");
    }

    B_AVAILABLE -= w.Value*(n-1);
    return Wallet(w.Value * n);
}

Wallet operator* (int n, Wallet& w) {
    if ((w.Value * (n-1)) > B_AVAILABLE) {
        throw std::runtime_error("Not enough B in the world to multiply Wallet");
    }

    B_AVAILABLE -= w.Value*(n-1);
    return Wallet(w.Value * n);
}

//Wypisanie zawartości portfela(liczby monet)
std::ostream& operator<<(std::ostream& os, const Wallet &w) {
    os << "Wallet[" << getBalanceString(w.Value) << " B]";
    return os;
}

/** OPERATORY PORÓWNUJĄCE */


bool operator== (const Wallet &w1, const Wallet &w2) {
    return (w1.getUnits() == w2.getUnits());
}

bool operator!= (const Wallet& w1, const Wallet& w2) {
    return !(w1 == w2);
}

bool operator< (const Wallet& w1, const Wallet& w2) {
    return (w1.getUnits() < w2.getUnits());
}

bool operator<= (const Wallet& w1, const Wallet& w2) {
    return (w1 < w2) || (w1 == w2);
}

bool operator> (const Wallet& w1, const Wallet& w2) {
    return !(w1 <= w2);
}

bool operator>= (const Wallet& w1, const Wallet& w2) {
    return !(w1 < w2);
}