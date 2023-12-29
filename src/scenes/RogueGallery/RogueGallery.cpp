#include "RogueGallery.h"
#include "../SceneManager.h"

RogueGallery::RogueGallery() {
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
	_camera.updateCameraSize(GlobalContext::screenSize);

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

	_camera.setActiveCamera();
	GlobalContext::normalShader.use();

	_uiManager.draw();

	_batch.begin();
	for (int i = 0; i < INMATE_COUNT; ++i) {
		if (!_crew[i].isSelected() && _crew[i].wasClicked(_camera)) {
			_crew[i].Select();
			_selectedCrew.push_back(&_crew[i]);
			
			_selectedCrew[0]->unSelect();
			_selectedCrew.pop_front();
		}
		
		_crew[i].draw(_batch);
	}

	_batch.endAndRender();
}

void RogueGallery::loadGame() {
	if (_goToTutorial) {
		// we will go to the tutorial once its implemented LOSER
	} else {
		std::vector<uint8_t> IDS;
		for (int i = 0; i < _selectedCrew.size(); ++i) {
			IDS.push_back(_selectedCrew[i]->getPlayerID());
		}
		GlobalContext::sceneManager->loadGame(IDS);
	}
}