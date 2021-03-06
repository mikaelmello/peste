#ifndef DOOR_H
#define DOOR_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <string>
#include "Blocker.hpp"
#include "Component.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"
#include "Types.hpp"
#include "Vec2.hpp"

class Door : public Component {
 public:
  Door(GameObject& associated, Helpers::DoorStyle style, Vec2 position,
       bool open, Helpers::KeyType key = Helpers::KeyType::NOKEY);

  ~Door();

  void Start() override;

  void NotifyCollision(std::shared_ptr<GameObject> other) override;

  void Update(float dt) override;

  void Open();

  void Close();

  bool IsOpen();

  bool Is(Types type) const override;

  void Render() override;

  Helpers::KeyType GetKey();

  void SetKey(Helpers::KeyType k);

  const Types Type = DoorType;

 private:
  void ShowOpenDialog();
  void HideOpenDialog();
  void ShowCloseDialog();
  void HideCloseDialog();

  Vec2 position;
  bool colliding;
  bool open;

  std::shared_ptr<GameObject> openDoorGo;
  std::shared_ptr<GameObject> closeDoorGo;
  std::shared_ptr<GameObject> blockerGo;

  std::shared_ptr<Blocker> blocker;

  std::string openPath;
  std::string closePath;

  Helpers::KeyType key;
};

#endif
