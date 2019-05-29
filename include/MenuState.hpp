#ifndef MENUSTATE_H
#define MENUSTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define MENU_BACKGROUND_SPRITE "assets/img/temp_menu_bg.jpg"
#define MENU_PLAY_BUTTON_SPRITE "assets/img/temp_button.png"
#define MENU_TUTORIAL_BUTTON_SPRITE "assets/img/temp_button.png"
#define MENU_QUIT_BUTTON_SPRITE "assets/img/temp_button.png"
#define MENU_CURSOR_SPRITE "assets/img/temp_cursor.png"

#include <limits>
#include "Helpers.hpp"
#include "Music.hpp"
#include "State.hpp"

class MenuState : public State {
 public:
  MenuState();

  ~MenuState();

  void Start();

  void Pause();

  void Resume();

  bool QuitRequested() const;

  void LoadAssets();

  void Update(float dt);

  void Render();

 private:
  struct Cursor {
    enum CursorState { play, tutorial, quit, Count };

    Cursor() : cursor_state(CursorState::play), cursor_go(nullptr) {}

    inline Cursor& operator++() {
      int i = static_cast<int>(cursor_state);
      i = (i + 1) % CursorState::Count;
      cursor_state = static_cast<CursorState>(i);
      cursor_go->box = cursor_go->box.GetCentered(positions[i]);
      return *this;
    }

    inline Cursor operator++(int) {
      Cursor result(*this);
      ++(*this);
      return result;
    }

    inline Cursor& operator--() {
      int i = static_cast<int>(cursor_state);
      i = i != 0 ? i - 1 : Count - 1;
      cursor_state = static_cast<CursorState>(i);
      cursor_go->box = cursor_go->box.GetCentered(positions[i]);
      return *this;
    }

    inline Cursor operator--(int) {
      Cursor result(*this);
      --(*this);
      return result;
    }

    void setQuit() {
      cursor_state = Cursor::CursorState::quit;
      cursor_go->box = cursor_go->box.GetCentered(
          positions[static_cast<int>(CursorState::quit)]);
    }

    std::vector<Vec2> positions;

    CursorState cursor_state;

    GameObject* cursor_go;
  };

  Cursor cursor;

  const Vec2 position;

  const int button_offset;
};

#endif
