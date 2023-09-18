#pragma once

#include <Jauntlet/Collision/Collision2D.h>
#include <Jauntlet/Rendering/SpriteBatch.h>
#include <Jauntlet/Rendering/SpriteFont.h>
#include <vector>

class Navigation {
public:
	Navigation();

	std::vector<std::vector<int>> genNav(); //new nav positions

	void drawNav(std::vector<std::vector<int>>& navPoints, Jauntlet::SpriteFont& font, Jauntlet::SpriteBatch& spriteBatch);

	void toggleNav();
	bool isNavOpen();
	std::vector<Jauntlet::BoxCollider2D> getColliders();
private:
	bool _navOpen = false;
	std::vector<Jauntlet::BoxCollider2D> _navColliders;
};