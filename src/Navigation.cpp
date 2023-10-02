#include "Navigation.h"

#include <chrono>
#include <Jauntlet/Rendering/ResourceManager.h>
#include <random>

#include <iostream>

const std::string bgTextures[] = {"Textures/NavGround1.png", "Textures/NavGround2.png", "Textures/NavGround3.png", "Textures/NavGround4.png"};
const int outcoveAmt = 4;
const int layerAmt = 3;
static std::string strX = "X";
static Jauntlet::Color white = Jauntlet::Color(0, 0, 0, 0);
static Jauntlet::Color blue = Jauntlet::Color(0, 0, 0, 0);
static Jauntlet::Color orange = Jauntlet::Color(0, 0, 0, 0);
static int seed = std::chrono::system_clock::now().time_since_epoch().count(); //temp

Navigation::Navigation() {
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

void Navigation::genNav(Jauntlet::UIManager& UIM, Jauntlet::SpriteFont* spriteFont, int* screenWidth, int* screenHeight) {
	//set stuff
	UIManager = UIM;

	//clear stuff
	_navTextures.clear();
	_points.clear();
	_positions.clear();
	_navColliders.clear();

	//read in textures
	for (int i = 0; i < 4; i++) {
		_navTextures.push_back(Jauntlet::ResourceManager::getTexture(bgTextures[i]).id);
	}

	//draw background
	Jauntlet::UIElement background = Jauntlet::UIElement(); //when jack implements a constructor for UI elements via sprites rather than text, i'll do this part.
	

	int layersHeight = _map.size(); //store the total height of the "layers"

	for (int y = 0; y < _map.size(); y++) {

		int layerSpan = _map[y].size(); //store the "span" (width) of all the points on this "layer"

		for (int x = 0; x < _map[y].size(); x++) {
			int point = _map[y][x]; //
			_positions.push_back(glm::vec2((*screenWidth / 2) + (layerSpan * -40) + (x * 80), (*screenHeight / 2) + (layersHeight * -50) + (y * 100)));
			if (point == 0) { // white X
				_points.push_back(Jauntlet::UITextElement(spriteFont, &strX, &white, &_positions[_points.size()]));
				continue;
			}
			if (point == 1) { // blue X
				_points.push_back(Jauntlet::UITextElement(spriteFont, &strX, &blue, &_positions[_points.size()]));
				continue;
			}
			if (point == 2) { // orange X
				_points.push_back(Jauntlet::UITextElement(spriteFont, &strX, &orange, &_positions[_points.size()]));
				continue;
			}
		}
	}

	for (int i = 0; i < _points.size(); i++) {
		UIM.addElement(&_points[i]);
	}

	//generate hitboxes on screenspace for hover/click interactions
	_navColliders.clear();

	for (int y = 0; y < _map.size(); y++) {
	
		int layerSpan = _map[y].size();

		for (int x = 0; x < _map[y].size(); x++) {
			_navColliders.push_back( Jauntlet::BoxCollider2D( glm::vec2(32), glm::vec2( (layerSpan * -40) + (x * 80), (layersHeight * -50) + (y * 100) ) ));
		}
	}
}

void Navigation::toggleNav() {
	_navOpen = !_navOpen;
	if (!_navOpen) {
		//make stuff visible
		white = Jauntlet::Color(0, 0, 0, 0);
		blue = Jauntlet::Color(0, 0, 0, 0);
		orange = Jauntlet::Color(0, 0, 0, 0);
	}
	else
	{
		//make stuff invis
		white = Jauntlet::Color(255, 255, 255, 255);
		blue = Jauntlet::Color(55, 55, 255, 255);
		orange = Jauntlet::Color(255, 155, 55, 255);
	}
}

bool Navigation::isNavOpen() {
	return _navOpen;
}

std::vector<Jauntlet::BoxCollider2D> Navigation::getColliders() {
	return _navColliders;
}