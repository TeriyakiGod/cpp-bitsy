#ifndef BITSYGAMEPARSER_H
#define BITSYGAMEPARSER_H

#include "BitsyGameData.h"
#include <iostream>

// Class responsible for parsing the Bitsy game data
class BitsyGameParser {
public:
    // Static method to parse the game data and return a BitsyGameData object
    static BitsyGameData parseGameData(const std::string& filePath);

private:
    static void parseGameTitle(BitsyGameData& game, const std::string& line);  // Parse the game title
    static void parseSettings(BitsyGameData& game, const std::string& line);  // Parse game settings
    static void parsePalette(BitsyGameData& game, std::istream& file, const std::string& firstLine);  // Parse a palette
    static void parseRoom(BitsyGameData& game, std::istream& file, const std::string& firstLine);  // Parse a room
    static void parseTile(BitsyGameData& game, std::istream& file, const std::string& firstLine);  // Parse a tile
    static void parseAvatar(BitsyGameData& game, std::istream& file, const std::string& firstLine);  // Parse the avatar
    static void parseSprite(BitsyGameData& game, std::istream& file, const std::string& firstLine);  // Parse a sprite
    static void parseItem(BitsyGameData& game, std::istream& file, const std::string& firstLine);  // Parse an item
    static void parseDialogue(BitsyGameData& game, std::istream& file, const std::string& firstLine);  // Parse a dialogue
    static void parseVariable(BitsyGameData& game, std::istream& file, const std::string& firstLine);  // Parse a variable
    static void parseTune(BitsyGameData& game, std::istream& file, const std::string& firstLine);  // Parse a tune
    static void parseBlip(BitsyGameData& game, std::istream& file, const std::string& firstLine);  // Parse a blip sound
};

#endif // BITSYGAMEPARSER_H
