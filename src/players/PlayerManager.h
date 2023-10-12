/* Purpose: Manages all instances of players as a collective, and handles inputs sent to each one.
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include "Player.h"
#include "../pathfinding/PathRenderer.h"

#include <Jauntlet/InputManager.h>
#include <Jauntlet/Camera2D.h>
#include <Jauntlet/Rendering/SpriteBatch.h>

class PlayerManager {
public: 
	PlayerManager();
	// initialize player manager
	void init(int initialPlayers, Jauntlet::TileMap* tileWalls);
	// adds an already defined player to the player manager
	void addPlayer(Player& player);
	// creates a new player and places it into the player manager
	void createPlayer(int x, int y);
	
	// processes a click to see if we operate on the players, returns true if we are, false if we arent.
	bool processInput(Jauntlet::InputManager* inputManager, Jauntlet::Camera2D* activeCamera, Jauntlet::TileMap* navTileMap);

	// returns true if a player is selected
	bool isPlayerSelected();
	// Updates what station the selected player is assigned to
	void assignStation(PlayerStation* newStation);

	// updates the state of players
	void update();
	// draw all players
	void draw(Jauntlet::SpriteBatch& spriteBatch);
private:
	std::vector<Player> _players;
	int _selectedPlayer = -1;

	PathRenderer _pathRenderer;
	glm::vec2 _storedMousePos = glm::vec2(0);
};