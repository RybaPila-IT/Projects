#ifndef STARWARS_IMPERIALFLEET_H
#define STARWARS_IMPERIALFLEET_H

#include <iostream>
#include <list>
#include <typeinfo>
#include <type_traits>
#include "rebelfleet.h"

template <typename U>
class ImperialStarship{
private:
    U shield;
    U attackPower;

public:

    using valueType = U;

    explicit ImperialStarship(U shield, U attackPower): shield(shield), attackPower(attackPower) {}
    const U getShield() const {return shield;}
    const U getAttackPower() const { return attackPower;}

    void takeDamage(U damage) {

        U result = shield-damage;

        if(result <= 0)
            shield = 0;
        else
            shield = result;

    }

};


template<class I, typename U >
void attack(I &imperialShip, Explorer<U>& rebelShip) {
    if (rebelShip.getShield() == 0 || imperialShip.getShield() == 0)
        return;
    else {
        rebelShip.takeDamage(imperialShip.getAttackPower());
    }
}

template<class I, typename U >
void attack(I &imperialShip, U& rebelShip) {
    if (rebelShip.getShield() == 0 || imperialShip.getShield() == 0)
        return;
    else {
        rebelShip.takeDamage(imperialShip.getAttackPower());
        imperialShip.takeDamage(rebelShip.getAttackPower());
    }
}

template <typename U>
        using DeathStar = ImperialStarship<U>;

template <typename U>
        using ImperialDestroyer = ImperialStarship<U>;

template <typename U>
        using TIEFighter = ImperialStarship<U>;


#endif //STARWARS_IMPERIALFLEET_H
