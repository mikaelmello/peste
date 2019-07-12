#ifndef PLAYER_H
#define PLAYER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <string>
#include "Component.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"
#include "Sound.hpp"
#include "Sprite.hpp"
#include "Types.hpp"

#define PLAYER_WALKING_SOUND "assets/audio/hope/walking.wav"
#define PLAYER_RUNNING_SOUND "assets/audio/hope/running.wav"
#define PLAYER_YAWN_SOUND_1 "assets/audio/hope/yawn1.wav"
#define PLAYER_YAWN_SOUND_2 "assets/audio/hope/yawn2.wav"
#define PLAYER_YAWN_SOUND_3 "assets/audio/hope/yawn3.wav"

#define PLAYER_FRONT_ANIM "assets/img/hope/front_anim.png"
#define PLAYER_BACK_ANIM "assets/img/hope/back_anim.png"
#define PLAYER_LEFT_ANIM "assets/img/hope/left_anim.png"
#define PLAYER_RIGHT_ANIM "assets/img/hope/right_anim.png"

#define PLAYER_FRONT "assets/img/hope/front.png"
#define PLAYER_BACK "assets/img/hope/back.png"
#define PLAYER_LEFT "assets/img/hope/left.png"
#define PLAYER_RIGHT "assets/img/hope/right.png"

#define PLAYER_FRONT_ANIM_LAMP "assets/img/hope/front_anim_lamp.png"
#define PLAYER_BACK_ANIM_LAMP "assets/img/hope/back_anim_lamp.png"
#define PLAYER_LEFT_ANIM_LAMP "assets/img/hope/left_anim_lamp.png"
#define PLAYER_RIGHT_ANIM_LAMP "assets/img/hope/right_anim_lamp.png"

#define PLAYER_FRONT_LAMP "assets/img/hope/front_lamp.png"
#define PLAYER_BACK_LAMP "assets/img/hope/back_lamp.png"
#define PLAYER_LEFT_LAMP "assets/img/hope/left_lamp.png"
#define PLAYER_RIGHT_LAMP "assets/img/hope/right_lamp.png"

class Player : public Component {
 public:
  Player(GameObject& associated, Vec2 position);

  ~Player();

  void Start() override;

  void NotifyCollision(std::shared_ptr<GameObject> other) override;

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render() override;

  const Types Type = PlayerType;

  Vec2 position;

  bool leaveBasement = false;

  bool lamp = false;

 private:
  const double speed = 30;

  int frameCount;

  float frameTime;

  bool running = false;

  Timer sleepTimer;

  Timer walkSoundTimer;

  void OpenIdleSprite(const std::shared_ptr<Sprite>& sprite,
                      Helpers::Direction lastDirection);

  void OpenWalkingSprite(const std::shared_ptr<Sprite>& sprite,
                         Helpers::Direction direction);

  Helpers::Direction lastDirection;

  std::shared_ptr<GameObject> priorityChanger_go;

  std::vector<Helpers::KeyType> keys;

  std::shared_ptr<Sound> sound;

  std::shared_ptr<Sound> walkingSound;
};

#endif
