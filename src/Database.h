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
private:
	sqlite3* database;

	bool TrySave(DrillManager& drill, PlayerManager& playerManager);

	bool TrySavePlayer(const Player& player);

	bool TrySaveDrill(const PlayerResources& playerResources);

	bool TrySaveItem(const Holdable& holdable);

	int _saveID = 1;
};