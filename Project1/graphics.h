#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <vector>
#include "defs.h"

struct Graphics {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;

    SDL_Texture* bgTexture = nullptr;
    SDL_Texture* treeTexture = nullptr;
    std::vector<SDL_Texture*> fruitTextures;

    Mix_Chunk* dropSound = nullptr;
    Mix_Chunk* fallSound = nullptr;
    Mix_Music* bgMusic = nullptr;

    SDL_Rect replayBtn = { 340, 500, 120, 50 };

    SDL_Color black = { 0, 0, 0, 255 };
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color grey = { 120, 120, 120, 255 };
    SDL_Color blue = { 0, 120, 255, 255 };
    SDL_Color red = { 255, 80, 80, 255 };

    void init();
    void quit();

    SDL_Texture* loadTexture(const std::string& path);
    void renderText(const std::string& text, int x, int y, SDL_Color color, int size = 20);
    void renderButton(SDL_Rect rect, SDL_Color color, const std::string& label);
    void renderReplayButton();
};

#endif
