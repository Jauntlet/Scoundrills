#include <Jauntlet/Errors.h>
#include <Jauntlet/Filesystems/FileManager.h>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include "Database.h"
#include "src/drill/PlayerResources.h"
#include "src/players/Player.h"
#include "src/players/PlayerManager.h"

// values that would never show up realistically that we can use as placeholders
#define TEST_INT -69
#define TEST_FLOAT -69.420
#define TEST_STRING "sixty nine four hundred twenty"

Database::Database(int saveID) {
    _saveID = saveID;
    sqlite3_open("saves.db", &database);

    std::cout << "creating database with saveID " << _saveID << std::endl;

    sqlite3_exec(database, "DROP TABLE Players", nullptr, nullptr, nullptr);
    sqlite3_exec(database, "DROP TABLE Drills" , nullptr, nullptr, nullptr);
    sqlite3_exec(database, "DROP TABLE Items"  , nullptr, nullptr, nullptr);

    sqlite3_exec(database,
        "CREATE TABLE IF NOT EXISTS Players ("
        "saveID INTEGER,"
        "positionX DOUBLE,"
        "positionY DOUBLE,"
        "heldItemId INTEGER,"
        "health INTEGER,"
        "playerID INTEGER"
        ");",
        nullptr, nullptr, nullptr);

    sqlite3_exec(database,
        "CREATE TABLE IF NOT EXISTS Drills ("
        "saveID INTEGER,"
        "heat DOUBLE,"
        "water DOUBLE,"
        "food INTEGER,"
        "copper INTEGER"
        ");",
        nullptr, nullptr, nullptr);

    sqlite3_exec(database,
        "CREATE TABLE IF NOT EXISTS Items ("
        "saveID INTEGER,"
        "itemID INTEGER,"
        "positionX DOUBLE,"
        "positionY DOUBLE,"
        "type TEXT"
        ");",
        nullptr, nullptr, nullptr);

    sqlite3_close(database);

    std::cout << "database created!" << std::endl;
}

void Database::Test() {
    sqlite3_open("saves.db", &database);

    PlayerResources _playerResources;

    _playerResources.heat = 1.0f;
    _playerResources.water =  2.0f;
    _playerResources.food = 3;
    _playerResources.copper = 4;

    if (TrySaveDrill(_playerResources)) {
        std::cout << "drill saving confirmed" << std::endl;
    } else {
        std::cout << "TrySaveDrill FAILED" << std::endl;
    }

    PlayerResources playerResources;

    TryLoadInResources(_saveID, playerResources);

    sqlite3_close(database);

    std::cout << playerResources.heat << ", " << playerResources.water << ", " << playerResources.food << ", " << playerResources.copper << std::endl;
}

bool Database::TrySave(DrillManager& drill, PlayerManager& playerManager) {
    if (!TrySaveDrill(*drill.resources)) {
        Jauntlet::error("Failed to save drill!");
        return false;
    }

    std::vector<Holdable*> holdables = drill.getAllHoldables();
    std::vector<Player*> players = playerManager.getAllPlayers();

    // loop through all players not holding an item and save them
    for (int i = 0; i < players.size(); ++i) {
        if (players[i]->heldItem == nullptr) {
            if (!TrySavePlayer(*players[i], 0)) {
                Jauntlet::error("Failed to save non-item holding player #" + std::to_string(i));
                return false;
            }
            players.erase(players.begin() + i);
            --i;
        }
    }

    for (int i = 0; i < holdables.size(); ++i) {
        if (!TrySaveItem(*holdables[i], i + 1)) { // ID 0 is reserved for no item, so we add 1 to i
            Jauntlet::error("Failed to save holdable #" + std::to_string(i));
            return false;
        }
        for (int j = 0; j < players.size(); ++j) {
            // save players holding items with correct itemID
            if (players[j]->heldItem == holdables[i]) {
                if (!TrySavePlayer(*players[j], i + 1)) {
                    Jauntlet::error("Failed to save player holding item ID #" + std::to_string(i + 1));
                    return false;
                }
                players.erase(players.begin() + j);
                break;
            }
        }
    }

    return true;
}

bool Database::TrySavePlayer(const Player& player, int itemID) {
    int saveID      = _saveID;
    float positionX = player.getPosition().x;
    float positionY = player.getPosition().y;
    int heldItemID  = itemID;
    int health      = player.getHealth();
    int playerID    = player.getPlayerID();
    
	std::string command = "INSERT INTO Players (saveID, positionX, positionY, heldItemId, health, playerID) VALUES("
		+ std::to_string(saveID)     + ", "
        + std::to_string(positionX)  + ", "
		+ std::to_string(positionY)  + ", "
        + std::to_string(heldItemID) + ", "
		+ std::to_string(health)     + ", "
        + std::to_string(playerID)   + ");";

	int rc = sqlite3_exec(database, command.c_str(), nullptr, nullptr, nullptr);

	return rc == SQLITE_OK;
}

bool Database::TrySaveDrill(const PlayerResources& playerResources) {
    int saveID  = _saveID;
    float heat  = playerResources.heat;
    float water = playerResources.water;
    int food    = playerResources.food;
    int copper  = playerResources.copper;
    
    std::string command = "INSERT INTO Drills (saveID, heat, water, food, copper) VALUES("
		+ std::to_string(saveID) + ", "
        + std::to_string(heat)   + ", "
		+ std::to_string(water)  + ", "
        + std::to_string(food)   + ", "
		+ std::to_string(copper) + ");";

	int rc = sqlite3_exec(database, command.c_str(), nullptr, nullptr, nullptr);

	return rc == SQLITE_OK;
}

bool Database::TrySaveItem(const Holdable& holdable, int itemSaveID) {
    int saveID       = _saveID;
    int itemID       = itemSaveID;
    float positionX  = holdable.position.x;
    float positionY  = holdable.position.y;
    std::string type = "";
    switch (holdable.itemType) {
        case (HoldableType::ICE):
            type = "ICE";
            break;
        case (HoldableType::PIPE):
            type = "PIPE";
            break;
        case (HoldableType::SCRAP):
            type = "SCRAP";
            break;
        case (HoldableType::WATER):
            type = "WATER";
            break;
        default:
            Jauntlet::error("INVALID ITEM TYPE ATTEMPTED SAVED");
            return false;
    }
    
    std::string command = "INSERT INTO Items (saveID, itemID, positionX, positionY, type) VALUES("
		+ std::to_string(saveID)    + ", "
        + std::to_string(itemID)    + ", "
        + std::to_string(positionX) + ", "
		+ std::to_string(positionY) + ", "
        +                type       + ");";

	int rc = sqlite3_exec(database, command.c_str(), nullptr, nullptr, nullptr);

	return rc == SQLITE_OK;
}

bool Database::TryLoadInPlayers(PlayerManager& playerManager) {
    // prepared sqlite "statement" (idk)
    sqlite3_stmt *stmt;
    
    // test values, these should NEVER appear in game.
    float positionX = TEST_FLOAT;
    float positionY = TEST_FLOAT;
    int heldItemID  = TEST_INT;
    int health      = TEST_INT;
    int playerID    = TEST_INT;

    // our query
    // grab everything from Players
    const char *query = "SELECT * FROM Players";

    // try preparing the database
    int rc = sqlite3_prepare_v2(database, query, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        // database couldn't be prepared
        return false;
    }

    int row = 0;

    std::vector<Player> players;

    // process query
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        
        if (sqlite3_column_int(stmt, 0) != _saveID) {
            ++row;
            continue;
        }
        
        positionX  = sqlite3_column_double(stmt, 1);
        positionY  = sqlite3_column_double(stmt, 2);
        heldItemID = sqlite3_column_int(stmt, 3);
        health     = sqlite3_column_int(stmt, 4);
        playerID   = sqlite3_column_int(stmt, 5);

        Player player(glm::vec2(positionX,positionY), playerID, health, false);

        if (heldItemID != 0) {
            // handle held items TODO:FIXME
        }

        players.push_back(player);

        ++row;
    }

    if (rc == SQLITE_DONE || rc == SQLITE_ROW) {
        std::cout << "we are done reading DB, searched " << row << " rows" << std::endl;
    } else {
        std::cout << "some sort of error, i guess. please check the DB manually: " << rc << std::endl;
    }

    // finalize the statement (i still dont know)
    sqlite3_finalize(stmt);

    // replace players

    // TODO:FIXME
    

    // we did it!!!
    return true;
}

bool Database::TryLoadInResources(int saveID, PlayerResources& playerResources) {
    // prepared sqlite "statement" (idk)
    sqlite3_stmt *stmt;
    
    // our query
    // grab everything from Drills
    const char *query = "SELECT * FROM Drills";

    // try preparing the database
    int rc = sqlite3_prepare_v2(database, query, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        // database couldn't be prepared
        return false;
    }

    float heat  = TEST_FLOAT; //playerResources.heat;
    float water = TEST_FLOAT; //playerResources.water;
    int food    = TEST_INT; //playerResources.food;
    int copper  = TEST_INT; //playerResources.copper;

    int row = 0;

    // process query
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        
        if (sqlite3_column_int(stmt, 0) != _saveID) {
            ++row;
            continue;
        }
        
        heat   = sqlite3_column_double(stmt, 1);
        water  = sqlite3_column_double(stmt, 2);
        food   = sqlite3_column_int(stmt, 3);
        copper = sqlite3_column_int(stmt, 4);
        
        ++row;

        break;
    }

    if (rc == SQLITE_DONE || rc == SQLITE_ROW) {
        std::cout << "we are done reading DB, searched " << row << " rows" << std::endl;
    } else {
        std::cout << "some sort of error, i guess. please check the DB manually: " << rc << std::endl;
    }

    // finalize the statement (i still dont know)
    sqlite3_finalize(stmt);

    // replace playerResources
    playerResources.heat = heat;
    playerResources.water = water;
    playerResources.food = food;
    playerResources.copper = copper;

    // we did it!!!
    return true;
}