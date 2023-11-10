#include "PathRenderer.h"
#include <Jauntlet/JMath.h>
#include "../players/PlayerManager.h"

#include <iostream>
PathRenderer::PathRenderer(Jauntlet::TileMap* tileMap, PlayerManager* players) : 
	_textureID(Jauntlet::ResourceManager::getTexture("Textures/Arrow Paths.png").id), 
	_players(players),
	_tilemap(tileMap) {
	// Empty
}

void PathRenderer::createPath(glm::vec2 start, glm::vec2 end) {
	if (!_players->isValidDestination(end)) {
		clearPath();
		return;
	}
	std::vector<glm::vec2> path = Pathfinding::findPath(_tilemap, *_players, start, end);

	glm::vec2 lastPos = start;
	
	if (path.size() < 3) {
		// path is too small to go anywhere
		return;
	}

	path.pop_back();
	//path.erase(path.begin());
	path[0] = path.back();
	path.pop_back();

	_spriteBatch.begin();
	glm::ivec2 direction = glm::ivec2(0), lastDir  = glm::sign(start - path[0]);
 	while (!path.empty()) {
		if (path.size() > 1) {
			direction = glm::sign(path[0] - path.back());

			if (direction != lastDir && lastDir != glm::ivec2(0)) {
				if (lastDir.x == 1) {
					if (direction.y == 1) _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 8.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
					else _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 6.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (lastDir.x == -1) {
					if (direction.y == 1) _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 9.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
					else _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 7.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (lastDir.y == 1) {
					if (direction.x == 1) _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 7.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
					else _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 6.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (lastDir.y == -1) {
					if (direction.x == 1) _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 9.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
					else _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 8.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
			}
			else if (direction.x == 1) _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 4.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
			else if (direction.x == -1) _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 4.0f, 0, (1.0f / 18.0f), 1 }, M_PI /*180 degrees in radians*/, _textureID);
			else if (direction.y == 1) _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 5.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
			else _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 5.0f, 0, (1.0f / 18.0f), 1 }, M_PI /*180 degrees in radians*/, _textureID);
		}
		else { // we are at the part of the line, we draw the arrow head.
			direction = glm::sign(path[0] - end);

			if (direction != lastDir) {
				if (direction.x == 1) {
					if (lastDir.y == 1) _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 11.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
					else _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 17.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (direction.x == -1) {
					if (lastDir.y == 1) _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 10.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
					else _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 16.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (direction.y == 1) {
					if (lastDir.x == 1) _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 14.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
					else _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 15.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (lastDir.x == 1) _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 12.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				else _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 13.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
			}
			else if (direction.x == -1) _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { 0, 0, (1.0f / 18.0f), 1 }, _textureID);
			else if (direction.x == 1) _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 3.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
			else if (direction.y == -1) _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f), 0, (1.0f / 18.0f), 1 }, _textureID);
			else if (direction.y == 1) _spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 2.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
		}
		lastDir = direction;
		path[0] = path.back(); 
		path.pop_back();
	}
	_spriteBatch.end();
}
void PathRenderer::clearPath() {
	_spriteBatch.begin();
	_spriteBatch.end();
}
void PathRenderer::drawPath() {
	_spriteBatch.render();
}