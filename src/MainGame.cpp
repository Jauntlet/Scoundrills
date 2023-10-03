#include <Jauntlet/Rendering/ResourceManager.h>
#include <iostream>

#include "Jauntlet/Rendering/TextureCache.h"
#include "Jauntlet/UI/UIElement.h"
#include "MainGame.h"

MainGame::MainGame() :
	_players(3),
	_window() {
}

void _bruh() {
	std::cout << "bruh" << std::endl;
}

void MainGame::run() {
	initSystems();

	Jauntlet::Color _fpsColor = Jauntlet::Color(0,255,0,255); // #TODO: DELTEME

	_uiManager = Jauntlet::UIManager(&_hudCamera); // #TODO: DELTEME

	Jauntlet::UITextElement* _fpsCounter = new Jauntlet::UITextElement(&_spriteFont, &_fpsText, &_fpsColor, &_fpsPosition); // #TODO: DELTEME

	_uiManager.addElement(_fpsCounter); // #TODO: DELTEME


	GLuint _buttonTexture = Jauntlet::ResourceManager::getTexture("Textures/button.png").id;

	glm::vec2* buttonPos = new glm::vec2(10,10);

	Jauntlet::UIButtonElement* _button = new Jauntlet::UIButtonElement(&_inputManager, _bruh, _buttonTexture, buttonPos, glm::vec2(512,512), Jauntlet::UIElement::ORIGIN_PIN::TOP_LEFT);

	_uiManager.addElement(_button);

	_uiManager.setScale((_screenHeight / 1080.0f) * (_screenWidth / 1920.0f));; // #TODO: DELETEME

	_uiManager.resolvePositions(); // #TODO: DELETEME

	_navigation.genNav(_uiManager, &_spriteFont, &_screenWidth, &_screenHeight);
	_navigation.genNav(_uiManager, &_spriteFont, &_screenWidth, &_screenHeight);

	gameLoop();
}

void MainGame::initSystems() {
	Jauntlet::init();

	_window.create("Jauntlet Game Engine", _screenWidth, _screenHeight, Jauntlet::WindowFlags::RESIZEABLE);
	_window.setBackgroundColor(Jauntlet::Color(76, 24, 32));
	_window.setWindowIcon("Textures/Icon.png");

	SDL_ShowCursor(1); // show the mouse cursor. can be set to 0 later for replacements.

	initShaders();

	_camera.init(_screenWidth, _screenHeight);
	_hudCamera.init(_screenWidth, _screenHeight);

	_cameraManager.init(&_camera, &_inputManager, &_players, &_drill.drillWalls);

	// initialize player spriteBatch
	_playerSpriteBatch.init();
	_HUDSpriteBatch.init();

	// initializes spritefont
	_spriteFont.init(&_hudCamera, "Fonts/HandelGo.ttf", 256);

	_drill.init();
	
	_hudCamera.setActiveCamera(&_colorProgram); // #TODO: DELETEME
	_fpsPosition = glm::vec2(0, 0); // #TODO: DELTEME
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

	_selectedTilePos = _drill.drillWalls.RoundWorldPos(_camera.convertScreenToWorld(_inputManager.getMouseCoords()));

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
		_navigation.genNav(_uiManager, &_spriteFont, &_screenWidth, &_screenHeight);
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
			Jauntlet::BoxCollider2D adjustedCollider = Jauntlet::BoxCollider2D(_navigation.getColliders()[j].GetSize(), glm::vec2(_screenWidth / 2.0f + _navigation.getColliders()[j].position.x, _screenHeight / 2.0f - _navigation.getColliders()[j].position.y + 16));
			if (data.getCollision(&adjustedCollider, _inputManager.getMouseCoords())) {
				std::cout << "hovered over collider: " << j << std::endl;
			}
		}
	}
}

void MainGame::drawGame() {
	// Reset screen
	_window.clearScreen();
	
	_colorProgram.use();
	
	glActiveTexture(GL_TEXTURE0);

	// Reading information into shaders
	glUniform1i(_colorProgram.getUniformLocation("imageTexture"), 0);
	_camera.setActiveCamera(&_colorProgram);

	_drill.draw();

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

	_fpsText = std::to_string((int)_fps); // #TODO: DELTEME

	_uiManager.draw();

	_HUDSpriteBatch.begin();

	_HUDSpriteBatch.endAndRender();
}