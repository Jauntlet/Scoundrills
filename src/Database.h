/* Purpose: Manages inputs directed towards the camera and any other info the camera needs to function.
 * Main Contributer(s): Jack Kennedy / Xander Mooney
 */

#pragma once

#include <string>
#include <sqlite3.h>

#include "players/Player.h"
#include "drill/PlayerResources.h"
#include "interactable/Holdable.h"

class Database {
public:
	Database();
private:
	sqlite3* database;

	bool TrySavePlayer(const Player& player);

	bool TrySaveDrill(const PlayerResources& playerResources);

	bool TrySaveItem(const Holdable& holdable);

	int _saveID = 1;
};