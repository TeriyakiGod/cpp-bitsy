#include "BitsyGameParser.h"
#include <fstream>
#include <sstream>

// Helper functions for reading multiple lines and frames
std::vector<std::string> readLines(std::istream& file, int count) {
    std::vector<std::string> lines;
    std::string line;
    for (int i = 0; i < count; ++i) {
        if (std::getline(file, line)) {
            lines.push_back(line);
        }
    }
    return lines;
}

std::vector<std::string> readFrames(std::istream& file) {
    std::vector<std::string> frames = readLines(file, 8);
    std::string line;
    if (std::getline(file, line) && line.find(">") != std::string::npos) {
        std::vector<std::string> secondFrame = readLines(file, 8);
        frames.insert(frames.end(), secondFrame.begin(), secondFrame.end());
    } else {
        file.seekg(-line.size() - 1, std::ios_base::cur);
    }
    return frames;
}

BitsyGame BitsyGameParser::parseGameData(const std::string& filePath) {
    BitsyGame game;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filePath << std::endl;
        return game;
    }

    std::string line;
    try {
        if (std::getline(file, line)) {
            parseGameTitle(game, line);
        }

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string token;
            ss >> token;

            if (token == "!") {
                parseSettings(game, line);
            } else if (token == "PAL") {
                parsePalette(game, file, line);
            } else if (token == "ROOM") {
                parseRoom(game, file, line);
            } else if (token == "TIL") {
                parseTile(game, file, line);
            } else if (token == "SPR" && line.find("A") != std::string::npos) {
                parseAvatar(game, file, line);
            } else if (token == "SPR") {
                parseSprite(game, file, line);
            } else if (token == "ITM") {
                parseItem(game, file, line);
            } else if (token == "DLG") {
                parseDialogue(game, file, line);
            } else if (token == "TUNE") {
                parseTune(game, file, line);
            } else if (token == "BLIP") {
                parseBlip(game, file, line);
            } else if (token == "VAR") {
                parseVariable(game, file, line);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error while parsing file: " << e.what() << std::endl;
    }

    file.close();
    return game;
}

void BitsyGameParser::parseGameTitle(BitsyGame& game, const std::string& line) {
    game.title = line;
}

void BitsyGameParser::parseSettings(BitsyGame& game, const std::string& line) {
    std::stringstream ss(line);
    std::string key;
    int value;
    ss >> key >> key >> value;

    if (key == "VER_MAJ") game.settings.verMaj = value;
    else if (key == "VER_MIN") game.settings.verMin = value;
    else if (key == "ROOM_FORMAT") game.settings.roomFormat = value;
    else if (key == "DLG_COMPAT") game.settings.dlgCompat = value;
    else if (key == "TXT_MODE") game.settings.txtMode = value;
}

void BitsyGameParser::parsePalette(BitsyGame& game, std::istream& file, const std::string& firstLine) {
    Palette palette;
    palette.id = std::stoi(firstLine.substr(4));  // Extract palette ID

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

    game.palettes.push_back(palette);
}

void BitsyGameParser::parseRoom(BitsyGame& game, std::istream& file, const std::string& firstLine) {
    Room room;
    room.id = std::stoi(firstLine.substr(5));  // Extract room ID

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
    while (std::getline(file, line) && !line.empty()) {
        if (line.find("NAME") != std::string::npos) {
            room.name = line.substr(5);  // Extract room name
        } else if (line.find("ITM") != std::string::npos) {
            int itemId, x, y;
            sscanf(line.c_str(), "ITM %d %d,%d", &itemId, &x, &y);
            room.items.emplace_back(itemId, std::make_pair(x, y));
        } else if (line.find("EXT") != std::string::npos) {
            Exit ext;
            char effect[64] = {0};
            sscanf(line.c_str(), "EXT %d,%d %d %d,%d FX %s DLG %d",
                   &ext.startPosition.first, &ext.startPosition.second,
                   &ext.destinationRoomId, &ext.destinationPosition.first,
                   &ext.destinationPosition.second, effect, &ext.dialogueId);
            ext.effect = effect;
            room.exits.push_back(ext);
        } else if (line.find("END") != std::string::npos) {
            End end;
            sscanf(line.c_str(), "END %d %d,%d", &end.dialogueId, &end.position.first, &end.position.second);
            room.endings.push_back(end);
        } else if (line.find("PAL") != std::string::npos) {
            room.paletteId = std::stoi(line.substr(4));
        } else if (line.find("TUNE") != std::string::npos) {
            room.tuneId = std::stoi(line.substr(5));
        }
    }

    game.rooms.push_back(room);
}

void BitsyGameParser::parseTile(BitsyGame& game, std::istream& file, const std::string& firstLine) {
    Tile tile;
    tile.id = firstLine[4];
    tile.frames = readFrames(file);  // Read frames

    std::string line;
    std::getline(file, line);
    if (line.find("NAME") != std::string::npos) {
        tile.name = line.substr(5);  // Extract tile name
    }

    std::getline(file, line);
    tile.wall = (line.find("WAL") != std::string::npos);

    game.tiles.push_back(tile);
}

void BitsyGameParser::parseAvatar(BitsyGame& game, std::istream& file, const std::string& firstLine) {
    Avatar avatar;
    avatar.id = 'A';  // Avatar always has the ID 'A'
    avatar.frames = readFrames(file);  // Read frames

    std::string line;
    std::getline(file, line);
    sscanf(line.c_str(), "POS %d %d,%d", &avatar.roomId, &avatar.position.first, &avatar.position.second);

    while (std::getline(file, line) && !line.empty()) {
        if (line.find("ITM") != std::string::npos) {
            int itemId;
            sscanf(line.c_str(), "ITM %d", &itemId);
            avatar.inventory.push_back(itemId);
        }
    }

    game.avatar = avatar;
}

void BitsyGameParser::parseSprite(BitsyGame& game, std::istream& file, const std::string& firstLine) {
    Sprite sprite;
    sprite.id = firstLine[4];
    sprite.frames = readFrames(file);  // Read frames

    std::string line;
    while (std::getline(file, line) && !line.empty()) {
        if (line.find("NAME") != std::string::npos) {
            sprite.name = line.substr(5);  // Extract sprite name
        } else if (line.find("DLG") != std::string::npos) {
            sprite.dialogId = std::stoi(line.substr(4));  // Extract dialogue ID
        } else if (line.find("BLIP") != std::string::npos) {
            sprite.blipId = std::stoi(line.substr(5));  // Extract blip sound ID
        } else if (line.find("POS") != std::string::npos) {
            sscanf(line.c_str(), "POS %d %d,%d", &sprite.roomId, &sprite.position.first, &sprite.position.second);
        }
    }

    game.sprites.push_back(sprite);
}

void BitsyGameParser::parseItem(BitsyGame& game, std::istream& file, const std::string& firstLine) {
    Item item;
    item.id = std::stoi(firstLine.substr(4));
    item.frames = readFrames(file);  // Read frames

    std::string line;
    while (std::getline(file, line) && !line.empty()) {
        if (line.find("NAME") != std::string::npos) {
            item.name = line.substr(5);  // Extract item name
        } else if (line.find("DLG") != std::string::npos) {
            item.dialogId = std::stoi(line.substr(4));  // Extract dialogue ID
        } else if (line.find("BLIP") != std::string::npos) {
            item.blipId = std::stoi(line.substr(5));  // Extract blip sound ID
        }
    }

    game.items.push_back(item);
}

void BitsyGameParser::parseDialogue(BitsyGame& game, std::istream& file, const std::string& firstLine) {
    Dialogue dlg;
    dlg.id = std::stoi(firstLine.substr(4));
    std::getline(file, dlg.text);

    std::string line;
    std::getline(file, line);
    if (line.find("NAME") != std::string::npos) {
        dlg.name = line.substr(5);  // Extract dialogue name
    }

    game.dialogues.push_back(dlg);
}

void BitsyGameParser::parseVariable(BitsyGame& game, std::istream& file, const std::string& firstLine) {
    Variable var;
    var.name = firstLine.substr(4);  // Extract variable name

    std::string line;
    std::getline(file, line);
    var.value = line;  // Assign the value to the variable

    game.variables[var.name] = var;
}

void BitsyGameParser::parseTune(BitsyGame& game, std::istream& file, const std::string& firstLine) {
    Tune tune;
    tune.id = std::stoi(firstLine.substr(5));  // Extract tune ID

    std::string line;
    bool isTreble = true;
    while (std::getline(file, line) && !line.empty()) {
        if (line.find(">") != std::string::npos) {
            isTreble = true;
        } else if (line.find("NAME") != std::string::npos) {
            tune.name = line.substr(5);
        } else if (line.find("KEY") != std::string::npos) {
            tune.key = line.substr(4);
        } else if (line.find("TMP") != std::string::npos) {
            tune.tempo = line.substr(4);
        } else if (line.find("SQR") != std::string::npos) {
            std::stringstream ss(line.substr(4));
            ss >> tune.trebleInstrument >> tune.bassInstrument;
        } else if (line.find("ARP") != std::string::npos) {
            tune.arpeggio = line.substr(4);
        } else {
            if (isTreble) {
                tune.treblePatterns.push_back(line);
            } else {
                tune.bassPatterns.push_back(line);
            }
            isTreble = !isTreble;
        }
    }

    game.tunes.push_back(tune);
}

void BitsyGameParser::parseBlip(BitsyGame& game, std::istream& file, const std::string& firstLine) {
    Blip blip;
    blip.id = std::stoi(firstLine.substr(5));  // Extract blip ID

    std::getline(file, blip.notes);  // Read blip notes

    std::string line;
    while (std::getline(file, line) && !line.empty()) {
        if (line.find("NAME") != std::string::npos) {
            blip.name = line.substr(5);
        } else if (line.find("ENV") != std::string::npos) {
            std::stringstream ss(line.substr(4));
            int envVal;
            while (ss >> envVal) {
                blip.env.push_back(envVal);  // Add envelope values
            }
        } else if (line.find("BEAT") != std::string::npos) {
            std::stringstream ss(line.substr(5));
            int beatVal;
            while (ss >> beatVal) {
                blip.beat.push_back(beatVal);  // Add beat values
            }
        } else if (line.find("SQR") != std::string::npos) {
            blip.squareWave = line.substr(4);
        } else if (line.find("RPT") != std::string::npos) {
            blip.repeat = std::stoi(line.substr(4));
        }
    }

    game.blips.push_back(blip);
}
