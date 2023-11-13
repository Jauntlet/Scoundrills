/* Purpose: Initializes all parts of the game and runs the main game loop.
 * Main Contributer(s): Xander Mooney / Jack Kennedy / Christopher Kowalewski
 */
#pragma once
#include "CameraManager.h"
#include "drill/DrillManager.h"
#include <Jauntlet/Jauntlet.h>
#include <Jauntlet/Rendering/Particles/Particle.h>
#include <Jauntlet/Rendering/Window.h>
#include <Jauntlet/Time.h>
#include <Jauntlet/UI/UIButtonElement.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UITextElement.h>
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

	int  _screenWidth = 1024, _screenHeight = 768;
	Jauntlet::Window _window;

	Jauntlet::Camera2D _camera, _hudCamera;

	PlayerResources _resources;

	Jauntlet::InputManager _inputManager;

	UICoordinator _uiCoordinator;

	DrillManager _drill;

	CameraManager _cameraManager;

	PlayerManager _players;

	SelectedTileRenderer _selectedTile;
	
	Jauntlet::TextRenderer _textRenderer;

	Jauntlet::GLSLProgram _colorProgram;

	GameState _gameState = GameState::PLAY;

	float _fps = 0;

	Jauntlet::SpriteBatch _playerSpriteBatch;

	bool paused = false;

	// defines scale of movement for the camera. if set to 1, the camera will follow the mouse, if set to 0, the mouse has no control over the camera.
	const float _CAMERA_MOVEMENT_SCALE = 0.5f;

	glm::vec2 jim = glm::vec2(0);
	Jauntlet::Particle jimp;
};