#include "MenuState.hpp"
#include <stdexcept>
#include "Camera.hpp"
#include "CreditsState.hpp"
#include "Game.hpp"
#include "GameData.hpp"
#include "Helpers.hpp"
#include "Resources.hpp"
#include "RoomState.hpp"
#include "Sound.hpp"
#include "TutorialState.hpp"

MenuState::MenuState()
    : played(false),
      position(175, 380),
      options(nullptr),
      im(InputManager::GetInstance()) {}

MenuState::~MenuState() {}

void MenuState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
  }

  quitRequested |= im.QuitRequested();

  if (im.KeyPress(ESCAPE_KEY)) {
    options->Last();
  }

  if (im.KeyPress(DOWN_ARROW_KEY) || im.KeyPress(JOY_DOWN_KEY)) {
    (*options)++;
  }

  if (im.KeyPress(UP_ARROW_KEY) || im.KeyPress(JOY_UP_KEY)) {
    (*options)--;
  }

  auto sound_cpt = background->GetComponent(SoundType);
  if (!sound_cpt) {
    throw std::runtime_error("sem som em MenuState::Update");
  }
  auto sound = std::dynamic_pointer_cast<Sound>(sound_cpt);
  played = true;
  sound->Stop();

  if (im.KeyPress(SPACE_BAR_KEY) || im.JoyKeyPress(JOY_A_KEY)) {
    switch (options->GetOperation()) {
      case Cursor::play:
        played = true;
        sound->Stop();
        GameData::Init();
        Game::GetInstance().Push(new RoomState());
        break;
      case Cursor::tutorial:
        Game::GetInstance().Push(new TutorialState());
        break;
      case Cursor::credits:
        Game::GetInstance().Push(new CreditsState());
        break;
      case Cursor::quit:
        popRequested |= true;
        break;
      default:
        throw std::logic_error(
            "cursor is acessing an invalid index of button's array");
    }
  }
}

void MenuState::Start() {
  Camera::Unfollow();
  Camera::pos = Vec2(0, 0);

  LoadAssets();
  StartArray();
  started = true;
}

void MenuState::Pause() {}

void MenuState::Resume() {
  auto sound_cpt = background->GetComponent(SoundType);
  auto sound = std::dynamic_pointer_cast<Sound>(sound_cpt);

  try {
    if (played) {
      sound->Play(10000);
      played = false;
    }
  } catch (const std::exception& ex) {
    printf("%s\n", ex.what());
  }
}

void MenuState::LoadAssets() {
  auto background_go = std::make_shared<GameObject>();
  Sound* background_sound = new Sound(*background_go, MAIN_THEME);
  Sprite* background_sprite =
      new Sprite(*background_go, MENU_BACKGROUND_SPRITE);
  background_go->AddComponent(background_sound);
  background_go->AddComponent(background_sprite);
  objects.push_back(background_go);

  auto box_go = std::make_shared<GameObject>(10001);
  Sprite* box_sprite = new Sprite(*box_go, MENU_BOX_SPRITE);
  box_go->AddComponent(box_sprite);
  box_go->box.x = 15;
  box_go->box.y = 250;
  objects.push_back(box_go);

  auto cursor_go = std::make_shared<GameObject>(10003);
  Sprite* cursor_sprite = new Sprite(*cursor_go, MENU_CURSOR_SPRITE);
  cursor_go->AddComponent(cursor_sprite);
  objects.push_back(cursor_go);

  auto play_button_go = std::make_shared<GameObject>(10002);
  Sprite* play_button_sprite =
      new Sprite(*play_button_go, MENU_PLAY_BUTTON_SPRITE);
  play_button_go->AddComponent(play_button_sprite);
  buttons.push_back(play_button_go);
  objects.push_back(play_button_go);

  auto tutorial_button_go = std::make_shared<GameObject>(10002);
  Sprite* tutorial_button_sprite =
      new Sprite(*tutorial_button_go, MENU_TUTORIAL_BUTTON_SPRITE);
  tutorial_button_go->AddComponent(tutorial_button_sprite);
  buttons.push_back(tutorial_button_go);
  objects.push_back(tutorial_button_go);

  auto credits_button_go = std::make_shared<GameObject>(10002);
  Sprite* credits_button_sprite =
      new Sprite(*credits_button_go, MENU_CREDITS_BUTTON_SPRITE);
  credits_button_go->AddComponent(credits_button_sprite);
  buttons.push_back(credits_button_go);
  objects.push_back(credits_button_go);

  auto quit_button_go = std::make_shared<GameObject>(10002);
  Sprite* quit_button_sprite =
      new Sprite(*quit_button_go, MENU_QUIT_BUTTON_SPRITE);
  quit_button_go->AddComponent(quit_button_sprite);
  buttons.push_back(quit_button_go);
  objects.push_back(quit_button_go);

  for (unsigned i = 0; i < buttons.size(); i++) {
    int offset = 20;
    buttons[i]->box = buttons[i]->box.GetCentered(
        position + Vec2(0, i * (buttons[i]->box.h + offset)));
  }

  SortObjects();
  options = std::make_shared<Options>(*cursor_go, buttons);
  background_sound->Play(10000);
  background = background_go;
}

void MenuState::Render() { RenderArray(); }
