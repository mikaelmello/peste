#ifndef ANTAGONIST_H
#define ANTAGONIST_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ANTAGONIST_SPRITE "assets/img/static_terry.png"

#include <string>
#include "Component.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"

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
};

#endif
