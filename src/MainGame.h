/* Purpose: Initializes all parts of the game and runs the main game loop.
 * Main Contributer(s): Xander Mooney / Jack Kennedy / Christopher Kowalewski
 */
#pragma once

#include <Jauntlet/Jauntlet.h>
#include <Jauntlet/Time.h>
#include <Jauntlet/UI/UIButtonElement.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UITextElement.h>
#include <Jauntlet/Window.h>
#include <SDL/SDL.h>

#include "CameraManager.h"
#include "drill/DrillManager.h"
#include "glm/fwd.hpp"
#include "Navigation.h"
#include "players/PlayerManager.h"
#include "PlayerResources.h"
#include "SelectedTileRenderer.h"
#include "UICoordinator.h"

enum class GameState {
	PLAY, EXIT
};

class MainGame {
public:
	MainGame();

	void run();

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();
	void drawHUD();

	Jauntlet::Window _window;

	Jauntlet::GLSLProgram _colorProgram;
	
	Jauntlet::Camera2D _camera, _hudCamera;
	CameraManager _cameraManager;
	
	Jauntlet::InputManager _inputManager;

	GameState _gameState = GameState::PLAY;

	int  _screenWidth = 1024, _screenHeight = 768;
	float _fps = 0;

	Jauntlet::SpriteBatch _playerSpriteBatch;

	Jauntlet::SpriteBatch _HUDSpriteBatch;
	Jauntlet::SpriteFont _spriteFont;
	
	PlayerManager _players;

	DrillManager _drill;
	UICoordinator _uiCoordinator;
	PlayerResources _resources;
	SelectedTileRenderer _selectedTile;

	// defines scale of movement for the camera. if set to 1, the camera will follow the mouse, if set to 0, the mouse has no control over the camera.
	const float _CAMERA_MOVEMENT_SCALE =  0.5f;
};