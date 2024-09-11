#ifndef BITSYGAMEPARSER_H
#define BITSYGAMEPARSER_H

#include "BitsyGame.h"
#include <iostream>

// Class responsible for parsing the Bitsy game data
class BitsyGameParser {
public:
    // Static method to parse the game data and return a BitsyGame object
    static BitsyGame parseGameData(const std::string& filePath);

private:
    static void parseGameTitle(BitsyGame& game, const std::string& line);  // Parse the game title
    static void parseSettings(BitsyGame& game, const std::string& line);  // Parse game settings
    static void parsePalette(BitsyGame& game, std::istream& file, const std::string& firstLine);  // Parse a palette
    static void parseRoom(BitsyGame& game, std::istream& file, const std::string& firstLine);  // Parse a room
    static void parseTile(BitsyGame& game, std::istream& file, const std::string& firstLine);  // Parse a tile
    static void parseAvatar(BitsyGame& game, std::istream& file, const std::string& firstLine);  // Parse the avatar
    static void parseSprite(BitsyGame& game, std::istream& file, const std::string& firstLine);  // Parse a sprite
    static void parseItem(BitsyGame& game, std::istream& file, const std::string& firstLine);  // Parse an item
    static void parseDialogue(BitsyGame& game, std::istream& file, const std::string& firstLine);  // Parse a dialogue
    static void parseVariable(BitsyGame& game, std::istream& file, const std::string& firstLine);  // Parse a variable
    static void parseTune(BitsyGame& game, std::istream& file, const std::string& firstLine);  // Parse a tune
    static void parseBlip(BitsyGame& game, std::istream& file, const std::string& firstLine);  // Parse a blip sound
};

#endif // BITSYGAMEPARSER_H
