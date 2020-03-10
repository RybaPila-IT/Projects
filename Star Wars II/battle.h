#ifndef STARWARS2_BATTLE_H
#define STARWARS2_BATTLE_H

#include "rebelfleet.h"
#include "imperialfleet.h"
#include <vector>
#include <cstddef>
#include <memory>
#include <cassert>

using Time = int;

class Clock {
  Time currentTime;
  Time maxTime;

 public:
  Clock(Time t0, Time t1);

  bool isAttackTime();

  void shiftTime(Time timeStep);
};

class SpaceBattle {

 private:
  std::vector<std::shared_ptr<RebelStarship>> rebels;
  std::vector<std::shared_ptr<ImperialStarship>> imperials;
  Clock clock;

  void iterateAttacks();

 public:

  SpaceBattle(std::vector<std::shared_ptr<RebelStarship>> rebels,
              std::vector<std::shared_ptr<ImperialStarship>> imperials,
              Time t0, Time t1);

  size_t countRebelFleet();

  size_t countImperialFleet();

  void tick(Time timeStep);

  class Builder {

   private:
    std::vector<std::shared_ptr<RebelStarship>> rebels;
    std::vector<std::shared_ptr<ImperialStarship>> imperials;
    Time t0;
    Time t1;
    Clock clock;

   public:
    Builder() : rebels(), imperials(), t0(-1), t1(-1), clock(t0, t1) {};

    Builder &ship(XWing &xwing) {
      rebels.push_back(std::make_shared<XWing>(xwing));
      return *this;
    }

    Builder &ship(StarCruiser &cruiser) {
      rebels.push_back(std::make_shared<StarCruiser>(cruiser));
      return *this;
    }

    Builder &ship(Explorer &explorer) {
      rebels.push_back(std::make_shared<Explorer>(explorer));
      return *this;
    }

    Builder &ship(DeathStar &deathStar) {
      imperials.push_back(std::make_shared<DeathStar>(deathStar));
      return *this;
    }

    Builder &ship(ImperialDestroyer &destroyer) {
      imperials.push_back(std::make_shared<ImperialDestroyer>(destroyer));
      return *this;
    }

    Builder &ship(TIEFighter &fighter) {
      imperials.push_back(std::make_shared<TIEFighter>(fighter));
      return *this;
    }

    Builder &ship(Squadron &squadron) {
      imperials.push_back(std::make_shared<Squadron>(squadron));
      return *this;
    }

    Builder &startTime(Time begin) {
      t0 = begin;
      return *this;
    }

    Builder &maxTime(Time end) {
      t1 = end;
      return *this;
    }

    SpaceBattle build() {

        assert( t0 >=0 && t1 >= t0);

      return SpaceBattle(this->rebels, this->imperials,
                         this->t0, this->t1);
    }
  };
};

void attack(std::shared_ptr<ImperialStarship> imperial,
            std::shared_ptr<RebelStarship> rebel);

#endif //STARWARS2_BATTLE_H
