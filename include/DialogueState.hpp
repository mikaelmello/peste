#ifndef DIALOGUESTATE_H
#define DIALOGUESTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Music.hpp"
#include "State.hpp"

class DialogueState : public State {
 public:
  DialogueState(const std::vector<std::pair<std::string, std::string>>& script);

  ~DialogueState();

  void Start();

  void Pause();

  void Resume();

  bool QuitRequested() const;

  void LoadAssets();

  void Update(float dt);

  void Render();

 private:
  unsigned int dialogueIndex;
  std::vector<std::pair<std::string, std::string>> script;
};

#endif
