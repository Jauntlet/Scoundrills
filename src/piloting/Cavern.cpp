#include "Cavern.h"
#include "src/scenes/GlobalContext.h"

#include <Jauntlet/Rendering/Textures/ResourceManager.h>

Cavern::Cavern(PlayerResources* resourceManager, Jauntlet::Camera2D* Camera) : _uiManager(Camera), _resources(resourceManager)
{
	_confirmTexture = Jauntlet::ResourceManager::getTexture("Textures/CheckBox.png").id; //temporary texture
	_confirmButton = Jauntlet::UIButtonElement(&GlobalContext::inputManager, [&]() -> void { updateResources(); }, _confirmTexture, &_confirmPos, glm::vec2(100), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
	_uiManager.addElement(&_confirmButton, &GlobalContext::normalShader);
	_confirmButton.visible = false;
	_uiManager.optimize();
	_uiManager.resolvePositions();
}

void Cavern::setType(int type) {
	_type = type;
}

void Cavern::display() {
	//make all elements visible
	_confirmButton.visible = true;
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
	_confirmButton.visible = false;
}

Jauntlet::UIManager* Cavern::getUIManager() {
	return &_uiManager;
}