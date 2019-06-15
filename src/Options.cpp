#include "Options.hpp"

#include <stdexcept>

Options::Options(GameObject& c, std::vector<std::shared_ptr<GameObject>>& b)
    : cursor(c), buttons(b), op(0) {
  if (b.size() == 0) {
    throw std::invalid_argument("must have at least 1 option");
  }

  cursor.box = cursor.box.GetCentered(buttons[0]->box.Center());
}

Options::~Options() {}

Options& Options::operator++() {
  op = (op + 1) % buttons.size();
  cursor.box = cursor.box.GetCentered(buttons[op]->box.Center());
  return *this;
}

Options Options::operator++(int) {
  Options result(*this);
  ++(*this);
  return result;
}

Options& Options::operator--() {
  op = op != 0 ? op - 1 : buttons.size() - 1;
  cursor.box = cursor.box.GetCentered(buttons[op]->box.Center());
  return *this;
}

Options Options::operator--(int) {
  Options result(*this);
  --(*this);
  return result;
}

void Options::Last() {
  op = buttons.size() - 1;
  cursor.box = cursor.box.GetCentered(buttons[op]->box.Center());
}

int Options::GetOperation() const { return op; }