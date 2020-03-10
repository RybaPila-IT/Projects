#include "rebelfleet.h"
#include <cassert>
#include <algorithm>

static const long long StarCruiserMinSpeed = 99999;
static const long long StarCruiserMaxSpeed = 299795;
static const long long OthersMinSpeed = 299796;
static const long long OthersMaxSpeed = 2997960;

/** SHIP AND ITS IMPLEMENTATION */

Ship::Ship(ShieldPoints shield) : shieldPoints(shield) {
    assert(shield >= 0);
}

ShieldPoints Ship::getShield() { return shieldPoints; }

void Ship::takeDamage(AttackPower damage) {
  shieldPoints = std::max(0, shieldPoints - (ShieldPoints) damage);
}

bool Ship::isDestroyed() {
  return shieldPoints == 0;
}

/** REBELSTARSHIP AND ITS IMPLEMENTATION */

RebelStarship::RebelStarship(ShieldPoints shield, Speed speed) :
    Ship(shield), speed(speed) {
    assert(speed >= 0);
}

Speed RebelStarship::getSpeed() { return speed; }

/** XWING AND ITS IMPLEMENTATION */

XWing::XWing(ShieldPoints shield, Speed speed, AttackPower power) :
    RebelStarship(shield, speed), attackPower(power) {
  assert(OthersMinSpeed <= speed && speed <= OthersMaxSpeed);
}

AttackPower XWing::getAttackPower() { return attackPower; }

void XWing::answerAttack(std::shared_ptr<Ship> ship) {
  ship->takeDamage(this->getAttackPower());
}

XWing createXWing(ShieldPoints shield, Speed speed, AttackPower power) {
  return XWing(shield, speed, power);
};

/** EXPLORER AND ITS IMPLEMENTATION */

Explorer::Explorer(ShieldPoints shield, Speed speed) : RebelStarship(shield,
                                                                     speed) {
  assert(OthersMinSpeed <= speed && speed <= OthersMaxSpeed);
}

void Explorer::answerAttack(std::shared_ptr<Ship>) {}

Explorer createExplorer(ShieldPoints shield, Speed speed) {
  return Explorer(shield, speed);
}

/** STARCRUISER AND ITS IMPLEMENTATION */

StarCruiser::StarCruiser(ShieldPoints shield, Speed speed, AttackPower power) :
    RebelStarship(shield, speed), attackPower(power) {
  assert(StarCruiserMinSpeed <= speed && speed <= StarCruiserMaxSpeed);
}

void StarCruiser::answerAttack(std::shared_ptr<Ship> ship) {
  ship->takeDamage(this->getAttackPower());
}

AttackPower StarCruiser::getAttackPower() { return attackPower; }

StarCruiser
createStarCruiser(ShieldPoints shield, Speed speed, AttackPower power) {
  return StarCruiser(shield, speed, power);
}