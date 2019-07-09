#include "EnemyState.hpp"
#include "Antagonist.hpp"
#include "Camera.hpp"
#include "CameraFollower.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "TileMap.hpp"

EnemyState::EnemyState() : State() {
  // printf("%lf %lf\n", currentTileMap->GetInitialPosition().x,
  //    currentTileMap->GetInitialPosition().y);

  Camera::Unfollow();
}

EnemyState::~EnemyState() {}

void EnemyState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
  }

  Camera::Update(dt);

  InputManager& im = InputManager::GetInstance();
  quitRequested |= im.QuitRequested();
  popRequested |= im.KeyPress(ESCAPE_KEY);

  UpdateArray(dt);
}

void EnemyState::Start() {
  LoadAssets();
  StartArray();
  started = true;
}

void EnemyState::Pause() {}

void EnemyState::Resume() {}

void EnemyState::LoadAssets() {
  GameObject* bckgGo = new GameObject(5);
  Sprite* bckgSprite = new Sprite(*bckgGo, "assets/img/black.jpg");
  bckgGo->AddComponent(new CameraFollower(*bckgGo));
  bckgGo->AddComponent(bckgSprite);
  objects.emplace_back(bckgGo);

  GameObject* mapGo = new GameObject(5);
  currentTileMap = new TileMap(*mapGo, "assets/map/map.json");
  currentTileMap->SetParallax(1, 0, 0);
  mapGo->AddComponent(currentTileMap);
  objects.emplace_back(mapGo);

  GameObject* antagonist_go = new GameObject(5);
   std::vector<Vec2> points = {{68, 199},  {88, 132},  {110, 66},
                              {199, 106}, {332, 132}, {326, 195}};
  Antagonist* antagonist = new Antagonist(*antagonist_go, points);
  antagonist_go->AddComponent(antagonist);
  objects.emplace_back(antagonist_go);

  Camera::Follow(antagonist_go);
}

void EnemyState::Render() { RenderArray(); }
