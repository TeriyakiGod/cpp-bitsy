#include "../include/BitsyGame.h"
#include <iostream>
#include <sstream>
#include <cassert>

void test_parse_game_title() {
    BitsyGame game;
    std::string titleLine = "My Awesome Bitsy Game";
    game.parseGameTitle(titleLine);

    assert(game.title == "My Awesome Bitsy Game");
    std::cout << "test_parse_game_title passed!" << std::endl;
}

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

void test_parse_avatar_single_frame() {
    BitsyGame game;

    // Simulate the input for an avatar with a single frame
    std::istringstream input(
        "SPR A\n"
        "00011000\n"
        "00011000\n"
        "00011000\n"
        "00111100\n"
        "01111110\n"
        "10111101\n"
        "00100100\n"
        "00100100\n"
        "POS 2 4,4\n"
        "ITM 0 1\n"
        "ITM 1 1\n"
    );

    std::string firstLine;
    std::getline(input, firstLine);

    // Call the parseAvatar method
    game.parseAvatar(input, firstLine);

    // Test the result
    Avatar &avatar = game.avatar;

    // Test the avatar ID
    assert(avatar.id == 'A');

    // Test the frame (8 lines)
    assert(avatar.frames.size() == 8);  // 1 frame of 8 lines
    assert(avatar.frames[0] == "00011000");
    assert(avatar.frames[1] == "00011000");
    assert(avatar.frames[7] == "00100100");

    // Test the position (room and coordinates)
    assert(avatar.roomId == 2);         // Room ID 2
    assert(avatar.position.first == 4); // X coordinate
    assert(avatar.position.second == 4); // Y coordinate

    // Test the inventory
    assert(avatar.inventory.size() == 2);    // 2 items
    assert(avatar.inventory[0] == 0);        // Item 0
    assert(avatar.inventory[1] == 1);        // Item 1

    std::cout << "test_parse_avatar_single_frame passed!" << std::endl;
}

void test_parse_avatar_two_frames() {
    BitsyGame game;

    // Simulate the input for an avatar with two frames and no items
    std::istringstream input(
        "SPR A\n"
        "00011000\n"
        "00011000\n"
        "00011000\n"
        "00111100\n"
        "01111110\n"
        "10111101\n"
        "00100100\n"
        "00100100\n"
        ">\n"
        "11111111\n"
        "10000001\n"
        "10000001\n"
        "10011001\n"
        "10011001\n"
        "10000001\n"
        "10000001\n"
        "11111111\n"
        "POS 3 5,5\n"
    );

    std::string firstLine;
    std::getline(input, firstLine);

    // Call the parseAvatar method
    game.parseAvatar(input, firstLine);

    // Test the result
    Avatar &avatar = game.avatar;

    // Test the avatar ID
    assert(avatar.id == 'A');

    // Test the frames (16 lines: 8 per frame)
    assert(avatar.frames.size() == 16);  // 2 frames, each of 8 lines
    assert(avatar.frames[0] == "00011000");
    assert(avatar.frames[7] == "00100100");  // End of first frame
    assert(avatar.frames[8] == "11111111");  // Start of second frame
    assert(avatar.frames[15] == "11111111"); // End of second frame

    // Test the position (room and coordinates)
    assert(avatar.roomId == 3);         // Room ID 3
    assert(avatar.position.first == 5); // X coordinate
    assert(avatar.position.second == 5); // Y coordinate

    // Test the inventory (empty)
    assert(avatar.inventory.empty());

    std::cout << "test_parse_avatar_two_frames passed!" << std::endl;
}

void test_parse_sprite_single_frame() {
    BitsyGame game;

    // Simulate the input for a sprite with a single frame and a name
    std::istringstream input(
        "SPR a\n"
        "00011000\n"
        "00011000\n"
        "00011000\n"
        "00111100\n"
        "01111110\n"
        "10111101\n"
        "00100100\n"
        "00100100\n"
        "NAME cat\n"
        "POS 1 3,3\n"
    );

    std::string firstLine;
    std::getline(input, firstLine);

    // Call the parseSprite method
    game.parseSprite(input, firstLine);

    // Test the result
    Sprite &sprite = game.sprites[0];

    // Test the sprite ID
    assert(sprite.id == 'a');

    // Test the frame (8 lines)
    assert(sprite.frames.size() == 8);  // 1 frame of 8 lines
    assert(sprite.frames[0] == "00011000");
    assert(sprite.frames[7] == "00100100");

    // Test the sprite name
    assert(sprite.name == "cat");

    // Test the position (room and coordinates)
    assert(sprite.roomId == 1);         // Room ID 1
    assert(sprite.position.first == 3); // X coordinate
    assert(sprite.position.second == 3); // Y coordinate

    std::cout << "test_parse_sprite_single_frame passed!" << std::endl;
}

void test_parse_sprite_with_dialog_and_blip() {
    BitsyGame game;

    // Simulate the input for a sprite with two frames, a name, dialog, and blip sound
    std::istringstream input(
        "SPR a\n"
        "00000000\n"
        "00000000\n"
        "01010001\n"
        "01110001\n"
        "01110010\n"
        "01111100\n"
        "00111100\n"
        "00100100\n"
        ">\n"
        "00000000\n"
        "00000000\n"
        "01010001\n"
        "01110001\n"
        "01110010\n"
        "01111100\n"
        "00111100\n"
        "00100100\n"
        "NAME cat\n"
        "DLG 0\n"
        "POS 1 6,10\n"
        "BLIP 1\n"
    );

    std::string firstLine;
    std::getline(input, firstLine);

    // Call the parseSprite method
    game.parseSprite(input, firstLine);

    // Test the result
    Sprite &sprite = game.sprites[0];

    // Test the sprite ID
    assert(sprite.id == 'a');

    // Test the frames (16 lines: 8 per frame)
    assert(sprite.frames.size() == 16);  // 2 frames, each of 8 lines
    assert(sprite.frames[0] == "00000000");
    assert(sprite.frames[7] == "00100100");  // End of first frame
    assert(sprite.frames[8] == "00000000");  // Start of second frame
    assert(sprite.frames[15] == "00100100"); // End of second frame

    // Test the sprite name
    assert(sprite.name == "cat");

    // Test the dialog ID
    assert(sprite.dialogId == 0);

    // Test the blip sound ID
    assert(sprite.blipId == 1);

    // Test the position (room and coordinates)
    assert(sprite.roomId == 1);         // Room ID 1
    assert(sprite.position.first == 6); // X coordinate
    assert(sprite.position.second == 10); // Y coordinate

    std::cout << "test_parse_sprite_with_dialog_and_blip passed!" << std::endl;
}

void test_parse_dialogue() {
    BitsyGame game;

    // Simulate the input for three dialogues
    std::istringstream input(
        "DLG 0\n"
        "I'm a cat{pg}{wvy}{shk}{/shk}{/wvy}i like talking{rbw}{/rbw}{drws \"A\"}\n"
        "NAME cat dialog\n"
        "DLG 1\n"
        "You found a nice warm cup of tea\n"
        "NAME tea dialog\n"
        "DLG 2\n"
        "A key! {wvy}What does it open?{wvy}\n"
        "NAME key dialog\n"
    );

    // Parse the first dialogue
    std::string firstLine;
    std::getline(input, firstLine);
    game.parseDialogue(input, firstLine);

    // Parse the second dialogue
    std::getline(input, firstLine);
    game.parseDialogue(input, firstLine);

    // Parse the third dialogue
    std::getline(input, firstLine);
    game.parseDialogue(input, firstLine);

    // Test the result for the first dialogue
    Dialogue &dlg0 = game.dialogues[0];
    assert(dlg0.id == 0);
    assert(dlg0.text == "I'm a cat{pg}{wvy}{shk}{/shk}{/wvy}i like talking{rbw}{/rbw}{drws \"A\"}");
    assert(dlg0.name == "cat dialog");

    // Test the result for the second dialogue
    Dialogue &dlg1 = game.dialogues[1];
    assert(dlg1.id == 1);
    assert(dlg1.text == "You found a nice warm cup of tea");
    assert(dlg1.name == "tea dialog");

    // Test the result for the third dialogue
    Dialogue &dlg2 = game.dialogues[2];
    assert(dlg2.id == 2);
    assert(dlg2.text == "A key! {wvy}What does it open?{wvy}");
    assert(dlg2.name == "key dialog");

    std::cout << "test_parse_dialogue passed!" << std::endl;
}

void test_parse_item_with_dialog_and_blip() {
    BitsyGame game;

    // Simulate the input for an item with one frame, a name, dialogue, and blip sound
    std::istringstream input(
        "ITM 1\n"
        "00000000\n"
        "00111100\n"
        "00100100\n"
        "00111100\n"
        "00010000\n"
        "00011000\n"
        "00010000\n"
        "00011000\n"
        "NAME key\n"
        "DLG 2\n"
        "BLIP 2\n"
    );

    std::string firstLine;
    std::getline(input, firstLine);

    // Call the parseItem method
    game.parseItem(input, firstLine);

    // Test the result
    Item &item = game.items[0];

    // Test the item ID
    assert(item.id == 1);

    // Test the frame (8 lines)
    assert(item.frames.size() == 8);  // 1 frame of 8 lines
    assert(item.frames[0] == "00000000");
    assert(item.frames[7] == "00011000");

    // Test the item name
    assert(item.name == "key");

    // Test the dialogue ID
    assert(item.dialogId == 2);

    // Test the blip sound ID
    assert(item.blipId == 2);

    std::cout << "test_parse_item_with_dialog_and_blip passed!" << std::endl;
}

void test_parse_item_with_two_frames() {
    BitsyGame game;

    // Simulate the input for an item with two frames
    std::istringstream input(
        "ITM 2\n"
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
        "NAME coin\n"
    );

    std::string firstLine;
    std::getline(input, firstLine);

    // Call the parseItem method
    game.parseItem(input, firstLine);

    // Test the result
    Item &item = game.items[0];

    // Test the item ID
    assert(item.id == 2);

    // Test the frames (16 lines: 8 per frame)
    assert(item.frames.size() == 16);  // 2 frames, each of 8 lines
    assert(item.frames[0] == "11111111");
    assert(item.frames[7] == "11111111");  // End of first frame
    assert(item.frames[8] == "11111111");  // Start of second frame
    assert(item.frames[15] == "11111111"); // End of second frame

    // Test the item name
    assert(item.name == "coin");

    // Test no dialogue ID
    assert(item.dialogId == -1);

    // Test no blip sound ID
    assert(item.blipId == -1);

    std::cout << "test_parse_item_with_two_frames passed!" << std::endl;
}

void test_parse_variable() {
    BitsyGame game;

    // Simulate the input for multiple variables
    std::istringstream input(
        "VAR a\n"
        "42\n"
        "VAR d\n"
        "lala\n"
        "VAR c\n"
        "12kaka\n"
        "VAR 12bruh\n"
        "lol\n"
    );

    // Parse each variable
    std::string firstLine;

    // Parse variable 'a'
    std::getline(input, firstLine);
    game.parseVariable(input, firstLine);

    // Parse variable 'd'
    std::getline(input, firstLine);
    game.parseVariable(input, firstLine);

    // Parse variable 'c'
    std::getline(input, firstLine);
    game.parseVariable(input, firstLine);

    // Parse variable '12bruh'
    std::getline(input, firstLine);
    game.parseVariable(input, firstLine);

    // Test the result for variable 'a'
    assert(game.variables["a"].name == "a");
    assert(game.variables["a"].value == "42");

    // Test the result for variable 'd'
    assert(game.variables["d"].name == "d");
    assert(game.variables["d"].value == "lala");

    // Test the result for variable 'c'
    assert(game.variables["c"].name == "c");
    assert(game.variables["c"].value == "12kaka");

    // Test the result for variable '12bruh'
    assert(game.variables["12bruh"].name == "12bruh");
    assert(game.variables["12bruh"].value == "lol");

    std::cout << "test_parse_variable passed!" << std::endl;
}

void test_parse_tune_with_treble_and_bass() {
    BitsyGame game;

    // Simulate the input for a tune with alternating treble and bass patterns
    std::istringstream input(
        "TUNE 1\n"
        "3d,0,0,0,3d5,0,0,0,3l,0,0,0,3s,0,0,0\n"  // Treble bar
        "16l2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"  // Bass bar
        ">\n"
        "3l,0,0,0,s,0,4m,0,0,0,4r,0,0,0,0,0\n"  // Treble bar
        "16s2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"  // Bass bar
        ">\n"
        "3d,0,0,0,3d5,0,0,0,3l,0,0,0,3s,0,0,0\n"  // Treble bar
        "16d2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"  // Bass bar
        ">\n"
        "4l,0,0,0,s,0,3l,0,0,0,2s,0,2m,0,2r,0\n"  // Treble bar
        "16m2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"  // Bass bar
        "NAME finale fanfare\n"
        "KEY C,D,E,F,G,A,B d,r,m,f,s,l,t\n"
        "TMP SLW\n"
        "SQR P4 P2\n"
        "ARP UP\n"
    );

    std::string firstLine;
    std::getline(input, firstLine);

    // Call the parseTune method
    game.parseTune(input, firstLine);

    // Test the result
    Tune &tune = game.tunes[0];

    // Test the tune ID
    assert(tune.id == 1);

    // Test the treble patterns
    assert(tune.treblePatterns.size() == 4);
    assert(tune.treblePatterns[0] == "3d,0,0,0,3d5,0,0,0,3l,0,0,0,3s,0,0,0");
    assert(tune.treblePatterns[3] == "4l,0,0,0,s,0,3l,0,0,0,2s,0,2m,0,2r,0");

    // Test the bass patterns
    assert(tune.bassPatterns.size() == 4);
    assert(tune.bassPatterns[0] == "16l2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0");
    assert(tune.bassPatterns[3] == "16m2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0");

    // Test the tune name
    assert(tune.name == "finale fanfare");

    // Test the key
    assert(tune.key == "C,D,E,F,G,A,B d,r,m,f,s,l,t");

    // Test the tempo
    assert(tune.tempo == "SLW");

    // Test the instruments
    assert(tune.trebleInstrument == "P4");  // Treble instrument
    assert(tune.bassInstrument == "P2");    // Bass instrument

    // Test the arpeggio
    assert(tune.arpeggio == "UP");

    std::cout << "test_parse_tune_with_treble_and_bass passed!" << std::endl;
}

void test_parse_blip() {
    BitsyGame game;

    // Simulate the input for a blip
    std::istringstream input(
        "BLIP 1\n"
        "A3,E2,G\n"
        "NAME meow\n"
        "ENV 421 254 12 125 333\n"
        "BEAT 231 354\n"
        "SQR P2\n"
        "RPT 1\n"
    );

    std::string firstLine;
    std::getline(input, firstLine);

    // Call the parseBlip method
    game.parseBlip(input, firstLine);

    // Test the result
    Blip &blip = game.blips[0];

    // Test the blip ID
    assert(blip.id == 1);

    // Test the blip notes
    assert(blip.notes == "A3,E2,G");

    // Test the blip name
    assert(blip.name == "meow");

    // Test the envelope
    assert(blip.env.size() == 5);
    assert(blip.env[0] == 421);
    assert(blip.env[4] == 333);

    // Test the beat
    assert(blip.beat.size() == 2);
    assert(blip.beat[0] == 231);
    assert(blip.beat[1] == 354);

    // Test the square wave
    assert(blip.squareWave == "P2");

    // Test the repeat value
    assert(blip.repeat == 1);

    std::cout << "test_parse_blip passed!" << std::endl;
}

void test_parse_game_data() {
    BitsyGame game;

    // Simulate the file path for game.bitsy
    std::string filePath = "../game.bitsy";

    // Call the parseGameData method
    game.parseGameData(filePath);

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
}

int main() {
    test_parse_game_title();
    test_parse_settings();
    test_parse_palette();
    test_parse_room();
    test_parse_tile_single_frame();
    test_parse_tile_two_frames();
    test_parse_avatar_single_frame();
    test_parse_avatar_two_frames();
    test_parse_sprite_single_frame();
    test_parse_sprite_with_dialog_and_blip();
    test_parse_dialogue();
    test_parse_item_with_dialog_and_blip();
    test_parse_item_with_two_frames();
    test_parse_variable();
    test_parse_tune_with_treble_and_bass();
    test_parse_blip();
    test_parse_game_data();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
