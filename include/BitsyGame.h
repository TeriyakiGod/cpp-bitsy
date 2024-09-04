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
    std::string effect;
    int dialogueId;
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
    char id = 'A';
    std::vector<std::string> frames;
    std::pair<int, std::pair<int, int>> position;
    std::vector<int> inventory;
};

// Sprite structure
struct Sprite {
    char id;
    std::vector<std::string> frames;
    std::string name;
    std::pair<int, std::pair<int, int>> position;
    int dialogId;
    int blipId;
};

// Item structure
struct Item {
    int id;
    std::vector<std::string> frames;
    std::string name;
    int dialogId;
    int blipId;
};

// Dialogue structure
struct Dialogue {
    int id;
    std::string text;
    std::string name;
};

// Tune structure
struct Tune {
    int id;
    std::vector<std::string> patterns;
    std::string name;
    std::string key;
    std::string tempo;
    std::string square;
    std::string arpeggio;
};

// Blip structure
struct Blip {
    int id;
    std::string notes;
    std::string name;
    std::vector<int> envelope;
    std::vector<int> beat;
    std::string square;
    int repeat;
};

// Variable structure
struct Variable {
    std::string name;
    int value;
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
