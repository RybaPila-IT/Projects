#include <iostream>
#include <utility>
#include <numeric>
#include "battle.h"

SpaceBattle::SpaceBattle(
    std::vector<std::shared_ptr<RebelStarship>> rebels,
    std::vector<std::shared_ptr<ImperialStarship>> imperials, Time t0, Time t1)
    :
    rebels(std::move(rebels)), imperials(std::move(imperials)), clock(t0, t1) {}

size_t SpaceBattle::countRebelFleet() {
  return rebels.size();
}

size_t SpaceBattle::countImperialFleet() {
  size_t count = 0;
  for (const auto &imperial:imperials) {
    count += imperial->numberOfShips();
  }
  return count;
}

void attack(std::shared_ptr<ImperialStarship> imperial,
            std::shared_ptr<RebelStarship> rebel) {
  rebel->takeDamage(imperial->getAttackPower());
  rebel->answerAttack(imperial);
}

void SpaceBattle::tick(Time timeStep) {
  if (countImperialFleet() + countRebelFleet() == 0)
    std::cout << "DRAW\n";
  else if (countImperialFleet() == 0)
    std::cout << "REBELLION WON\n";
  else if (countRebelFleet() == 0)
    std::cout << "IMPERIUM WON\n";
  if (clock.isAttackTime()) {
    iterateAttacks();
  }
  clock.shiftTime(timeStep);
}

void SpaceBattle::iterateAttacks() {
  auto imperial = imperials.begin();
  while (imperial != imperials.end()) {
    auto rebel = rebels.begin();
    while (rebel != rebels.end()) {
      if (!(*rebel)->isDestroyed() && !(*imperial)->isDestroyed()) {
        attack(*imperial, *rebel);
      }
      if ((*rebel)->isDestroyed()) {
        rebel = rebels.erase(rebel);
      } else
        rebel++;
      if ((*imperial)->isDestroyed()) {
        imperial = imperials.erase(imperial);
        break;
      } else if (rebel == rebels.end())
        imperial++;
    }
  }
}

bool Clock::isAttackTime() {
  return (currentTime % 5) != 0 &&
         ((currentTime % 2) == 0 || (currentTime % 3) == 0);
}

void Clock::shiftTime(Time timeStep) {
  currentTime = (currentTime + timeStep) % (maxTime + 1);
}

Clock::Clock(Time t0, Time t1) : currentTime(t0), maxTime(t1) {};