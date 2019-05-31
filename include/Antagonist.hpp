#ifndef ANTAGONIST_H
#define ANTAGONIST_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ANTAGONIST_SPRITE "assets/img/idle_terry.png"

#include <string>
#include "Component.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "Timer.hpp"

class Antagonist : public Component {
 public:
  Antagonist(GameObject& associated, Vec2 position);

  ~Antagonist();

  void Start() override;

  void NotifyCollision(GameObject& other) override;

  void Update(float dt) override;

  bool Is(GameData::Types type) const override;

  void Render() override;

  const GameData::Types Type = GameData::Types::Antagonist;

 private:
  Vec2 position;

  std::vector<Vec2> path;

  Timer timer1;

  Timer timer2;

  int i;
};

#endif
