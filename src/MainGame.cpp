#include <Jauntlet/Rendering/ResourceManager.h>
#include <iostream>

#include "MainGame.h"

MainGame::MainGame() :
	_level(_textureCache, 64),
	_bricks("Textures/Drill Walls.png"),
	_players(3),
	_window() {
}

void MainGame::run() {
	initSystems();

	Jauntlet::Color _fpsColor = Jauntlet::Color(0,255,0,255); // #TODO: DELTEME

	_uiManager = Jauntlet::UIManager(&_hudCamera, &_HUDSpriteBatch); // #TODO: DELTEME

	Jauntlet::UITextElement* _fpsCounter = new Jauntlet::UITextElement(&_spriteFont, &_fpsText, &_fpsColor, &_fpsPosition); // #TODO: DELTEME

	_uiManager.addElement(_fpsCounter); // #TODO: DELTEME

	_uiManager.setScale((_screenHeight / 1080.0f) * (_screenWidth / 1920.0f));

	gameLoop();
}

void MainGame::initSystems() {
	Jauntlet::init();

	_window.create("Jauntlet Game Engine", _screenWidth, _screenHeight, Jauntlet::WindowFlags::RESIZEABLE);

	SDL_ShowCursor(1); // show the mouse cursor. can be set to 0 later for replacements.

	initShaders();

	_camera.init(_screenWidth, _screenHeight);
	_hudCamera.init(_screenWidth, _screenHeight);

	_cameraManager.init(&_camera, &_inputManager, &_players, &_level);

	// initialize player spriteBatch
	_playerSpriteBatch.init();
	_HUDSpriteBatch.init();

	// initializes spritefont
	_spriteFont.init(&_hudCamera, "Fonts/HandelGo.ttf", 256);

	// level loading
	_level.registerTileSet('W', _bricks, Jauntlet::TileCollision::SQUARE);
	_level.registerTile('F', "Textures/DrillFloor.png");

	_level.loadTileMap("Levels/level0.txt");
	//_level.loadTileMap("Levels/testAllTiles.txt");


	_navPoints = _navigation.genNav();

	_hudCamera.setActiveCamera(&_colorProgram); // #TODO: DELETEME
	_fpsPosition = glm::vec2(0, 0); // #TODO: DELTEME
	_uiManager.resolvePositions(); // #TODO: DELETEME
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

	_selectedTilePos = _level.RoundWorldPos(_camera.convertScreenToWorld(_inputManager.getMouseCoords()));

	_cameraManager.processInput();

	if (_inputManager.isKeyPressed(SDLK_F11) || (_inputManager.isKeyDown(SDLK_LALT) || _inputManager.isKeyDown(SDLK_RALT)) && _inputManager.isKeyPressed(SDLK_RETURN)) {
		_window.toggleFullscreen();
	}

	if (_inputManager.windowResized()) {
		_window.getWindowSize(); // This not only gets the window size, but also recalculates it incase of window resizing going unchecked.
		_screenWidth = _window.getWindowWidth();
		_screenHeight = _window.getWindowHeight();
		_camera.updateCameraSize(_screenWidth, _screenHeight);
		_hudCamera.updateCameraSize(_screenWidth, _screenHeight);
		_uiManager.resolvePositions();
		_uiManager.setScale((_screenHeight / 1080.0f) * (_screenWidth / 1920.0f));
	}

	//open nav
	if (_inputManager.isKeyPressed(SDLK_EQUALS)) {
		_navigation.toggleNav();
	}

	//mouse hover over navigation
	if (_navigation.isNavOpen()) {
		Jauntlet::Collision2D data = Jauntlet::Collision2D();
		for (int j = 0; j < _navigation.getColliders().size(); j++) {
			Jauntlet::BoxCollider2D adjustedCollider = Jauntlet::BoxCollider2D(_navigation.getColliders()[j].GetSize(), glm::vec2(_screenWidth / 2 + _navigation.getColliders()[j].position.x, _screenHeight / 2 - _navigation.getColliders()[j].position.y + 16));
			if (data.getCollision(&adjustedCollider, _inputManager.getMouseCoords())) {
				std::cout << "hovered over collider: " << j << std::endl;
			}
		}
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
	_camera.setActiveCamera(&_colorProgram);

	// Draw Level
	_level.draw();
	// Draw the player using a spriteBatch
	_playerSpriteBatch.begin();
	// draw the selected tile sprite
	_playerSpriteBatch.draw({_selectedTilePos.x, _selectedTilePos.y, 64, 64}, Jauntlet::ResourceManager::getTexture("Textures/WhiteSquare.png").id, 0);
	
	_players.draw(_playerSpriteBatch);

	_playerSpriteBatch.endAndRender();

	_colorProgram.unuse();
	
	drawHUD();
	
	_window.swapBuffer();
}

void MainGame::drawHUD() {
	_hudCamera.setActiveCamera(&_colorProgram);

	_HUDSpriteBatch.begin();

	_fpsText = std::to_string((int)_fps); // #TODO: DELTEME

	_uiManager.draw();

	_navigation.drawNav(_navPoints, _spriteFont, _HUDSpriteBatch);

	_HUDSpriteBatch.endAndRender();
}