/* Purpose: methods for saving/loading/deleting data
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

	// this cant *really* be stopped or accounted for. this 100% just overwrites data.
	void Load(DrillManager& drill, PlayerManager& playerManager);

	void Delete();

private:
	sqlite3* database;

	// === saving methods === 

	bool TrySavePlayer(const Player& player, int itemID);

	bool TrySaveDrill(const PlayerResources& playerResources);

	bool TrySaveItem(const Holdable& holdable, int itemID);

	// === loading methods ===

	bool TryLoadInResources(PlayerResources* playerResources);
	bool TryLoadInResources(PlayerResources* playerResources); // Trying is used for debugging
	
	// returns holdable pointer list
	std::vector<Holdable*> LoadInItems(DrillManager& drill);

	bool TryLoadInPlayers(PlayerManager& playerManager, DrillManager& drill);
	bool TryLoadInPlayers(PlayerManager& playerManager, DrillManager& drill); // Trying is used for debugging


	int _saveID = 1;
};