#include <cstddef>
#include <iostream>

#include "Database.h"
#include "src/drill/PlayerResources.h"
#include "src/players/Player.h"
#include <Jauntlet/Errors.h>
#include <Jauntlet/Filesystems/FileManager.h>

Database::Database() {
    _saveID = 1;
    while (Jauntlet::FileManager::findFile(std::to_string(_saveID) + ".db")) {
        ++_saveID;
    }

	sqlite3_open((std::to_string(_saveID) + ".db").c_str(), &database);

    sqlite3_exec(database, "DROP TABLE Players", nullptr, nullptr, nullptr);
    sqlite3_exec(database, "DROP TABLE Drills" , nullptr, nullptr, nullptr);
    sqlite3_exec(database, "DROP TABLE Items"  , nullptr, nullptr, nullptr);

    sqlite3_exec(database,
        "CREATE TABLE IF NOT EXISTS Players ("
        "saveID INTEGER,"
        "positionX REAL,"
        "positionY REAL,"
        "heldItemId INTEGER,"
        "health INTEGER,"
        "textureId INTEGER"
        ");",
        nullptr, nullptr, nullptr);

    sqlite3_exec(database,
        "CREATE TABLE IF NOT EXISTS Drills ("
        "saveID INTEGER,"
        "heat REAL,"
        "water REAL,"
        "food INTEGER,"
        "copper INTEGER"
        ");",
        nullptr, nullptr, nullptr);

    sqlite3_exec(database,
        "CREATE TABLE IF NOT EXISTS Items ("
        "saveID INTEGER,"
        "positionX REAL,"
        "positionY REAL,"
        "type TEXT"
        ");",
        nullptr, nullptr, nullptr);

	sqlite3_close(database);
}

Database::Database(int saveID) {
    _saveID = saveID;
    sqlite3_open((std::to_string(_saveID) + ".db").c_str(), &database);

    sqlite3_exec(database, "DROP TABLE Players", nullptr, nullptr, nullptr);
    sqlite3_exec(database, "DROP TABLE Drills", nullptr, nullptr, nullptr);
    sqlite3_exec(database, "DROP TABLE Items", nullptr, nullptr, nullptr);

    sqlite3_exec(database,
        "CREATE TABLE IF NOT EXISTS Players ("
        "saveID INTEGER,"
        "positionX REAL,"
        "positionY REAL,"
        "heldItemId INTEGER,"
        "health INTEGER,"
        "textureId INTEGER"
        ");",
        nullptr, nullptr, nullptr);

    sqlite3_exec(database,
        "CREATE TABLE IF NOT EXISTS Drills ("
        "saveID INTEGER,"
        "heat REAL,"
        "water REAL,"
        "food INTEGER,"
        "copper INTEGER"
        ");",
        nullptr, nullptr, nullptr);

    sqlite3_exec(database,
        "CREATE TABLE IF NOT EXISTS Items ("
        "saveID INTEGER,"
        "positionX REAL,"
        "positionY REAL,"
        "type TEXT"
        ");",
        nullptr, nullptr, nullptr);

    sqlite3_close(database);
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
    int health      = player.health;
    int texture     = player.getPlayerID();
    
	std::string command = "INSERT INTO Players (saveID, positionX, positionY, heldItemId, health, textureId) VALUES("
		+ std::to_string(saveID)     + ", "
        + std::to_string(positionX)  + ", "
		+ std::to_string(positionY)  + ", "
        + std::to_string(heldItemID) + ", "
		+ std::to_string(health)     + ", "
        + std::to_string(texture)    + ");";

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
    int itemID       = itemSaveID; // not currently implemented into SQL tables; FIXME!!!!
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
    
    std::string command = "INSERT INTO Items (saveID, positionX, positionY, type) VALUES("
		+ std::to_string(saveID)    + ", "
        + std::to_string(positionX) + ", "
		+ std::to_string(positionY) + ", "
        +                type       + ");";

	int rc = sqlite3_exec(database, command.c_str(), nullptr, nullptr, nullptr);

	return rc == SQLITE_OK;
}