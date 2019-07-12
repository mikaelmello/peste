#include "CreditsState.hpp"

CreditsState::CreditsState()
    : im(InputManager::GetInstance()), k(0), credits(CREDITS) {}

CreditsState::~CreditsState() {}

void CreditsState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
  }

  auto sprite_cpt = box->GetComponent(SpriteType);
  if (!sprite_cpt) {
    std::runtime_error("sem sprite em CreditsState::Update");
  }
  auto sprite = std::dynamic_pointer_cast<Sprite>(sprite_cpt);

  popRequested |= im.KeyPress(ESCAPE_KEY) || im.JoyKeyPress(JOY_B_KEY);
  quitRequested |= im.QuitRequested();

  if (im.KeyPress(SPACE_BAR_KEY) || im.JoyKeyPress(JOY_A_KEY)) {
    k = (k + 1) % credits.size();
    sprite->Open(credits[k]);
  }
  UpdateArray(dt);
}

void CreditsState::Start() {
  LoadAssets();
  StartArray();
  started = true;
}

void CreditsState::Pause() {}

void CreditsState::Resume() {}

void CreditsState::LoadAssets() {
  GameObject* background_go = new GameObject();
  Sprite* background_sprite =
      new Sprite(*background_go, CREDIT_BACKGROUND_SPRITE);
  background_go->AddComponent(background_sprite);
  objects.emplace_back(background_go);

  auto box_go = std::make_shared<GameObject>(10001);
  Sprite* box_sprite = new Sprite(*box_go, credits[k]);
  box_go->AddComponent(box_sprite);
  box_go->box.x = 15;
  box_go->box.y = 250;
  objects.push_back(box_go);
  box = box_go;

  auto cursor_go = std::make_shared<GameObject>(10003);
  Sprite* cursor_sprite = new Sprite(*cursor_go, MENU_CURSOR_SPRITE);
  cursor_go->AddComponent(cursor_sprite);
  objects.push_back(cursor_go);
  cursor_go->box.x = 111;
  cursor_go->box.y = 620;
}

void CreditsState::Render() { RenderArray(); }
