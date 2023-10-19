/* Purpose:
 * Main Contributer(s):
 */
#pragma once

class PlayerResources {
public:
	PlayerResources();
	PlayerResources(float Water, unsigned int Food, unsigned int Copper, unsigned int Parts, float Heat); //with starting resources

	unsigned int food = 0, copper = 0, parts = 0;
	float water = 0.0f, heat = 0.0f;
private:
};