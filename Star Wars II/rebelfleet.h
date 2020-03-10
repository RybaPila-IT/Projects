#ifndef STARWARS2_REBELFLEET_H
#define STARWARS2_REBELFLEET_H

#include <memory>

using ShieldPoints = int;
using Speed = int;
using AttackPower = int;

class Ship {

 protected:
  Ship(ShieldPoints shield);

  ShieldPoints shieldPoints;

 public:
  virtual ShieldPoints getShield();

  virtual void takeDamage(AttackPower damage);

  bool isDestroyed();
};

class RebelStarship : public Ship {

 protected:
  Speed speed;

  RebelStarship(ShieldPoints shield, Speed speed);

 public:
  Speed getSpeed();

  virtual void answerAttack(std::shared_ptr<Ship>) {};

  virtual ~RebelStarship() {}
};

class XWing : public RebelStarship {

 private:
  AttackPower attackPower;

 public:
  XWing(ShieldPoints shield, Speed speed, AttackPower power);

  AttackPower getAttackPower();

  void answerAttack(std::shared_ptr<Ship> ship) override;
};

class Explorer : public RebelStarship {

 public:
  Explorer(ShieldPoints shield, Speed speed);

  void answerAttack(std::shared_ptr<Ship>) override;
};

class StarCruiser : public RebelStarship {

 private:
  AttackPower attackPower;

 public:
  StarCruiser(ShieldPoints shield, Speed speed, AttackPower power);

  AttackPower getAttackPower();

  void answerAttack(std::shared_ptr<Ship> ship) override;

};

Explorer createExplorer(ShieldPoints shield, Speed speed);

XWing createXWing(ShieldPoints shield, Speed speed, AttackPower power);

StarCruiser
createStarCruiser(ShieldPoints shield, Speed speed, AttackPower power);

#endif //STARWARS2_REBELFLEET_H