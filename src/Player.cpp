#include "Player.hpp"
#include <iostream>
#include <memory>
#include <string>
#include "Blocker.hpp"
#include "Camera.hpp"
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
#include "Sprite.hpp"
#include "TileMap.hpp"
#include "Types.hpp"

using namespace Helpers;

#define SCRIPT_TYPE std::vector<std::pair<std::string, std::string>>

#define PLAYER_FRONT_ANIM "assets/img/hope/front_anim.png"
#define PLAYER_BACK_ANIM "assets/img/hope/back_anim.png"
#define PLAYER_LEFT_ANIM "assets/img/hope/left_anim.png"
#define PLAYER_RIGHT_ANIM "assets/img/hope/right_anim.png"

#define PLAYER_FRONT "assets/img/hope/front.png"
#define PLAYER_BACK "assets/img/hope/back.png"
#define PLAYER_LEFT "assets/img/hope/left.png"
#define PLAYER_RIGHT "assets/img/hope/right.png"

Player::Player(GameObject& associated, Vec2 position)
    : Component(associated),
      position(position),
      frameCount(1),
      frameTime(1),
      lastDirection(Helpers::Direction::NONE) {
  Sprite* sprite = new Sprite(associated, PLAYER_FRONT);
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

  Sound* sound = new Sound(associated);
  associated.AddComponent(sound);
  auto sound_cpt = associated.GetComponent(SoundType);
  sound_ptr = std::dynamic_pointer_cast<Sound>(sound_cpt);
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
      auto door = std::dynamic_pointer_cast<Door>(door_cpt);
      if (door->GetKey() != Helpers::KeyType::NOKEY) {
        if (std::find(keys.begin(), keys.end(), door->GetKey()) == keys.end()) {
          if (door->GetKey() == door->GetKey() == Helpers::KeyType::CROWBAR) {
            SCRIPT_TYPE s = {std::make_pair<std::string, std::string>(
                "HOPE", "Está emperrada, não consigo abrir...")};
            // inserir som de porta emperrada
            GameData::InitDialog(s);
          } else {
            sound_ptr->Stop();
            sound_ptr->Open("assets/audio/doors/locked_door.wav");
            sound_ptr->Play();
            SCRIPT_TYPE s = {std::make_pair<std::string, std::string>(
                "HOPE", "Está trancado, onde será que está a chave?")};
            // inserir som de porta trancada
            GameData::InitDialog(s);
          }
          return;
        } else {
          if (door->GetKey() == Helpers::KeyType::CROWBAR) {
            SCRIPT_TYPE s = {std::make_pair<std::string, std::string>(
                "HOPE",
                "Vou tentar forçar a porta com isso aqui... Consegui!")};
            // inserir som de porta sendo arrombada
            GameData::InitDialog(s);
          } else {
            sound_ptr->Stop();
            sound_ptr->Open("assets/audio/doors/open_door.wav");
            sound_ptr->Play();
            SCRIPT_TYPE s = {std::make_pair<std::string, std::string>(
                "HOPE", "Consegui destrancar!")};
            // inserir som de porta abrindo
            GameData::InitDialog(s);
          }
          door->SetKey(Helpers::KeyType::NOKEY);
        }
      }
      if (door->IsOpen())
        door->Close();
      else
        door->Open();
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
        sound_ptr->Stop();
        sound_ptr->Open("assets/audio/furniture/piano.wav");
        sound_ptr->Play();
        SCRIPT_TYPE s = {std::make_pair<std::string, std::string>(
            "HOPE", "Um piano de cauda? Nossa, que luxo!")};
        GameData::InitDialog(s);
      } else if (furniture->GetInteraction() == Helpers::Interaction::LOOK) {
        furniture->Look();
      } else if (furniture->GetInteraction() == Helpers::Interaction::SLEEP) {
        Lore::Sleep();
      }
    }
  }

  auto stairs_cpt = other->GetComponent(StairsType);
  if (stairs_cpt) {
    if (input.KeyPress(X_KEY)) {
      std::cout << GameData::hope_is_in << std::endl;
      if (GameData::hope_is_in == Helpers::Floor::GROUND_FLOOR) {
        GameData::hope_is_in = Helpers::Floor::FIRST_FLOOR;
        if (position.x < 275) {
          position.x = 220;
          position.y = 580;
        } else {
          position.x = 350;
          position.y = 580;
        }
      } else if (GameData::hope_is_in == Helpers::Floor::FIRST_FLOOR) {
        GameData::hope_is_in = Helpers::Floor::GROUND_FLOOR;
        if (position.x < 275) {
          position.x = 220;
          position.y = 211;
        } else {
          position.x = 350;
          position.y = 211;
        }
      }
    }
  }
}

void Player::Start() { sleepTimer.Restart(); }

void Player::Update(float dt) {
  Vec2 oldPos(position.x, position.y);
  InputManager& input = InputManager::GetInstance();
  bool canwalk = true;
  auto tilemap = Game::GetInstance().GetCurrentState().GetCurrentTileMap();
  int tileDim = tilemap->GetLogicalTileDimension();

  if (GameData::player_is_hidden) return;

  sleepTimer.Update(dt);
  int limit = 30;
  if (sleepTimer.Get() > limit && !GameData::DialogGameObject->IsRendering()) {
    std::vector<std::pair<std::string, std::string>> scripts[] = {
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
    sleepTimer.Restart();
    GameData::InitDialog(scripts[rand() % 3]);
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
    position = {50, 900};
    return;
  }

  auto sprite = std::dynamic_pointer_cast<Sprite>(spriteCpt);
  auto collider = std::dynamic_pointer_cast<Collider>(colliderCpt);

  auto move = speed * dt;
  if (input.IsKeyDown(LSHIFT_KEY)) {
    move *= 1.5;
    sprite->SetFrameTime(frameTime / 1.5f);
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
    OpenIdleSprite(sprite, lastDirection);
  } else if (lastDirection != direction) {
    OpenWalkingSprite(sprite, direction);
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
}

void Player::Render() {}

bool Player::Is(Types type) const { return type == this->Type; }

void Player::OpenIdleSprite(const std::shared_ptr<Sprite>& sprite,
                            Direction lastDirection) {
  frameCount = 1;
  frameTime = 1;
  sprite->SetFrameCount(frameCount);
  sprite->SetFrameTime(frameTime);

  switch (lastDirection) {
    case Direction::UP:
      sprite->Open(PLAYER_BACK);
      break;
    case Direction::DOWN:
      sprite->Open(PLAYER_FRONT);
      break;
    case Direction::LEFT:
      sprite->Open(PLAYER_LEFT);
      break;
    case Direction::RIGHT:
      sprite->Open(PLAYER_RIGHT);
      break;
    case Direction::UPLEFT:
      sprite->Open(PLAYER_BACK);
      break;
    case Direction::UPRIGHT:
      sprite->Open(PLAYER_BACK);
      break;
    case Direction::DOWNLEFT:
      sprite->Open(PLAYER_FRONT);
      break;
    case Direction::DOWNRIGHT:
      sprite->Open(PLAYER_FRONT);
      break;
    case Direction::NONE:
      sprite->Open(PLAYER_FRONT);
      break;
  }
}

void Player::OpenWalkingSprite(const std::shared_ptr<Sprite>& sprite,
                               Direction direction) {
  frameCount = 1;
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
  frameTime = 0.4 / frameCount;
  sprite->SetFrameTime(frameTime);
}
