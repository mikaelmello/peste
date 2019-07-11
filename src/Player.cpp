#include "Player.hpp"
#include <iostream>
#include <memory>
#include <string>
#include "Blocker.hpp"
#include "Camera.hpp"
#include "CameraAction.hpp"
#include "Collider.hpp"
#include "Door.hpp"
#include "Furniture.hpp"
#include "Game.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "Item.hpp"
#include "Lore.hpp"
#include "PriorityChanger.hpp"
#include "SleepState.hpp"
#include "Sprite.hpp"
#include "TileMap.hpp"
#include "Types.hpp"

using namespace Helpers;

#define PLAYER_FRONT_ANIM "assets/img/hope/front_anim.png"
#define PLAYER_BACK_ANIM "assets/img/hope/back_anim.png"
#define PLAYER_LEFT_ANIM "assets/img/hope/left_anim.png"
#define PLAYER_RIGHT_ANIM "assets/img/hope/right_anim.png"

#define PLAYER_FRONT "assets/img/hope/front.png"
#define PLAYER_BACK "assets/img/hope/back.png"
#define PLAYER_LEFT "assets/img/hope/left.png"
#define PLAYER_RIGHT "assets/img/hope/right.png"

std::vector<std::string> YawnSounds = {
    PLAYER_YAWN_SOUND_1,
    PLAYER_YAWN_SOUND_2,
    PLAYER_YAWN_SOUND_3,
};

Player::Player(GameObject& associated, Vec2 position)
    : Component(associated),
      position(position),
      frameCount(8),
      frameTime(0.1),
      lastDirection(Helpers::Direction::NONE) {
  Sprite* sprite = new Sprite(associated, PLAYER_FRONT, 8, 0.1);
  Collider* collider =
      new Collider(associated, {0.5, 0.3}, {0, sprite->GetHeight() * 0.35f});
  associated.AddComponent(sprite);
  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();
  // just for debugging purposes
  associated.AddComponent(collider);

  State& state = Game::GetInstance().GetCurrentState();

  GameObject* pcGo = new GameObject(associated.priority);
  pcGo->box = associated.box;
  PriorityChanger* priChanger = new PriorityChanger(*pcGo, associated, true);
  pcGo->AddComponent(priChanger);
  priorityChanger_go = state.AddObject(pcGo);

  sound = std::make_shared<Sound>(associated);
  walkingSound = std::make_shared<Sound>(associated);
  walkingSound->Open(PLAYER_WALKING_SOUND);
}

Player::~Player() { priorityChanger_go->RequestDelete(); }

void Player::NotifyCollision(std::shared_ptr<GameObject> other) {
  if (other->IsDead()) {
    return;
  }

  InputManager& input = InputManager::GetInstance();

  auto item_cpt = other->GetComponent(ItemType);
  if (item_cpt) {
    if (input.KeyPress(X_KEY)) {
      auto item = std::dynamic_pointer_cast<Item>(item_cpt);
      if (item->GetKeyType() != Helpers::KeyType::NOKEY) {
        keys.push_back(item->GetKeyType());
        if (item->GetKeyType() == Helpers::KeyType::KITCHEN) {
          GameData::got_key1 = true;
        }
      }
      auto ok = GameData::AddToInventory(other);
      if (!ok) {
        SCRIPT_TYPE s = {std::make_pair<std::string, std::string>(
            "HOPE", "Não tem mais espaço na minha bolsa.")};
        GameData::InitDialog(s);
      }
    }
  }

  auto door_cpt = other->GetComponent(DoorType);
  if (door_cpt) {
    if (input.KeyPress(X_KEY)) {
      auto soundPlayed = false;
      auto door = std::dynamic_pointer_cast<Door>(door_cpt);
      if (door->GetKey() != Helpers::KeyType::NOKEY) {
        if (std::find(keys.begin(), keys.end(), door->GetKey()) == keys.end()) {
          if (door->GetKey() == Helpers::KeyType::CROWBAR) {
            SCRIPT_TYPE s[] = {
                {{"HOPE", "Está emperrada, não consigo abrir..."}},
                {{"HOPE",
                  "A fechadura parece estar quebrada, como será que vou "
                  "entrar?"}},
                {{"HOPE",
                  "Acho que nenhuma chave vai me ajudar a abrir esta "
                  "porta..."}},
            };
            // inserir som de porta emperrada
            GameData::InitDialog(s[rand() % 3]);
          } else {
            SCRIPT_TYPE s[] = {
                {{"HOPE", "Está trancado, onde será que está a chave?"}},
                {{"HOPE", "Não tenho a chave daqui..."}},
                {{"HOPE", "Preciso da chave..."}},
            };
            // inserir som de porta trancada
            GameData::InitDialog(s[rand() % 3]);
          }
          sound->Open("assets/audio/doors/locked_door.wav");
          sound->Play();
          return;
        } else {
          if (door->GetKey() == Helpers::KeyType::CROWBAR) {
            SCRIPT_TYPE s[] = {
                {{"HOPE", "Usar o pé de cabra foi uma ótima ideia!"}},
                {{"HOPE",
                  "Só com um pé de cabra pra conseguir abrir esta porta "
                  "mesmo."}}};
            // inserir som de porta sendo arrombada
            GameData::InitDialog(s[rand() % 2]);
            Lore::FirstMonsterSpawn();
          } else {
            SCRIPT_TYPE s[] = {{{"HOPE", "Consegui destrancar!"}},
                               {{"HOPE", "A chave era a certa!"}}};
            // inserir som de porta abrindo
            GameData::InitDialog(s[rand() % 2]);
          }
          if (door->GetKey() == Helpers::KeyType::LIBRARY) {
            Lore::UnlockLibrary();
            sound->Open("assets/audio/sound_effects/metal_clacking.wav");
            sound->Play();
          }
          soundPlayed = true;
          sound->Open("assets/audio/doors/open_door.wav");
          sound->Play(1, 64);
          door->SetKey(Helpers::KeyType::NOKEY);
        }
      }
      if (!soundPlayed) {
        sound->Open("assets/audio/doors/open_door_short.wav");
        sound->Play();
      }
      if (door->IsOpen()) {
        door->Close();
      } else {
        door->Open();
      }
    }
  }

  auto furniture_cpt = other->GetComponent(FurnitureType);
  if (furniture_cpt) {
    if (input.KeyPress(X_KEY)) {
      auto furniture = std::dynamic_pointer_cast<Furniture>(furniture_cpt);
      if (furniture->GetInteraction() == Helpers::Interaction::HIDE) {
        if (!GameData::player_is_hidden) {
          associated.DisableRender();
          GameData::player_is_hidden = true;
        } else {
          associated.EnableRender();
          GameData::player_is_hidden = false;
        }
      } else if (furniture->GetInteraction() == Helpers::Interaction::PLAY) {
        sound->Open("assets/audio/furniture/piano.wav");
        sound->Play();
        SCRIPT_TYPE s = {std::make_pair<std::string, std::string>(
            "HOPE", "Um piano de cauda? Nossa, que luxo!")};
        GameData::InitDialog(s);
      } else if (furniture->GetInteraction() == Helpers::Interaction::LOOK) {
        furniture->Look();
      } else if (furniture->GetInteraction() == Helpers::Interaction::SLEEP) {
        furniture->RemoveInteraction();
        Lore::Sleep();
      }
    }
  }

  auto stairs_cpt = other->GetComponent(StairsType);
  if (stairs_cpt) {
    if (input.KeyPress(X_KEY)) {
      std::cout << GameData::PlayerFloor() << std::endl;
      if (GameData::PlayerFloor() == Helpers::Floor::GROUND_FLOOR) {
        if (position.x < 275) {
          position.x = 220;
          position.y = 580;
        } else if (position.x < 373) {
          position.x = 350;
          position.y = 580;
        } else {
          position.x = 366;
          position.y = 1297;
        }
      } else if (GameData::PlayerFloor() == Helpers::Floor::FIRST_FLOOR) {
        if (position.x < 275) {
          position.x = 220;
          position.y = 211;
        } else {
          position.x = 350;
          position.y = 211;
        }
      } else {
        position.x = 388;
        position.y = 206;
      }
    }
  }
}

void Player::Start() { sleepTimer.Restart(); }

void Player::Update(float dt) {
  // std::cout << position.ToString() << std::endl;
  Vec2 oldPos(position.x, position.y);
  InputManager& input = InputManager::GetInstance();
  bool canwalk = true;
  auto tilemap = Game::GetInstance().GetCurrentState().GetCurrentTileMap();
  int tileDim = 8;

  if (GameData::player_is_hidden) return;

  if (Lore::Slept == 0) {
    sleepTimer.Update(dt);
    int limit = 30;
    if (sleepTimer.Get() > limit &&
        !GameData::DialogGameObject->IsRendering()) {
      SCRIPT_TYPE scripts[] = {
          {
              {"Hope", "Que sono..."},
          },
          {
              {"Hope", "Não aguento mais ficar em pé... onde fica uma cama?"},
          },
          {
              {"Hope", "Estou caindo de sono, preciso dormir..."},
          },
      };
      sound->Open(YawnSounds[rand() % 3]);
      sound->Play();
      sleepTimer.Restart();
      GameData::InitDialog(scripts[rand() % 3]);
    }
  } else if (Lore::Slept == 1) {
    SCRIPT_TYPE script = {
        {"Hope", "MEU DEUS! QUE BARULHO É ESSE?!?!"},
    };
    GameData::InitDialog(script);

    auto playerPos = GameData::PlayerGameObject->box.Center();
    CameraAction::Start(playerPos, {77 * 8, 795 * 8});

    Lore::Slept++;
  }

  auto spriteCpt = associated.GetComponent(SpriteType);
  auto colliderCpt = associated.GetComponent(ColliderType);

  if (!spriteCpt || !colliderCpt) {
    throw std::runtime_error("Nao tem sprite nem collider no player");
  }

  // hack remove
  if (input.IsKeyDown(LSHIFT_KEY) && input.IsKeyDown(SDLK_t)) {
    position = {50, 800};
    return;
  }
  if (input.IsKeyDown(LSHIFT_KEY) && input.IsKeyDown(SDLK_d)) {
    position = {70, 900};
    return;
  }
  if (input.IsKeyDown(LSHIFT_KEY) && input.IsKeyDown(SDLK_c)) {
    position = {291, 58};
    return;
  }

  auto sprite = std::dynamic_pointer_cast<Sprite>(spriteCpt);
  auto collider = std::dynamic_pointer_cast<Collider>(colliderCpt);

  auto move = speed * dt;
  auto nowRunning = false;
  if (input.IsKeyDown(LSHIFT_KEY)) {
    nowRunning = true;
    move *= 2;
    sprite->SetFrameTime(frameTime / 2);
  } else {
    sprite->SetFrameTime(frameTime);
  }

  bool up = false;
  bool down = false;
  bool left = false;
  bool right = false;

  associated.box.x = position.x * tileDim - sprite->GetWidth() / 2;
  associated.box.y = position.y * tileDim - sprite->GetHeight();

  int w = round(collider->box.w / tileDim);  // comprimento da box em celulas
  if (w % 2 == 1) w--;
  int h = round(collider->box.h / tileDim);  // altura da box em

  // inicio da "box de colisao" imaginaria com celulas
  int x = position.x - w / 2;
  int y = position.y - h;

  if (input.IsKeyDown(UP_ARROW_KEY)) {
    canwalk = true;
    up = true;
    for (int i = 0; i < w && canwalk; i++) {
      canwalk &= tilemap->CanWalk(x + i, y - move);
    }
    if (canwalk) {
      position.y -= move;
    }
  }
  if (input.IsKeyDown(DOWN_ARROW_KEY)) {
    canwalk = true;
    down = true;
    for (int i = 0; i < w && canwalk; i++) {
      canwalk &= tilemap->CanWalk(x + i, y + h + move);
    }
    if (canwalk) {
      position.y += move;
    }
  }
  if (input.IsKeyDown(LEFT_ARROW_KEY)) {
    canwalk = true;
    left = true;
    for (int i = 0; i < h && canwalk; i++) {
      canwalk &= tilemap->CanWalk(x - move, y + i);
    }
    if (canwalk) {
      position.x -= move;
    }
  }
  if (input.IsKeyDown(RIGHT_ARROW_KEY)) {
    canwalk = true;
    right = true;
    for (int i = 0; i < h && canwalk; i++) {
      canwalk &= tilemap->CanWalk(x + w + move, y + i);
    }
    if (canwalk) {
      position.x += move;
    }
  }

  auto direction = combine_moves(up, down, left, right);

  if (direction == Direction::NONE) {
    running = false;
    OpenIdleSprite(sprite, lastDirection);
  } else {
    walkSoundTimer.Update(dt);
    float timeout = running ? 0.218 : 0.459;
    if (nowRunning != running) {
      running = nowRunning;
      walkingSound->Open(running ? PLAYER_RUNNING_SOUND : PLAYER_WALKING_SOUND);
    }

    if (walkSoundTimer.Get() >= timeout) {
      walkSoundTimer.Restart();
      if (nowRunning) {
        walkingSound->Play(1, 32);
      } else {
        walkingSound->Play(1, 32);
      }
    }
    if (lastDirection != direction) {
      OpenWalkingSprite(sprite, direction);
    }
  }
  lastDirection = direction;

  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();

  auto pcCpt = priorityChanger_go->GetComponent(PriorityChangerType);
  if (!pcCpt) {
    throw std::runtime_error("Sem PC CPT");
  }
  auto pc = std::dynamic_pointer_cast<PriorityChanger>(pcCpt);
  pc->SetRect(dt, associated.box);

  std::cout << position.x << " " << position.y << std::endl;
}

void Player::Render() {}

bool Player::Is(Types type) const { return type == this->Type; }

void Player::OpenIdleSprite(const std::shared_ptr<Sprite>& sprite,
                            Direction lastDirection) {
  switch (lastDirection) {
    case Direction::UP:
      sprite->Open(PLAYER_BACK);
      frameCount = 4;
      break;
    case Direction::DOWN:
      sprite->Open(PLAYER_FRONT);
      frameCount = 8;
      break;
    case Direction::LEFT:
      sprite->Open(PLAYER_LEFT);
      frameCount = 4;
      break;
    case Direction::RIGHT:
      sprite->Open(PLAYER_RIGHT);
      frameCount = 4;
      break;
    case Direction::UPLEFT:
      sprite->Open(PLAYER_BACK);
      frameCount = 4;
      break;
    case Direction::UPRIGHT:
      sprite->Open(PLAYER_BACK);
      frameCount = 4;
      break;
    case Direction::DOWNLEFT:
      sprite->Open(PLAYER_FRONT);
      frameCount = 8;
      break;
    case Direction::DOWNRIGHT:
      sprite->Open(PLAYER_FRONT);
      frameCount = 8;
      break;
  }

  if (lastDirection != Direction::NONE) {
    sprite->SetFrameCount(frameCount);
    frameTime = 0.1;
    sprite->SetFrameTime(frameTime);
  }
}

void Player::OpenWalkingSprite(const std::shared_ptr<Sprite>& sprite,
                               Direction direction) {
  switch (direction) {
    case Direction::UP:
      sprite->Open(PLAYER_BACK_ANIM);
      frameCount = 5;
      break;
    case Direction::DOWN:
      sprite->Open(PLAYER_FRONT_ANIM);
      frameCount = 5;
      break;
    case Direction::LEFT:
      sprite->Open(PLAYER_LEFT_ANIM);
      frameCount = 8;
      break;
    case Direction::RIGHT:
      sprite->Open(PLAYER_RIGHT_ANIM);
      frameCount = 8;
      break;
    case Direction::UPLEFT:
      sprite->Open(PLAYER_BACK_ANIM);
      frameCount = 5;
      break;
    case Direction::UPRIGHT:
      sprite->Open(PLAYER_BACK_ANIM);
      frameCount = 5;
      break;
    case Direction::DOWNLEFT:
      sprite->Open(PLAYER_FRONT_ANIM);
      frameCount = 5;
      break;
    case Direction::DOWNRIGHT:
      sprite->Open(PLAYER_FRONT_ANIM);
      frameCount = 5;
      break;
    default:
      return OpenIdleSprite(sprite, direction);
  }

  sprite->SetFrameCount(frameCount);
  frameTime = 0.8 / frameCount;
  sprite->SetFrameTime(frameTime);
}
