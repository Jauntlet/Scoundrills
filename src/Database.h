/* Purpose: Manages inputs directed towards the camera and any other info the camera needs to function.
 * Main Contributer(s): Jack Kennedy / Xander Mooney
 */

#pragma once

#include <string>
#include <sqlite3.h>

#include "players/PlayerManager.h"
#include "drill/PlayerResources.h"
#include "drill/DrillManager.h"
class Database {
public:
	Database();

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

	// returns resources
	bool TryLoadInResources(int saveID, PlayerResources& playerResources);

	bool TryLoadInPlayers(PlayerResources& playerResources);

	int _saveID = 1;
};