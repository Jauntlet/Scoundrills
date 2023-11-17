#include "Navigation.h"
#include "Jauntlet/UI/UIButtonElement.h"
#include "src/scenes/GlobalContext.h"

#include <chrono>
#include <Jauntlet/Rendering/ResourceManager.h>
#include <random>

const int layerCount = 5; //amt of layers (Y axis)
const int layerWidth = 5; //amt of destinations on each layer (X axis)

const std::string bgTextures[] = {"Textures/NavGround1.png"};
static int seed = std::chrono::system_clock::now().time_since_epoch().count(); //temp

Navigation::Navigation(Jauntlet::Camera2D* camera) : 
	_xTure(Jauntlet::ResourceManager::getTexture("Textures/xmark.png").id), 
	_caret(Jauntlet::ResourceManager::getTexture("Textures/caret.png").id),
	_uiManager(camera)
{
	//generate some randomness
	std::mt19937 random = std::mt19937(seed);

	//create navPoints
	for (int y = 0; y < layerCount; y++) {
		for (int x = 0; x < layerWidth; x++) {
			_map[y][x] = (random() % 3); //0, 1, 2
		}
	}
}

Navigation::~Navigation() {
	delete _background;
	delete _caretElement;
}

Jauntlet::UIManager* Navigation::genNav() {
	////clear stuff
	_navTextures.clear();
	_points.clear();
	_positions.clear();

	size_t sizeSum = 0;
	for (int i = 0; i < layerCount; ++i) {
		sizeSum += layerWidth;
	}

	_positions.reserve(sizeof(glm::vec2) * sizeSum);
	_points.reserve(sizeof(Jauntlet::UIButtonElement) * sizeSum);
	_navTextures.reserve(sizeof(GLuint) * 4);

	//read in textures
	for (int i = 0; i < 1; ++i) {
		_navTextures.push_back(Jauntlet::ResourceManager::getTexture(bgTextures[i]).id);
	}

	//draw background
	if (_background == NULL) {
		_background = new Jauntlet::UISpriteElement(_navTextures[0], &_bgPos, glm::vec2(640, 1024), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
		_uiManager.addElement(_background, &GlobalContext::normalShader);
	}

	for (int y = 0; y < layerCount; y++) {
		for (int x = 0; x < layerWidth; x++) {
			int point = _map[y][x]; //The point type according to the generated "map," will determine the chance of encountering water, ores, etc. when arriving there.
			_positions.push_back(glm::vec2(125 * (x+1) - 62.5 * (layerWidth + 1), 187.5 * (y+1) - 93.75 * (layerCount + 1))); //0 is the center of the screen.
			if (_positions[_positions.size() - 1].y < -250 || _positions[_positions.size() - 1].y > 500) continue;
			if (point == 0) { // white X
				int destID = _positions.size() - 1;
				Jauntlet::UIButtonElement button = Jauntlet::UIButtonElement(&GlobalContext::inputManager, [&, destID]() -> void { selectNav(destID); }, _xTure, &_positions[_positions.size() - 1], glm::vec2(40), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
				_points.push_back(button);
				continue;
			}
			if (point == 1) { // blue X
				int destID = _positions.size() - 1;
				Jauntlet::UIButtonElement button = Jauntlet::UIButtonElement(&GlobalContext::inputManager, [&, destID]() -> void { selectNav(destID); }, _xTure, &_positions[_positions.size() - 1], glm::vec2(40), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
				_points.push_back(button);
				continue;
			}
			if (point == 2) { // no X
				continue;
			}
		}
	}

	for (int i = 0; i < _points.size(); ++i) {
		_uiManager.addElement(&_points[i], &GlobalContext::normalShader);
	}

	//update visibility
	_background->visible = _navOpen;
	for (int i = 0; i < _points.size(); ++i) {
		_points[i].visible = _navOpen;
	}

	//create caret (selector icon)
	_caretElement = new Jauntlet::UISpriteElement(_caret, &_caretPos, glm::vec2(31.25, 18.75), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
	_uiManager.addElement(_caretElement, &GlobalContext::normalShader);
	_caretElement->visible = false;

	//Return UIManager
	return &_uiManager;

	// optimize batches
	//_uiManager.optimize();
	//_uiManager.resolvePositions();
}

void Navigation::toggleNav() {
	_navOpen = !_navOpen;
	//update visibility
	_background->visible = _navOpen;
	if (_caretSet) {
		_caretElement->visible = _navOpen;
	}
	
	for (int i = 0; i < _points.size(); ++i) {
		_points[i].visible = _navOpen;
	}
}

bool Navigation::isNavOpen() {
	return _navOpen;
}

void Navigation::selectNav(int id) {
	_caretSet = true;
	_destination = id;
	_caretPos = _positions[id] + glm::vec2(0, 37.5);
	if (!_caretElement->visible) {
		_caretElement->visible = true;
	}

	_uiManager.resolvePositions();
}

void Navigation::updateTravel() {
	if (_destination != -1) {
		_progress += 0.1f;
		float newX = 0.0f;
		float newY = 1.0f;
		refreshPositions(newX, newY);
		if (_progress >= 1.0f) {
			_destination = -1; //set dest
			//remove caret

			_caretElement->visible = false;
			//delete _caretElement;
		}
	}
	else
	{
		_progress = 0.0f;
	}
}

void Navigation::refreshPositions(float shiftX, float shiftY) {
	for (int i = 0; i < _positions.size(); i++) {
		_positions[i] = glm::vec2(_positions[i].x - shiftX, _positions[i].y + shiftY);
	}
}