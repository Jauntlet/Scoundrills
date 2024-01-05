#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>

#include "../MainGame/UICoordinator.h"
#include "../PauseMenu.h"
#include "Tutorial.h"
#include "src/scenes/GlobalContext.h"
#include "src/scenes/Tutorial/Dialogue.h"

Tutorial::Tutorial(const std::vector<uint8_t>& playerIDs) {
	GlobalContext::window.setBackgroundColor(Jauntlet::Color(97, 60, 47));
	_uiCoordinator.applyNewScreenSize(GlobalContext::screenSize);

	for (int i = 0; i < playerIDs.size(); ++i) {
		_players.createPlayer(glm::vec2(64 * (i + 1) + 5 * 64, -64 * 24), playerIDs[i], true);
		_players.getPathRenderer()->createPath(glm::vec2(64 * (i + 1) + 5 * 64, -64 * 24), glm::vec2(64 * (i + 1) + 5 * 64, -64 * 19));
		_players.getAllPlayers().back()->navigateTo(_drill, *_players.getPathRenderer(), glm::vec2(64 * (i + 1) + 5 * 64, -64 * 19));
	}
	_players.getPathRenderer()->clearPath();

	_camera.setScale(2.75f);
	_camera.setPosition(glm::vec2(1337.25, -3475.54));

	_drill.off();
	_drill.boilerWater = 0;
	_uiCoordinator.hideAll();

	_dialogue.pushNewText("Welcome new recruits!\nToday we will be going over\noperating the drill.");

	GlobalContext::inputManager.clearLastButtonPressed();

	_cameraManager.cameraUnlocked = false;
}

void Tutorial::windowResized() {

	glm::vec2 newCenter = glm::vec2(GlobalContext::screenSize) / 2.0f - _camera.getSize() / 2.0f;

	//_camera.setScale(_camera.getScale() * (_camera.getSize().y / GlobalContext::screenSize.y));
	//_camera.setPosition(glm::vec2(_camera.getPosition().x - newCenter.x, _camera.getPosition().y - newCenter.y));

	_camera.updateSize(GlobalContext::screenSize);
	_hudCamera.updateSize(GlobalContext::screenSize);
	_uiCoordinator.applyNewScreenSize(GlobalContext::screenSize);
	_dialogue.resize();
}

void Tutorial::gameLoop() {
	processInput();
 
	_camera.update();
	_hudCamera.update();
	_drill.update();

	drawGame();
}

void Tutorial::nextDialogue() {
	++_sequence;

	switch (_sequence) {
		case 1:
			_dialogue.pushNewText("This gal is an old relic\nbut she gets the job done.");
			break;
		case 2:
			_camera.transitionToPosition(glm::vec2(275, -480));
			_camera.transitionToScale(0.25f);
			_dialogue.pushNewText("Here is the layout of the drill.\nTraversing quickly is key\nto operating it.");
			break;
		case 3:
			_cameraManager.cameraUnlocked = true;
			_dialogue.pushNewText("Try moving around your view\nby using WASD.");
			break;
		case 4:
			_dialogue.pushNewText("Great! Now use your scroll\nwheel to zoom in and out!");
			break;
		case 5:
			_cameraManager.cameraUnlocked = false;
			_dialogue.pushNewText("Brilliant! Now lets learn\nabout operating the drill!");
			break;
		case 6:
			_uiCoordinator.showWater();
			_dialogue.pushNewText("in the top left is our water\nsupply.");
			break;
		case 7:
			_camera.transitionToScale(2.0f);
			_camera.transitionToPosition(glm::vec2(437, -1425));
			_dialogue.pushNewText("This is directly tied to\nour water tank.");
			break;
		case 8:
			_dialogue.pushNewText("if you take a crew member to\nthe tank they will pick\nup water");
			break;
		case 9:
			_camera.transitionToPosition(glm::vec2(2037, -200));
			_dialogue.pushNewText("This is important to\nkeep our boiler working.");
			break;
		case 10:
			_dialogue.pushNewText("Our boiler runs on water\nand needs a constant\nsupply to keep the\ndrill running.");
			break;
		case 11:
			_camera.transitionToScale(2.75f);
			_camera.transitionToPosition(glm::vec2(1337.25, -3475.54));
			_dialogue.pushNewText("Lets practice operating the drill.");
			break;
		case 12:
			_camera.transitionToScale(0.6f);
			_camera.transitionToPosition(glm::vec2(200, -675));
			_hasControl = true;
			_cameraManager.cameraUnlocked = false;
			_dialogue.pushNewText("Try selecting a player and\nthen selecting the water tank.");
			break;
		default:
			std::vector<uint8_t> output;
			for (Player* player : _players.getAllPlayers()) {
				output.push_back(player->getPlayerID());
			}
			GlobalContext::sceneManager->loadGame(output);
			break;
	}
}

void Tutorial::processInput() {
	if (_players.getAllPlayers().size() == 0) {
		return;
	}

	_players.update(_drill);

	_cameraManager.processInput();

	if (GlobalContext::inputManager.lastButtonPressed() != SDLK_ESCAPE || GlobalContext::inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		if (_sequence == 3) {
			if (GlobalContext::inputManager.isKeyPressed(SDLK_w)) {
				_pressedW = true;
			}
			if (GlobalContext::inputManager.isKeyPressed(SDLK_a)) {
				_pressedA = true;
			}
			if (GlobalContext::inputManager.isKeyPressed(SDLK_s)) {
				_pressedS = true;
			}
			if (GlobalContext::inputManager.isKeyPressed(SDLK_d)) {
				_pressedD = true;
			}
			if (_pressedW && _pressedA && _pressedS && _pressedD) {
				nextDialogue();
			}
		} else if (_sequence == 4) {
			if (GlobalContext::inputManager.deltaScroll > 0) {
				_scrolledUp = true;
			} else if (GlobalContext::inputManager.deltaScroll < 0) {
				_scrolledDown = true;
			} else if (_scrolledUp && _scrolledDown) {
				nextDialogue();
			}
		} else {
			_dialogue.doneReadingText() ? nextDialogue() : _dialogue.pushAllText();
			GlobalContext::inputManager.clearLastButtonPressed();
		}
		
	}

	//open nav
	if (GlobalContext::inputManager.isKeyPressed(SDLK_TAB)) {
		_uiCoordinator.navigation->toggleNav();
	}

	if (GlobalContext::inputManager.isKeyPressed(SDLK_ESCAPE)) {
		GlobalContext::pauseMenu->togglePauseMenu();
	}

	if (GlobalContext::pauseMenu->isPaused()) {
		Jauntlet::Time::setTimeScale(0.0f);
	}
	else {
		Jauntlet::Time::setTimeScale(1.0f);
	}
}

void Tutorial::drawGame() {
	GlobalContext::normalShader.use();
	_camera.setActive();

	_drill.drawLayerOne();

	// Draw the player using a spriteBatch
	_playerSpriteBatch.begin();
	_players.draw(_playerSpriteBatch);
	_playerSpriteBatch.endAndRender();
	_officer.draw();

	_drill.drawLayerTwo();

	if (!_hasControl && !GlobalContext::pauseMenu->isPaused()) {
		_selectedTile.draw(&_camera);
	}

	_dialogue.update();

	GlobalContext::normalShader.unuse();

	drawHUD();
}

void Tutorial::drawHUD() {
	_hudCamera.setActive();

	_uiCoordinator.draw();
}