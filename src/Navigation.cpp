#include "Navigation.h"

#include <chrono>
#include <Jauntlet/Rendering/ResourceManager.h>
#include <random>

const std::string bgTextures[] = {"Textures/NavGround1.png", "Textures/NavGround2.png", "Textures.NavGround3.png", "Textures.NavGround4.png"};

const int outcoveAmt = 4;
const int layerAmt = 3;
static int seed = std::chrono::system_clock::now().time_since_epoch().count(); //temp

Navigation::Navigation() {
	// Empty
}

std::vector<std::vector<int>> Navigation::genNav() {
	std::vector<std::vector<int>> map = std::vector<std::vector<int>>();
	std::mt19937 r = std::mt19937(seed);

	for (int y = 0; y < layerAmt; y++) {
		std::vector<int> layer = std::vector<int>();

		int temp = outcoveAmt + (r() % 2 - 1); //outcoveAmt - 1, outcoveAmt, outcoveAmt + 1
		for (int x = 0; x < temp; x++) {
			layer.push_back(r() % 3); //0, 1, 2
		}

		map.push_back(layer);
	}

	//generate hitboxes on screenspace for hover/click interactions
	_navColliders.clear();

	int layersHeight = map.size();

	for (int y = 0; y < map.size(); y++) {
	
		int layerSpan = map[y].size();

		for (int x = 0; x < map[y].size(); x++) {
			_navColliders.push_back( Jauntlet::BoxCollider2D( glm::vec2(32), glm::vec2( (layerSpan * -40) + (x * 80), (layersHeight * -50) + (y * 100) ) ));
		}
	}

	return map;
}

void Navigation::drawNav(std::vector<std::vector<int>>& navPoints, Jauntlet::SpriteFont& font, Jauntlet::SpriteBatch& spriteBatch) {
	//If the navigation menu isn't meant to be open, simply don't render it.
	if (!_navOpen) {
		return;
	}

	//Render Background
	/*int frame = std::mt19937(seed)() % 16;
	switch (frame) {
	case 0: //render one of the nonblank backgrounds
		spriteBatch.draw(glm::vec4(-320, -320, 640, 640), glm::vec4(0, 0, 1, 1), Jauntlet::ResourceManager::getTexture(bgTextures[1]).id, 1, Jauntlet::Color(255, 255, 255, 255));
		break;
	case 1: //render one of the nonblank backgrounds
		spriteBatch.draw(glm::vec4(-320, -320, 640, 640), glm::vec4(0, 0, 1, 1), Jauntlet::ResourceManager::getTexture(bgTextures[2]).id, 1, Jauntlet::Color(255, 255, 255, 255));
		break;
	case 2: //render one of the nonblank backgrounds
		spriteBatch.draw(glm::vec4(-320, -320, 640, 640), glm::vec4(0, 0, 1, 1), Jauntlet::ResourceManager::getTexture(bgTextures[3]).id, 1, Jauntlet::Color(255, 255, 255, 255));
		break;
	default: //render "blank" background
		spriteBatch.draw(glm::vec4(-320, -320, 640, 640), glm::vec4(0, 0, 1, 1), Jauntlet::ResourceManager::getTexture(bgTextures[0]).id, 1, Jauntlet::Color(255, 255, 255, 255));
		break;
	}*/

	//only one background variation for testing
	spriteBatch.draw(glm::vec4(-320, -320, 640, 640), glm::vec4(0, 0, 1, 1), Jauntlet::ResourceManager::getTexture(bgTextures[0]).id, 1, Jauntlet::Color(255, 255, 255, 255));

	//Render navPoints
	int layersHeight = navPoints.size(); //store the total height of the "layers"

	for (int y = 0; y < navPoints.size(); y++) {

		int layerSpan = navPoints[y].size(); //store the "span" (width) of all the points on this "layer"
		
		for (int x = 0; x < navPoints[y].size(); x++) {
			int point = navPoints[y][x]; //

			if (point == 0) { // white X
				font.draw(spriteBatch, "X", glm::vec2((layerSpan * -40) + (x * 80), (layersHeight * -50) + (y * 100)), glm::vec2(.2), 0, Jauntlet::Color(255, 255, 255, 255));
				continue;
			}
			if (point == 1) { // blue X
				font.draw(spriteBatch, "X", glm::vec2((layerSpan * -40) + (x * 80), (layersHeight * -50) + (y * 100)), glm::vec2(.2), 0, Jauntlet::Color(0, 0, 255, 255));
				continue;
			}
			if (point == 2) { // orange X
				font.draw(spriteBatch, "X", glm::vec2((layerSpan * -40) + (x * 80), (layersHeight * -50) + (y * 100)), glm::vec2(.2), 0, Jauntlet::Color(255, 165, 0, 255));
				continue;
			}
		}
	}
}

void Navigation::toggleNav() {
	_navOpen = !_navOpen;
}

bool Navigation::isNavOpen() {
	return _navOpen;
}

std::vector<Jauntlet::BoxCollider2D> Navigation::getColliders() {
	return _navColliders;
}