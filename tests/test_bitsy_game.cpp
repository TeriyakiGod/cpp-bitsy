#include <BitsyGameData.h>
#include <BitsyGameParser.h>
#include <iostream>
#include <sstream>
#include <cassert>

void test_parse_game_data() {
    BitsyGameData gameData;

    // Simulate the file path for gameData.bitsy
    std::string filePath = "../game.bitsy";

    // Call the parseGameData method
    gameData = BitsyGameParser::parseGameData(filePath);

    // Test the gameData title
    assert(gameData.title == "game");

    // Test settings
    assert(gameData.settings.verMaj == 8);
    assert(gameData.settings.verMin == 12);
    assert(gameData.settings.roomFormat == 1);
    assert(gameData.settings.dlgCompat == 0);
    assert(gameData.settings.txtMode == 0);

    // Test palettes
    assert(gameData.palettes.size() == 1);
    assert(gameData.palettes[0].id == 0);
    assert(gameData.palettes[0].name == "blueprint");

    // Test room
    assert(gameData.rooms.size() == 1);
    assert(gameData.rooms[0].name == "example room");
    assert(gameData.rooms[0].tiles[0][0] == '0');
    assert(gameData.rooms[0].paletteId == 0);

    // Test tile
    assert(gameData.tiles.size() == 1);
    assert(gameData.tiles[0].id == 'a');
    assert(gameData.tiles[0].name == "block");

    // Test avatar
    assert(gameData.avatar.id == 'A');
    assert(gameData.avatar.position.first == 4);
    assert(gameData.avatar.position.second == 4);

    // Test sprite
    assert(gameData.sprites.size() == 1);
    assert(gameData.sprites[0].name == "cat");
    assert(gameData.sprites[0].dialogId == 0);

    // Test items
    assert(gameData.items.size() == 2);
    assert(gameData.items[0].name == "tea");
    assert(gameData.items[1].name == "key");

    // Test dialogues
    assert(gameData.dialogues.size() == 3);
    assert(gameData.dialogues[0].name == "cat dialog");
    assert(gameData.dialogues[1].name == "tea dialog");
    assert(gameData.dialogues[2].name == "key dialog");

    // Test variables
    assert(gameData.variables["a"].value == "42");

    // Test tunes
    assert(gameData.tunes.size() == 3);
    assert(gameData.tunes[0].name == "finale fanfare");
    assert(gameData.tunes[1].name == "tuneful town");

    // Test blips
    assert(gameData.blips.size() == 2);
    assert(gameData.blips[0].name == "meow");
    assert(gameData.blips[1].name == "pick up key");

    std::cout << "test_parse_game_data passed!" << std::endl;

    // Test the printGameStats method
    gameData.printGameStats();
}

int main() {
    test_parse_game_data();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
