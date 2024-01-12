#include "Navigation.h"
#include "Jauntlet/UI/UIButtonElement.h"
#include "src/scenes/GlobalContext.h"
#include "Cavern.h"

#include <Jauntlet/JMath.h>
#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include <string>

const int layerCount = 5; //amt of layers (Y axis)
const int layerWidth = 5; //amt of destinations on each layer (X axis)
const float baseSpeed = 1.5; //This over distance determines the speed the drill moves to any destination; more is faster, less is slower

const std::string bgTextures[] = {"Textures/NavBackgroundPrototype.png"};

Navigation::Navigation(Jauntlet::Camera2D* camera, PlayerResources* resourceManager) : 
	_navTexture(Jauntlet::ResourceManager::getTexture(bgTextures[0]).id),
	_xTure(Jauntlet::ResourceManager::getTexture("Textures/Nav Icon/NavIcon1.png").id), 
	_waTure(Jauntlet::ResourceManager::getTexture("Textures/Nav Icon/NavIcon2.png").id), 
	_approaTure(Jauntlet::ResourceManager::getTexture("Textures/Nav Icon/NavIcon3.png").id), 
	_dangTure(Jauntlet::ResourceManager::getTexture("Textures/Nav Icon/NavIcon4.png").id), 
	_coppTure(Jauntlet::ResourceManager::getTexture("Textures/Nav Icon/NavIcon5.png").id), 
	_caret(Jauntlet::ResourceManager::getTexture("Textures/caret.png").id),
	_drillIcon(Jauntlet::ResourceManager::getTexture("Textures/drillNav.png").id),
	_background(Jauntlet::UISpriteAnimatedElement(_navTexture, &_bgPos, glm::vec2(640, 1024), Jauntlet::UIElement::ORIGIN_PIN::CENTER, &_backgroundAnimation)),
	_uiManager(camera),
	cavern(resourceManager, camera)
{
	//generate some randomness
	_random = std::mt19937(Jauntlet::Time::getTime());

	//create navPoints
	for (int y = 0; y < layerCount; y++) {
		for (int x = 0; x < layerWidth; x++) {
			_map[y][x] = (_random() % 20); //0-19, 20 total
		}
	}

	_backgroundAnimation.play(0, 2, 0.3f);
}

Navigation::~Navigation() {
	//delete _background;
	delete _caretElement;
	_caretElement = nullptr;
	delete _drillIconElement;
	_drillIconElement = nullptr;
}

Jauntlet::UIManager* Navigation::genNav() {
	////clear stuff
	_points.clear();
	_positions.clear();

	size_t sizeSum = layerCount * layerWidth;

	_positions.reserve(sizeof(glm::vec2) * sizeSum);
	_points.reserve(sizeof(Jauntlet::UIButtonElement) * sizeSum);

	//read in textures
	//_navTexture = Jauntlet::ResourceManager::getTexture(bgTextures[0]).id;

	_bgPos = glm::vec2(0, 0);

	//draw background
	//_background = Jauntlet::UISpriteAnimatedElement(_navTexture, &_bgPos, glm::vec2(640, 1024), Jauntlet::UIElement::ORIGIN_PIN::CENTER, &_backgroundAnimation);
	_uiManager.addElement(&_background, &GlobalContext::normalShader);

	//draw drill icon
	if (_drillIconElement == nullptr)
		_drillIconElement = new Jauntlet::UISpriteElement(_drillIcon, &_iconPos, glm::vec2(60), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
	_uiManager.addElement(_drillIconElement, &GlobalContext::normalShader);
	_drillIconElement->visible = _navOpen;

	//loop to generate points
	for (int y = 0; y < layerCount; y++) {
		for (int x = 0; x < layerWidth; x++) {
			int point = _map[y][x]; //The point type according to the generated "map," will determine the chance of encountering water, ores, etc. when arriving there.
			
			if (point <= 3) { //1 in 5
				continue; // no icon
			}
			
			_positions.emplace_back(125 * (x+1) - 62.5 * (layerWidth + 1), 187.5 * (y-1)); //0 is the center of the screen.
			bool visible = true;
			
			if (_positions[_positions.size() - 1].y < -250 || _positions[_positions.size() - 1].y > 500) {
				visible = false;
			}

			if (point <= 8) { // question mark (1 in 4)
				int destID = _positions.size() - 1;
				Jauntlet::UIButtonElement button = Jauntlet::UIButtonElement(&GlobalContext::inputManager, [&, destID, x, y]() -> void { selectNav(destID, glm::ivec2(x, y)); }, _xTure, &_positions[_positions.size() - 1], glm::vec2(40), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
				button.visible = visible;
				_points.push_back(button);
				continue;
			}

			if (point <= 10) { // water icon (1 in 10)
				int destID = _positions.size() - 1;
				Jauntlet::UIButtonElement button = Jauntlet::UIButtonElement(&GlobalContext::inputManager, [&, destID, x, y]() -> void { selectNav(destID, glm::ivec2(x, y)); }, _waTure, &_positions[_positions.size() - 1], glm::vec2(40), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
				button.visible = false;
				_points.push_back(button);
				continue;
			}

			if (point <= 13) { // approacher icon (1 in 20)
				int destID = _positions.size() - 1;
				Jauntlet::UIButtonElement button = Jauntlet::UIButtonElement(&GlobalContext::inputManager, [&, destID, x, y]() -> void { selectNav(destID, glm::ivec2(x, y)); }, _approaTure, &_positions[_positions.size() - 1], glm::vec2(40), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
				button.visible = false;
				_points.push_back(button);
				continue;
			}

			if (point <= 16) { // danger icon (3 in 20)
				int destID = _positions.size() - 1;
				Jauntlet::UIButtonElement button = Jauntlet::UIButtonElement(&GlobalContext::inputManager, [&, destID, x, y]() -> void { selectNav(destID, glm::ivec2(x, y)); }, _dangTure, &_positions[_positions.size() - 1], glm::vec2(40), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
				button.visible = false;
				_points.push_back(button);
				continue;
			}

			if (point <= 19) { // copper icon (3 in 20)
				int destID = _positions.size() - 1;
				Jauntlet::UIButtonElement button = Jauntlet::UIButtonElement(&GlobalContext::inputManager, [&, destID, x, y]() -> void { selectNav(destID, glm::ivec2(x, y)); }, _coppTure, &_positions[_positions.size() - 1], glm::vec2(40), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
				button.visible = false;
				_points.push_back(button);
				continue;
			}
		}
	}

	for (size_t i = 0; i < _points.size(); ++i) {
		_uiManager.addElement(&_points[i], &GlobalContext::normalShader);
	}

	//update visibility
	_background.visible = _navOpen;
	for (size_t i = 0; i < _points.size(); ++i) {
		_points[i].visible = _navOpen;
	}

	//create caret (selector icon)
	if (_caretElement == nullptr)
		_caretElement = new Jauntlet::UISpriteElement(_caret, &_caretPos, glm::vec2(31.25, 18.75), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
	_uiManager.addElement(_caretElement, &GlobalContext::normalShader);
	_caretElement->visible = false;

	// optimize batches
	_uiManager.optimize();
	_uiManager.resolvePositions();

	//Return UIManager
	return &_uiManager;
}

void Navigation::update() {
	_backgroundAnimation.update();
}

void Navigation::toggleNav() {
	_navOpen = !_navOpen;
	//update visibility
	_background.visible = _navOpen;
	_drillIconElement->visible = _navOpen;
	if (_caretSet) {
		_caretElement->visible = _navOpen;
	}
	
	for (size_t i = 0; i < _points.size(); ++i) {
		_points[i].visible = _navOpen;
	}
	updateVisibility();
}

bool Navigation::isNavOpen() {
	return _navOpen;
}

void Navigation::selectNav(int id, glm::ivec2 xy) {
	if (_destination != -1) return;
	//check for interference
	if (_drillRow != xy.y - 1) {
		int tempX = xy.x;
		for (int y = xy.y-1; y > _drillRow; y--) { //loop for amount of layers to traverse
			if (_columnOver == xy.x) { //straight up/down
				if (_map[y][xy.x] != 0) return;
			}
			else if (_columnOver < tempX) { //drill to left
				tempX--;
				if (_map[y][tempX] != 0) return;
			}
			else if (_columnOver > tempX) { //drill to right
				tempX++;
				if (_map[y][tempX] != 0) return;
			}
		}
	}

	//set a few things (necessary)
	//the order of operations also matters
	_caretSet = true;
	_destination = id;
	_speed = baseSpeed/JMath::Distance(_iconPos, _positions[id]);
	_shiftPos = glm::vec2(_positions[id].x, 187.5 * (xy.y - _drillRow));
	_caretPos = _positions[id] + glm::vec2(0, 37.5);
	_rowsTravelled = xy.y - _drillRow;
	_columnsTravelled = xy.x - _columnOver;
	_columnOver = xy.x;
	_drillRow = xy.y;

	//visibility check/update
	if (!_caretElement->visible) {
		_caretElement->visible = true;
	}

	//fix ui manager
	_uiManager.resolvePositions();
}

void Navigation::updateTravel() {
	if (_destination != -1) {
		_progress += Jauntlet::Time::getDeltaTime() * _speed;
		refreshPositions(_shiftPos.x * Jauntlet::Time::getDeltaTime() * _speed, _shiftPos.y * Jauntlet::Time::getDeltaTime() * _speed);
		if (_progress >= 1.0f) {
			//remove selector caret
			_caretElement->visible = false;
			
			//adjust for rows travelled
			//std::cout << _rowsTravelled << std::endl;
			
			//remap caverns linearly
			_mappedCaverns.clear();
			for (int y = 0; y < layerCount; y++) {
				for (int x = 0; x < layerWidth; x++) {
					if (_map[y][x] != 0) _mappedCaverns.push_back(_map[y][x]);
				}
			}
			
			//recycle map (spawn new points and remove old ones
			recycleMap(_rowsTravelled);
			_rowsTravelled = 0; //reset rows travelled
			_columnsTravelled = 0; //reset columns travelled
			_columnOver = layerWidth/2; //reset to middle
			_drillRow = -1; //reset drill's row
			_depth++; //increase depth

			//determine cavern type
			int cavType = 0;
			
			if (_mappedCaverns[_destination] <= 8) {
				cavType = 1;
			}
			else if (_mappedCaverns[_destination] <= 10) {
				cavType = 2;
			}
			else if (_mappedCaverns[_destination] <= 13) {
				cavType = 3;
			}
			else if (_mappedCaverns[_destination] <= 16) {
				cavType = 4;
			}
			else if (_mappedCaverns[_destination] <= 19) {
				cavType = 5;
			}

			//Call cavern event
			spawnCavern(cavType);
			
			_destination = -1; //set dest
			_progress = 0.0f;
		}
	} else {
		_progress = 0.0f; //no destination is set -- reset progress (to next dest)
	}
}

void Navigation::refreshPositions(float shiftX, float shiftY) {
	for (size_t i = 0; i < _positions.size(); i++) {
		_positions[i] = glm::vec2(_positions[i].x - shiftX, _positions[i].y - shiftY);
	}
	_caretPos = glm::vec2(_caretPos.x - shiftX, _caretPos.y - shiftY);
	updateVisibility(); //Update point visiblity
	_uiManager.resolvePositions();
}

void Navigation::recycleMap(int r) {
	//shift by y
	for (int y = r; y < layerCount+r; y++) {
		for (int x = 0; x < layerWidth; x++) {
			if (layerCount <= y) {
				_map[y - r][x] = (_random() % 20);
				continue;
			}

			_map[y - r][x] = _map[y][x];
		}
	}


	//shift by x
	if (_columnOver != layerWidth/2) {
		for (int y = 0; y < layerCount; y++) {
			int temp[layerWidth]; //space for maximum possible temporarily stored points to shift to the right
			for (int x = 0; x < layerWidth+glm::abs(_columnsTravelled); x++) {
				if (_columnsTravelled > 0) { //drill goes from left to right
					if (x + _columnsTravelled < layerWidth) {
						_map[y][x] = _map[y][x + _columnsTravelled];
					}
					else if (x < layerWidth) { //gen new
						_map[y][x] = (_random() % 20);
					}
				}
				else { //drill goes from right to left
					if (x >= layerWidth) {
						break;
					}
					if (x < glm::abs(_columnsTravelled)) { //first few -- store and generate
						temp[x] = _map[y][x];
						_map[y][x] = (_random() % 20);
					}
					else { //get from storage to add back
						temp[x] = _map[y][x];
						_map[y][x] = temp[x + _columnsTravelled];
					}
				}
			}
		}
	}

	for (size_t j = 0; j < _points.size(); j++) {
		_points[j].visible = false;
	}

	//clear buttons
	_uiManager.removeAllElements();

	//gen a nav :)
	genNav();

	//update visibility
	updateVisibility();
}

void Navigation::updateVisibility() {
	for (size_t i = 0; i < _positions.size(); i++) {
		if (_navOpen)
			_points[i].visible = !(_positions[i].y < -350 || _positions[i].y > 400) && !(_positions[i].x < -280 || _positions[i].x > 280);
		else
			_points[i].visible = false;
	}
}

Jauntlet::UIManager* Navigation::getUIManager() {
	return &_uiManager;
}

Jauntlet::UIManager* Navigation::getCavernManager() {
	return cavern.getUIManager();
}

void Navigation::setCavernPlayerManager(PlayerManager* manager) {
	cavern.setPlayerManager(manager);
}

void Navigation::spawnCavern(int type) {
	cavern.setType(type);
	cavern.display();
}

int Navigation::getDepth() {
	return _depth;
}

void Navigation::setDepth(int depth) {
	_depth = depth;
}

bool Navigation::getMoving() {
	return _destination != -1;
}

std::string Navigation::getMap() {
	std::string output = "";
	for (int y = 0; y < layerCount; y++) {
		for (int x = 0; x < layerWidth; x++) {
			output += std::to_string(_map[y][x]) + "|";
		}
	}
	return output;
}

void Navigation::setMap(std::string newMap) {
	std::string strFull = newMap;
	for (int y = 0; y < layerCount; y++) {
		for (int x = 0; x < layerWidth; x++) {
			std::string strNum = strFull.substr(0, strFull.find("|"));
			strFull = strFull.substr(strFull.find("|"), strFull.length() - 1);
			_map[y][x] = std::stoi(strNum);
		}
	}

	//reset visuals
	_uiManager.removeAllElements();
	genNav();
	updateVisibility();
}

void Navigation::setMap(int newMap[5][5]) {
	for (int y = 0; y < layerCount; y++) {
		for (int x = 0; x < layerWidth; x++) {
			_map[y][x] = newMap[y][x];
		}
	}
}