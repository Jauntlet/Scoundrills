#pragma once

#include <Jauntlet/Jauntlet.h>
#include <Jauntlet/Time.h>
#include <Jauntlet/Window.h>

#include "glm/fwd.hpp"
#include <Jauntlet/Rendering/GLSLProgram.h>
#include <Jauntlet/Camera2D.h>
#include <Jauntlet/InputManager.h>
#include <Jauntlet/TileMap.h>

#include "TileHandler.h"
#include "CameraManager.h"

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

	int _screenWidth = 1024, _screenHeight = 768;
	Jauntlet::Window _window;

	Jauntlet::GLSLProgram _colorProgram;
	
	CameraManager _cameraManager;
	Jauntlet::Camera2D _hudCamera;

	Jauntlet::InputManager _inputManager;

	GameState _gameState = GameState::PLAY;

	float _fps = 0;

	TileHandler _tileHandler;
	Jauntlet::SpriteBatch _spriteBatch;
	glm::vec2 _selectedTilePos = glm::vec2(0);
};