// main.cpp
#include "BitsyGame.h"
#include <iostream>

int main() {
    BitsyGame game;
    game.parseGameData("game.bitsy");
    
    std::cout << "Game Title: " << game.title << std::endl;

    return 0;
}
