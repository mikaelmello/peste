#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#include "Sprite.hpp"
#include <stdexcept>
#include <string>
#include "Camera.hpp"
#include "Component.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "Resources.hpp"
#include "SDL_include.h"
#include "Timer.hpp"
#include "Types.hpp"

Sprite::Sprite(GameObject& associated, int frameCount, float frameTime,
               float secondsToSelfDestruct)
    : Component(associated),
      frameCount(frameCount),
      currentFrame(0),
      frameTime(frameTime),
      secondsToSelfDestruct(secondsToSelfDestruct),
      scale(1, 1) {}

Sprite::Sprite(GameObject& associated, const std::string& file, int frameCount,
               float frameTime, float secondsToSelfDestruct)
    : Sprite(associated, frameCount, frameTime, secondsToSelfDestruct) {
  Open(file);
}

Sprite::~Sprite() {}

void Sprite::Open(const std::string& file) {
  this->file = file;
  texture = Resources::GetImage(file);
  int return_code =
      SDL_QueryTexture(texture.get(), nullptr, nullptr, &width, &height);
  if (return_code != 0) {
    throw std::runtime_error("Could not query invalid texture from " + file +
                             ": " + IMG_GetError());
  }

  associated.box.w = width / frameCount;
  associated.box.h = height;
  SetClip(0, 0, width / frameCount, height);
}

void Sprite::SetClip(int x, int y, int w, int h) { clipRect = {x, y, w, h}; }

void Sprite::Render(int x, int y) {
  SDL_Rect temp_rect = {x, y, (int)round(scale.x * clipRect.w),
                        (int)round(scale.y * clipRect.h)};
  int return_code = SDL_RenderCopyEx(
      Game::GetInstance().GetRenderer(), texture.get(), &clipRect, &temp_rect,
      associated.angleDeg, nullptr, SDL_FLIP_NONE);
  if (return_code != 0) {
    throw std::runtime_error("Could not copy sprite to rendering target: " +
                             std::string(IMG_GetError()));
  }
}

void Sprite::Render() {
  Render(associated.box.x - Camera::pos.x, associated.box.y - Camera::pos.y);
}

void Sprite::Update(float dt) {
  currentFrameCount.Update(dt);
  selfDestructCount.Update(dt);

  if (secondsToSelfDestruct > 0 &&
      selfDestructCount.Get() > secondsToSelfDestruct) {
    associated.RequestDelete();
  }

  if (currentFrameCount.Get() > frameTime) {
    int framesElapsed = currentFrameCount.Get() / frameTime;
    int nextFrame = (currentFrame + framesElapsed) % frameCount;
    SetFrame(nextFrame);
    currentFrameCount.Restart();
  }
}

bool Sprite::Is(Types type) const { return type == this->Type; }

int Sprite::GetWidth() const { return round(scale.x * (width / frameCount)); }

int Sprite::GetHeight() const { return round(scale.y * height); }

bool Sprite::IsOpen() const { return (bool)texture; }

std::string Sprite::GetFile() { return this->file; }

void Sprite::SetScaleX(float scaleX, float scaleY) {
  Vec2 oldCenter = associated.box.Center();

  if (scaleX != 0) {
    scale.x = scaleX;
    associated.box.w = GetWidth();
  }
  if (scaleY != 0) {
    scale.y = scaleY;
    associated.box.h = GetHeight();
  }

  associated.box.SetCenter(oldCenter);
}

void Sprite::SetFrame(int frame) {
  currentFrame = frame;
  SetClip(currentFrame * width / frameCount, 0, width / frameCount, height);
}

void Sprite::SetFrameCount(int frameCount) {
  this->frameCount = frameCount;
  associated.box.w = GetWidth();
  SetFrame(0);
}

void Sprite::SetFrameTime(float frameTime) { this->frameTime = frameTime; }

Vec2 Sprite::GetScale() { return scale; }

void Sprite::SetDimensions(int maxWidth, int maxHeight) {
  double xScale = (double)maxWidth / (double)width;
  double yScale = (double)maxHeight / (double)height;
  double chosenScale = std::min(xScale, yScale);
  SetScaleX(chosenScale, chosenScale);
}
