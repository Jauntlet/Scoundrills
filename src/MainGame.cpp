#include <Jauntlet/Rendering/ResourceManager.h>
#include <Jauntlet/Externals/tinyfiledialogs.h>
#include <Jauntlet/Errors.h>
#include "MainGame.h"

MainGame::MainGame() : 
	_window("Jauntlet Game Engine", _screenWidth, _screenHeight, Jauntlet::WindowFlags::RESIZEABLE),
	_hudCamera(_screenWidth, _screenHeight),
	_cameraManager(glm::vec2(_screenWidth, _screenHeight), &_inputManager)
{
	// Empty

}

void MainGame::run() {
	initSystems();

	gameLoop();
}

void MainGame::initSystems() {
	Jauntlet::init();

	_window.setBackgroundColor(Jauntlet::Color(76, 24, 32));

	initShaders();
	Jauntlet::ResourceManager::setMissingTexture("Textures/Icon.png");

	_tileHandler.loadFile();
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
		_tileHandler.updateTile(_tileHandler.WorldPosToTilePos(_selectedTilePos), 0);
	}
	if (_inputManager.deltaScroll != 0) {
		_tileHandler.changeSelectedTile(_inputManager.deltaScroll);
	}

	if (_inputManager.isKeyDown(SDLK_LCTRL)) {
		if (_inputManager.isKeyPressed(SDLK_s)) {
			if (_inputManager.isKeyDown(SDLK_LSHIFT)) {
				// user is pressing CTRL+SHIFT+S
				_tileHandler.saveSelectedTileMapAs();
			}
			else {
				// user is pressing CTRL+S
				_tileHandler.saveAllFiles();
			}
		}
		else if (_inputManager.isKeyPressed(SDLK_l)) {
			// user is pressing CTRL+L
			_tileHandler.loadFile();
		}
	}
	else if (_inputManager.isKeyPressed(SDLK_LSHIFT)) {
		// user is pressing shift and not ctrl
		_tileHandler.toggleShadedTileMapView();
	}

	if (_inputManager.isKeyPressed(SDLK_DOWN)) {
		_tileHandler.changeSelectedTileMap(-1);
	}
	if (_inputManager.isKeyPressed(SDLK_UP)) {
		_tileHandler.changeSelectedTileMap(1);
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
	_cameraManager.ActivateCamera();

	_tileHandler.draw();

	_spriteBatch.begin();
	_spriteBatch.draw({ _selectedTilePos.x, _selectedTilePos.y, 64, 64 }, Jauntlet::ResourceManager::getTexture("Textures/WhiteSquare.png").id, 0);
	
	_spriteBatch.endAndRender();
	
	_hudCamera.setActiveCamera();

	_spriteBatch.begin();
	_spriteBatch.draw({ -_screenWidth/2,-_screenHeight/2, 80,80 }, Jauntlet::ResourceManager::getTexture(_tileHandler.getSelectedTileTexture()).id, 0);
	_spriteBatch.endAndRender();

	_colorProgram.unuse();
	
	_window.swapBuffer();
}