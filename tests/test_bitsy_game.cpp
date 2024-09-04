// tests/test_bitsy_game.cpp
#include "BitsyGame.h"
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

// Additional tests for other parsing methods...

int main() {
    test_parse_game_title();
    test_parse_settings();
    // Call additional test functions here...

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
