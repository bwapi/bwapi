#pragma once

#include <SDL.h>
#include <chrono>

#include <BW/Bitmap.h>
#include "GameRender.h"

class Window
{
public:
  static Window instance;

private:
  SDL_Window* m_window = nullptr;
  SDL_Renderer* m_renderer = nullptr;
  SDL_Palette* m_palette = nullptr;
  SDL_PixelFormat* m_pixelFormat = nullptr;
  SDL_Texture* m_texture = nullptr;

  SDL_Surface* m_windowSurface;
  SDL_Surface* m_bwSurface;

  int m_width = 640, m_height = 480;
public:
  ~Window()
  {
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
  }

  void create()
  {
    m_window = SDL_CreateWindow("StarCraft SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(m_renderer, m_width, m_height);

    m_windowSurface = SDL_CreateRGBSurface(0, m_width, m_height, 32, 0, 0, 0, 0);
    
    m_bwSurface = SDL_CreateRGBSurface(0, m_width, m_height, 8, 0, 0, 0, 0);
    m_palette = SDL_AllocPalette(256);
    SDL_SetSurfacePalette(m_bwSurface, m_palette);

    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, m_width, m_height);
  }

  void update()
  {
    // Throttle the updates to 33ms
    using Clock = std::chrono::high_resolution_clock;
    using milliseconds = std::chrono::milliseconds;

    static auto lastUpdate = Clock::now();
    auto now = Clock::now();
    if (std::chrono::duration_cast<milliseconds>(now - lastUpdate).count() < 33)
      return;

    lastUpdate = now;

    // Update the pixels
    Render::DrawGameUpdate();

    // Blit the results to the texture and render it to the window
    SDL_BlitSurface(m_bwSurface, nullptr, m_windowSurface, nullptr);

    void * pixels = nullptr;
    int pitch = 0;
    SDL_LockTexture(m_texture, nullptr, &pixels, &pitch);
    SDL_ConvertPixels(m_windowSurface->w, m_windowSurface->h, m_windowSurface->format->format, m_windowSurface->pixels, m_windowSurface->pitch,
      SDL_PIXELFORMAT_RGBA8888, pixels, pitch);
    SDL_UnlockTexture(m_texture);

    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);

    SDL_RenderPresent(m_renderer);
  }

  int width()
  {
    return m_width;
  }

  int height()
  {
    return m_height;
  }

  BW::Bitmap getBitmap()
  {
    return BW::Bitmap(m_bwSurface->w, m_bwSurface->h, m_bwSurface->pixels);
  }

  BW::Bitmap lockSurface()
  {
    SDL_LockSurface(m_bwSurface);
    return getBitmap();
  }

  void unlockSurface()
  {
    SDL_UnlockSurface(m_bwSurface);
  }

  void updatePalette(unsigned firstentry, unsigned numentries, SDL_Color *pPalEntries)
  {
    SDL_SetPaletteColors(m_palette, pPalEntries, firstentry, numentries);
    SDL_SetSurfacePalette(m_bwSurface, m_palette);
  }

};
