#ifndef GAMEDATA_H
#define GAMEDATA_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Vec2.hpp"

class GameData {
 public:
  enum Types {
    CameraFollower,
    Collider,
    Sound,
    Sprite,
    Text,
    TileMap,
    Player,
    Antagonist,
  };

  static Vec2 hope_position;
};

#endif
