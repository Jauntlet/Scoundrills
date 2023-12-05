/* Purpose: Display a menu that allows the player to select a "destination" in-game that progresses them towards the end goal.
 * Main Contributor(s): Christopher Kowalewski, Jack Kennedy, Xander Mooney
 */
#pragma once

#include <Jauntlet/Rendering/GLSLProgram.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UIButtonElement.h>
#include <Jauntlet/UI/UISpriteAnimatedElement.h>
#include <vector>

class Navigation {
public:
	Navigation(Jauntlet::Camera2D* camera);
	~Navigation(); //deletes hanging references

	Jauntlet::UIManager* genNav(); //new nav positions

	void update();

	bool isNavOpen(); //returns if the thing is thinging
	void toggleNav(); //toggles visibility (on/off)
	void selectNav(int id); //sets selected destination
	void updateTravel(); //Updates the "tick" of travel; Moves the points closer to the drill to simulate going to the outcove. When reaching the destination, set destination to null.
	
	Jauntlet::UIManager* getUIManager();
private:
	GLuint _navTexture = 0;
	GLuint _xTure = 0;
	GLuint _caret = 0;
	GLuint _drillIcon = 0;
	bool _navOpen = false; //visibility of menu
	int _destination = -1; //id value
	glm::vec2 _shiftPos = glm::vec2(0);
	float _progress = -1; //progress towards next destination
	//int _nextRow = 1; //the row which is selectable to navigate to
	Jauntlet::UIManager _uiManager;
	std::vector<Jauntlet::UIButtonElement> _points;
	glm::vec2 _bgPos = glm::vec2(0);
	glm::vec2 _caretPos = glm::vec2(0);
	glm::vec2 _iconPos = glm::vec2(0, -365);
	std::vector<glm::vec2> _positions;
	int _map[5][5]; //Y (count) first X (width) second

	Jauntlet::Animation _backgroundAnimation = Jauntlet::Animation(3);
	Jauntlet::UISpriteAnimatedElement _background;
	Jauntlet::UISpriteElement* _caretElement = nullptr;
	Jauntlet::UISpriteElement* _drillIconElement = nullptr;
	
	bool _caretSet = false;

	void refreshPositions(float shiftX, float shiftY);
	void updateVisibility();
};