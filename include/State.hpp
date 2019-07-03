#ifndef STATE_H
#define STATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include "GameObject.hpp"
#include "Music.hpp"
#include "Sprite.hpp"
#include "TileMap.hpp"
#include "TileSet.hpp"

class State {
 public:
  State();

  virtual ~State();

  virtual void LoadAssets() = 0;

  virtual void Update(float dt) = 0;

  virtual void Render() = 0;

  virtual void Start() = 0;

  virtual void Pause() = 0;

  virtual void Resume() = 0;

  virtual TileMap* GetCurrentTileMap();

  virtual std::shared_ptr<GameObject> AddObject(GameObject* go);

  virtual std::shared_ptr<GameObject> GetObjectPtr(GameObject* go);

  bool QuitRequested() const;

  bool PopRequested() const;

  void SortObjects();

 protected:
  void StartArray();

  virtual void UpdateArray(float dt);

  virtual void RenderArray();

  bool popRequested;
  bool quitRequested;
  bool started;

  TileMap* currentTileMap;

  struct GameObjectComp {
    bool operator()(const std::shared_ptr<GameObject>& lhs,
                    const std::shared_ptr<GameObject>& rhs) const {
      return lhs->priority < rhs->priority;
    }
  };

  std::vector<std::shared_ptr<GameObject>> objects;
  std::queue<std::shared_ptr<GameObject>> enqueuedObjects;
};

#endif
