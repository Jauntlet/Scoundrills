#include "MainGame.h"
#include <Jauntlet/tinyfiledialogs.h>
#include <Jauntlet/IOManager.h>

// initialize the window via an initializer list
MainGame::MainGame() 
	: 
	_window("Jauntlet Game Engine", _screenWidth, _screenHeight, 0),
	_camera(_screenWidth, _screenHeight) 
{
	// Empty
}

void MainGame::run() {
	initSystems();
	gameLoop();
}

void MainGame::initSystems() {
	// initialize Jauntlet
	Jauntlet::init();

	// set some default window properties
	_window.setBackgroundColor(Jauntlet::Color(76, 24, 32));
	_window.setWindowIcon("Textures/Icon.png");

	// set default missing Texture
	Jauntlet::ResourceManager::setMissingTexture("Textures/Icon.png");

	// initialize shaders
	initShaders();

	// DEBUGGING
	do {
		tinyfd_messageBox("File Systems Debugging", "To start, please delete the Logs folder within Scoundrills. This may be in the builds folder based on your setup. \n\nIf this shows again after pressing OK, there may be an error.", "ok", "info", 1);
	} while (Jauntlet::IOManager::findFolder("Logs"));

	if (Jauntlet::IOManager::createFolder("Logs")) {
		tinyfd_messageBox("File Systems Debugging", "Successfully created the Logs folder, please make sure it is there", "ok", "info", 1);
	}
	else {
		tinyfd_messageBox("File Systems Debugging", "Failed to create the Logs folder!", "ok", "error", 1);
	}

}

void MainGame::initShaders() {
	// compile a vertex and fragment shader, with properties to be able to render simple objects.
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void MainGame::gameLoop() {
	// set the max fps the game can run at. -1 uncaps framerate.
	Jauntlet::Time::setMaxFPS(60);

	while (_gameState == GameState::PLAY) {
		// start a frame
		Jauntlet::Time::beginFrame();
		// process user inputs
		processInput();
		// update the camera
		_camera.update();
		// draw the game
		drawGame();
		// end a frame. This returns the FPS of the game
		_fps = Jauntlet::Time::endFrame();
	}
}

void MainGame::processInput() {
	// process all possible inputs through the input manager
	_inputManager.processInput();

	// runs if the user quits the game through alt+f4, the X in the corner of the window, or any other means
	if (_inputManager.quitGameCalled()) {
		_gameState = GameState::EXIT;
	}
	// runs if the window is resized by the user during runtime
	if (_inputManager.windowResized()) {
		_window.resolveWindowSize(); // find the updated window size
		// update local variables
		_screenWidth = _window.getWindowWidth();
		_screenHeight = _window.getWindowHeight();
		// tell the camera the new window size
		_camera.updateCameraSize(_screenWidth, _screenHeight);
	}
}

void MainGame::drawGame() {
	// Reset screen
	_window.clearScreen();
	// activate shaders
	_colorProgram.use();
	// activate camera
	_camera.setActiveCamera();

	// --> Draw things you want to render here <--

	// disables shaders
	_colorProgram.unuse();
	// put the newly rendered objects on screen
	_window.swapBuffer();
}