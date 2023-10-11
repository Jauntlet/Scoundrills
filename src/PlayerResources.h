/* Purpose:
 * Main Contributer(s):
 */
#pragma once

class PlayerResources {
public:
	PlayerResources();
	PlayerResources(unsigned int water, unsigned int food, unsigned int copper, unsigned int parts, unsigned int heat); //with starting resources

	//getters
	unsigned int getWater();
	unsigned int getFood();
	unsigned int getCopper();
	unsigned int getParts();
	unsigned int getHeat();

	// 'w' = Water, 'f' = Food, 'c' = Copper, 'p' = Parts, 'h' = Heat
	unsigned int get(char code); //custom getter

	//setters
	void setWater(unsigned int value);
	void setFood(unsigned int value);
	void setCopper(unsigned int value);
	void setParts(unsigned int value);
	void setHeat(unsigned int value);

	// 'w' = Water, 'f' = Food, 'c' = Copper, 'p' = Parts, 'h' = Heat
	void set(char code, unsigned int value);//custom setter

private:
	unsigned int _water;
	unsigned int _food;
	unsigned int _copper;
	unsigned int _parts;
	unsigned int _heat;
};