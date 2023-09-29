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
	_window.setBackgroundColor(Jauntlet::Color(76, 24, 32));

	initShaders();

	_cameraManager.init(glm::vec2(_screenWidth, _screenHeight), &_inputManager);
	_hudCamera.init(_screenWidth, _screenHeight);

	_tileHandler.loadFile("Levels/newLevel.jml");
	_spriteBatch.init();
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

		if (!_inputManager.isKeyDown(SDLK_LCTRL)) {
			_cameraManager.Update();
		}
		_hudCamera.update();

		drawGame();

		_fps = Jauntlet::Time::endFrame();
	}
}

void MainGame::processInput() {
	_inputManager.processInput();

	_selectedTilePos = _tileHandler.RoundWorldPos(_cameraManager.camera.convertScreenToWorld(_inputManager.getMouseCoords()));

	if (_inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
		_tileHandler.updateTile(_tileHandler.WorldPosToTilePos(_selectedTilePos));
	}
	if (_inputManager.isKeyDown(SDL_BUTTON_RIGHT)) {
		_tileHandler.setSelectedTile(0);
		_tileHandler.updateTile(_tileHandler.WorldPosToTilePos(_selectedTilePos));
		_tileHandler.changeSelectedTile(_inputManager.deltaScroll);
	}
	if (_inputManager.deltaScroll != 0) {
		_tileHandler.changeSelectedTile(_inputManager.deltaScroll);
	}

	if (_inputManager.isKeyDown(SDLK_s) && _inputManager.isKeyDown(SDLK_LCTRL)) {
		_tileHandler.saveFile("Levels/newLevel.jml");
	}

	if (_inputManager.quitGameCalled()) {
		_gameState = GameState::EXIT;
	}

	if (_inputManager.isKeyPressed(SDLK_F11) || (_inputManager.isKeyDown(SDLK_LALT) || _inputManager.isKeyDown(SDLK_RALT)) && _inputManager.isKeyPressed(SDLK_RETURN)) {
		_window.toggleFullscreen();
	}

	if (_inputManager.windowResized()) {
		_window.resolveWindowSize();
		_screenWidth = _window.getWindowWidth();
		_screenHeight = _window.getWindowHeight();
		_cameraManager.updateCameraSize(_screenWidth, _screenHeight);
		_hudCamera.updateCameraSize(_screenWidth, _screenHeight);
	}
}

void MainGame::drawGame() {
	// Reset screen
	_window.clearScreen();
	
	_colorProgram.use();

	//glActiveTexture(GL_TEXTURE0);

	// Reading information into shaders
	glUniform1i(_colorProgram.getUniformLocation("imageTexture"), 0);
	_cameraManager.ActivateCamera(&_colorProgram);

	_tileHandler.draw();

	_spriteBatch.begin();
	_spriteBatch.draw({ _selectedTilePos.x, _selectedTilePos.y, 64, 64 }, Jauntlet::ResourceManager::getTexture("Textures/WhiteSquare.png").id, 0);
	
	_spriteBatch.endAndRender();
	
	_hudCamera.setActiveCamera(&_colorProgram);

	_spriteBatch.begin();
	_spriteBatch.draw({ -_screenWidth/2,-_screenHeight/2, 80,80 }, Jauntlet::ResourceManager::getTexture(_tileHandler.getSelectedTileTexture()).id, 0);
	_spriteBatch.endAndRender();

	_colorProgram.unuse();
	
	_window.swapBuffer();
}