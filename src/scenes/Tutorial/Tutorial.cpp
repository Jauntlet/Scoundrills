#include "Tutorial.h"
#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include "../MainGame/UICoordinator.h"
#include "../PauseMenu.h"
#include "src/scenes/GlobalContext.h"

Tutorial::Tutorial(const std::vector<uint8_t>& playerIDs) {
	GlobalContext::window.setBackgroundColor(Jauntlet::Color(97, 60, 47));
	_uiCoordinator.applyNewScreenSize(glm::ivec2(GlobalContext::screenSize.x, GlobalContext::screenSize.y));

	for (int i = 0; i < playerIDs.size(); ++i) {
		_players.createPlayer(glm::vec2(64 * (i + 1) + 5 * 64, -64 * 23), playerIDs[i], true);
		_players.getPathRenderer()->createPath(glm::vec2(64 * (i + 1) + 5 * 64, -64 * 23), glm::vec2(64 * (i + 1) + 5 * 64, -64 * 20));
		_players.getAllPlayers().back()->navigateTo(_drill, *_players.getPathRenderer(), glm::vec2(64 * (i + 1) + 5 * 64, -64 * 20));
	}
	_players.getPathRenderer()->clearPath();

	_camera.setScale(2.75f);
	_camera.setPosition(glm::vec2(1337.25, -3475.54));

	_drill.off();
	_uiCoordinator.hideAll();

	_dialogue.pushNewText("Welcome new recruits!\nToday we will be going over\noperating the drill.");

	GlobalContext::inputManager.clearLastButtonPressed();
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
		case (1):
			_dialogue.pushNewText("This gal is a relic\nbut she gets the job done.");
			break;
		case (2):
			_camera.transitionToPosition(glm::vec2(24 * 64 * 0.5f, 30 * 64 * 0.5f * -1));
			_camera.transitionToScale(0.25f);
			_dialogue.pushNewText("Here is the layout of the drill.\nTraversing quickly is key\nto operating it.");
			break;
		case (3):
			_cameraLocked = false;
			_dialogue.pushNewText("Try moving around your view\nby clicking and dragging\nyour mouse.");
			_sequence = 2;
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

	if (!_cameraLocked) {
		_cameraManager.processInput();
	}

	if (GlobalContext::inputManager.lastButtonPressed() != SDLK_ESCAPE || GlobalContext::inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		_dialogue.doneReadingText() ? nextDialogue() : _dialogue.pushAllText();
		GlobalContext::inputManager.clearLastButtonPressed();
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

	if (!_cameraLocked && !GlobalContext::pauseMenu->isPaused()) {
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