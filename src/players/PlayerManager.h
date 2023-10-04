#pragma once
#include "Player.h"

#include <Jauntlet/InputManager.h>
#include <Jauntlet/Camera2D.h>
#include <Jauntlet/Rendering/SpriteBatch.h>

class PlayerManager {
public: 
	PlayerManager(int initialPlayers);
	// adds an already defined player to the player manager
	void addPlayer(Player& player);
	// creates a new player and places it into the player manager
	void createPlayer(int x, int y);
	
	// processes a click to see if we operate on the players, returns true if we are, false if we arent.
	bool processInput(Jauntlet::InputManager* inputManager, Jauntlet::Camera2D* activeCamera, Jauntlet::TileMap* navTileMap);

	// updates the state of players
	void update();
	// draw all players
	void draw(Jauntlet::SpriteBatch& spriteBatch);
private:
	std::vector<Player> _players;
	int _selectedPlayer = -1;
};