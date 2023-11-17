/* Purpose: Display a menu that allows the player to select a "destination" in-game that progresses them towards the end goal.
 * Main Contributor(s): Christopher Kowalewski, Jack Kennedy, Xander Mooney
 */
#pragma once

#include <Jauntlet/Rendering/GLSLProgram.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UIButtonElement.h>
#include <Jauntlet/UI/UISpriteElement.h>
#include <vector>

class Navigation {
public:
	Navigation(Jauntlet::Camera2D* camera);
	~Navigation(); //deletes hanging references

	Jauntlet::UIManager* genNav(); //new nav positions

	bool isNavOpen(); //returns if the thing is thinging
	void toggleNav(); //toggles visibility (on/off)
	void selectNav(int id); //sets selected destination
	void updateTravel(); //Updates the "tick" of travel; Moves the points closer to the drill to simulate going to the outcove. When reaching the destination, set destination to null.
private:
	std::vector<GLuint> _navTextures;
	GLuint _xTure = 0;
	GLuint _caret = 0;
	bool _navOpen = false; //visibility of menu
	int _destination = -1; //id value
	float _progress = -1; //0-1; 0 being just selected dest, 1 being @ dest.
	Jauntlet::UIManager _uiManager;
	std::vector<Jauntlet::UIButtonElement> _points;
	glm::vec2 _bgPos = glm::vec2(0);
	glm::vec2 _caretPos = glm::vec2(0);
	std::vector<glm::vec2> _positions;
	int _map[5][5]; //Y (count) first X (width) second
	Jauntlet::UISpriteElement* _background = nullptr;
	Jauntlet::UISpriteElement* _caretElement = nullptr;
	bool _caretSet = false;

	void refreshPositions(float shiftX, float shiftY);
};