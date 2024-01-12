/* Purpose: Display a menu that allows the player to select a "destination" in-game that progresses them towards the end goal.
 * Main Contributor(s): Christopher Kowalewski, Jack Kennedy, Xander Mooney
 */
#pragma once

#include <Jauntlet/Rendering/GLSLProgram.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UIButtonElement.h>
#include <Jauntlet/UI/UISpriteAnimatedElement.h>
#include <random>
#include <vector>
#include "../drill/PlayerResources.h"
#include "Cavern.h"

class Navigation {
public:
	Navigation(Jauntlet::Camera2D* camera, PlayerResources* resourceManager);
	~Navigation(); //deletes hanging references

	Jauntlet::UIManager* genNav(); //new nav positions

	void update();

	// returns if the nav is open
	bool isNavOpen();
	// toggles visibility (on/off)
	void toggleNav();
	// sets selected destination
	void selectNav(int id, glm::ivec2 xy);
	// Updates the "tick" of travel; Moves the points closer to the drill to simulate going to the outcove. When reaching the destination, set destination to null.
	void updateTravel();
	// brings up the menu for a cavern
	void spawnCavern(int type);
	// retrieve gameplay value -- a display of how many "points" the player reached
	int getDepth();
	// set new depth value
	void setDepth(int depth);
	//is the drill moving?
	bool getMoving();
	//gets a map for saving (as a comma-separated numbers)
	std::string getMap();
	//sets a map from predefined data
	void setMap(std::string newMap);
	void setMap(int newMap[5][5]);

	Jauntlet::UIManager* getUIManager();
	Jauntlet::UIManager* getCavernManager();

	void setCavernPlayerManager(PlayerManager* manager);

	// event handler
	Cavern cavern;
private:
	// random -- uses one consistent seed (generated based on system time on class construction)
	std::mt19937 _random;

	// textures
	GLuint _navTexture = 0;
	GLuint _xTure = 0;
	GLuint _waTure = 0;
	GLuint _approaTure = 0;
	GLuint _dangTure = 0;
	GLuint _coppTure = 0;
	GLuint _caret = 0;
	GLuint _drillIcon = 0;

	// boolean toggles
	bool _navOpen = false; //visibility of menu
	bool _caretSet = false;

	// mapping-related
	int _map[5][5]; //Y (count) first X (width) second
	std::vector<int> _mappedCaverns;
	std::vector<glm::vec2> _positions;

	// various internal map-position control variables (like indexes on an array)
	int _destination = -1; //target position's id value
	int _columnOver = 2; //the column the drill icon rests over currently
	int _drillRow = -1; //the drill's current row (-1 is above row 0 on the map)
	int _rowsTravelled = 0; //y movement
	int _columnsTravelled = 0; //x movement

	// these variables specifically relate to progress towards the next selected destination
	float _progress = 0.0f; // progress towards next destination
	float _speed = 1; // use the constant baseSpeed in Navigation.cpp to adjust speed. Speed determines the seconds it takes to "travel" any distance in the navigation menu
	int _depth = 0; // higher number means "deeper" underground. May change later.

	//positions of elements on screen
	glm::vec2 _bgPos = glm::vec2(0);
	glm::vec2 _caretPos = glm::vec2(0);
	glm::vec2 _iconPos = glm::vec2(0, -365);
	glm::vec2 _shiftPos = glm::vec2(0); // position determining how to move nav points when a destination is selected

	Jauntlet::UIManager _uiManager;

	// UI Elements -- individual
	Jauntlet::Animation _backgroundAnimation = Jauntlet::Animation(3);
	Jauntlet::UISpriteAnimatedElement _background;
	Jauntlet::UISpriteElement* _caretElement = nullptr;
	Jauntlet::UISpriteElement* _drillIconElement = nullptr;

	// UI Elements -- vectors
	std::vector<Jauntlet::UIButtonElement> _points;

	//private methods
	void refreshPositions(float shiftX, float shiftY); // moves every "point" on the nav by shiftX, shiftY units respectively
	void recycleMap(int rows); // uses the points we see on the map to determine how many rows/columns to remove from the old map, basically making space for new points
	void updateVisibility(); // makes any points off the edge of the menu invisible, otherwise they are all visible. If visibility is toggled off, no points are made visible.
};