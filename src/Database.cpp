#include <Jauntlet/Errors.h>
#include <Jauntlet/Filesystems/FileManager.h>
#include <cmath>
#include <cstddef>
#include <string>
#include <vector>

#include "Database.h"
#include "Jauntlet/Time.h"
#include "drill/DrillManager.h"
#include "drill/PlayerResources.h"
#include "interactable/Holdable.h"
#include "piloting/Navigation.h"
#include "players/Player.h"
#include "players/PlayerManager.h"

Database::Database(int saveID) {
    _saveID = saveID;
    sqlite3_open("saves.db", &database);

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
        "copper INTEGER,"
        "nav TEXT,"
        "depth DOUBLE,"
        "time DOUBLE"
        ");",
        nullptr, nullptr, nullptr);

    sqlite3_exec(database,
        "CREATE TABLE IF NOT EXISTS Items ("
        "saveID INTEGER,"
        "itemID INTEGER,"
        "positionX DOUBLE,"
        "positionY DOUBLE,"
        "type INTEGER"
        ");",
        nullptr, nullptr, nullptr);

    sqlite3_close(database);
}

bool Database::TrySave(DrillManager& drill, PlayerManager& playerManager) {
    this->Delete();

    sqlite3_open("saves.db", &database);

    if (!TrySaveDrill(drill)) {
        Jauntlet::error("Failed to save drill!");
        Jauntlet::error(sqlite3_errmsg(database));
        return false;
    }

    std::vector<Holdable*> holdables = drill.getAllHoldables();
    std::vector<Player*> players = playerManager.getAllPlayers();

    // loop through all players not holding an item and save them
    for (size_t i = 0; i < players.size(); ++i) {
        if (players[i]->heldItem == nullptr) {
            if (!TrySavePlayer(*players[i], 0)) {
                Jauntlet::error("Failed to save non-item holding player #" + std::to_string(i));
                return false;
            }
            players.erase(players.begin() + i);
            --i;
        }
    }

    for (size_t i = 0; i < holdables.size(); ++i) {
        if (!TrySaveItem(*holdables[i], i + 1)) { // ID 0 is reserved for no item, so we add 1 to i
            Jauntlet::error("Failed to save holdable #" + std::to_string(i));
            return false;
        }
        for (size_t j = 0; j < players.size(); ++j) {
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

    sqlite3_close(database);

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

bool Database::TrySaveDrill(DrillManager& drill) {
    int saveID      = _saveID;
    float heat      = drill.resources->heat;
    float water     = drill.resources->water;
    int food        = drill.resources->food;
    int copper      = drill.resources->copper;
    std::string nav = drill.navigation.getMap();
    float depth     = drill.navigation.getDepth();
    float playtime  = drill.resources->playtime;

    std::string command = "INSERT INTO Drills (saveID, heat, water, food, copper, nav, depth, time) VALUES("
		+ std::to_string(saveID)   + ", "
        + std::to_string(heat)     + ", "
		+ std::to_string(water)    + ", "
        + std::to_string(food)     + ", "
		+ std::to_string(copper)   + ", "
        + "\"" + nav + "\""        + ", "
        + std::to_string(depth)    + ", "
        + std::to_string(playtime) + ");";

	int rc = sqlite3_exec(database, command.c_str(), nullptr, nullptr, nullptr);        

	return rc == SQLITE_OK;
}

bool Database::TrySaveItem(const Holdable& holdable, int itemSaveID) {
    int saveID      = _saveID;
    int itemID      = itemSaveID;
    float positionX = holdable.position.x;
    float positionY = holdable.position.y;
    char type       = (char)holdable.itemType;
    
    std::string command = "INSERT INTO Items (saveID, itemID, positionX, positionY, type) VALUES("
		+ std::to_string(saveID)    + ", "
        + std::to_string(itemID)    + ", "
        + std::to_string(positionX) + ", "
		+ std::to_string(positionY) + ", "
        + std::to_string(type)      + ");";

	int rc = sqlite3_exec(database, command.c_str(), nullptr, nullptr, nullptr);

	return rc == SQLITE_OK;
}

bool Database::TryLoadInPlayers(PlayerManager& playerManager, DrillManager& drill) {
    // prepared sqlite "statement" (idk)
    sqlite3_stmt *stmt;
    
    // test values, these should NEVER appear in game.
    float positionX;
    float positionY;
    int heldItemID;
    int health;
    int playerID;

    // our query
    // grab everything from Players
    const char* query = "SELECT * FROM Players";

    // try preparing the database
    int rc = sqlite3_prepare_v2(database, query, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        // database couldn't be prepared
        return false;
    }

    int row = 0;

    std::vector<Holdable*> items = Database::LoadInItems(drill);

    // process query
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        
        // skip over data if its not on the save we grab
        if (sqlite3_column_int(stmt, 0) != _saveID) {
            ++row;
            continue;
        }
        
        positionX  = sqlite3_column_double(stmt, 1);
        positionY  = sqlite3_column_double(stmt, 2);
        heldItemID = sqlite3_column_int(stmt, 3);
        health     = sqlite3_column_int(stmt, 4);
        playerID   = sqlite3_column_int(stmt, 5);

        // Originally Jack had us creating the player, adding them to a vector, initalizing all its data, and then pushing it to the player manager.
        // This however, led to the instance of the player being created and destroyed multiple time as the data is copied to new instances and locations in memory.
        // This led to some issues such as the player not occupying stations because the destructor for players unoccupies any stations they are occupying. -xm

        Player* player = playerManager.createPlayer(glm::vec2(positionX, positionY), playerID, health);

        if (player != nullptr) {
            if (heldItemID != 0) {
                player->heldItem = items[heldItemID - 1];
            }

            // see if the player destination matches the anchor point of a station, and if it does, occupy the station.
            if (drill.doesPosMatchStationDest(player->getPosition()) != nullptr) {
                player->forceOccupyStation(drill.doesPosMatchStationDest(player->getPosition()));
            }
        }

        ++row;
    }

    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        Jauntlet::error("error! query ended with code " + std::to_string(rc));
    }

    // finalize the statement (i still dont know)
    sqlite3_finalize(stmt);

    // we did it!!!
    return true;
}

bool Database::TryLoadInDrill(DrillManager& drill) {
    // prepared sqlite "statement" (idk)
    sqlite3_stmt *stmt;
    
    // our query
    // grab everything from Drills
    const char* query = "SELECT * FROM Drills";

    // try preparing the database
    int rc = sqlite3_prepare_v2(database, query, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        // database couldn't be prepared
        return false;
    }

    float heat;
    float water;
    int food;
    int copper;
    std::string nav;
    float depth;
    float playTime;

    int row = 0;

    // process query
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        // skip over data if its not on the save we grab
        if (sqlite3_column_int(stmt, 0) != _saveID) {
            ++row;
            continue;
        }
        
        heat   = sqlite3_column_double(stmt, 1);
        water  = sqlite3_column_double(stmt, 2);
        food   = sqlite3_column_int(stmt, 3);
        copper = sqlite3_column_int(stmt, 4);
        nav    = reinterpret_cast< char const* >(sqlite3_column_text(stmt, 5)); // unsigned to signed char
        depth = sqlite3_column_double(stmt, 7);
        playTime = sqlite3_column_double(stmt, 8);
        
        ++row;

        break;
    }

    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        Jauntlet::error("error! query ended with code " + std::to_string(rc));
    }

    // finalize the statement (i still dont know)
    sqlite3_finalize(stmt);

    // replace playerResources
    drill.resources->heat = heat;
    drill.resources->water = water;
    drill.resources->food = food;
    drill.resources->copper = copper;

    drill.navigation.setMap(nav);

    drill.navigation.setDepth(depth);
    drill.resources->playtime = playTime;

    // we did it!!!
    return true;
}

std::vector<Holdable*> Database::LoadInItems(DrillManager& drill) {
    // prepared sqlite "statement" (idk)
    sqlite3_stmt *stmt;

    // test values, these should NEVER appear in game.
    int itemID;
    float positionX;
    float positionY;
    char type;

    // our query
    // grab everything from Players
    const char* query = "SELECT * FROM Items";

    // try preparing the database
    int rc = sqlite3_prepare_v2(database, query, -1, &stmt, nullptr);

    int row = 0;

    std::vector<Holdable*> holdables;

    // process query
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        
        // skip over data if its not on the save we grab
        if (sqlite3_column_int(stmt, 0) != _saveID) {
            ++row;
            continue;
        }
        
        //itemID    = sqlite3_column_int(stmt, 1);
        positionX = sqlite3_column_double(stmt, 2);
        positionY = sqlite3_column_double(stmt, 3);
        type      = sqlite3_column_int(stmt, 4);

        // we have NO saftey if this bugs out during reading we are GONE
        holdables.push_back(drill.addHoldable(glm::vec2(positionX,positionY),(HoldableType)type));

        ++row;
    }

    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        Jauntlet::error("error! query ended with code " + std::to_string(rc));
    }

    // finalize the statement (i still dont know)
    sqlite3_finalize(stmt);

    // we did it!!!
    return holdables;
}

void Database::Load(DrillManager& drill, PlayerManager& playerManager) {
    
    sqlite3_open("saves.db", &database);

    bool result;
    
    result = TryLoadInDrill(drill);
    
    if (!result) {
        Jauntlet::error("tryloadinresources FAILED");
    }

    result = TryLoadInPlayers(playerManager, drill);

    if (!result) {
        Jauntlet::error("tryloadinplayers FAILED");
    }

    sqlite3_close(database);
}

void Database::Delete() {
    sqlite3_open("saves.db", &database);

	sqlite3_exec(database, ("DELETE FROM Players WHERE saveID = " + std::to_string(_saveID) + ";").c_str(), nullptr, nullptr, nullptr);
	sqlite3_exec(database, ("DELETE FROM Drills WHERE saveID = " + std::to_string(_saveID) + ";").c_str(), nullptr, nullptr, nullptr);
	sqlite3_exec(database, ("DELETE FROM Items WHERE saveID = " + std::to_string(_saveID) + ";").c_str(), nullptr, nullptr, nullptr);

    sqlite3_close(database);
}

void Database::Delete(int saveID) {
    sqlite3* database;
    
    sqlite3_open("saves.db", &database);

	sqlite3_exec(database, ("DELETE FROM Players WHERE saveID = " + std::to_string(saveID) + ";").c_str(), nullptr, nullptr, nullptr);
	sqlite3_exec(database, ("DELETE FROM Drills WHERE saveID = " + std::to_string(saveID) + ";").c_str(), nullptr, nullptr, nullptr);
	sqlite3_exec(database, ("DELETE FROM Items WHERE saveID = " + std::to_string(saveID) + ";").c_str(), nullptr, nullptr, nullptr);

    sqlite3_close(database);
}

bool Database::IsSlotFull(int saveID) {
    uint8_t contains = 0;
    sqlite3* database;
    sqlite3_stmt *stmt;
    const char* query;
    int rc;

    sqlite3_open("saves.db", &database);

    query = "SELECT * FROM Items";
    rc = sqlite3_prepare_v2(database, query, -1, &stmt, nullptr);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (sqlite3_column_int(stmt, 0) == saveID) {
            contains++;
            break;
        }
    }
    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        Jauntlet::error("error! query ended with code " + std::to_string(rc));
    }
    sqlite3_finalize(stmt);

    query = "SELECT * FROM Players";
    rc = sqlite3_prepare_v2(database, query, -1, &stmt, nullptr);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (sqlite3_column_int(stmt, 0) == saveID) {
            contains++;
            break;
        }
    }
    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        Jauntlet::error("error! query ended with code " + std::to_string(rc));
    }
    sqlite3_finalize(stmt);

    query = "SELECT * FROM Drills";
    rc = sqlite3_prepare_v2(database, query, -1, &stmt, nullptr);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (sqlite3_column_int(stmt, 0) == saveID) {
            contains++;
            break;
        }
    }
    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        Jauntlet::error("error! query ended with code " + std::to_string(rc));
    }
    sqlite3_finalize(stmt);
    
    sqlite3_close(database);

    return contains != 0;
}

// unused, but could have been for removing all data.
/*void Database::Purge() {
    sqlite3* database;
    
    sqlite3_open("saves.db", &database);
    
    sqlite3_exec(database, "DROP TABLE Players", nullptr, nullptr, nullptr);
    sqlite3_exec(database, "DROP TABLE Drills" , nullptr, nullptr, nullptr);
    sqlite3_exec(database, "DROP TABLE Items"  , nullptr, nullptr, nullptr);

    sqlite3_close(database);
}*/

float* Database::GetSaveData(int saveID) {
    static float bruh[2];

    bruh[0] = -1;
    bruh[1] = -1;

    sqlite3* database;
    sqlite3_stmt *stmt;

    sqlite3_open("saves.db", &database);

    const char* query = "SELECT * FROM Drills";
    int rc = sqlite3_prepare_v2(database, query, -1, &stmt, nullptr);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (sqlite3_column_int(stmt, 0) == saveID) {
            bruh[0] = sqlite3_column_double(stmt, 6);
            bruh[1] = sqlite3_column_double(stmt, 7);
            break;
        }
    }
    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        Jauntlet::error("error! query ended with code " + std::to_string(rc));
        Jauntlet::error("error message: " + std::to_string(rc) + " " + sqlite3_errmsg(database));
    }
    sqlite3_finalize(stmt);
    
    sqlite3_close(database);

    return bruh;
}