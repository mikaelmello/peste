#ifndef HELPERS_H
#define HELPERS_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <string>
#include <vector>
#include "GameObject.hpp"

namespace Helpers {

// Divide uma string em um vetor de strings de acordo um caractere delimitador
// Criado por Evan Teran
// Disponível em:
// https://stackoverflow.com/questions/236129/most-elegant-way-to-split-a-string
template <typename Out>
void split(const std::string& s, char delim, Out result);

// Divide uma string em um vetor de strings de acordo um caractere delimitador
// Criado por Evan Teran
// Disponível em:
// https://stackoverflow.com/questions/236129/most-elegant-way-to-split-a-string
std::vector<std::string> split(const std::string& s, char delim);

std::vector<std::string> split(const std::string& text,
                               const std::string& delims);

bool is_whitespace(const std::string& s);

float rad_to_deg(float rad);

float deg_to_rad(float deg);

enum Direction {
  NONE,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  UPLEFT,
  UPRIGHT,
  DOWNLEFT,
  DOWNRIGHT,
};

Direction combine_moves(bool up, bool down, bool left, bool right);

enum Action {
  IDLE,
  MOVING,
  SUSPECTING,
  CHASING,
  ATTACKING,
};

// Verifica se @param object pode andar sobre a posição @param p.
// @param object tem que possuir um collider e a posição considerada
// é a centro-horizontal / fim-vertical.
bool CanWalk(GameObject& object, Vec2& p);

bool CanWalk(GameObject& object, std::pair<int, int>& p);

enum Interaction {
  HIDE,
  LOOK,
  PLAY,
  SLEEP,
  NOTHING,
};

enum Floor {
  BASEMENT,
  GROUND_FLOOR,
  FIRST_FLOOR,
  NO_FLOOR,
};

enum KeyType {
  CROWBAR,
  KITCHEN,
  LIBRARY,
  EXIT,
  KEY4,
  NOKEY,
};

enum DoorStyle {
  DOUBLE,
  SINGLE,
  LAB,
};

}  // end namespace Helpers

#endif
