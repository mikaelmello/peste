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
  static int Slept;
  static void Sleep();
  static void FirstMonsterSpawn();
};

#endif
