#include "State.hpp"
#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include "Camera.hpp"
#include "CameraFollower.hpp"
#include "Collider.hpp"
#include "Collision.hpp"
#include "Component.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"
#include "InputManager.hpp"
#include "Music.hpp"
#include "Sound.hpp"
#include "Sprite.hpp"
#include "TileMap.hpp"
#include "TileSet.hpp"
#include "Vec2.hpp"

State::State() : quitRequested(false), started(false), popRequested(false) {}

State::~State() {}

std::weak_ptr<GameObject> State::AddObject(GameObject* go) {
  std::shared_ptr<GameObject> goSharedPtr(go);
  objects.insert(goSharedPtr);

  if (started) {
    goSharedPtr->Start();
  }

  return std::weak_ptr<GameObject>(goSharedPtr);
}

std::weak_ptr<GameObject> State::GetObjectPtr(GameObject* go) {
  auto it = std::find_if(objects.begin(), objects.end(),
                         [&](auto& go2) { return go == go2.get(); });

  if (it == objects.end()) {
    return std::weak_ptr<GameObject>();
  }

  return std::weak_ptr<GameObject>(*it);
}

bool State::QuitRequested() const { return quitRequested; }

bool State::PopRequested() const { return popRequested; }

void State::StartArray() {
  std::for_each(objects.begin(), objects.end(), [&](auto& go) { go->Start(); });
}

void State::UpdateArray(float dt) {
  std::for_each(objects.begin(), objects.end(),
                [&](auto& go) { go->Update(dt); });
}

void State::RenderArray() {
  std::for_each(objects.begin(), objects.end(), [](auto& go) { go->Render(); });
}

TileMap* State::GetCurrentTileMap() { return currentTileMap; }
