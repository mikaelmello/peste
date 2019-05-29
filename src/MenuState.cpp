#include "MenuState.hpp"
#include <stdexcept>
#include "Camera.hpp"
#include "CreditsState.hpp"
#include "Game.hpp"
#include "Helpers.hpp"
#include "Resources.hpp"
#include "RoomState.hpp"
#include "TutorialState.hpp"

MenuState::MenuState()
    : position({100, 100}), im(InputManager::GetInstance()) {}

MenuState::~MenuState() {}

void MenuState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
  }

  quitRequested |= im.QuitRequested();

  if (im.KeyPress(ESCAPE_KEY)) {
    cursor.setQuit();
  }

  if (im.KeyPress(DOWN_ARROW_KEY)) {
    cursor++;
  }

  if (im.KeyPress(UP_ARROW_KEY)) {
    cursor--;
  }

  if (im.KeyPress(SPACE_BAR_KEY)) {
    switch (cursor.cursor_state) {
      case Cursor::CursorState::play:
        Game::GetInstance().Push(new RoomState());
        break;
      case Cursor::CursorState::tutorial:
        Game::GetInstance().Push(new TutorialState());
        break;
      case Cursor::CursorState::credits:
        Game::GetInstance().Push(new CreditsState());
        break;
      case Cursor::CursorState::quit:
        popRequested |= true;
        break;
      default:
        throw std::logic_error("cursor is acessing an invalid index");
    }
  }
  UpdateArray(dt);
}

void MenuState::Start() {
  Camera::Unfollow();
  Camera::pos = Vec2(0, 0);

  LoadAssets();
  StartArray();
  started = true;
}

void MenuState::Pause() {}

void MenuState::Resume() {}

void MenuState::LoadAssets() {
  std::vector<GameObject*> buttons;

  cursor.cursor_go = new GameObject(10001);
  Sprite* cursor_sprite = new Sprite(*cursor.cursor_go, MENU_CURSOR_SPRITE);
  cursor.cursor_go->AddComponent(cursor_sprite);
  objects.emplace(cursor.cursor_go);

  GameObject* background_go = new GameObject();
  Sprite* background_sprite =
      new Sprite(*background_go, MENU_BACKGROUND_SPRITE);
  background_go->AddComponent(background_sprite);
  objects.emplace(background_go);

  GameObject* play_button_go = new GameObject();
  Sprite* play_button_sprite =
      new Sprite(*play_button_go, MENU_PLAY_BUTTON_SPRITE);
  play_button_go->AddComponent(play_button_sprite);
  buttons.push_back(play_button_go);
  objects.emplace(play_button_go);

  GameObject* tutorial_button_go = new GameObject();
  Sprite* tutorial_button_sprite =
      new Sprite(*tutorial_button_go, MENU_TUTORIAL_BUTTON_SPRITE);
  tutorial_button_go->AddComponent(tutorial_button_sprite);
  buttons.push_back(tutorial_button_go);
  objects.emplace(tutorial_button_go);

  GameObject* credits_button_go = new GameObject();
  Sprite* credits_button_sprite =
      new Sprite(*credits_button_go, MENU_CREDITS_BUTTON_SPRITE);
  credits_button_go->AddComponent(credits_button_sprite);
  buttons.push_back(credits_button_go);
  objects.emplace(credits_button_go);

  GameObject* quit_button_go = new GameObject();
  Sprite* quit_button_sprite =
      new Sprite(*quit_button_go, MENU_QUIT_BUTTON_SPRITE);
  quit_button_go->AddComponent(quit_button_sprite);
  buttons.push_back(quit_button_go);
  objects.emplace(quit_button_go);

  for (int i = 0; i < buttons.size(); i++) {
    buttons[i]->box =
        buttons[i]->box.GetCentered(position + Vec2(0, i * buttons[i]->box.h));
    cursor.positions.push_back(buttons[i]->box.Center());
  }

  cursor.cursor_go->box = cursor.cursor_go->box.GetCentered(position);
}

void MenuState::Render() { RenderArray(); }
