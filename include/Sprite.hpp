#ifndef SPRITE_H
#define SPRITE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define INCLUDE_SDL
#include <string>
#include "Component.hpp"
#include "GameObject.hpp"
#include "SDL_include.h"
#include "Timer.hpp"
#include "Types.hpp"

class Sprite : public Component {
 public:
  Sprite(GameObject& associated, int frameCount = 1, float frameTime = 1,
         float secondsToSelfDestruct = 0);

  Sprite(GameObject& associated, const std::string& file, int frameCount = 1,
         float frameTime = 1, float secondsToSelfDestruct = 0);

  ~Sprite();

  void Open(const std::string& file);

  void SetClip(int x, int y, int w, int h);

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render(int x, int y);

  void Render() override;

  int GetWidth() const;

  int GetHeight() const;

  bool IsOpen() const;

  void SetScaleX(float scaleX, float scaleY);

  void SetDimensions(int maxWidth, int maxHeight);

  void SetFrame(int frame);

  void SetFrameCount(int frameCount);

  void SetFrameTime(float frameTime);

  Vec2 GetScale();

  const Types Type = SpriteType;

 private:
  std::shared_ptr<SDL_Texture> texture;
  int width;
  int height;
  int frameCount;
  int currentFrame;
  float frameTime;
  float secondsToSelfDestruct;
  Timer currentFrameCount;
  Timer selfDestructCount;
  SDL_Rect clipRect;
  Vec2 scale;
};

#endif
