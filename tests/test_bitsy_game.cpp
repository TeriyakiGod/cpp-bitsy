#include <BitsyGame.h>
#include <BitsyGameParser.h>
#include <iostream>
#include <sstream>
#include <cassert>

void test_parse_game_data() {
    BitsyGame game;

    // Simulate the file path for game.bitsy
    std::string filePath = "../game.bitsy";

    // Call the parseGameData method
    game = BitsyGameParser::parseGameData(filePath);

    // Test the game title
    assert(game.title == "game");

    // Test settings
    assert(game.settings.verMaj == 8);
    assert(game.settings.verMin == 12);
    assert(game.settings.roomFormat == 1);
    assert(game.settings.dlgCompat == 0);
    assert(game.settings.txtMode == 0);

    // Test palettes
    assert(game.palettes.size() == 1);
    assert(game.palettes[0].id == 0);
    assert(game.palettes[0].name == "blueprint");

    // Test room
    assert(game.rooms.size() == 1);
    assert(game.rooms[0].name == "example room");
    assert(game.rooms[0].tiles[0][0] == '0');
    assert(game.rooms[0].paletteId == 0);

    // Test tile
    assert(game.tiles.size() == 1);
    assert(game.tiles[0].id == 'a');
    assert(game.tiles[0].name == "block");

    // Test avatar
    assert(game.avatar.id == 'A');
    assert(game.avatar.position.first == 4);
    assert(game.avatar.position.second == 4);

    // Test sprite
    assert(game.sprites.size() == 1);
    assert(game.sprites[0].name == "cat");
    assert(game.sprites[0].dialogId == 0);

    // Test items
    assert(game.items.size() == 2);
    assert(game.items[0].name == "tea");
    assert(game.items[1].name == "key");

    // Test dialogues
    assert(game.dialogues.size() == 3);
    assert(game.dialogues[0].name == "cat dialog");
    assert(game.dialogues[1].name == "tea dialog");
    assert(game.dialogues[2].name == "key dialog");

    // Test variables
    assert(game.variables["a"].value == "42");

    // Test tunes
    assert(game.tunes.size() == 3);
    assert(game.tunes[0].name == "finale fanfare");
    assert(game.tunes[1].name == "tuneful town");

    // Test blips
    assert(game.blips.size() == 2);
    assert(game.blips[0].name == "meow");
    assert(game.blips[1].name == "pick up key");

    std::cout << "test_parse_game_data passed!" << std::endl;

    // Test the printGameStats method
    game.printGameStats();
}

int main() {
    test_parse_game_data();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
