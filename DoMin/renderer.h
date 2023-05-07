#ifndef Renderer_h_
#define Renderer_h_

#include "DEFINE.h"
#include "site.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height);
  ~Renderer();

  bool isPlaying = false;

  void renderText(const char* text, SDL_Color color,SDL_Rect rect);
  void renderText(const char* text, SDL_Color color,int x, int y);
  void renderImage(SDL_Rect rect, const char* imageName);

  SDL_Surface* loadImage(const char* fileName);

  void renderSite(Site &site);
  void onGUI(Site *site, int mine, const char* time);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;

  bool anyCellClicked = false;
};

#endif