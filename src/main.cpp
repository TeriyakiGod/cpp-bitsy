// main.cpp
#include "BitsyGame.h"
#include "BitsyGameParser.h"
#include <iostream>

int main() {
    BitsyGame game;
    game = BitsyGameParser::parseGameData("../game.bitsy");
    
    std::cout << "Game Title: " << game.title << std::endl;

    return 0;
}
