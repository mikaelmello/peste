#include "EndingState.hpp"

#include <iostream>
#include "GameData.hpp"
#include "Sprite.hpp"

EndingState::EndingState(int ending)
    : ending(ending), im(InputManager::GetInstance()) {}

EndingState::~EndingState() {}

void EndingState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
  }
  quitRequested |= im.QuitRequested();
  popRequested |= im.KeyPress(ESCAPE_KEY);

  UpdateArray(dt);
}

void EndingState::Start() {
  LoadAssets();
  StartArray();
  started = true;
  SortObjects();
  GameData::InitDialog(dialog);
}

void EndingState::Pause() {}

void EndingState::Resume() {}

void EndingState::LoadAssets() {
  SCRIPT_TYPE good_script = {{"Terry ", "Para onde vamos?"},
                             {"Hope ", "Para um lugar seguro."},
                             {"Fim ", " "}};

  SCRIPT_TYPE bad_script = {
      {"Sr. e Sra. Hyde, ",
       "Escolhi a profissão de enfermeiro porque gosto de cuidar das pessoas. "
       "Quando cheguei na casa, coisa ruins aconteceram e infelizmente não sei "
       "onde o Terry está. Me desculpe."},

      {" ", "Por favor, não me procurem mais."},
      {"Fim ", " "}};

  std::string sprite_path =
      ending == 1 ? GOOD_ENDING_SPRITE : BAD_ENDING_SPRITE;
  dialog = ending == 1 ? good_script : bad_script;

  auto background_go = std::make_shared<GameObject>();
  auto background_sprite = new Sprite(*background_go, sprite_path);
  background_go->AddComponent(background_sprite);
  objects.push_back(background_go);

  auto dialogGo = std::make_shared<GameObject>(1500);
  auto dialog = new Dialog(*dialogGo);
  dialogGo->AddComponent(dialog);
  objects.push_back(dialogGo);
  GameData::DialogGameObject = dialogGo;
}

void EndingState::Render() { RenderArray(); }
