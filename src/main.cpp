// main.cpp
#include "BitsyGameData.h"
#include "BitsyGameParser.h"
#include <iostream>

int main() {
    BitsyGameData game;
    game = BitsyGameParser::parseGameData("../game.bitsy");
    
    std::cout << "Game Title: " << game.title << std::endl;

    return 0;
}
