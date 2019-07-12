#ifndef LORE_H
#define LORE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <string>

class Lore {
 public:
  static bool HasEnteredMasterBedroom;
  static bool NicePerson;
  static bool Caught;
  static int Slept;
  static int CaughtCount;
  static void Sleep();
  static void FirstMonsterSpawn();
  static void UnlockBasement();
  static void PlayerCaught();
};

#endif
