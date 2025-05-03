#include "graphics.h"

void Graphics::init() {
    window = SDL_CreateWindow("Lucklyst",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    font = TTF_OpenFont(FONT_PATH, 20);


    bgTexture = loadTexture(BG_PATH);
    treeTexture = loadTexture(TREE_PATH);

    SDL_Texture* tex1 = loadTexture("asset/apple.png");
    SDL_Texture* tex2 = loadTexture("asset/blackberry.png");
    SDL_Texture* tex3 = loadTexture("asset/strawberry.png");
    
    fruitTextures = { tex1, tex2, tex3 };


    dropSound = Mix_LoadWAV(DROP_SOUND);
    fallSound = Mix_LoadWAV(FALL_SOUND);
    bgMusic = Mix_LoadMUS(BG_MUSIC);
}

void Graphics::quit() {
    for (size_t i = 0; i < fruitTextures.size(); ++i) {
        SDL_DestroyTexture(fruitTextures[i]);
    }
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(treeTexture);

    Mix_FreeChunk(dropSound);
    Mix_FreeChunk(fallSound);
    Mix_FreeMusic(bgMusic);

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* Graphics::loadTexture(const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) return nullptr;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}




void Graphics::renderText(const std::string& text, int x, int y, SDL_Color color, int size) {
    TTF_Font* tempFont = TTF_OpenFont(FONT_PATH, size);
    if (!tempFont) return;

    SDL_Surface* surface = TTF_RenderText_Solid(tempFont, text.c_str(), color);
    if (!surface) {
        TTF_CloseFont(tempFont);
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, nullptr, &dst);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(tempFont);
}

void Graphics::renderButton(SDL_Rect rect, SDL_Color color, const std::string& label) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
    renderText(label, rect.x + 10, rect.y + 8, white);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

void Graphics::renderReplayButton() {
    renderButton(replayBtn, blue, "Replay");
}
