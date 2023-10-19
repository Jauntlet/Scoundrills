#include "PathRenderer.h"

PathRenderer::PathRenderer() : _textureID(0), _tilemap(nullptr) {
	// Empty
}
void PathRenderer::init(Jauntlet::TileMap* tileMap) {
	_tilemap = tileMap;
	_spriteBatch.init();
	_textureID = Jauntlet::ResourceManager::getTexture("Textures/Arrow Paths.png").id;
}

void PathRenderer::createPath(glm::vec2 start, glm::vec2 end) {
	bool reachedDest = false;
	std::vector<glm::vec2> path = Pathfinding::findPath(_tilemap, start, end, reachedDest);
	if (!reachedDest) {
		path.clear();
		clearPath();
		return;
	}

	glm::vec2 lastPos = start;
	
	path[0] = path.back();
	path.pop_back();

	_spriteBatch.begin();
	glm::ivec2 direction = glm::vec2(0), lastDir  = glm::vec2(0);
 	while (!path.empty()) {
		if (path.size() > 1) {
			direction = glm::sign(path[0] - path.back());

			if (direction != lastDir && lastDir != glm::ivec2(0)) {
				if (lastDir.y == 1 && direction.x == -1 || direction.y == -1 && lastDir.x == 1) { // from going down to going right and vice versa
					_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 6.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (lastDir.y == 1 && direction.x == 1 || direction.y == -1 && lastDir.x == -1) { // from going down to going left and vice versa
					_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 7.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (lastDir.y == -1 && direction.x == -1 || direction.y == 1 && lastDir.x == 1) { // from going up to going right and vice versa
					_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 8.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (lastDir.y == -1 && direction.x == 1 || direction.y == 1 && lastDir.x == -1) { // from going up to going left and vice versa
					_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 9.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
			}
			else if (direction.x == 1) {
				_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 4.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
			}
			else if (direction.x == -1) {
				_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 4.0f, 0, (1.0f / 18.0f), 1 }, 180, _textureID);
			}
			else if (direction.y == 1) {
				_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 5.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
			}
			else {
				_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 5.0f, 0, (1.0f / 18.0f), 1 }, 180, _textureID);
			}
		}
		else { // we are at the part of the line, we draw the arrow head.
			direction = glm::sign(path[0] - end);

			if (direction != lastDir) {
				if (direction.x == -1 && lastDir.y == 1) {
					_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 10.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (direction.x == 1 && lastDir.y == 1) {
					_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 11.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (direction.y == -1 && lastDir.x == 1) {
					_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 12.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (direction.y == -1 && lastDir.x == -1) {
					_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 13.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (direction.y == 1 && lastDir.x == 1) {
					_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 14.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (direction.y == 1 && lastDir.x == -1) {
					_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 15.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (direction.x == -1 && lastDir.y == -1) {
					_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 16.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
				else if (direction.x == 1 && lastDir.y == -1) {
					_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 17.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
				}
			}
			else if (direction.x == -1) {
				_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { 0, 0, (1.0f / 18.0f), 1 }, _textureID);
			}
			else if (direction.x == 1) {
				_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 3.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
			}
			else if (direction.y == -1) {
				_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f), 0, (1.0f / 18.0f), 1 }, _textureID);
			}
			else if (direction.y == 1) {
				_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { (1.0f / 18.0f) * 2.0f, 0, (1.0f / 18.0f), 1 }, _textureID);
			}
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