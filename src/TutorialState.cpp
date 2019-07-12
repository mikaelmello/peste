#include "TutorialState.hpp"
#include "InputManager.hpp"

TutorialState::TutorialState() : im(InputManager::GetInstance()) {}

TutorialState::~TutorialState() {}

void TutorialState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
  }

  popRequested |= im.KeyPress(ESCAPE_KEY) || im.JoyKeyPress(JOY_B_KEY);
  quitRequested |= im.QuitRequested();

  UpdateArray(dt);
}

void TutorialState::Start() {
  LoadAssets();
  StartArray();
  started = true;
}

void TutorialState::Pause() {}

void TutorialState::Resume() {}

void TutorialState::LoadAssets() {
  GameObject* background_tutorial = new GameObject();
  Sprite* background_sprite =
      new Sprite(*background_tutorial, TUTORIAL_BACKGROUND_SPRITE);
  background_tutorial->AddComponent(background_sprite);
  objects.emplace_back(background_tutorial);

  auto box_go = std::make_shared<GameObject>(10001);
  Sprite* box_sprite = new Sprite(*box_go, TUTORIAL_BOX);
  box_go->AddComponent(box_sprite);
  box_go->box.x = 15;
  box_go->box.y = 250;
  objects.push_back(box_go);
}

void TutorialState::Render() { RenderArray(); }
