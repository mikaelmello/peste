#include "Options.hpp"

#include <stdexcept>

Options::Options(GameObject& c, std::vector<std::shared_ptr<GameObject>> b)
    : op(0), cursor(c), buttons(b) {
  if (b.size() == 0) {
    throw std::invalid_argument("must have at least 1 option");
  }
  SetCursorPosition(0);
}

Options::~Options() {}

Options& Options::operator++() {
  op = (op + 1) % buttons.size();
  SetCursorPosition(op);
  return *this;
}

Options Options::operator++(int) {
  Options result(*this);
  ++(*this);
  return result;
}

Options& Options::operator--() {
  op = op != 0 ? op - 1 : buttons.size() - 1;
  SetCursorPosition(op);
  return *this;
}

Options Options::operator--(int) {
  Options result(*this);
  --(*this);
  return result;
}

void Options::Last() {
  op = buttons.size() - 1;
  SetCursorPosition(op);
}

int Options::GetOperation() const { return op; }

void Options::SetCursorPosition(unsigned i) {
  cursor.box.x = (buttons[i]->box.x + buttons[i]->box.w * 0.85);
  cursor.box.y = buttons[i]->box.y;
}