#ifndef BITSYGAME_H
#define BITSYGAME_H

#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <utility>

// Data structures used by BitsyGame
struct Palette {
    int id;  // Unique ID for the palette
    std::tuple<int, int, int> color1, color2, color3;  // RGB colors
    std::string name;  // Name of the palette
};

struct Room {
    int id;  // Unique ID for the room
    std::vector<std::vector<char>> tiles;  // 16x16 grid of tile IDs
    std::vector<std::pair<int, std::pair<int, int>>> items;  // List of items with positions
    std::vector<struct Exit> exits;  // Exits in the room
    std::vector<struct End> endings;  // Endings in the room
    int paletteId;  // Palette ID used in the room
    int tuneId;  // Tune ID
    std::string name;  // Room name
};

struct Tile {
    char id;  // Tile ID
    std::vector<std::string> frames;  // One or two frames of the tile
    std::string name;  // Tile name
    bool wall = false;  // Wall property
};

struct Sprite {
    char id;  // Sprite ID
    std::vector<std::string> frames;  // One or two frames
    std::string name;  // Sprite name
    int dialogId = -1;  // Dialogue ID (optional)
    int blipId = -1;  // Blip sound ID (optional)
    int roomId;  // Room ID
    std::pair<int, int> position;  // Position (x, y)
};

struct Avatar {
    char id = 'A';  // Avatar always has ID 'A'
    std::vector<std::string> frames;  // Frames for the avatar
    int roomId;  // Room ID where avatar starts
    std::pair<int, int> position;  // Position (x, y)
    std::vector<int> inventory;  // Avatar's inventory (item IDs)
};

struct Item {
    int id;  // Item ID
    std::vector<std::string> frames;  // One or two frames
    std::string name;  // Item name
    int dialogId = -1;  // Dialogue ID (optional)
    int blipId = -1;  // Blip sound ID (optional)
};

struct Exit {
    std::pair<int, int> startPosition;  // Starting position of exit
    int destinationRoomId;  // Destination room ID
    std::pair<int, int> destinationPosition;  // Destination position (x, y)
    std::string effect;  // Exit effect (optional)
    int dialogueId;  // Dialogue ID (optional)
};

struct End {
    int dialogueId;  // Dialogue ID
    std::pair<int, int> position;  // Position of ending (x, y)
};

struct Dialogue {
    int id;  // Dialogue ID
    std::string text;  // Dialogue text
    std::string name;  // Dialogue name (optional)
};

struct Variable {
    std::string name;  // Variable name
    std::string value;  // Variable value
};

struct Tune {
    int id;  // Tune ID
    std::vector<std::string> treblePatterns;  // Treble patterns
    std::vector<std::string> bassPatterns;  // Bass patterns
    std::string key;  // Key of the tune
    std::string tempo;  // Tempo of the tune
    std::string trebleInstrument;  // Treble instrument setting
    std::string bassInstrument;  // Bass instrument setting
    std::string arpeggio;  // Arpeggio (optional)
    std::string name;  // Tune name (optional)
};

struct Blip {
    int id;  // Blip ID
    std::string notes;  // Blip notes
    std::vector<int> env;  // Envelope values
    std::vector<int> beat;  // Beat values
    std::string squareWave;  // Square wave setting
    int repeat = 0;  // Repeat count
    std::string name;  // Blip name (optional)
};

struct Settings {
        int verMaj, verMin, roomFormat, dlgCompat, txtMode;  // Game settings
    };

// BitsyGame
class BitsyGame {
public:
    void printGameStats() const;  // Print game statistics
    std::string title;  // Game title
    Settings settings;  // Game settings
    std::vector<Palette> palettes;  // Palettes in the game
    std::vector<Room> rooms;  // Rooms in the game
    std::vector<Tile> tiles;  // Tiles in the game
    std::vector<Sprite> sprites;  // Sprites in the game
    Avatar avatar;  // Avatar
    std::vector<Item> items;  // Items in the game
    std::vector<Dialogue> dialogues;  // Dialogues in the game
    std::map<std::string, Variable> variables;  // Game variables
    std::vector<Tune> tunes;  // Tunes in the game
    std::vector<Blip> blips;  // Blip sounds in the game
};

#endif // BITSYGAME_H
