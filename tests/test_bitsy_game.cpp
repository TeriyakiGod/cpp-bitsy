#include "../include/BitsyGame.h"
#include <iostream>
#include <sstream>
#include <cassert>

// Test for parsing the game title
void test_parse_game_title() {
    BitsyGame game;
    std::string titleLine = "My Awesome Bitsy Game";
    game.parseGameTitle(titleLine);

    assert(game.title == "My Awesome Bitsy Game");
    std::cout << "test_parse_game_title passed!" << std::endl;
}

// Test for parsing game settings
void test_parse_settings() {
    BitsyGame game;
    std::string settingsLine1 = "! VER_MAJ 8";
    std::string settingsLine2 = "! VER_MIN 12";
    std::string settingsLine3 = "! ROOM_FORMAT 1";
    std::string settingsLine4 = "! DLG_COMPAT 0";
    std::string settingsLine5 = "! TXT_MODE 0";

    game.parseSettings(settingsLine1);
    game.parseSettings(settingsLine2);
    game.parseSettings(settingsLine3);
    game.parseSettings(settingsLine4);
    game.parseSettings(settingsLine5);

    assert(game.settings.verMaj == 8);
    assert(game.settings.verMin == 12);
    assert(game.settings.roomFormat == 1);
    assert(game.settings.dlgCompat == 0);
    assert(game.settings.txtMode == 0);

    std::cout << "test_parse_settings passed!" << std::endl;
}

void test_parse_palette() {
    BitsyGame game;
    
    // Simulate the input for a palette definition
    std::istringstream input(
        "PAL 0\n"
        "0,82,204\n"
        "128,159,255\n"
        "255,255,255\n"
        "NAME blueprint\n"
    );

    // First line should be PAL 0
    std::string firstLine;
    std::getline(input, firstLine);
    
    // Call the parsePalette method
    game.parsePalette(input, firstLine);

    // Test the results
    assert(game.palettes.size() == 1);
    Palette &palette = game.palettes[0];

    assert(palette.id == 0);

    // Test the first color
    assert(std::get<0>(palette.color1) == 0);
    assert(std::get<1>(palette.color1) == 82);
    assert(std::get<2>(palette.color1) == 204);

    // Test the second color
    assert(std::get<0>(palette.color2) == 128);
    assert(std::get<1>(palette.color2) == 159);
    assert(std::get<2>(palette.color2) == 255);

    // Test the third color
    assert(std::get<0>(palette.color3) == 255);
    assert(std::get<1>(palette.color3) == 255);
    assert(std::get<2>(palette.color3) == 255);

    // Test the palette name
    assert(palette.name == "blueprint");

    std::cout << "test_parse_palette passed!" << std::endl;
}

void test_parse_room() {
    BitsyGame game;
    
    std::istringstream input(
        "ROOM 0\n"
        "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"
        "0,a,a,a,a,a,a,a,a,a,a,a,a,a,a,0\n"
        "0,a,0,0,0,0,0,0,0,0,0,0,0,0,a,0\n"
        "0,a,0,0,0,0,0,0,0,0,0,0,0,0,a,0\n"
        "0,a,0,0,0,0,0,0,0,0,0,0,0,0,a,0\n"
        "0,a,0,0,0,0,0,0,0,0,0,0,0,0,a,0\n"
        "0,a,0,0,0,0,0,0,0,0,0,0,0,0,a,0\n"
        "0,a,0,0,0,0,0,0,0,0,0,0,0,0,a,0\n"
        "0,a,0,0,0,0,0,0,0,0,0,0,0,0,a,0\n"
        "0,a,0,0,0,0,0,0,0,0,0,0,0,0,a,0\n"
        "0,a,0,0,0,0,0,0,0,0,0,0,0,0,a,0\n"
        "0,a,0,0,0,0,0,0,0,0,0,0,0,0,a,0\n"
        "0,a,0,0,0,0,0,0,0,0,0,0,0,0,a,0\n"
        "0,a,0,0,0,0,0,0,0,0,0,0,0,0,a,0\n"
        "0,a,a,a,a,a,a,a,a,a,a,a,a,a,a,0\n"
        "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"
        "NAME example room\n"
        "ITM 0 11,5\n"
        "ITM 1 9,8\n"
        "EXT 2,2 0 13,13 FX wave DLG 5\n"
        "EXT 2,2 1 13,13 FX slide_d DLG 7\n"
        "END 3 13,2\n"
        "END 6 2,2\n"
        "PAL 0\n"
        "TUNE 2\n"
    );

    // Parse the room
    std::string firstLine;
    std::getline(input, firstLine);
    game.parseRoom(input, firstLine);

    // Test room details
    Room &room = game.rooms[0];
    assert(room.id == 0);
    assert(room.name == "example room");

    // Test items
    assert(room.items.size() == 2);
    assert(room.items[0].first == 0);
    assert(room.items[0].second.first == 11);
    assert(room.items[0].second.second == 5);

    // Test exits
    assert(room.exits.size() == 2);
    assert(room.exits[0].startPosition.first == 2);
    assert(room.exits[0].startPosition.second == 2);
    assert(room.exits[0].destinationRoomId == 0);
    assert(room.exits[0].destinationPosition.first == 13);
    assert(room.exits[0].destinationPosition.second == 13);
    assert(room.exits[0].effect == "wave");
    assert(room.exits[0].dialogueId == 5);

    // Test endings
    assert(room.endings.size() == 2);
    assert(room.endings[0].dialogueId == 3); // Now we check the dialogue ID, not end ID
    assert(room.endings[0].position.first == 13);
    assert(room.endings[0].position.second == 2);

    assert(room.endings[1].dialogueId == 6);
    assert(room.endings[1].position.first == 2);
    assert(room.endings[1].position.second == 2);

    std::cout << "test_parse_room passed!" << std::endl;
}

void test_parse_tile_single_frame() {
    BitsyGame game;

    // Simulate the input for a tile with a single frame
    std::istringstream input(
        "TIL a\n"
        "11111111\n"
        "10000001\n"
        "10000001\n"
        "10011001\n"
        "10011001\n"
        "10000001\n"
        "10000001\n"
        "11111111\n"
        "NAME block\n"
        "WAL true\n"
    );

    std::string firstLine;
    std::getline(input, firstLine);

    // Call the parseTile method
    game.parseTile(input, firstLine);

    // Test the result
    assert(game.tiles.size() == 1);
    Tile &tile = game.tiles[0];

    // Test the tile ID
    assert(tile.id == 'a');

    // Test the first frame (8 lines)
    assert(tile.frames.size() == 8);  // 8 lines, 1 frame
    assert(tile.frames[0] == "11111111");
    assert(tile.frames[1] == "10000001");
    assert(tile.frames[7] == "11111111");

    // Test the tile name
    assert(tile.name == "block");

    // Test the wall property
    assert(tile.wall == true);

    std::cout << "test_parse_tile_single_frame passed!" << std::endl;
}

void test_parse_tile_two_frames() {
    BitsyGame game;

    // Simulate the input for a tile with two frames
    std::istringstream input(
        "TIL b\n"
        "11111111\n"
        "10000001\n"
        "10000001\n"
        "10011001\n"
        "10011001\n"
        "10000001\n"
        "10000001\n"
        "11111111\n"
        ">\n"
        "11111111\n"
        "10000001\n"
        "10000001\n"
        "10011001\n"
        "10011001\n"
        "10000001\n"
        "10000001\n"
        "11111111\n"
        "NAME stone\n"
    );

    std::string firstLine;
    std::getline(input, firstLine);

    // Call the parseTile method
    game.parseTile(input, firstLine);

    // Test the result
    assert(game.tiles.size() == 1);
    Tile &tile = game.tiles[0];

    // Test the tile ID
    assert(tile.id == 'b');

    // Test the first frame (8 lines)
    assert(tile.frames.size() == 16);  // 16 lines, 2 frames
    assert(tile.frames[0] == "11111111");
    assert(tile.frames[1] == "10000001");
    assert(tile.frames[7] == "11111111");

    // Test the second frame (8 lines starting after '>')
    assert(tile.frames[8] == "11111111");
    assert(tile.frames[9] == "10000001");
    assert(tile.frames[15] == "11111111");

    // Test the tile name
    assert(tile.name == "stone");

    // Since there is no "WAL", the tile should not be a wall
    assert(tile.wall == false);

    std::cout << "test_parse_tile_two_frames passed!" << std::endl;
}

int main() {
    test_parse_game_title();
    test_parse_settings();
    test_parse_palette();
    test_parse_room();
    test_parse_tile_single_frame();
    test_parse_tile_two_frames();
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
