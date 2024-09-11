#include "BitsyGame.h"
#include <sstream>
#include <iostream>
#include <fstream> 

// Helper to read a number of lines from the file
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

// Helper to read two frames if available
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
    while (std::getline(file, line) && !line.empty()) {
        if (line.find("NAME") != std::string::npos) {
            room.name = line.substr(5);
        } else if (line.find("ITM") != std::string::npos) {
            int itemId, x, y;
            sscanf(line.c_str(), "ITM %d %d,%d", &itemId, &x, &y);
            room.items.emplace_back(itemId, std::make_pair(x, y));
        } else if (line.find("EXT") != std::string::npos) {
            Exit ext;
            char effect[64] = {0}; // Buffer to store the effect
            sscanf(line.c_str(), "EXT %d,%d %d %d,%d FX %s DLG %d",
                   &ext.startPosition.first, &ext.startPosition.second,
                   &ext.destinationRoomId,
                   &ext.destinationPosition.first, &ext.destinationPosition.second,
                   effect, &ext.dialogueId);
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

    rooms.push_back(room); // Add room to the rooms vector
}

void BitsyGame::parseTile(std::istream& file, const std::string& firstLine) {
    Tile tile;
    tile.id = firstLine[4];
    tile.frames = readFrames(file); // Use helper function to read frames

    std::string line;
    std::getline(file, line);
    if (line.find("NAME") != std::string::npos) {
        tile.name = line.substr(5); // Extract tile name
    }

    std::getline(file, line);
    tile.wall = line.find("WAL") != std::string::npos;

    tiles.push_back(tile);
}

void BitsyGame::parseAvatar(std::istream& file, const std::string& firstLine) {
    Avatar avatar;
    avatar.id = 'A'; // Avatar always has the ID 'A'
    avatar.frames = readFrames(file); // Use helper function to read frames

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

    this->avatar = avatar;
}

void BitsyGame::parseSprite(std::istream& file, const std::string& firstLine) {
    Sprite sprite;
    sprite.id = firstLine[4]; // Extract sprite ID
    sprite.frames = readFrames(file); // Use helper function to read frames

    std::string line;
    while (std::getline(file, line) && !line.empty()) {
        if (line.find("NAME") != std::string::npos) {
            sprite.name = line.substr(5);
        } else if (line.find("DLG") != std::string::npos) {
            sprite.dialogId = std::stoi(line.substr(4));
        } else if (line.find("BLIP") != std::string::npos) {
            sprite.blipId = std::stoi(line.substr(5));
        } else if (line.find("POS") != std::string::npos) {
            sscanf(line.c_str(), "POS %d %d,%d", &sprite.roomId, &sprite.position.first, &sprite.position.second);
        }
    }

    sprites.push_back(sprite); // Add the sprite to the sprites list
}

void BitsyGame::parseItem(std::istream& file, const std::string& firstLine) {
    Item item;
    item.id = std::stoi(firstLine.substr(4)); // Extract the item ID
    item.frames = readFrames(file); // Use helper function to read frames

    std::string line;
    while (std::getline(file, line) && !line.empty()) {
        if (line.find("NAME") != std::string::npos) {
            item.name = line.substr(5); // Extract item name
        } else if (line.find("DLG") != std::string::npos) {
            item.dialogId = std::stoi(line.substr(4)); // Extract dialogue ID
        } else if (line.find("BLIP") != std::string::npos) {
            item.blipId = std::stoi(line.substr(5)); // Extract blip sound ID
        }
    }

    items.push_back(item); // Add the item to the list of items
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

void BitsyGame::parseVariable(std::istream& file, const std::string& firstLine) {
    Variable var;
    var.name = firstLine.substr(4); // Extract the variable name (after "VAR ")

    // Get the variable value from the next line
    std::string line;
    std::getline(file, line);
    var.value = line;  // Value can be any string, not necessarily a number

    variables[var.name] = var;  // Add the variable to the map
}

void BitsyGame::parseTune(std::istream& file, const std::string& firstLine) {
    Tune tune;
    tune.id = std::stoi(firstLine.substr(5)); // Extract the tune ID
    std::string line;

    bool isTreble = true;
    while (std::getline(file, line) && !line.empty()) {
        if (line.find(">") != std::string::npos) {
            isTreble = true; // After ">", expect treble first in the next bar
        } else if (line.find("NAME") != std::string::npos) {
            tune.name = line.substr(5); // Extract tune name
        } else if (line.find("KEY") != std::string::npos) {
            tune.key = line.substr(4); // Extract key
        } else if (line.find("TMP") != std::string::npos) {
            tune.tempo = line.substr(4); // Extract tempo
        } else if (line.find("SQR") != std::string::npos) {
            std::stringstream ss(line.substr(4));
            ss >> tune.trebleInstrument >> tune.bassInstrument; // Parse instruments
        } else if (line.find("ARP") != std::string::npos) {
            tune.arpeggio = line.substr(4); // Extract arpeggio
        } else {
            // Alternate between treble and bass for each bar
            if (isTreble) {
                tune.treblePatterns.push_back(line);
            } else {
                tune.bassPatterns.push_back(line);
            }
            isTreble = !isTreble; // Switch to bass after treble, and vice versa
        }
    }

    tunes.push_back(tune); // Add the parsed tune to the tunes list
}

void BitsyGame::parseBlip(std::istream& file, const std::string& firstLine) {
    Blip blip;
    blip.id = std::stoi(firstLine.substr(5)); // Extract the blip ID
    std::string line;

    // Parse blip notes
    std::getline(file, line);
    blip.notes = line; // Store the notes

    // Parse additional fields
    while (std::getline(file, line) && !line.empty()) {
        if (line.find("NAME") != std::string::npos) {
            blip.name = line.substr(5); // Extract blip name
        } else if (line.find("ENV") != std::string::npos) {
            std::stringstream ss(line.substr(4));
            int envVal;
            while (ss >> envVal) {
                blip.env.push_back(envVal); // Store envelope values
            }
        } else if (line.find("BEAT") != std::string::npos) {
            std::stringstream ss(line.substr(5));
            int beatVal;
            while (ss >> beatVal) {
                blip.beat.push_back(beatVal); // Store beat values
            }
        } else if (line.find("SQR") != std::string::npos) {
            blip.squareWave = line.substr(4); // Store square wave setting
        } else if (line.find("RPT") != std::string::npos) {
            blip.repeat = std::stoi(line.substr(4)); // Store repeat value
        }
    }

    blips.push_back(blip); // Add the parsed blip to the blips list
}

void BitsyGame::parseGameData(const std::string& filePath) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filePath << std::endl;
        return; // Stop if the file can't be opened
    }

    std::string line;

    try {
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
    } catch (const std::exception& e) {
        std::cerr << "Error while parsing file: " << e.what() << std::endl;
        file.close(); // Ensure the file is closed if an error occurs
        return;
    }

    file.close(); // Close the file after processing
}