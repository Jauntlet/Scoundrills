#pragma once
#include <Jauntlet/Jauntlet.h>
#include <Jauntlet/Time.h>
#include <Jauntlet/Window.h>
#include <Jauntlet/Rendering/GLSLProgram.h>
#include <Jauntlet/Rendering/ResourceManager.h>
#include <Jauntlet/Camera2D.h>
#include <Jauntlet/InputManager.h>

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

	Jauntlet::Window _window;

	Jauntlet::GLSLProgram _colorProgram;
	
	Jauntlet::Camera2D _camera;

	Jauntlet::InputManager _inputManager;

	GameState _gameState = GameState::PLAY;

	int _screenWidth = 1024, _screenHeight = 768;
	float _fps = 0;
};