#ifndef _LOGIC__H
#define _LOGIC__H

#include <SDL.h>
#include <vector>
#include <string>
#include "graphics.h"

struct InputField {
    SDL_Rect rect;
    std::string text;
    bool active = false;
};

struct Customer {
    std::string name, id, dob, acc;
};

struct Fruit {
    SDL_Texture* texture;
    SDL_Rect rect;
    float speed;
    std::string customerName;
    bool falling = false;
    bool landed = false;
};

struct GameLogic {
    std::vector<InputField> inputFields;
    std::vector<Customer> customers;
    std::vector<Fruit> fruits;

    int currentInput = 0;
    bool inGame = false;
    bool treeShaking = false;
    bool showCursor = true;
    Uint32 lastCursorToggle = 0;

    std::string currentCustomerShown = "";
    Uint32 showNameStartTime = 0;

    SDL_Rect addBtn = { 320, 270, 100, 40 };
    SDL_Rect playBtn = { 440, 270, 100, 40 };

    void initInput();
    void toggleCursor();
    void startGame(Graphics& gfx);
    void updateFruits(Graphics& gfx);
    void renderGame(Graphics& gfx);
    void renderInputUI(Graphics& gfx);
    void handleInputEvent(SDL_Event& e, Graphics& gfx);
    void renderInputField(InputField& field, Graphics& gfx);
    void renderCustomerTable(Graphics& gfx, int x, int y);
};

#endif
