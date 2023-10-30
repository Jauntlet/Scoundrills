/* Purpose: Manages all instances of players as a collective, and handles inputs sent to each one.
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include "Player.h"
#include "../pathfinding/PathRenderer.h"
#include "../drill/DrillManager.h"
#include <Jauntlet/InputManager.h>
#include <Jauntlet/Camera2D.h>
#include <Jauntlet/Rendering/SpriteBatch.h>

class PlayerManager {
public: 
	PlayerManager(int initialPlayers, DrillManager* drill);
	// adds an already defined player to the player manager
	void addPlayer(Player& player);
	// creates a new player and places it into the player manager
	void createPlayer(int x, int y);
	
	// processes a click to see if we operate on the players, returns true if we are, false if we arent.
	bool processInput(Jauntlet::InputManager* inputManager, Jauntlet::Camera2D* activeCamera);
	// returns if a position is a valid destination to pathfind to.
	bool isValidDestination(glm::vec2 worldPos);

	// returns true if a player is selected
	bool isPlayerSelected();

	// updates the state of players
	void update();
	// draw all players
	void draw(Jauntlet::SpriteBatch& spriteBatch);
private:
	std::vector<Player> _players;
	int _selectedPlayer = -1;

	PathRenderer _pathRenderer;
	DrillManager* _drill;
	glm::vec2 _storedMousePos = glm::vec2(0);
};