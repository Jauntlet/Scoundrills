#include "PathRenderer.h"
PathRenderer::PathRenderer() : _textureID(0) {
	// Empty
}
void PathRenderer::init(Jauntlet::TileMap* tileMap) {
	_tilemap = tileMap;
	_spriteBatch.init();
	_textureID = Jauntlet::ResourceManager::getTexture("Textures/Arrow Paths.png").id;
}

void PathRenderer::drawPath(glm::vec2 start, glm::vec2 end) {
	std::vector<glm::vec2> path = Pathfinding::findPath(_tilemap, start, end);
	glm::vec2 lastPos = start;

	// get rid of start element
	path[0] = path.back();
	path.pop_back();

	_spriteBatch.begin();
	while (!path.empty()) {
		glm::vec2 direction = glm::sign(glm::vec2(path[0].x - start.x, path[0].y - start.y));

		if (direction.x != 0) {
			_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { 0.5, 0, 0.6, 1 }, _textureID);
		}
		else { // direction is in the Y
			_spriteBatch.draw({ path[0].x, path[0].y, 64, 64 }, { 0.6, 0, 0.7, 1 }, _textureID);
		}
	}
	_spriteBatch.endAndRender();
}