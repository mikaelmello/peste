#ifndef ACTIONMESSAGE_H
#define ACTIONMESSAGE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <string>
#include "Component.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"
#include "Types.hpp"

class ActionMessage : public Component {
 public:
  ActionMessage(GameObject& associated, Vec2 position, const std::string& file);

  ~ActionMessage();

  void Start() override;

  void NotifyCollision(std::shared_ptr<GameObject> other) override;

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render() override;

  void UpdateSprite(const std::string& file);

  const Types Type = ActionMessageType;

 private:
  std::shared_ptr<Sprite> sprite_ptr;
};

#endif
