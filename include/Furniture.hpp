#ifndef FURNITURE_H
#define FURNITURE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <string>
#include "Component.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"
#include "Types.hpp"

class Furniture : public Component {
 public:
  Furniture(GameObject& associated, const std::string& file, Vec2 position,
            Helpers::Interaction interaction, bool fullblock = true,
            const std::string& script = "");

  ~Furniture();

  void Start() override;

  void NotifyCollision(std::shared_ptr<GameObject> other) override;

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render() override;

  void ShowInteractionDialog();

  void HideInteractionDialog();

  Helpers::Interaction GetInteraction();

  void Look();

  const Types Type = FurnitureType;

 private:
  std::shared_ptr<GameObject> interactMsgGo;
  std::shared_ptr<GameObject> blockerGo;

  bool interact;
  bool colliding;

  Helpers::Interaction interaction;

  std::string script;
};

#endif
