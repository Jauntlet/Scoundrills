#pragma once

#include <Jauntlet/Collision/Collision2D.h>
#include <Jauntlet/Rendering/SpriteFont.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UITextElement.h>
#include <vector>

class Navigation {
public:
	Navigation();

	void genNav(Jauntlet::UIManager& UIM, Jauntlet::SpriteFont* spriteFont, int* screenWidth, int* screenheight); //new nav positions

	void toggleNav();
	bool isNavOpen();
	std::vector<Jauntlet::BoxCollider2D> getColliders();
private:
	std::vector<GLuint> _navTextures;
	bool _navOpen = false;
	std::vector<Jauntlet::BoxCollider2D> _navColliders;
	std::vector<Jauntlet::UITextElement> _points;
	std::vector<glm::vec2> _positions;
};