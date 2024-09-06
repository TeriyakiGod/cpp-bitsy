#ifndef BITSY_GAME_H
#define BITSY_GAME_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <tuple>

// Settings structure
struct Settings {
    int verMaj;
    int verMin;
    int roomFormat;
    int dlgCompat;
    int txtMode;
};

// Palette structure
struct Palette {
    int id;
    std::tuple<int, int, int> color1;
    std::tuple<int, int, int> color2;
    std::tuple<int, int, int> color3;
    std::string name;
};

struct Exit {
    std::pair<int, int> startPosition; // (x, y)
    int destinationRoomId;
    std::pair<int, int> destinationPosition; // (x, y) in the destination room
    std::string effect = "none";
    int dialogueId = -1;
};

struct End {
    int dialogueId;
    std::pair<int, int> position; // (x, y)
};

struct Room {
    int id;
    std::vector<std::vector<char>> tiles;
    std::string name;
    std::vector<std::pair<int, std::pair<int, int>>> items;
    std::vector<Exit> exits; // Store exits
    std::vector<End> endings; // Store endings
    int paletteId;
    int tuneId;
};

// Tile structure
struct Tile {
    char id;
    std::vector<std::string> frames;
    std::string name;
    bool wall;
};

// Avatar structure
struct Avatar {
    char id = 'A';                        // Avatar ID is always 'A'
    std::vector<std::string> frames;      // Frames for the avatar's animation
    int roomId;                           // Room ID where the avatar is located
    std::pair<int, int> position;         // (x, y) position within the room
    std::vector<int> inventory;           // List of items in the avatar's inventory
};

struct Sprite {
    char id;                              // Sprite ID
    std::vector<std::string> frames;      // Frames for the sprite's animation
    int roomId;                           // Room ID where the sprite is located
    std::pair<int, int> position;         // (x, y) position within the room
    int dialogId = -1;                    // Dialogue ID (-1 if no dialogue)
    int blipId = -1;                      // Blip sound ID (-1 if no blip)
    std::string name;                     // Sprite name
};

// Item structure
struct Item {
    int id;                              // Item ID
    std::vector<std::string> frames;     // Frames for the item's animation
    std::string name;                    // Item name
    int dialogId = -1;                   // Dialogue ID (-1 if no dialogue)
    int blipId = -1;                     // Blip sound ID (-1 if no blip)
};

// Dialogue structure
struct Dialogue {
    int id;
    std::string text;
    std::string name;
};

// Tune structure
struct Tune {
    int id;                              // Tune ID
    std::vector<std::string> treblePatterns; // Treble patterns (melody)
    std::vector<std::string> bassPatterns;   // Bass patterns (harmony)
    std::string name;                    // Tune name
    std::string key;                     // Musical key
    std::string tempo;                   // Tempo
    std::string trebleInstrument;        // Instrument for treble (P2, P4, P8)
    std::string bassInstrument;          // Instrument for bass (P2, P4, P8)
    std::string arpeggio;                // Arpeggio settings
};

// Blip structure
struct Blip {
    int id;                    // Blip ID
    std::string notes;         // Musical notes
    std::string name;          // Blip name
    std::vector<int> env;      // Envelope values
    std::vector<int> beat;     // Beat values
    std::string squareWave;    // Square wave setting (P2, P4, etc.)
    int repeat = 0;            // Repeat value
};

// Variable structure
struct Variable {
    std::string name;
    std::string value;
};

// BitsyGame class
class BitsyGame {
public:
    std::string title;
    Settings settings;
    Avatar avatar;
    std::vector<Palette> palettes;
    std::vector<Room> rooms;
    std::vector<Tile> tiles;
    std::vector<Sprite> sprites;
    std::vector<Item> items;
    std::vector<Dialogue> dialogues;
    std::vector<Tune> tunes;
    std::vector<Blip> blips;
    std::unordered_map<std::string, Variable> variables;

    void parseGameData(const std::string& filePath);
    
    void parseGameTitle(const std::string& line);
    void parseSettings(const std::string& line);
    void parsePalette(std::istream& file, const std::string& firstLine);
    void parseRoom(std::istream& file, const std::string& firstLine);
    void parseTile(std::istream& file, const std::string& firstLine);
    void parseAvatar(std::istream& file, const std::string& firstLine);
    void parseSprite(std::istream& file, const std::string& firstLine);
    void parseItem(std::istream& file, const std::string& firstLine);
    void parseDialogue(std::istream& file, const std::string& firstLine);
    void parseTune(std::istream& file, const std::string& firstLine);
    void parseBlip(std::istream& file, const std::string& firstLine);
    void parseVariable(std::istream& file, const std::string& firstLine);
};

#endif // BITSY_GAME_H
