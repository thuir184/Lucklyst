#include "logic.h"
#include "defs.h"
#include <ctime>
#include <random>   
#include <algorithm>


using namespace std;

void GameLogic::initInput() {
    inputFields = {
        {{500, 100, 200, 30}, ""},
        {{500, 140, 200, 30}, ""},
        {{500, 180, 200, 30}, ""},
        {{500, 220, 200, 30}, ""}
    };
    inputFields[0].active = true;
}

void GameLogic::toggleCursor() {
    Uint32 now = SDL_GetTicks(); // trả về số ms đã trôi qua khi bắt đầu nhập 
    if (now - lastCursorToggle >= CURSOR_BLINK_INTERVAL) {
        showCursor = !showCursor;
        lastCursorToggle = now;
    }
}

void GameLogic::startGame(Graphics& gfx) {
    inGame = true;
    treeShaking = true;
    fruits.clear();

    if (!customers.empty()) {
        random_device rd;
        mt19937 g(rd());
        shuffle(customers.begin(), customers.end(), g);

        struct Branch { int minX, maxX, minY, maxY; };
        vector<Branch> branches = {
            {330, 370, 90, 110}, {310, 390, 130, 160}, {280, 420, 180, 210}
        };

        for (size_t i = 0; i < customers.size(); ++i) {
            SDL_Texture* tex = gfx.fruitTextures[rand() % gfx.fruitTextures.size()];
            Branch branch = branches[rand() % branches.size()];
            int x = branch.minX + rand() % (branch.maxX - branch.minX);
            int y = branch.minY + rand() % (branch.maxY - branch.minY);

           fruits.push_back({ tex, { x, y, 40, 40 }, 0, customers[i].name });
        }
        Mix_PlayMusic(gfx.bgMusic, -1);
    }
}

void GameLogic::updateFruits(Graphics& gfx) {
    if (treeShaking) {
        Mix_PlayChannel(-1, gfx.dropSound, 0);
        for (int i = 0; i < 10; ++i) {
           SDL_Rect treeRect;
           if (i % 2 == 0) {
                treeRect = { 300 + 5, 80, 200, 300 };
            }
            else {
                treeRect = { 300 - 5, 80, 200, 300 };
            }
            SDL_RenderCopy(gfx.renderer, gfx.treeTexture, NULL, &treeRect);
            SDL_RenderPresent(gfx.renderer);
            SDL_Delay(30);
        }
        for (size_t i = 0; i < fruits.size(); ++i) {
            Fruit& fruit = fruits[i];
            if (!fruit.falling && !fruit.landed) {
                fruit.falling = true;
                fruit.speed = 2.0f + rand() % 3;
                break;
            }
        }
        treeShaking = false;
    }

    for (size_t i = 0; i < fruits.size(); ++i) {
        Fruit& fruit = fruits[i];
        if (fruit.falling && !fruit.landed) {
            fruit.rect.y += (int)fruit.speed;
            if (fruit.rect.y >= 550) {
                fruit.landed = true;
                fruit.falling = false;
                currentCustomerShown = fruit.customerName;
                showNameStartTime = SDL_GetTicks();
                Mix_HaltMusic();
                Mix_PlayChannel(-1, gfx.fallSound, 0);
            }
        }
    }
}

void GameLogic::renderGame(Graphics& gfx) {
    SDL_RenderCopy(gfx.renderer, gfx.bgTexture, NULL, NULL);
    SDL_Rect treeRect = { 300, 80, 200, 300 };
    SDL_RenderCopy(gfx.renderer, gfx.treeTexture, NULL, &treeRect);

    for (size_t i = 0; i < fruits.size(); ++i) {
        Fruit& fruit = fruits[i];
        SDL_RenderCopy(gfx.renderer, fruit.texture, NULL, &fruit.rect);
    }

    if (!currentCustomerShown.empty()) {
        gfx.renderText("Congratulations! " + currentCustomerShown, 180, 420, gfx.red, 32);
        gfx.renderReplayButton();
    }
}

void GameLogic::renderInputField(InputField& field, Graphics& gfx) {
    if (field.active) {
        SDL_SetRenderDrawColor(gfx.renderer, 200, 100, 100, 255);
    }
    else {
        SDL_SetRenderDrawColor(gfx.renderer, 100, 100, 100, 255);
    }
    SDL_RenderFillRect(gfx.renderer, &field.rect);
    gfx.renderText(field.text, field.rect.x + 5, field.rect.y + 5, gfx.white);

    if (field.active && showCursor) {
        int textW = 0, textH = 0;
        TTF_SizeText(gfx.font, field.text.c_str(), &textW, &textH);
        int cursorX = field.rect.x + 5 + textW;
        SDL_SetRenderDrawColor(gfx.renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(gfx.renderer, cursorX, field.rect.y + 5, cursorX, field.rect.y + field.rect.h - 5);
    }
    SDL_SetRenderDrawColor(gfx.renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(gfx.renderer, &field.rect);
}

void GameLogic::renderCustomerTable(Graphics& gfx, int x, int y) {
    gfx.renderText("Name", x, y, gfx.black);
    gfx.renderText("ID Number", x + 160, y, gfx.black);
    gfx.renderText("Date of Birth", x + 320, y, gfx.black);
    gfx.renderText("Account Number", x + 500, y, gfx.black);
    SDL_RenderDrawLine(gfx.renderer, x - 5, y + 25, x + 700, y + 25);

    int rowHeight = 30;
    for (size_t i = 0; i < customers.size(); ++i) {
        SDL_SetRenderDrawColor(gfx.renderer, 200, 200, 200, 255);
        SDL_Rect row = { x - 5, y + (int)(i + 1) * rowHeight - 5, 700, rowHeight };
        SDL_RenderFillRect(gfx.renderer, &row);
        gfx.renderText(customers[i].name, x, y + (int)(i + 1) * rowHeight, gfx.black);
        gfx.renderText(customers[i].id, x + 160, y + (int)(i + 1) * rowHeight, gfx.black);
        gfx.renderText(customers[i].dob, x + 320, y + (int)(i + 1) * rowHeight, gfx.black);
        gfx.renderText(customers[i].acc, x + 500, y + (int)(i + 1) * rowHeight, gfx.black);
        SDL_RenderDrawRect(gfx.renderer, &row);
    }
}

void GameLogic::renderInputUI(Graphics& gfx) {
    SDL_RenderCopy(gfx.renderer, gfx.bgTexture, NULL, NULL);

    gfx.renderText("Name", 380, 105, gfx.black);
    gfx.renderText("ID Number", 360, 145, gfx.black);
    gfx.renderText("Date of Birth", 340, 185, gfx.black);
    gfx.renderText("Account Number", 320, 225, gfx.black);

    for (size_t i = 0; i < inputFields.size(); ++i) {
        renderInputField(inputFields[i], gfx);
    }

    gfx.renderButton(addBtn, gfx.blue, "SUBMIT");
    gfx.renderButton(playBtn, gfx.red, "PLAY");

    renderCustomerTable(gfx, 80, 340);
}

void GameLogic::handleInputEvent(SDL_Event& e, Graphics& gfx) {
    SDL_Point mouse = { e.button.x, e.button.y };

    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (!inGame && SDL_PointInRect(&mouse, &addBtn)) {
            if (!inputFields[0].text.empty()) {
                customers.push_back({ inputFields[0].text, inputFields[1].text,
                                       inputFields[2].text, inputFields[3].text });
                for (size_t i = 0; i < inputFields.size(); ++i) {
                    inputFields[i].text = "";
                }
            }
        }
        else if (!inGame && SDL_PointInRect(&mouse, &playBtn)) {
            startGame(gfx);
        }
        else if (inGame && SDL_PointInRect(&mouse, &gfx.replayBtn)) {
            inGame = false;
            currentCustomerShown = "";
            fruits.clear();
            customers.clear();
            for (size_t i = 0; i < inputFields.size(); ++i) {
                inputFields[i].text = "";
            }
            Mix_HaltMusic();
        }
    }
    else if (!inGame && e.type == SDL_TEXTINPUT) {
        inputFields[currentInput].text += e.text.text;
    }
    else if (!inGame && e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_BACKSPACE && !inputFields[currentInput].text.empty()) {
            inputFields[currentInput].text.pop_back();
        }
        else if (e.key.keysym.sym == SDLK_TAB) {
            inputFields[currentInput].active = false;
            currentInput = (currentInput + 1) % inputFields.size();
            inputFields[currentInput].active = true;
        }
    }
}
