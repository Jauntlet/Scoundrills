/* Purpose: methods for saving/loading data
 * Main Contributer(s): Jack Kennedy / Xander Mooney
 */

#pragma once

#include <sqlite3.h>
#include <string>

#include "drill/DrillManager.h"
#include "drill/PlayerResources.h"
#include "players/PlayerManager.h"

class Database {
public:
	Database(int saveID);

	// try to save the game
	bool TrySave(DrillManager& drill, PlayerManager& playerManager);

	// test the database
	void Test();
private:
	sqlite3* database;

	// === saving methods === 

	bool TrySavePlayer(const Player& player, int itemID);

	bool TrySaveDrill(const PlayerResources& playerResources);

	bool TrySaveItem(const Holdable& holdable, int itemID);

	// === loading methods ===

	bool Load(DrillManager& drill, PlayerManager& playerManager);

	// returns resources
	bool TryLoadInResources(PlayerResources& playerResources);
	
	std::vector<Holdable*> LoadInItems(DrillManager& drill);

	bool TryLoadInPlayers(PlayerManager& playerManager);


	int _saveID = 1;
};