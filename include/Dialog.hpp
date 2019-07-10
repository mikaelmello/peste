#ifndef DIALOG_H
#define DIALOG_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <string>
#include "Component.hpp"
#include "GameObject.hpp"
#include "Types.hpp"

typedef std::vector<std::pair<std::string, std::string>> SCRIPT_TYPE;

class Dialog : public Component {
 public:
  Dialog(GameObject& associated);

  ~Dialog();

  void Start() override;

  void NotifyCollision(std::shared_ptr<GameObject> other) override;

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render() override;

  void SetScript(SCRIPT_TYPE script);

  const Types Type = DialogType;

 private:
  void Clear();

  void UpdateTexts();
  uint32_t dialogIndex;
  SCRIPT_TYPE script;

  std::shared_ptr<GameObject> interlocutorGo;
  std::shared_ptr<GameObject> textGo;
};

#endif
