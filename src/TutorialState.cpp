#include "TutorialState.hpp"
#include "InputManager.hpp"

TutorialState::TutorialState()
    : im(InputManager::GetInstance()), k(0), tutorials(TUTORIALS) {}

TutorialState::~TutorialState() {}

void TutorialState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
  }

  popRequested |= im.KeyPress(ESCAPE_KEY) || im.JoyKeyPress(JOY_B_KEY);
  quitRequested |= im.QuitRequested();

  UpdateArray(dt);

  auto sprite_cpt = box->GetComponent(SpriteType);
  if (!sprite_cpt) {
    std::runtime_error("sem sprite em CreditsState::Update");
  }
  auto sprite = std::dynamic_pointer_cast<Sprite>(sprite_cpt);

  popRequested |= im.KeyPress(ESCAPE_KEY) || im.JoyKeyPress(JOY_B_KEY);
  quitRequested |= im.QuitRequested();

  if (im.KeyPress(SPACE_BAR_KEY) || im.JoyKeyPress(JOY_A_KEY)) {
    k = (k + 1) % tutorials.size();
    sprite->Open(tutorials[k]);
  }
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
  box = box_go;

  auto cursor_go = std::make_shared<GameObject>(10003);
  Sprite* cursor_sprite = new Sprite(*cursor_go, MENU_CURSOR_SPRITE);
  cursor_go->AddComponent(cursor_sprite);
  objects.push_back(cursor_go);
  cursor_go->box.x = 100;
  cursor_go->box.y = 630;
}

void TutorialState::Render() { RenderArray(); }
