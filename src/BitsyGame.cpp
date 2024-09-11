#include "BitsyGame.h"
#include <iostream>

void BitsyGame::printGameStats() const {
    std::cout << "Game Title: " << title << std::endl;
    std::cout << "Version: " << settings.verMaj << "." << settings.verMin << std::endl;
    std::cout << "Number of Palettes: " << palettes.size() << std::endl;
    std::cout << "Number of Rooms: " << rooms.size() << std::endl;
    std::cout << "Number of Tiles: " << tiles.size() << std::endl;
    std::cout << "Number of Sprites: " << sprites.size() << std::endl;
    std::cout << "Number of Items: " << items.size() << std::endl;
    std::cout << "Number of Dialogues: " << dialogues.size() << std::endl;
    std::cout << "Number of Tunes: " << tunes.size() << std::endl;
    std::cout << "Number of Blips: " << blips.size() << std::endl;
    std::cout << "Number of Variables: " << variables.size() << std::endl;
}
