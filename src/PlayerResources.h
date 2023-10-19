/* Purpose: Store and manage all interactions with the player's resources, materials, inventory, etc. Generally anything numeric to do with the crew/drill is stored here.
 * Main Contributor(s): Christopher Kowalewski
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