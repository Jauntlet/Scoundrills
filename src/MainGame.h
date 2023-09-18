#pragma once

#include <Jauntlet/Jauntlet.h>
#include <Jauntlet/Time.h>
#include <Jauntlet/Window.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UITextElement.h>
#include <SDL/SDL.h>

#include "Navigation.h"
#include "Player.h"
#include "glm/fwd.hpp"

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
	
	Jauntlet::Camera2D _camera;
	Jauntlet::Camera2D _hudCamera;
	
	Jauntlet::InputManager _inputManager;

	Jauntlet::TextureCache _textureCache;

	GameState _gameState = GameState::PLAY;

	int _screenWidth = 1024, _screenHeight = 768;
	float _fps = 0;

	glm::vec2 _oldMouse = glm::vec2(0);
	glm::vec2 _deltaMouse = glm::vec2(0);

	Jauntlet::SpriteBatch _playerSpriteBatch;

	Jauntlet::TileMap _level;
	Jauntlet::TileSet _bricks;

	Jauntlet::SpriteBatch _HUDSpriteBatch;
	Jauntlet::SpriteFont _spriteFont;

	Jauntlet::UIManager _uiManager;
	
	Player _player;
	glm::vec2 _selectedTilePos = glm::vec2(0);

	Navigation _navigation;
	std::vector<std::vector<int>> _navPoints;
};