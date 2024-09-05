// src/BitsyGame.cpp
#include "BitsyGame.h"

void BitsyGame::parseGameTitle(const std::string& line) {
    title = line; // The entire first line is the title
}

void BitsyGame::parseSettings(const std::string& line) {
    std::stringstream ss(line);
    std::string key;
    int value;
    ss >> key >> key >> value;

    if (key == "VER_MAJ") settings.verMaj = value;
    else if (key == "VER_MIN") settings.verMin = value;
    else if (key == "ROOM_FORMAT") settings.roomFormat = value;
    else if (key == "DLG_COMPAT") settings.dlgCompat = value;
    else if (key == "TXT_MODE") settings.txtMode = value;
}

void BitsyGame::parsePalette(std::istream& file, const std::string& firstLine) {
    Palette palette;
    palette.id = std::stoi(firstLine.substr(4)); // Extract the palette ID
    int r, g, b;

    for (int i = 0; i < 3; ++i) {
        std::string line;
        std::getline(file, line);
        sscanf(line.c_str(), "%d,%d,%d", &r, &g, &b);

        if (i == 0) palette.color1 = std::make_tuple(r, g, b);
        else if (i == 1) palette.color2 = std::make_tuple(r, g, b);
        else palette.color3 = std::make_tuple(r, g, b);
    }

    std::string nameLine;
    std::getline(file, nameLine);
    if (nameLine.find("NAME") != std::string::npos) {
        palette.name = nameLine.substr(5);
    }

    palettes.push_back(palette);
}

void BitsyGame::parseRoom(std::istream& file, const std::string& firstLine) {
    Room room;
    room.id = std::stoi(firstLine.substr(5)); // Extract the room ID

    // Parse 16x16 tiles grid
    for (int i = 0; i < 16; ++i) {
        std::string line;
        std::getline(file, line);
        std::vector<char> row;

        for (char c : line) {
            if (c != ',') row.push_back(c);
        }

        room.tiles.push_back(row);
    }

    std::string line;
    while (std::getline(file, line)) {
        // Handle "NAME" (room name)
        if (line.find("NAME") != std::string::npos) {
            room.name = line.substr(5); // Extract the room name after "NAME "
        }

        // Parse items
        else if (line.find("ITM") != std::string::npos) {
            int itemId, x, y;
            sscanf(line.c_str(), "ITM %d %d,%d", &itemId, &x, &y);
            room.items.emplace_back(itemId, std::make_pair(x, y));
        }

        // Parse exits
        else if (line.find("EXT") != std::string::npos) {
            Exit ext;
            char effect[64] = {0}; // Buffer to store the effect
            sscanf(line.c_str(), "EXT %d,%d %d %d,%d FX %s DLG %d",
                   &ext.startPosition.first, &ext.startPosition.second,
                   &ext.destinationRoomId,
                   &ext.destinationPosition.first, &ext.destinationPosition.second,
                   effect, &ext.dialogueId);
            ext.effect = effect;
            room.exits.push_back(ext);
        }

        // Parse endings
        else if (line.find("END") != std::string::npos) {
            End end;
            sscanf(line.c_str(), "END %d %d,%d", &end.dialogueId, &end.position.first, &end.position.second);
            room.endings.push_back(end);
        }

        // Parse palette
        else if (line.find("PAL") != std::string::npos) {
            room.paletteId = std::stoi(line.substr(4));
        }

        // Parse tune
        else if (line.find("TUNE") != std::string::npos) {
            room.tuneId = std::stoi(line.substr(5));
        }
    }

    rooms.push_back(room); // Add room to the rooms vector
}

void BitsyGame::parseTile(std::istream& file, const std::string& firstLine) {
    Tile tile;
    tile.id = firstLine[4];
    std::string line;

    for (int i = 0; i < 8; ++i) {
        std::getline(file, line);
        tile.frames.push_back(line);
    }

    std::getline(file, line);
    if (line.find(">") != std::string::npos) {
        for (int i = 0; i < 8; ++i) {
            std::getline(file, line);
            tile.frames.push_back(line);
        }
        std::getline(file, line);   
        if (line.find("NAME") != std::string::npos) {
        tile.name = line.substr(5);
        }
    }
    else if (line.find("NAME") != std::string::npos) {
        tile.name = line.substr(5);
    }

    std::getline(file, line);
    if (line.find("WAL") != std::string::npos) {
        tile.wall = true;
    }
    else {
        tile.wall = false;
    }

    tiles.push_back(tile);
}

void BitsyGame::parseAvatar(std::istream& file, const std::string& firstLine) {
    Avatar avatar;
    std::string line;

    for (int i = 0; i < 8; ++i) {
        std::getline(file, line);
        avatar.frames.push_back(line);
    }

    std::getline(file, line);
    if (line.find(">") != std::string::npos) {
        for (int i = 0; i < 8; ++i) {
            std::getline(file, line);
            avatar.frames.push_back(line);
        }
    }

    while (std::getline(file, line) && line.find("POS") == std::string::npos) {
        if (line.find("ITM") != std::string::npos) {
            int itemId, count;
            sscanf(line.c_str(), "ITM %d %d", &itemId, &count);
            avatar.inventory.push_back(itemId);
        }
    }

    sscanf(line.c_str(), "POS %d %d,%d", &avatar.position.first, &avatar.position.second.first, &avatar.position.second.second);
    this->avatar = avatar;
}

void BitsyGame::parseSprite(std::istream& file, const std::string& firstLine) {
    Sprite sprite;
    sprite.id = firstLine[4];
    std::string line;

    for (int i = 0; i < 8; ++i) {
        std::getline(file, line);
        sprite.frames.push_back(line);
    }

    std::getline(file, line);
    if (line.find(">") != std::string::npos) {
        for (int i = 0; i < 8; ++i) {
            std::getline(file, line);
            sprite.frames.push_back(line);
        }
    }

    while (std::getline(file, line) && line.find("POS") == std::string::npos) {
        if (line.find("DLG") != std::string::npos) {
            sscanf(line.c_str(), "DLG %d", &sprite.dialogId);
        } else if (line.find("BLIP") != std::string::npos) {
            sscanf(line.c_str(), "BLIP %d", &sprite.blipId);
        }
    }

    sscanf(line.c_str(), "POS %d %d,%d", &sprite.position.first, &sprite.position.second.first, &sprite.position.second.second);
    sprites.push_back(sprite);
}

void BitsyGame::parseItem(std::istream& file, const std::string& firstLine) {
    Item item;
    item.id = std::stoi(firstLine.substr(4));
    std::string line;

    for (int i = 0; i < 8; ++i) {
        std::getline(file, line);
        item.frames.push_back(line);
    }

    std::getline(file, line);
    if (line.find("NAME") != std::string::npos) {
        item.name = line.substr(5);
    }

    std::getline(file, line);
    if (line.find("DLG") != std::string::npos) {
        sscanf(line.c_str(), "DLG %d", &item.dialogId);
    }

    std::getline(file, line);
    if (line.find("BLIP") != std::string::npos) {
        sscanf(line.c_str(), "BLIP %d", &item.blipId);
    }

    items.push_back(item);
}

void BitsyGame::parseDialogue(std::istream& file, const std::string& firstLine) {
    Dialogue dlg;
    dlg.id = std::stoi(firstLine.substr(4));
    std::getline(file, dlg.text);
    std::string line;
    std::getline(file, line);
    if (line.find("NAME") != std::string::npos) {
        dlg.name = line.substr(5);
    }
    dialogues.push_back(dlg);
}

void BitsyGame::parseTune(std::istream& file, const std::string& firstLine) {
    Tune tune;
    tune.id = std::stoi(firstLine.substr(5));
    std::string line;

    while (std::getline(file, line) && line.find(">") == std::string::npos) {
        tune.patterns.push_back(line);
    }

    while (std::getline(file, line) && line.find("NAME") == std::string::npos) {
        // Continue parsing until NAME is found
    }

    if (line.find("NAME") != std::string::npos) {
        tune.name = line.substr(5);
    }
    tunes.push_back(tune);
}

void BitsyGame::parseBlip(std::istream& file, const std::string& firstLine) {
    Blip blip;
    blip.id = std::stoi(firstLine.substr(5));
    std::getline(file, blip.notes);
    std::string line;

    while (std::getline(file, line) && line.find("NAME") == std::string::npos) {
        // Parse until NAME is found
    }

    if (line.find("NAME") != std::string::npos) {
        blip.name = line.substr(5);
    }
    blips.push_back(blip);
}

void BitsyGame::parseVariable(std::istream& file, const std::string& firstLine) {
    Variable var;
    var.name = firstLine.substr(4);
    std::string line;
    std::getline(file, line);
    var.value = std::stoi(line);
    variables[var.name] = var;
}

void BitsyGame::parseGameData(const std::string& filePath) {
    std::ifstream file(filePath);
    std::string line;

    // Parse the first line as the game title
    if (std::getline(file, line)) {
        parseGameTitle(line);
    }

    // Continue parsing the rest of the file
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        ss >> token;

        if (token == "!") {
            parseSettings(line);
        } else if (token == "PAL") {
            parsePalette(file, line);
        } else if (token == "ROOM") {
            parseRoom(file, line);
        } else if (token == "TIL") {
            parseTile(file, line);
        } else if (token == "SPR" && line.find("A") != std::string::npos) {
            parseAvatar(file, line); // Parse avatar separately
        } else if (token == "SPR") {
            parseSprite(file, line); // Parse other sprites
        } else if (token == "ITM") {
            parseItem(file, line);
        } else if (token == "DLG") {
            parseDialogue(file, line);
        } else if (token == "TUNE") {
            parseTune(file, line);
        } else if (token == "BLIP") {
            parseBlip(file, line);
        } else if (token == "VAR") {
            parseVariable(file, line);
        }
    }
}
