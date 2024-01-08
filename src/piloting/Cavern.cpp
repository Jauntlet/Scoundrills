#include "Cavern.h"
#include "src/players/PlayerManager.h"
#include "src/scenes/GlobalContext.h"

#include <Jauntlet/Rendering/Textures/ResourceManager.h>

Cavern::Cavern(PlayerResources* resourceManager, Jauntlet::Camera2D* Camera) :
	_uiManager(Camera), _resources(resourceManager), _camera(Camera),
	_descriptionElement(GlobalContext::textRenderer, &_description, &_descriptionColor, &_descriptionPos, 0.3f)
{
	_backgroundTexture = Jauntlet::ResourceManager::getTexture("Textures/Caverns/cavernBackground.png").id;
	_backgroundElement = Jauntlet::UISpriteElement(_backgroundTexture, &_backgroundPos, glm::vec2(1200, 900), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
	_confirmTexture = Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id; //temporary texture
	_confirmButton = Jauntlet::UIButtonElement(&GlobalContext::inputManager, [&]() -> void { updateResources(); }, _confirmTexture, &_confirmPos, glm::vec2(100), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
	_uiManager.addElement(&_backgroundElement, &GlobalContext::normalShader);
	_uiManager.addElement(&_confirmButton, &GlobalContext::normalShader);
	_uiManager.addElement(&_descriptionElement, &Jauntlet::TextRenderer::textShader);
	_backgroundElement.visible = false;
	_confirmButton.visible = false;
	_descriptionElement.visible = false;
	_uiManager.optimize();
	_uiManager.resolvePositions();
}

void Cavern::setType(int type) {
	_type = type;
	switch (type) {
	case 1:
		//random
		_description = "Fate unknown...";
		break;
	case 2:
		//water
		_description = "An underground lake! yippee!";
		break;
	case 3:
		//new inmate
		_description = "You should theoretically have new company...";
		break;
	case 4:
		//danger (heat)
		_description = "oh no! heat rises";
		break;
	case 5:
		//copper
		_description = "You got copper woo";
		break;
	default:
		//invalid location
		_description = "Bro how (game broke)";
		break;
	}
}

void Cavern::display() {
	//make all elements visible
	_backgroundElement.visible = true;
	_confirmButton.visible = true;
	_descriptionElement.visible = true;
}

void Cavern::onScreenResize() {
	_descriptionPos = glm::vec2(_camera->getSize().x/5, 150);
	_open = true;
}

void Cavern::updateResources() {
	switch (_type) {
	case 1:
		//random
		break;
	case 2:
		//water
		_resources->water += 15;
		_resources->heat -= 50;
		break;
	case 3:
		//new inmate
		_playerManager->createPlayer({ 384, -128 }, _random() % 5 + 1);
		break;
	case 4:
		//danger (heat)
		_resources->heat += 30;
		break;
	case 5:
		//copper
		_resources->copper += 5;
		break;
	default:
		//invalid location
		break;
	}

	//update visibility
	_backgroundElement.visible = false;
	_confirmButton.visible = false;
	_descriptionElement.visible = false;

	_open = false;
}

Jauntlet::UIManager* Cavern::getUIManager() {
	return &_uiManager;
}

bool Cavern::isOpen() {
	return _open;
}

void Cavern::setPlayerManager(PlayerManager* manager) {
	_playerManager = manager;
}