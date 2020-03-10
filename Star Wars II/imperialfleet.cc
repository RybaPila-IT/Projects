#include <numeric>
#include <utility>
#include <iostream>
#include "imperialfleet.h"
#include <cassert>

/** IMPERIALSTARSHIP AND ITS IMPLEMENTATION */

ImperialStarship::ImperialStarship(ShieldPoints shield, AttackPower power) :
    Ship(shield), attackPower(power) {
    assert(power >= 0);
}

AttackPower ImperialStarship::getAttackPower() { return attackPower; }

size_t ImperialStarship::numberOfShips() const {
  return 1;
}

/** DEATHSTAR AND ITS IMPLEMENTATION */

DeathStar::DeathStar(ShieldPoints shield, AttackPower power) :
    ImperialStarship(shield, power) {}

DeathStar createDeathStar(ShieldPoints shield, AttackPower power) {
  return DeathStar(shield, power);
}

/** IMPERIALDESTROYER AND ITS IMPLEMENTATION */

ImperialDestroyer::ImperialDestroyer(ShieldPoints shield, AttackPower power) :
    ImperialStarship(shield, power) {}

ImperialDestroyer
createImperialDestroyer(ShieldPoints shield, AttackPower power) {
  return ImperialDestroyer(shield, power);
}

/** TIEFighter AND ITS IMPLEMENTATION */

TIEFighter::TIEFighter(ShieldPoints shield, AttackPower power) :
    ImperialStarship(shield, power) {}

TIEFighter createTIEFighter(ShieldPoints shield, AttackPower power) {
  return TIEFighter(shield, power);
}

ShieldPoints accumulateShield(std::vector<ImperialStarship> ships) {
  ShieldPoints health = 0;
  for (auto ship: ships)
    health += ship.getShield();
  return health;
}

AttackPower accumulateAttackPower(std::vector<ImperialStarship> ships) {
  AttackPower attack = 0;
  for (auto ship: ships) {
    if (ship.getShield() != 0)
      attack += ship.getAttackPower();
  }
  return attack;
}

/** SQUADRON AND ITS IMPLEMENTATION */

Squadron::Squadron(std::vector<ImperialStarship> vector) : ImperialStarship(
    accumulateShield(vector), accumulateAttackPower(vector)) {
  for (auto ship:vector) {
    ships.push_back(std::make_shared<ImperialStarship>(ship));
  }
}

Squadron::Squadron(std::initializer_list<ImperialStarship> list)
    : ImperialStarship(accumulateShield(list),
                       accumulateAttackPower(list)) {
  for (auto ship:list) {
    ships.push_back(std::make_shared<ImperialStarship>(ship));
  }
}

ShieldPoints Squadron::getShield() {
  return shieldPoints;
}

AttackPower Squadron::getAttackPower() {
  return attackPower;
}

void Squadron::takeDamage(AttackPower damage) {
  auto ship = ships.begin();
  while (ship != ships.end()) {
    shieldPoints -= (*ship)->getShield();
    (*ship)->takeDamage(damage);
    shieldPoints += (*ship)->getShield();
    if ((*ship)->isDestroyed()) {
      attackPower -= (*ship)->getAttackPower();
      ship = ships.erase(ship);
    } else
      ship++;
  }
}

size_t Squadron::numberOfShips() const {
  return ships.size();
}

Squadron createSquadron(std::initializer_list<ImperialStarship> list) {
  return Squadron(list);
}

Squadron createSquadron(std::vector<ImperialStarship> vector) {
  return Squadron(std::move(vector));
}
