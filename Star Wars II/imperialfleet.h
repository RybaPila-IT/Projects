#ifndef STARWARS2_IMPERIALFLEET_H
#define STARWARS2_IMPERIALFLEET_H

#include "rebelfleet.h"
#include <vector>
#include <cstddef>
#include <memory>

class ImperialStarship : public Ship {

 protected:
  AttackPower attackPower;

  ImperialStarship(ShieldPoints shield, AttackPower power);

 public:
  virtual AttackPower getAttackPower();

  virtual size_t numberOfShips() const;
};


class DeathStar : public ImperialStarship {

 public:
  DeathStar(ShieldPoints shield, AttackPower power);
};

class ImperialDestroyer : public ImperialStarship {

 public:
  ImperialDestroyer(ShieldPoints shield, AttackPower power);
};


class TIEFighter : public ImperialStarship {

 public:
  TIEFighter(ShieldPoints shield, AttackPower power);
};


class Squadron : public ImperialStarship {

 private:
  std::vector<std::shared_ptr<ImperialStarship>> ships;

 public:
  Squadron(std::initializer_list<ImperialStarship> list);

  Squadron(std::vector<ImperialStarship> vector);

  ShieldPoints getShield() override;

  AttackPower getAttackPower() override;

  void takeDamage(AttackPower damage) override;

  size_t numberOfShips() const override;

};

DeathStar createDeathStar(ShieldPoints shield, AttackPower power);

ImperialDestroyer
createImperialDestroyer(ShieldPoints shield, AttackPower power);

TIEFighter createTIEFighter(ShieldPoints shield, AttackPower power);

Squadron createSquadron(std::initializer_list<ImperialStarship> list);

Squadron createSquadron(std::vector<ImperialStarship> vector);

#endif //STARWARS2_IMPERIALFLEET_H
