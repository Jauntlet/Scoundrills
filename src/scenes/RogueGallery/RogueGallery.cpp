#include "RogueGallery.h"
#include "../SceneManager.h"

#include <iostream>

RogueGallery::RogueGallery(int saveID, bool tutorialMode) :
	_crew{SelectableCrew(1, tutorialMode), SelectableCrew(2, tutorialMode), SelectableCrew(3, tutorialMode), SelectableCrew(4, tutorialMode), SelectableCrew(5, tutorialMode)},
	_goToTutorial(tutorialMode)
{
	_saveID = saveID;
	GlobalContext::window.setBackgroundColor(Jauntlet::Color(200,200,200));

	_uiManager.addElement(&_confirmButton, &GlobalContext::normalShader);
	_uiManager.addElement(&_titleElement, &Jauntlet::TextRenderer::textShader);
	_uiManager.addElement(&_subtitleElement, &Jauntlet::TextRenderer::textShader);
	_uiManager.addElement(&_confirmTextElement, &Jauntlet::TextRenderer::textShader);
	
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

	_batch.begin();
	for (SelectableCrew& inmate : _crew) {
		if (!inmate.isSelected() && inmate.wasClicked(_camera)) {
			inmate.Select();
			_selectedCrew.push_back(&inmate);
			
			_selectedCrew.front()->unSelect();
			_selectedCrew.pop_front();

		}
		
		inmate.draw(_batch);
	}

	_batch.endAndRender();
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