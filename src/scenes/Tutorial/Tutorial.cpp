#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <iostream>

#include "../MainGame/UICoordinator.h"
#include "../PauseMenu.h"
#include "Jauntlet/Rendering/ProgressBar.h"
#include "Jauntlet/Time.h"
#include "Tutorial.h"
#include "src/interactable/Holdable.h"
#include "src/scenes/GlobalContext.h"
#include "src/scenes/Tutorial/Dialogue.h"

// we overwrite the defined min by windows because windows *really* likes its own min function that is not used elsewhere
#pragma warning (disable:4005)
#define min min

Tutorial::Tutorial(int saveID, const std::vector<uint8_t>& playerIDs) {
	_saveID = saveID;
	
	GlobalContext::window.setBackgroundColor(Color(97, 60, 47));
	_uiCoordinator.applyNewScreenSize(GlobalContext::screenSize);

	for (size_t i = 0; i < playerIDs.size(); ++i) {
		_players.createPlayer(glm::vec2(64 * (i + 1) + 5 * 64, -64 * 23), playerIDs[i], true);
		_players.getPathRenderer()->createPath(glm::vec2(64 * (i + 1) + 5 * 64, -64 * 23), glm::vec2(64 * (i + 1) + 5 * 64, -64 * 18));
		_players.getAllPlayers().back()->navigateTo(_drill, *_players.getPathRenderer(), glm::vec2(64 * (i + 1) + 5 * 64, -64 * 18));

		_displayPlayers.emplace_back(GlobalContext::playerIDtoTexture(playerIDs[i], true), glm::vec4(0,0,0.03571428571428571f,1), GlobalContext::playerIDtoTexture(playerIDs[i]), glm::vec4(0,0,0.03571428571428571f,1), glm::vec4(64 * (i + 1) + 5 * 64, -64 * 19, 64, 64));
		_displayPlayers[i].progress = 0;
	}
	_players.getPathRenderer()->clearPath();

	_camera.setScale(2.75f);
	_camera.setPosition(glm::vec2(1337.25, -3275.54));

	_drill.boilerWater = 0;
	_drill.off();
	_uiCoordinator.hideAll();

	_dialogue.pushNewText("Welcome new recruits!\nToday we will be going over\noperating the drill.");

	GlobalContext::inputManager.clearLastButtonPressed();

	_cameraManager.cameraUnlocked = false;
}

void Tutorial::windowResized() {

	glm::vec2 newCenter = glm::vec2(GlobalContext::screenSize) / 2.0f - _camera.getSize() / 2.0f;

	_camera.updateSize(GlobalContext::screenSize);
	_hudCamera.updateSize(GlobalContext::screenSize);
	_uiCoordinator.applyNewScreenSize(GlobalContext::screenSize);
	_dialogue.resize();
}

void Tutorial::gameLoop() {
	processInput();
 
	_camera.update();
	_hudCamera.update();
	_drill.update(&_uiCoordinator);

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
			_camera.transitionToPosition(glm::vec2(1337.25, -3275.54));
			_dialogue.pushNewText("Lets practice operating the drill.");
			break;
		case 12:
			_camera.transitionToScale(0.6f);
			_camera.transitionToPosition(glm::vec2(200, -675));
			_hasControl = true;
			_cameraManager.cameraUnlocked = false;
			_dialogue.pushNewText("First click on a player!");
			break;
		case 13:
			_dialogue.pushNewText("Now click on the water tank!");
			break;
		case 14:
			_camera.transitionToScale(2.0f);
			_camera.transitionToPosition(glm::vec2(437, -1425));
			_dialogue.pushNewText("Great! You have collected water!");
			break;
		case 15:
			_camera.transitionToScale(0.45f);
			_camera.transitionToPosition(glm::vec2(450, -350));
			_dialogue.pushNewText("Now lets fill the boiler\nwith that water!");
			break;
		case 16:
			_camera.transitionToScale(2.0f);
			_camera.transitionToPosition(glm::vec2(2037, -200));
			_dialogue.pushNewText("The drill can now run with\nthe supply of water!");
			break;
		case 17:
			_uiCoordinator.showDrillButton();
			_dialogue.pushNewText("Click this button to start\nthe drill!");
			break;
		case 18:
			_uiCoordinator.hideAll();
			_uiCoordinator.showWater();
			_camera.transitionToScale(0.25f);
			_camera.transitionToPosition(glm::vec2(275, -480));
			_dialogue.pushNewText("The drill is now running!");
			break;
		case 19:
			_dialogue.pushNewText("But now we need to figure out\nwhere to take the drill.");
			break;
		case 20:
			_dialogue.pushNewText("We will practice moving\nthe drill but we wont\nactually go anywhere.");
			break;
		case 21:
			_camera.transitionToScale(1.5f);
			_camera.transitionToPosition(glm::vec2(1100, -2500));
			_dialogue.pushNewText("Here is the steering wheel.\nThis is how we get the drill\nto move.");
			break;
		case 22:
			_camera.transitionToScale(0.75f);
			_camera.transitionToPosition(glm::vec2(450, -1250));
			_dialogue.pushNewText("Move another member to the\nsteering wheel!");
			break;
		case 23:
			_dialogue.pushNewText("Now that someone is piloting\nthe drill you can view\nNavigation!");
			break;
		case 24:
			_dialogue.pushNewText("Press TAB to open\nNavigation!");
			break;
		case 25:
			_dialogue.pushNewText("Navigation is how we\ndetermine where the drill\ngoes");
			break;
		case 26:
			_dialogue.pushNewText("Each icon is a cavern\nthe drill can go to.");
			break;
		case 27:
			_dialogue.pushNewText("You can get different resources\nfrom each one.");
			break;
		case 28:
			_dialogue.pushNewText("You can only travel to\nunobstructed caverns.");
			break;
		case 29:
			_dialogue.pushNewText("Try clicking on a point\nto travel to it!");
			break;
		case 30:
			_dialogue.pushNewText("Great work!");
			break;
		case 31:
			_dialogue.pushNewText("Once you reach a location\nyour crew will collect\nresources from the cavern.");
			break;
		case 32:
			_dialogue.pushNewText("Some caverns will have better\nresources than others.");
			break;
		case 33:
			_dialogue.pushNewText("So choose wisely!");
			break;
		case 34:
			if (_drill.navigation.isNavOpen()) {
				_drill.navigation.toggleNav();
			}

			_camera.transitionToPosition(glm::vec2(1400, -700));
			_camera.transitionToScale(1.5f);
			_drill.burstSpecificPipe(glm::ivec2(23,10));
			_dialogue.pushNewText("Oh no! A pipe in the drill\nhas burst!");
			break;
		case 35:
			_uiCoordinator.showTemperature();
			_dialogue.pushNewText("its very important to fix\nburst pipes as to keep the\ndrill temperature down!");
			break;
		case 36:
			_drill.addHoldable(glm::vec2(15*64,-640), HoldableType::SCRAP);
			_camera.transitionToPosition(glm::vec2(1425, -875));
			_dialogue.pushNewText("Here is a piece of scrap!");
			break;
		case 37:
			_dialogue.pushNewText("Scrap can be melted into\nmaterials in the forge!");
			_camera.transitionToPosition(glm::vec2(1600, -1300));
			break;
		case 38:
			_dialogue.pushNewText("Materials can then be used in\nthe pipe workbench to\ncraft a new pipe!");
			_camera.transitionToPosition(glm::vec2(1700, -2000));
			_uiCoordinator.showParts();
			break;
		case 39:
			_dialogue.pushNewText("First lets pick up that scrap!");
			_camera.transitionToPosition(glm::vec2(1200, -500));
			_camera.transitionToScale(0.5f);
			break;
		case 40:
			_dialogue.pushNewText("Now we take it to the forge!");
			_camera.transitionToScale(1.0f);
			_camera.transitionToPosition(glm::vec2(1000, -800));
			break;
		case 41:
			_uiCoordinator.showParts();
			_dialogue.pushNewText("Now we wait a moment\nfor the scrap to melt!");
			break;
		case 42:
			_dialogue.pushNewText("And finally we can\nmake a pipe at the\nworkbench!");
			_camera.transitionToScale(0.6f);
			_camera.transitionToPosition(glm::vec2(700, -700));
			break;
		case 43:
			_dialogue.pushNewText("Last step now!\ntake the pipe to the burst pipe\n to repair it!");
			_camera.transitionToPosition(glm::vec2(1200, -1200));
			_camera.transitionToScale(0.35f);
			break;
		case 44:
			_camera.transitionToScale(2.75f);
			_camera.transitionToPosition(glm::vec2(1337.25, -3275.54));
			_dialogue.pushNewText("Thank you for your time.");
			break;
		case 45:
			_dialogue.pushNewText("This covers all basic operations\nof the drill.");
			break;
		case 46:
			_dialogue.hide();
			_officer.walkOffscreen();
			break;
		case 47:
			for (uint8_t i = 0; i < _players.size(); ++i) {
				_players.getAllPlayers()[i]->forcePosition(glm::vec2(64 * (i + 1) + 5 * 64, -64 * 23));
				_players.getPathRenderer()->createPath(glm::vec2(64 * (i + 1) + 5 * 64, -64 * 23), glm::vec2(64 * (i + 1) + 5 * 64, -64 * 18));
				_players.getAllPlayers()[i]->navigateTo(_drill, *_players.getPathRenderer(), glm::vec2(64 * (i + 1) + 5 * 64, -64 * 18));
			}
			_players.getPathRenderer()->clearPath();
			break;
		case 48:
			break;
		case 49:
			_dialogue.show();
			_dialogue.pushNewText("It is time to escape!");
			break;
		case 50:
			_dialogue.pushNewText("Good Luck!");
			break;
		default:
			std::vector<uint8_t> output;
			for (Player* player : _players.getAllPlayers()) {
				output.push_back(player->getPlayerID());
			}
			GlobalContext::sceneManager->startGame(_saveID, output);
			break;
	}
}

void Tutorial::processInput() {
	_drill.navigation.cavern.hide();
	_resources.heat = 0;
	if (_sequence > 16) {
		_drill.boilerWater = 60;
	}

	if (_players.getAllPlayers().size() == 0) {
		return;
	}

	_players.update(_drill);
	
	_cameraManager.processInput();

	// in most cases, this switch statement will run the default case, however there are multiple cases
	// in which there are special instructions to move onto the next dialogue.
	switch (_sequence) {
	case 3:
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
		break;
	case 4:
		if (GlobalContext::inputManager.deltaScroll > 0) {
			_scrolledUp = true;
		}
		else if (GlobalContext::inputManager.deltaScroll < 0) {
			_scrolledDown = true;
		}
		else if (_scrolledUp && _scrolledDown) {
			nextDialogue();
		}
		break;
	case 12:
		if (_players.getSelectedPlayer() != nullptr) {
			nextDialogue();
		}
		break;
	case 13:
		for (Player* player : _players.getAllPlayers()) {
			if (player->heldItem != nullptr && player->heldItem->itemType == HoldableType::WATER) {
				nextDialogue();
				break;
			}
		}
		break;
	case 15:
		if (_drill.boilerWater != 0) {
			nextDialogue();
		}
		break;
	case 17:
		if (_drill.isOn()) {
			nextDialogue();
		}
		break;
	case 22:
		if (_drill.drillAssets.steeringWheel.isOccupied()) {
			nextDialogue();
		}
		break;
	case 24:
		if (_drill.navigation.isNavOpen()) {
			nextDialogue();
		}
		break;
	case 29:
		if (_drill.navigation.getMoving()) {
			nextDialogue();
		}
		break;
	case 39:
		for (Player* player : _players.getAllPlayers()) {
			if (player->heldItem != nullptr && player->heldItem->itemType == HoldableType::SCRAP) {
				nextDialogue();
				break;
			}
		}
		break;
	case 40:
		if (_drill.forge.playerAtStation()) {
			nextDialogue();
		}
		break;
	case 41:
		if (_drill.resources->copper > 0) {
			nextDialogue();
		} else {
			_drill.forge.meltingScrap += Jauntlet::Time::getDeltaTime() / 5;
		}
		break;
	case 42:
		if (_drill.resources->copper == 0) {
			nextDialogue();
		}
		break;
	case 43:
		if (_drill.BurstPipeCount() == 0) {
			nextDialogue();
		}
		break;
	case 46:
		if (!_officer.isWalking()) {
			nextDialogue();
		}
		break;
	case 47:
		if (!_players.getAllPlayers()[0]->isMoving()) {
			nextDialogue();
		}
	case 48:
		for (size_t i = 0; i < _displayPlayers.size(); ++i) {
			_displayPlayers[i].progress = glm::min(_displayPlayers[i].progress + Jauntlet::Time::getDeltaTime() * 0.25f, 1.0f);
		}
		if (_displayPlayers[0].progress == 1) {
			nextDialogue();
		}
		break;
	default:
		if (GlobalContext::inputManager.lastButtonPressed() != SDLK_ESCAPE || GlobalContext::inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
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
	if (_sequence < 48 && !(_sequence > 44 && _sequence < 47)) {
		_players.draw(_playerSpriteBatch);
	} else if (!(_sequence > 44 && _sequence < 47)) {
		for (size_t i = 0; i < _displayPlayers.size(); ++i) {
		_displayPlayers[i].draw(_playerSpriteBatch);
		}
	} 
	_playerSpriteBatch.endAndRender();

	if (_sequence < 47) {
		_officer.draw(_camera);
	}

	_drill.drawLayerTwo();

	if (_hasControl && !GlobalContext::pauseMenu->isPaused()) {
		_selectedTile.draw(&_camera);
	}


	GlobalContext::normalShader.unuse();

	drawHUD();
}

void Tutorial::drawHUD() {
	_hudCamera.setActive();

	if (_sequence == 17) {
		_dialogue.update();
		_uiCoordinator.draw();
	} else {
		_uiCoordinator.draw();
		_dialogue.update();
	}
}