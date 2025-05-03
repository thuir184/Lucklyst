#include "graphics.h"
#include "logic.h"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    Graphics gfx;
    gfx.init();

    GameLogic game;
    game.initInput();

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        game.toggleCursor();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else {
                game.handleInputEvent(e, gfx);
            }
        }

        SDL_RenderClear(gfx.renderer);

        if (game.inGame) {
            game.updateFruits(gfx);
            game.renderGame(gfx);
        }
        else {
            game.renderInputUI(gfx);
        }

        SDL_RenderPresent(gfx.renderer);
        SDL_Delay(16); 
    }

    gfx.quit();
    return 0;
}
