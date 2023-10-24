/* Purpose: Display a menu that allows the player to select a "destination" in-game that progresses them towards the end goal.
 * Main Contributor(s): Christopher Kowalewski
 */
#pragma once

#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UIButtonElement.h>
#include <Jauntlet/UI/UISpriteElement.h>
#include <vector>

class Navigation {
public:
	Navigation();
	~Navigation(); //deletes hanging references

	void genNav(Jauntlet::UIManager& UIM, Jauntlet::InputManager* inManager); //new nav positions

	bool isNavOpen(); //returns if the thing is thinging
	void toggleNav(); //toggles visibility (on/off)
	void selectNav(int id); //sets selected destination
	void updateTravel(); //Updates the "tick" of travel; Moves the points closer to the drill to simulate going to the outcove. When reaching the destination, set destination to null.
private:
	std::vector<GLuint> _navTextures;
	GLuint _xTure;
	bool _navOpen = false; //visibility of menu
	int _destination = -1; //id value
	float _progress; //0-1; 0 being just selected dest, 1 being @ dest.
	Jauntlet::UIManager* UIManager;
	std::vector<Jauntlet::UIButtonElement> _points;
	glm::vec2 _bgPos;
	std::vector<glm::vec2> _positions;
	std::vector<std::vector<int>> _map;
	Jauntlet::UISpriteElement* _background;
};