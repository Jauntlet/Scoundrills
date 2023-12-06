#include "PathRenderer.h"
#include <Jauntlet/JMath.h>

#include <iostream>
PathRenderer::PathRenderer(DrillManager* drill, PlayerManager* playerManager) : 
	_textureID(Jauntlet::ResourceManager::getTexture("Textures/Arrow Paths.png").id), 
	_drill(drill),
	_players(playerManager) {
	// Empty
}

void PathRenderer::createPath(glm::vec2 start, glm::vec2 end) {
	// check if the destination provided is valid
	if (!_drill->isValidDestination(end, _players)) {
		PlayerStation* station;
		if ((station = _drill->checkHoveringStation(end)) != nullptr && !station->isOccupied()) {
			end = _drill->drillWalls.RoundWorldPos(station->getAnchorPoint());
		}
		else {
			clearPath();
			return;
		}
	}
	// find the path
	_path = Pathfinding::findPath(*_drill, *_players, start, end);

	glm::vec2 lastPos = start;
	_path.pop_back();
	// _path[0] = _path.back();
	// _path.pop_back();
	
	if (_path.size() < 2) {
		// path is too small to go anywhere
		return;
	}

	// path processing
	// _path.pop_back();
	// _path[0] = _path.back();
	// _path.pop_back();

	_spriteBatch.begin();
	glm::ivec2 direction = glm::ivec2(0), lastDir  = glm::sign(_path[2] - _path[1]);
 	for (int i = 1; i < _path.size(); ++i) {
		if (i != 1) {
			direction = glm::sign(_path[i] - _path[i+1]);

			if (direction != lastDir && lastDir != glm::ivec2(0)) {
				if (lastDir.x == 1) {
					if (direction.y == 1) _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 8.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
					else _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 6.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (lastDir.x == -1) {
					if (direction.y == 1) _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 9.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
					else _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 7.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (lastDir.y == 1) {
					if (direction.x == 1) _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 7.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
					else _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 6.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (lastDir.y == -1) {
					if (direction.x == 1) _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 9.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
					else _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 8.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
			}
			else if (direction.x == 1) _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 4.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
			else if (direction.x == -1) _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 4.0f, 0, (1.0f / 18.0f), 1 }, M_PI /*180 degrees in radians*/, _textureID);
			else if (direction.y == 1) _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 5.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
			else _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 5.0f, 0, (1.0f / 18.0f), 1 }, M_PI /*180 degrees in radians*/, _textureID);
		}
		else { // we are at the part of the line, we draw the arrow head.
			direction = glm::sign(_path[1] - _path[0]);

			if (direction != lastDir) {
				if (direction.x == 1) {
					if (lastDir.y == 1) _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 11.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
					else _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 17.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (direction.x == -1) {
					if (lastDir.y == 1) _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 10.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
					else _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 16.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (direction.y == 1) {
					if (lastDir.x == 1) _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 14.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
					else _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 15.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (lastDir.x == 1) _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 12.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				else _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 13.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
			}
			else if (direction.x == -1) _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { 0, 0, (1.0f / 18.0f), 1 }, _textureID);
			else if (direction.x == 1) _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 3.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
			else if (direction.y == -1) _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f), 0, (1.0f / 18.0f), 1 }, _textureID);
			else if (direction.y == 1) _spriteBatch.draw({ _path[i].x, _path[i].y, 64, 64 }, { (1.0f / 18.0f) * 2.0f, 0, (1.0f / 18.0f), 1 }, _textureID);

			// correct the direction for the next line piece
			direction = glm::sign(_path[i] - _path[i+1]);
		}
		lastDir = direction;
		//_path[i] = _path.back(); 
		//_path.pop_back();
	}
	_spriteBatch.end();
}
void PathRenderer::clearPath() {
	_path.clear();
	_spriteBatch.begin();
	_spriteBatch.end();
}
void PathRenderer::drawPath() {
	_spriteBatch.render();
}

std::vector<glm::vec2> PathRenderer::getPath() {
	return _path;
}