#ifndef STARWARS_BATTLE_H
#define STARWARS_BATTLE_H


#include <type_traits>
#include <tuple>
#include <iostream>
#include <algorithm>
#include "rebelfleet.h"
#include "imperialfleet.h"

/* Funkcja wylicza w czasie kompilacji liczbę kwadratów liczb całkowitych(naturalnych)
   mieszczących się w zakresie t1 tzn. takich które są mniejsze równe t1.
   */
template <typename T>
constexpr size_t squareNumbers(T t1){

    size_t upperBound = (size_t)(t1);
    size_t beginning = 0;
    size_t amount = 0;

    while(beginning*beginning <= upperBound){
        amount++;
        beginning++;
    }

    return amount;
}



template <typename T, T t0, T t1, class... S>
class SpaceBattle{

    //Statyczne asercje sprawdzające poprawność podanych wartości szablonowych
    static_assert(std::is_integral<T>::value, "Illegal time representation");
    static_assert( t0 < t1, "Negative battle duration");
    static_assert( 0 < t1, "Illegal cycle length");

public:
    SpaceBattle(S... ships): spaceships(ships...)  {
        currTime = t0;
    }

    size_t countImperialFleet(){ return unwrapTuple(spaceships, std::index_sequence_for<S...>(), 1);}

    size_t countRebelFleet(){ return unwrapTuple(spaceships, std::index_sequence_for<S...>(), 0);}

    void tick(T timeStep) {
        if (countImperialFleet() == 0 && countRebelFleet() == 0) {
            std::cout<<"DRAW\n";
        }
        else if (countImperialFleet() == 0) {
            std::cout<<"REBELLION WON\n";
        }
        else if (countRebelFleet() == 0) {
            std::cout<<"IMPERIUM WON\n";
        }
        else if (isASquare()) {
            performAllAttacks(spaceships, std::index_sequence_for<S...>());
        }
        increaseTime(timeStep);
    }


private:

    constexpr static size_t possibleAttacksAmount(){
        return size_t(squareNumbers(t1));
    }

    constexpr static std::array<T, possibleAttacksAmount() > determineBattleTimes(){

        std::array<T, size_t ( possibleAttacksAmount() ) > attackTimes = std::array<T, size_t ( possibleAttacksAmount() ) >();
        size_t idx = 0;
        T value = 0;

        while(idx < attackTimes.size()){
            attackTimes[idx] = value*value;
            value++;
            idx++;
        }

        return attackTimes;
    }

    //Prywatne parametry
    std::array<T, possibleAttacksAmount() > battleTimes = determineBattleTimes();
    std::tuple<S...> spaceships;
    T currTime;


    /*Prywatne metoda "odpakowywująca" krotkę statków przy pomocy std::index_sequence */
    template <size_t... Is>
    size_t unwrapTuple(const std::tuple<S...>& spaceships, std::index_sequence<Is...>, int who){


        if(who == 1)
            return checkImperialType(std::get<Is>(spaceships)...);
        else
            return checkRebelType(std::get<Is>(spaceships)...);
    }

    bool isASquare() {
        return std::binary_search(battleTimes.begin(), battleTimes.end(), currTime);
    }

    /*Funkcja sprawdzająca, czy podany statek ship jest statkiem imperium,
     wywołuje się rekurencyjnie dla kolejnych statków */
    template <typename I, typename... R>
    size_t checkImperialType(I ship, R... ships){

        if( isImperial(ship) && ship.getShield() > 0)
            return 1 + checkImperialType(ships...);
        else
            return checkImperialType(ships...);

    }

    /*Funkcja sprawdzająca, czy podany statek ship jest statkiem rebelii,
     wywołuje się rekurencyjnie dla kolejnych statków */
    template <typename D, typename... R>
    size_t checkRebelType(D ship, R... ships){

        if(isRebel(ship) && ship.getShield() > 0)
            return 1 + checkRebelType(ships...);
        else
            return checkRebelType(ships...);

    }

    //Przypadek gdy listy są już puste
    size_t checkImperialType(){
        return 0;
    }

    //Przypadek gdy listy są już puste
    size_t checkRebelType(){
        return 0;
    }

    /* Prwyatna metoda sprawdzająca czy podany statek jest statkiem rebelii,
     * Jest parametryzowana typem staku rebelii.
     * */
    template <typename U, int minSpeed, int maxSpeed, bool canAttack>
    bool isRebel(RebelStarship<U, minSpeed, maxSpeed, canAttack> rebelShip) {

        if(rebelShip.getShield() == 0)
            return true;

        return true;
    }

    /* Prwyatna metoda sprawdzająca czy podany statek jest imperium,
     * Jest parametryzowana typem staku rebelii.
     * */
    template <typename U, int minSpeed, int maxSpeed, bool canAttack>
    bool isImperial(RebelStarship<U, minSpeed, maxSpeed, canAttack> rebelShip) {

        if(rebelShip.getShield() == 0)
            return false;

        return false;
    }

    template <typename U >
    bool isRebel(ImperialStarship<U> imperialShip) {

        if(imperialShip.getShield() == 0)
            return false;

        return false;
    }

    template <typename U>
    bool isImperial(ImperialStarship<U> imperialShip) {

        if(imperialShip.getShield() == 0)
            return true;

        return true;
    }

    /* Specjalizacja ogólnej funkcji performAttacksOfAShip*/
    template <typename U, int minSpeed, int maxSpeed, bool canAttack, typename J, typename... R>
    void performAttacksOfAShip(RebelStarship<U, minSpeed, maxSpeed, canAttack> rebelShip, J ship, R... ships) {

        if(ship.getShield() == 0)

        performAttacksOfAShip(rebelShip, ships...);
        return;
    }

    /*Prywatna metoda wykonywująca ataki w bitwie */
    template <typename I, typename J, typename... R>
    void performAttacksOfAShip(I& imperialShip, J& ship, R&... ships) {
        if (isRebel(ship) &&
            imperialShip.getShield() != 0 &&
            ship.getShield() != 0) {
            attack(imperialShip, ship);
        }
        performAttacksOfAShip(imperialShip, ships...);
    }

    template <typename I>
    void performAttacksOfAShip(I& imperialShip) {

        if(imperialShip.getShield() == 0)

        return;
    }

    template <typename I, size_t... Is>
    void performAllAttacksOfAShip(I& imperialShip, std::tuple<S...>& spaceships, std::index_sequence<Is...>) {
        performAttacksOfAShip(imperialShip, std::get<Is>(spaceships)...);
    }

    template <typename J, typename... R>
    void performAttacks(J& ship, R&... ships) {
        performAllAttacksOfAShip(ship, spaceships, std::index_sequence_for<S...>());
        performAttacks(ships...);
    }

    void performAttacks() {
        return;
    }

    template <size_t... Is>
    void performAllAttacks(std::tuple<S...>& spaceships, std::index_sequence<Is...>) {
        performAttacks(std::get<Is>(spaceships)...);
    }

    /* Metoda zwiększająca czas o timeStep */
    void increaseTime(T timeStep) {
        T step = timeStep % t1;
        if (t1 - step > currTime) {
            currTime += step;
        }
        else {
            currTime -= (t1 - step);
        }
    }



};


#endif //STARWARS_BATTLE_H