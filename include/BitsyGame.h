#ifndef BITSYGAME_H
#define BITSYGAME_H

#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <utility>
#include <iostream>

// Represents a color palette with 3 RGB colors and an optional name.
struct Palette {
    int id;  // Unique ID for the palette
    std::tuple<int, int, int> color1, color2, color3;  // 3 RGB colors
    std::string name;  // Name of the palette
};

// Represents a room with a 16x16 grid of tiles, items, exits, endings, and other attributes.
struct Room {
    int id;  // Unique ID for the room
    std::vector<std::vector<char>> tiles;  // 16x16 grid of tile IDs
    std::vector<std::pair<int, std::pair<int, int>>> items;  // List of items with their positions
    std::vector<struct Exit> exits;  // List of exits in the room
    std::vector<struct End> endings;  // List of possible endings in the room
    int paletteId;  // Palette ID associated with the room
    int tuneId;  // Tune ID associated with the room
    std::string name;  // Name of the room
};

// Represents a tile with one or two frames, a name, and an optional wall property.
struct Tile {
    char id;  // Unique ID for the tile
    std::vector<std::string> frames;  // One or two frames of the tile
    std::string name;  // Name of the tile
    bool wall = false;  // Indicates if the tile is a wall
};

// Represents a sprite with one or two frames, a name, position, and optional dialogue and blip IDs.
struct Sprite {
    char id;  // Unique ID for the sprite
    std::vector<std::string> frames;  // One or two frames of the sprite
    std::string name;  // Name of the sprite
    int dialogId = -1;  // Optional dialogue ID associated with the sprite
    int blipId = -1;  // Optional blip sound ID associated with the sprite
    int roomId;  // Room ID where the sprite is located
    std::pair<int, int> position;  // Position of the sprite in the room (x, y)
};

// Represents the avatar, a special sprite controlled by the player.
struct Avatar {
    char id = 'A';  // Avatar always has the ID 'A'
    std::vector<std::string> frames;  // One or two frames of the avatar
    int roomId;  // Room ID where the avatar starts
    std::pair<int, int> position;  // Starting position of the avatar (x, y)
    std::vector<int> inventory;  // List of item IDs in the avatar's inventory
};

// Represents an item with one or two frames, a name, and optional dialogue and blip IDs.
struct Item {
    int id;  // Unique ID for the item
    std::vector<std::string> frames;  // One or two frames of the item
    std::string name;  // Name of the item
    int dialogId = -1;  // Optional dialogue ID associated with the item
    int blipId = -1;  // Optional blip sound ID associated with the item
};

// Represents an exit with a start position, destination room, destination position, optional effect, and dialogue ID.
struct Exit {
    std::pair<int, int> startPosition;  // Start position of the exit (x, y)
    int destinationRoomId;  // ID of the destination room
    std::pair<int, int> destinationPosition;  // Position in the destination room (x, y)
    std::string effect;  // Optional effect (e.g., "wave", "slide_d")
    int dialogueId;  // Optional dialogue ID triggered by the exit
};

// Represents an ending with a dialogue ID and position.
struct End {
    int dialogueId;  // Dialogue ID for the ending
    std::pair<int, int> position;  // Position of the ending (x, y)
};

// Represents a dialogue with text and an optional name.
struct Dialogue {
    int id;  // Unique ID for the dialogue
    std::string text;  // Text of the dialogue
    std::string name;  // Optional name of the dialogue
};

// Represents a variable with a name and a string value.
struct Variable {
    std::string name;  // Name of the variable
    std::string value;  // Value of the variable
};

// Represents a tune with treble and bass patterns, a key, tempo, instruments, and an optional arpeggio.
struct Tune {
    int id;  // Unique ID for the tune
    std::vector<std::string> treblePatterns;  // Treble patterns of the tune
    std::vector<std::string> bassPatterns;  // Bass patterns of the tune
    std::string key;  // Key of the tune
    std::string tempo;  // Tempo of the tune
    std::string trebleInstrument;  // Treble instrument setting
    std::string bassInstrument;  // Bass instrument setting
    std::string arpeggio;  // Optional arpeggio setting
    std::string name;  // Optional name of the tune
};

// Represents a blip sound with notes, envelope, beat, square wave settings, repeat count, and a name.
struct Blip {
    int id;  // Unique ID for the blip
    std::string notes;  // Musical notes of the blip
    std::vector<int> env;  // Envelope values for the blip
    std::vector<int> beat;  // Beat values for the blip
    std::string squareWave;  // Square wave setting
    int repeat = 0;  // Number of times the blip repeats
    std::string name;  // Optional name of the blip
};

// Main class to handle Bitsy game data parsing
class BitsyGame {
public:
    std::string title;  // The title of the game
    struct {
        int verMaj, verMin, roomFormat, dlgCompat, txtMode;  // Game settings
    } settings;
    std::vector<Palette> palettes;  // List of color palettes
    std::vector<Room> rooms;  // List of rooms
    std::vector<Tile> tiles;  // List of tiles
    std::vector<Sprite> sprites;  // List of sprites
    Avatar avatar;  // The avatar (controlled by the player)
    std::vector<Item> items;  // List of items
    std::vector<Dialogue> dialogues;  // List of dialogues
    std::map<std::string, Variable> variables;  // Map of game variables
    std::vector<Tune> tunes;  // List of tunes
    std::vector<Blip> blips;  // List of blip sounds

    // Methods for parsing different parts of the game data
    void parseGameTitle(const std::string& line);  // Parse the game title
    void parseSettings(const std::string& line);  // Parse game settings
    void parsePalette(std::istream& file, const std::string& firstLine);  // Parse a color palette
    void parseRoom(std::istream& file, const std::string& firstLine);  // Parse a room
    void parseTile(std::istream& file, const std::string& firstLine);  // Parse a tile
    void parseAvatar(std::istream& file, const std::string& firstLine);  // Parse the avatar
    void parseSprite(std::istream& file, const std::string& firstLine);  // Parse a sprite
    void parseItem(std::istream& file, const std::string& firstLine);  // Parse an item
    void parseDialogue(std::istream& file, const std::string& firstLine);  // Parse a dialogue
    void parseVariable(std::istream& file, const std::string& firstLine);  // Parse a variable
    void parseTune(std::istream& file, const std::string& firstLine);  // Parse a tune
    void parseBlip(std::istream& file, const std::string& firstLine);  // Parse a blip sound
    void parseGameData(const std::string& filePath);  // Parse the entire game data from a file
};

#endif // BITSYGAME_H
