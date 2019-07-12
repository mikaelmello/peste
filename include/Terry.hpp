#ifndef TERRY_H
#define TERRY_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <string>
#include "Component.hpp"
#include "GameObject.hpp"
#include "Timer.hpp"
#include "Types.hpp"

class Terry : public Component {
 public:
  Terry(GameObject& associated, const std::string& file, Vec2 position);

  ~Terry();

  void Start() override;

  void NotifyCollision(std::shared_ptr<GameObject> other) override;

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render() override;

  void ShowTalkDialog();
  void HideTalkDialog();

  void Talk();

  void SetAnimation(int frameCount, float frameTime);

  void SetSprite(const std::string& file);

  const Types Type = TerryType;

 private:
  Vec2 position;

  bool colliding;

  std::shared_ptr<GameObject> blocker_go;
  std::shared_ptr<GameObject> priorityChanger_go;
  std::shared_ptr<GameObject> talkMessageGo;
};

#endif
