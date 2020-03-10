#include <iostream>
#include <type_traits>
#include <cassert>


#ifndef REBELFLEET_H
#define REBELFLEET_H

template <typename U, int minSpeed, int maxSpeed, bool canAttack>
class RebelStarship{

private:
    U shield;
    U speed;
    U attackPower;

public:

    using valueType = U;

    template <typename T = U, typename = std::enable_if_t<canAttack, T> >
        explicit RebelStarship(T shield, T speed, T attackPower): shield(shield), speed(speed), attackPower(attackPower) {
            assert(speed >= minSpeed && speed <= maxSpeed);
        };

    template <typename T = U, typename = std::enable_if_t<!canAttack, T> >
        explicit RebelStarship(T shield, T speed): shield(shield), speed(speed) {
            assert(speed >= minSpeed && speed <= maxSpeed);
        };

    template <typename T = U, typename = std::enable_if_t<canAttack, T> >
            const T getAttackPower() const { return attackPower;}

    const U getShield() const { return shield;}
    const U getSpeed() const { return speed;}

    void takeDamage(U damage) {

        U result = shield-damage;

        if(result <= 0)
            shield = 0;
        else
            shield = result;
    }

};


template <typename T>
    using Explorer = RebelStarship<T,299796, 2997960, false>;

template <typename T>
    using XWing = RebelStarship<T,299796, 2997960, true>;

template <typename T>
    using StarCruiser = RebelStarship<T, 99999, 299795, true>;

#endif //REBELFLEET_H
