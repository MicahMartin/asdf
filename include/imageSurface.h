#ifndef _imageSurface_h
#define _imageSurface_h

#include <SDL2_image/SDL_image.h>

class ImageSurface{
public:
  ImageSurface();
  ImageSurface(const char* imgPath);
  ~ImageSurface();

  void loadImg(const char* imgPath);
  void blitImg(SDL_Surface* surface);

private:
  SDL_Surface* imgBmp;
  /* data */
};
#endif
