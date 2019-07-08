#ifndef DIALOGUE_H
#define DIALOGUE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <string>
#include "Component.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"
#include "Types.hpp"

class Dialogue : public Component {
 public:
  Dialogue(GameObject& associated, const std::string& interlocutor,
           const std::string& text);

  ~Dialogue();

  void Start() override;

  void NotifyCollision(std::shared_ptr<GameObject> other) override;

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render() override;

  const Types Type = DialogueType;

 private:
  std::shared_ptr<GameObject> interlocutorGo;
  std::shared_ptr<GameObject> textGo;
};

#endif
