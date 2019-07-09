#ifndef OPTIONS_H
#define OPTIONS_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <vector>
#include "GameObject.hpp"

class Options {
 public:
  Options(GameObject& cursor, std::vector<std::shared_ptr<GameObject>> buttons);

  ~Options();

  Options& operator++();

  Options operator++(int);

  Options& operator--();

  Options operator--(int);

  void Last();

  int GetOperation() const;

 private:
  void SetCursorPosition(unsigned i);

  unsigned op;

  GameObject& cursor;

  std::vector<std::shared_ptr<GameObject>> buttons;
};

#endif
