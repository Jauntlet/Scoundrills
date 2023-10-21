#include "Navigation.h"

#include <chrono>
#include <Jauntlet/Rendering/ResourceManager.h>
#include <random>

const std::string bgTextures[] = {"Textures/NavGround1.png", "Textures/NavGround2.png", "Textures/NavGround3.png", "Textures/NavGround4.png"};
const int outcoveAmt = 4;
const int layerAmt = 3;
static int seed = std::chrono::system_clock::now().time_since_epoch().count(); //temp

Navigation::Navigation() : _background(nullptr) {
	if (_map.empty()) { //map not generated (so generate it)
		//generate some randomness
		std::mt19937 r = std::mt19937(seed);

		//create navPoints
		for (int y = 0; y < layerAmt; y++) {
			std::vector<int> layer = std::vector<int>();

			int temp = outcoveAmt + (r() % 2 - 1); //outcoveAmt - 1, outcoveAmt, outcoveAmt + 1
			for (int x = 0; x < temp; x++) {
				layer.push_back(r() % 3); //0, 1, 2
			}

			_map.push_back(layer);
		}
	}
}

Navigation::~Navigation() {
	delete _background;
}

void Navigation::genNav(Jauntlet::UIManager& UIM, Jauntlet::InputManager* inManager) {
	_xTure = Jauntlet::ResourceManager::getTexture("Textures/xmark.png").id;
	//set stuff
	UIManager = &UIM;
	_bgPos = glm::vec2(0);

	//clear stuff
	_navTextures.clear();
	_points.clear();
	_positions.clear();

	int sizeSum = 0;

	for (int i = 0; i < _map.size(); i++) {
		sizeSum += _map[i].size();
	}

	_positions.reserve(sizeof(glm::vec2) * sizeSum);
	_points.reserve(sizeof(Jauntlet::UIButtonElement) * sizeSum);

	//read in textures
	_navTextures.reserve(sizeof(GLuint) * 4);
	for (int i = 0; i < 4; i++) {
		_navTextures.push_back(Jauntlet::ResourceManager::getTexture(bgTextures[i]).id);
	}

	//draw background
	if (_background == NULL) {
		_background = new Jauntlet::UISpriteElement(_navTextures[0], &_bgPos, glm::vec2(1000), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
		UIM.addElement(_background);
	}

	int layersHeight = _map.size(); //store the total height of the "layers"

	for (int y = 0; y < _map.size(); y++) {

		int layerSpan = _map[y].size(); //store the "span" (width) of all the points on this "layer"

		for (int x = 0; x < _map[y].size(); x++) {
			int point = _map[y][x]; //The point type according to the generated "map," will determine the chance of encountering water, ores, etc. when arriving there.
			_positions.push_back(glm::vec2(200 * (x+1) - 100 * (layerSpan + 1), 300 * (y+1) - 150 * (layersHeight + 1))); //0 is the center of the screen.
			if (point == 0) { // white X
				_points.push_back(Jauntlet::UIButtonElement(inManager, [&]() -> void { selectNav(_positions.size() - 1); }, _xTure, &_positions[_positions.size() - 1], glm::vec2(128), Jauntlet::UIElement::ORIGIN_PIN::CENTER));
				continue;
			}
			if (point == 1) { // blue X
				_points.push_back(Jauntlet::UIButtonElement(inManager, [&]() -> void { selectNav(_positions.size() - 1); }, _xTure, &_positions[_positions.size() - 1], glm::vec2(128), Jauntlet::UIElement::ORIGIN_PIN::CENTER));
				continue;
			}
			if (point == 2) { // orange X
				_points.push_back(Jauntlet::UIButtonElement(inManager, [&]() -> void { selectNav(_positions.size() - 1); }, _xTure, &_positions[_positions.size() - 1], glm::vec2(128), Jauntlet::UIElement::ORIGIN_PIN::CENTER));
				continue;
			}
		}
	}

	for (int i = 0; i < _points.size(); i++) {
		UIM.addElement(&_points[i]);
	}

	//update visibility
	_background->visible = _navOpen;
	for (int i = 0; i < _points.size(); i++) {
		_points[i].visible = _navOpen;
	}
}

void Navigation::toggleNav() {
	_navOpen = !_navOpen;
	//update visibility
	_background->visible = _navOpen;
	for (int i = 0; i < _points.size(); i++) {
		_points[i].visible = _navOpen;
	}
}

bool Navigation::isNavOpen() {
	return _navOpen;
}

void Navigation::selectNav(int id) {
	destination = id;
}