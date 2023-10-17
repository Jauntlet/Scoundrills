/* Purpose:
 * Main Contributer(s):
 */
#pragma once

#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UIButtonElement.h>
#include <Jauntlet/UI/UISpriteElement.h>
#include <vector>

class Navigation {
public:
	Navigation();
	~Navigation();

	void genNav(Jauntlet::UIManager& UIM, Jauntlet::InputManager* inManager); //new nav positions

	bool isNavOpen();
	void toggleNav();
	void selectNav(int id);
private:
	std::vector<GLuint> _navTextures;
	GLuint _xTure;
	bool _navOpen = false;
	int destination;
	Jauntlet::UIManager UIManager;
	std::vector<Jauntlet::UIButtonElement> _points;
	glm::vec2 _bgPos;
	std::vector<glm::vec2> _positions;
	std::vector<std::vector<int>> _map;
	Jauntlet::UISpriteElement* _background;
};