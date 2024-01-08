#include "Cavern.h"
#include "src/players/PlayerManager.h"
#include "src/scenes/GlobalContext.h"

#include <Jauntlet/Rendering/Textures/ResourceManager.h>

Cavern::Cavern(PlayerResources* resourceManager, Jauntlet::Camera2D* Camera) :
	_uiManager(Camera), _resources(resourceManager), _camera(Camera),
	_descriptionElement(GlobalContext::textRenderer, &_description, &_descriptionColor, &_descriptionPos, Jauntlet::UIElement::ORIGIN_PIN::TOP, 0.3f)
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
		_description = "Your crew stumbles upon\nuncharted terrain...\n"; //base text

		//determine resources to add/remove
		_addHeat = -10 * (_random() % 3);
		_addWater = 10 * (_random() % 3);
		_addFood = 5 * (_random() % 5);
		_addCopper = 10 * (_random() % 2);

		//display resources in description
		if (_addHeat != 0) {
			_description += "\nHeat: " + std::to_string(_addHeat);
		}

		if (_addWater != 0) {
			_description += "\nAdditional Water: " + std::to_string(_addWater);
		}

		if (_addFood != 0) {
			_description += "\nAdditional Food: " + std::to_string(_addFood);
		}

		if (_addCopper != 0) {
			_description += "\nAdditional Copper: " + std::to_string(_addCopper);
		}

		_descriptionPos = glm::vec2(0, 150);
		break;
	case 2:
		//water
		_description = "An underground lake...\nIt almost glows with\na mysterious energy!\n";

		//determine resources to add/remove
		_addHeat = -10 * (_random() % 3);
		_addWater = 15 * (_random() % 4);

		//display resources in description
		if (_addHeat != 0) {
			_description += "\nHeat: " + std::to_string(_addHeat);
		}

		if (_addWater != 0) {
			_description += "\nWater: " + std::to_string(_addWater);
		}

		_descriptionPos = glm::vec2(0, 150);
		break;
	case 3:
		//new inmate
		_description = "Another drill has broken down nearby.\nA new inmate decides to join your crew!\n\nAdditional Crew: 1";
		_descriptionPos = glm::vec2(0, 150);
		break;
	case 4:
		//danger (heat)
		_description = "Everything is NOT okay as you discover\nlava exists underground.";

		//determine resources to add/remove
		_addHeat = 20 * (_random() % 3);
		_addWater = -10 * (_random() % 2);
		_addCopper = 5 * (_random() % 3);

		//display resources in description
		if (_addHeat != 0) {
			_description += "\nAdditional Heat: " + std::to_string(_addHeat) + " Jelvin";
		}

		if (_addWater != 0) {
			_description += "\nWater: " + std::to_string(_addWater);
		}

		if (_addCopper != 0) {
			_description += "\nAdditional Copper: " + std::to_string(_addCopper);
		}

		_descriptionPos = glm::vec2(0, 150);
		break;
	case 5:
		//copper
		_description = "It's time to pilfer the depths\nof ALL its glorious ores!";

		//determine resources to add/remove
		_addHeat = -10 * (_random() % 3);
		_addWater = 5 * (_random() % 3);
		_addFood = 5 * (_random() % 3);
		_addCopper = 10 * (_random() % 4+1);

		//display resources in description
		if (_addHeat != 0) {
			_description += "\nHeat: " + std::to_string(_addHeat);
		}

		if (_addWater != 0) {
			_description += "\nAdditional Water: " + std::to_string(_addWater);
		}

		if (_addFood != 0) {
			_description += "\nAdditional Food: " + std::to_string(_addFood);
		}

		if (_addCopper != 0) {
			_description += "\nAdditional Copper: " + std::to_string(_addCopper);
		}

		_descriptionPos = glm::vec2(0, 150);
		break;
	default:
		//invalid location
		_description = "";
		break;
	}

	_uiManager.optimize();
	_uiManager.resolvePositions();
}

void Cavern::display() {
	//make all elements visible
	_backgroundElement.visible = true;
	_confirmButton.visible = true;
	_descriptionElement.visible = true;

	_open = true;
}

void Cavern::updateResources() {
	//Add resources to inventory
	_resources->heat += _addHeat;
	_resources->water += _addWater;
	_resources->food += _addFood;
	_resources->copper += _addCopper;

	//in case of inmate
	if (_type == 3) {
		_playerManager->createPlayer({ 384, -128 }, _random() % 5 + 1);
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