/* Purpose: Manages all instances of players as a collective, and handles inputs sent to each one.
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include "Player.h"
#include "../pathfinding/PathRenderer.h"
#include <Jauntlet/Inputs/InputManager.h>
#include <Jauntlet/Rendering/Cameras/Camera2D.h>
#include <Jauntlet/Rendering/Textures/SpriteBatch.h>

class DrillManager;

class PlayerManager {
public: 
	PlayerManager(DrillManager* drill);
	// creates a new player and places it into the player manager
	void createPlayer(const glm::vec2& position, uint8_t playerID, bool isCop = false);
	// creates a new player and places it into the player manager
	void createPlayer(const glm::vec2& position, uint8_t playerID, int health, bool isCop = false);
	
	// processes a click to see if we operate on the players, returns true if we are, false if we arent.
	bool processInput(const Jauntlet::Camera2D& activeCamera);
	// tries to damage the players currently in the drill
	void damageTick(const int& drillHeat);
	
	// returns amount of players
	uint8_t size();

	// returns true if a player is selected
	bool isPlayerSelected();
	// returns a pointer to the selected player
	Player* getSelectedPlayer();
	// returns a vector to all players
	std::vector<Player*> getAllPlayers();
	// returns path renderer
	PathRenderer* getPathRenderer();

	// returns true if a player is at the exact specified position.
	bool posMatchesPlayerDest(const glm::vec2& worldPos);

	// updates the state of players
	void update(DrillManager& drill);
	// draw all players
	void draw(Jauntlet::SpriteBatch& spriteBatch);
private:
	std::vector<Player> _players;
	int _selectedPlayer = -1;
	float _damageTick = 0;

	DrillManager* _drill;

	PathRenderer _pathRenderer;
	glm::vec2 _storedMousePos = glm::vec2(0);
};