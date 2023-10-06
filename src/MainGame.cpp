#include <Jauntlet/Rendering/ResourceManager.h>
#include <iostream>

#include "Jauntlet/Rendering/TextureCache.h"
#include "Jauntlet/UI/UIElement.h"
#include "MainGame.h"

MainGame::MainGame() :
	_players(3),
	_window() {
}

void MainGame::run() {
	initSystems();

	gameLoop();
}

void MainGame::initSystems() {
	Jauntlet::init();

	_window.create("Jauntlet Game Engine", _screenWidth, _screenHeight, Jauntlet::WindowFlags::RESIZEABLE);
	_window.setBackgroundColor(Jauntlet::Color(76, 24, 32));
	_window.setWindowIcon("Textures/Icon.png");

	SDL_ShowCursor(1); // show the mouse cursor. can be set to 0 later for replacements.

	initShaders();

	Jauntlet::ResourceManager::setMissingTexture("Textures/Icon.png");

	_camera.init(_screenWidth, _screenHeight);
	_hudCamera.init(_screenWidth, _screenHeight);

	_cameraManager.init(&_camera, &_inputManager, &_players, &_drill.drillWalls);

	// initialize player spriteBatch
	_playerSpriteBatch.init();
	_HUDSpriteBatch.init();

	// initializes spritefont
	_spriteFont.init(&_hudCamera, "Fonts/HandelGo.ttf", 256);

	_uiCoordinator.init(glm::ivec2(_screenWidth, _screenHeight), &_hudCamera, &_spriteFont, &_inputManager, &_drill);
	
	_drill.init();
	_selectedTile.init(&_drill.drillFloor);
}

void MainGame::initShaders() {
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void MainGame::gameLoop() {
	Jauntlet::Time::setMaxFPS(60);

	while (_gameState == GameState::PLAY) {
		Jauntlet::Time::beginFrame();

		processInput();

		_players.update();
		_camera.update();
		_hudCamera.update();

		drawGame();

		_fps = Jauntlet::Time::endFrame();
	}
}

void MainGame::processInput() {
	_inputManager.processInput();

	if (_inputManager.quitGameCalled()) {
		_gameState = GameState::EXIT;
	}

	_cameraManager.processInput();

	if (_inputManager.isKeyPressed(SDLK_F11) || (_inputManager.isKeyDown(SDLK_LALT) || _inputManager.isKeyDown(SDLK_RALT)) && _inputManager.isKeyPressed(SDLK_RETURN)) {
		_window.toggleFullscreen();
	}

	if (_inputManager.windowResized()) {
		_window.resolveWindowSize();
		_screenWidth = _window.getWindowWidth();
		_screenHeight = _window.getWindowHeight();
		_camera.updateCameraSize(_screenWidth, _screenHeight);
		_hudCamera.updateCameraSize(_screenWidth, _screenHeight);
		_uiCoordinator.applyNewScreenSize(glm::ivec2(_screenWidth, _screenHeight));
	}

	//open nav
	if (_inputManager.isKeyPressed(SDLK_EQUALS)) {
		_uiCoordinator.navigation.toggleNav();
	}

	//mouse hover over navigation
	if (_uiCoordinator.navigation.isNavOpen()) {
		//real
	}
}

void MainGame::drawGame() {
	// Reset screen
	_window.clearScreen();
	
	_colorProgram.use();
	_camera.setActiveCamera();

	_drill.draw();

	// Draw the player using a spriteBatch
	_playerSpriteBatch.begin();
	
	_players.draw(_playerSpriteBatch);
	_playerSpriteBatch.endAndRender();
	
	_selectedTile.draw(&_camera, &_inputManager);

	_colorProgram.unuse();
	
	drawHUD();
	
	_window.swapBuffer();
}

void MainGame::drawHUD() {
	_hudCamera.setActiveCamera();

	_uiCoordinator.fpsText = std::to_string((int)_fps); // #TODO: DELTEME

	_uiCoordinator.draw();

	_HUDSpriteBatch.begin();

	_HUDSpriteBatch.endAndRender();
}