#include "Lore.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include "Antagonist.hpp"
#include "CameraAction.hpp"
#include "CaughtState.hpp"
#include "Furniture.hpp"
#include "Game.hpp"
#include "GameData.hpp"
#include "InputManager.hpp"
#include "Item.hpp"
#include "Resources.hpp"
#include "SDL_include.h"
#include "SleepState.hpp"
#include "Sprite.hpp"
#include "Stairs.hpp"
#include "Terry.hpp"

bool Lore::NicePerson = false;
bool Lore::HasEnteredMasterBedroom = false;
bool Lore::Caught = false;
bool Lore::Defended = false;
int Lore::Slept = 0;
int Lore::CaughtCount = 0;

void Lore::Init() {
  Lore::NicePerson = false;
  Lore::HasEnteredMasterBedroom = false;
  Lore::Caught = false;
  Lore::Defended = false;
  Lore::Slept = 0;
  Lore::CaughtCount = 0;
}

void Lore::Sleep() {
  if (!GameData::TerryBedGameObject) {
    throw std::runtime_error("Cama do terry nao setada no gamedata");
  }
  if (!GameData::TerryGameObject) {
    throw std::runtime_error("Terry nao setado no gamedata");
  }

  auto terryBedSpriteCpt =
      GameData::TerryBedGameObject->GetComponent(SpriteType);

  if (!terryBedSpriteCpt) {
    throw std::runtime_error("Cama do terry sem sprite");
  }

  auto sprite = std::dynamic_pointer_cast<Sprite>(terryBedSpriteCpt);
  sprite->Open("assets/img/furniture/bedDestroyed.png");

  GameData::TerryGameObject->RequestDelete();
  GameData::TerryGameObject.reset();

  auto& game = Game::GetInstance();
  game.Push(new SleepState());

  auto itemGo = new GameObject(7);
  auto item =
      new Item(*itemGo, "Pé de Cabra",
               "Bom para abrir coisas que normalmente não podem ser abertas.",
               "assets/img/item/crowbar.png", {291, 58},
               Helpers::KeyType::CROWBAR, 96, 96);
  itemGo->AddComponent(item);
  game.GetCurrentState().AddObject(itemGo);

  std::vector<std::string> script = {
      "É um caderno todo destruído...",
      "Ah! Parece que tem algo escrito na parte de dentro da capa.",
      "\"Hoje a mamãe me deu um caderno novo. Ela falou que era pra desenhos, "
      "mas eu vou usar como um amigo!\". . .",
      "\"Eu sinto falta dos meus amigos… Nunca mais consegui ir na escola "
      "depois que descobri que tô doente…\" . . .",
      "\"Mas o papai e a mamãe são médicos! Eles falaram que vai ficar tudo "
      "bem. "
      "Então eu acho que vai mesmo!\""};
  auto daily_go = new GameObject(7);
  auto daily =
      new Furniture(*daily_go, "assets/img/item/daily.png", {75, 795},
                    Helpers::Interaction::LOOK, false, script, false, false);
  daily->SetScale(0.4, 0.4);
  daily_go->AddComponent(daily);
  game.GetCurrentState().AddObject(daily_go);

  script = {
      "É uma folha de caderno rasgada...,",
      "\"Às vezes, eu chego a ter medo do papai... Eu sei que ele quer meu "
      "bem e tá fazendo essas coisas pra eu melhorar... Mas dói tanto.\" ...",
      "\"Antes eu ficava feliz que o papai e a mamãe estavam fazendo meus "
      "remédios. Agora eu nem consigo olhar pro papai sem chorar.\" ...",
      "Um post-it colado sobre a página diz, numa caligrafia diferente: "
      "\"Mostrar para o doutor.\""};

  daily_go = new GameObject(7);
  daily =
      new Furniture(*daily_go, "assets/img/item/daily.png", {29, 610},
                    Helpers::Interaction::LOOK, false, script, false, false);
  daily->SetScale(0.4, 0.4);
  daily_go->AddComponent(daily);
  game.GetCurrentState().AddObject(daily_go);

  script = {
      "\"Eu ouvi uns barulhos lá embaixo.\"",
      "\"Voz de criança? Não sei, mesmo... Eu tava feliz de ter gente da "
      "minha idade aqui. Talvez era algum primo que eu não conhecia.\"...",
      "\"Mas quando eu pedi pra saber quem tava lá, meu pai disse que era só "
      "ele e a mamãe.\"...",
      "\"Engraçado... Eu jurava que tinha ouvido a voz de uma criança da "
      "minha idade.\""};

  daily_go = new GameObject(7);
  daily =
      new Furniture(*daily_go, "assets/img/item/daily.png", {303, 312},
                    Helpers::Interaction::LOOK, false, script, false, false);
  daily->SetScale(0.4, 0.4);
  daily_go->AddComponent(daily);
  game.GetCurrentState().AddObject(daily_go);
}

void Lore::FirstMonsterSpawn() {
  auto& state = Game::GetInstance().GetCurrentState();
  auto ant = new GameObject(100);
  Antagonist* antagonist = new Antagonist(*ant, {{347, 579}});
  ant->AddComponent(antagonist);
  GameData::MonsterGameObject = state.AddObject(ant);
  GameData::LoadAntagonistPaths();

  auto playerPos = GameData::PlayerGameObject->box.Center();
  auto monsterPos = GameData::MonsterGameObject->box.Center();
  CameraAction::Start(playerPos, monsterPos);

  auto itemGo = new GameObject(7);
  auto item = new Item(*itemGo, "Chave", "O que será que ela abre?",
                       "assets/img/item/key2.png", {150, 906},
                       Helpers::KeyType::LIBRARY, 32, 32);
  itemGo->AddComponent(item);
  state.AddObject(itemGo);
}

void Lore::UnlockBasement() {
  auto& state = Game::GetInstance().GetCurrentState();

  //  Quando ele entrar na biblioteca, deve tocar um som alto e aparecer o
  //  basement.
  auto stairsGo = new GameObject(8);
  auto stairs = new Stairs(*stairsGo, Helpers::Direction::UP, {380, 210},
                           Helpers::Floor::BASEMENT);
  stairsGo->AddComponent(stairs);
  state.AddObject(stairsGo);
}

void Lore::PlayerCaught() {
  auto& game = Game::GetInstance();
  game.Push(new CaughtState());

  Helpers::TeleportPlayerSafeHouse();
  GameData::player_was_hit = false;
}

void Lore::DefendAgainstMonster() {
  Lore::Defended = true;

  auto& state = Game::GetInstance().GetCurrentState();

  auto ant_cpt = GameData::MonsterGameObject->GetComponent(AntagonistType);
  if (!ant_cpt) {
    throw std::runtime_error("sem ant em Lore::DefendAgainstMonster");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(ant_cpt);

  GameData::transformed_monster_in_terry = true;

  GameObject* terry_go = new GameObject(8);
  Terry* terry_cpt = new Terry(*terry_go, "assets/img/npc/idlenpc.png",
                               {ant->position.x - 10, ant->position.y - 10});
  terry_cpt->SetSprite("assets/img/terry/monster_to_terry.png");
  terry_cpt->SetAnimation(10, 0.3);
  terry_go->AddComponent(terry_cpt);
  GameData::TerryGameObject = state.AddObject(terry_go);

  GameData::MonsterGameObject->RequestDelete();

  GameData::already_begin_animation = true;
}
