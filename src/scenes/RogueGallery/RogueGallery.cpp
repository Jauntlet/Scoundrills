#include "RogueGallery.h"
#include "../SceneManager.h"
#include "Jauntlet/Inputs/InputManager.h"
#include "src/scenes/GlobalContext.h"

RogueGallery::RogueGallery(int saveID, bool tutorialMode) :
	_crew{SelectableCrew(1, tutorialMode), SelectableCrew(2, tutorialMode), SelectableCrew(3, tutorialMode), SelectableCrew(4, tutorialMode), SelectableCrew(5, tutorialMode)},
	_goToTutorial(tutorialMode)
{
	_saveID = saveID;
	GlobalContext::window.setBackgroundColor(Color(200,200,200));

	_uiManager.addElement(&_confirmButton, &GlobalContext::normalShader);
	_uiManager.addElement(&_titleElement, &TextRenderer::textShader);
	_uiManager.addElement(&_subtitleElement, &TextRenderer::textShader);
	_uiManager.addElement(&_confirmTextElement, &TextRenderer::textShader);
	
	_selectedCrew.push_back(&_crew[0]);
	_selectedCrew.push_back(&_crew[1]);
	_selectedCrew.push_back(&_crew[2]);
	
	windowResized();

	_music.playWAV("Sounds/death organs.wav", true);
}

void RogueGallery::windowResized() {
	_camera.updateSize(GlobalContext::screenSize);

	_uiManager.setScale(GlobalContext::screenSize.y / 1080.0f);
	_uiManager.optimize();
	_uiManager.resolvePositions();

	for (int i = 0; i < INMATE_COUNT; ++i) {
		_crew[i].position.x = -(GlobalContext::screenSize.x / 2.0f) + (GlobalContext::screenSize.x / (INMATE_COUNT + 1.0f) * (i + 1.0f)) - 120.0f;
		_crew[i].position.y = -120;
	}
}
void RogueGallery::gameLoop() {
	_camera.update();
	_music.update();

	_camera.setActive();
	GlobalContext::normalShader.use();

	_uiManager.draw();

	glm::vec2 controllerAxis = GlobalContext::inputManager.getControllerAxis(Axis::LeftStick);

	if (abs(controllerAxis.x) > 0.5 || abs(controllerAxis.y) > 0.5) {
		if (!_moving) {
			if (controllerAxis.y > 0.5) {
				_controllerOnCrew = !_controllerOnCrew;
			} else if (controllerAxis.y < -0.5) {
				_controllerOnCrew = !_controllerOnCrew;
			}

			_confirmButton.highlighted = !_controllerOnCrew;

			if (_controllerOnCrew) {
				if (controllerAxis.x > 0.5) {
					++_controllerSelectedIndex;
					if (_controllerSelectedIndex == INMATE_COUNT ) {
						_controllerSelectedIndex = 0;
					}
				} else if (controllerAxis.x < -0.5) {
					if (_controllerSelectedIndex == 0) {
						_controllerSelectedIndex = INMATE_COUNT - 1;
					} else {
						--_controllerSelectedIndex;
					}
				}
			}
			_moving = true;
		} 
	} else {
		_moving = false;
	}

	_batch.begin();
	for (SelectableCrew& inmate : _crew) {
		if ((!inmate.isSelected() && inmate.wasClicked(_camera)) || (_controllerOnCrew && GlobalContext::inputManager.isKeyPressed(CONTROLLER_FACE_EAST) && &_crew[_controllerSelectedIndex] == &inmate)) {
			inmate.Select();
			_selectedCrew.push_back(&inmate);
			
			_selectedCrew.front()->unSelect();
			_selectedCrew.pop_front();

		}
		inmate.draw(_batch);
	}
	
	if (_controllerOnCrew) {
		_batch.draw(glm::vec4(_crew[_controllerSelectedIndex].position, 240, 240), _selectionTextureID);	
	}
	
	_batch.endAndRender();

	if (GlobalContext::inputManager.isKeyPressed(SDLK_ESCAPE)) {
		GlobalContext::sceneManager->switchScene(GameState::MAINMENU);
	}
	if (!_controllerOnCrew && GlobalContext::inputManager.isKeyPressed(CONTROLLER_FACE_EAST)) {
		_confirmButton.click();
	}
}

void RogueGallery::loadGame() {
	std::vector<uint8_t> IDS;
	for (size_t i = 0; i < _selectedCrew.size(); ++i) {
		IDS.push_back(_selectedCrew[i]->getPlayerID());
	}

	if (_goToTutorial) {
		GlobalContext::sceneManager->loadTutorial(_saveID, IDS);
	} else {
		GlobalContext::sceneManager->startGame(_saveID, IDS);
	}
}