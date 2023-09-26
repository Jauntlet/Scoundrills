#include <Jauntlet/Rendering/ResourceManager.h>
#include <iostream>

#include "MainGame.h"

MainGame::MainGame() {
	// Empty
}

void MainGame::run() {
	initSystems();

	gameLoop();
}

void MainGame::initSystems() {
	Jauntlet::init();

	_window.create("Jauntlet Game Engine", _screenWidth, _screenHeight, Jauntlet::WindowFlags::RESIZEABLE);

	initShaders();

	_cameraManager.init(glm::vec2(_screenWidth, _screenHeight), &_inputManager);

	_tileHandler.loadFile("Levels/csv.txt");
	_selectedTileBatch.init();
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

		_cameraManager.Update();

		drawGame();

		_fps = Jauntlet::Time::endFrame();
	}
}

void MainGame::processInput() {
	_inputManager.processInput();

	_selectedTilePos = _tileHandler.RoundWorldPos(_cameraManager.camera.convertScreenToWorld(_inputManager.getMouseCoords()));

	if (_inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
		_tileHandler.updateTile(_tileHandler.WorldPosToTilePos(_selectedTilePos), 0);
	}

	if (_inputManager.quitGameCalled()) {
		_gameState = GameState::EXIT;
	}

	if (_inputManager.isKeyPressed(SDLK_F11) || (_inputManager.isKeyDown(SDLK_LALT) || _inputManager.isKeyDown(SDLK_RALT)) && _inputManager.isKeyPressed(SDLK_RETURN)) {
		_window.toggleFullscreen();
	}

	if (_inputManager.windowResized()) {
		_window.getWindowSize(); // This not only gets the window size, but also recalculates it incase of window resizing going unchecked.
		_screenWidth = _window.getWindowWidth();
		_screenHeight = _window.getWindowHeight();
		_cameraManager.updateCameraSize(_screenWidth, _screenHeight);
	}
}

void MainGame::drawGame() {
	// Reset screen
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	_colorProgram.use();
	
	glActiveTexture(GL_TEXTURE0);

	// Reading information into shaders
	glUniform1i(_colorProgram.getUniformLocation("imageTexture"), 0);
	_cameraManager.ActivateCamera(&_colorProgram);

	_tileHandler.draw();

	_selectedTileBatch.begin();
	_selectedTileBatch.draw({ _selectedTilePos.x, _selectedTilePos.y, 64, 64 }, Jauntlet::ResourceManager::getTexture("Textures/WhiteSquare.png").id, 0);
	_selectedTileBatch.endAndRender();

	_colorProgram.unuse();
	
	_window.swapBuffer();
}