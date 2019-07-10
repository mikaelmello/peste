#ifndef LORE_H
#define LORE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <string>

class Lore {
 public:
  ~Lore();

  static Lore& GetInstance();

 private:
  Lore();

  static Lore* instance;
};

#endif
