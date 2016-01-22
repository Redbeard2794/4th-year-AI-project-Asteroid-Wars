#ifndef HUD_H
#define HUD_H

#include "stdafx.h"
#include "SFML\Graphics.hpp"

class Hud {
private:
	sf::Texture overlayTexture;
	sf::Sprite overlaySprite;

	sf::Texture healthyIndicatorTexture;
	sf::Texture damagedIndicatorTexture;
	sf::Texture nearlyDeadIndicatorTexture;
	sf::Texture aboveMaxTexture;

	sf::Sprite healthIndicatorSprite;

	sf::Texture ShieldActiveTexture;
	sf::Sprite shieldSprite;

	sf::Texture speedBoostActiveTexture;
	sf::Sprite speedSprite;

	enum damageStates
	{
		ABOVEMAX,
		NODAMAGE,
		MILDDAMAGE,
		HEAVYDAMAGE
	};
	int currentDamageState;
	sf::Clock healthFlashClock;

	sf::RectangleShape mapRect;

	sf::Font font;
	sf::Text healthIndText;
	sf::Text orientationText; 

	sf::Texture orientationBackgroundTexture;
	sf::Sprite orientationBackgroundSprite;

	sf::Texture orientationIndicatorTexture;
	sf::Sprite orientationIndicatorSprite;

	float flashTime;//for changing how fast the health indicator flashes(low health, flash faster)

	sf::Texture playerBulletTexture;
	std::vector<sf::Sprite> availableBulletSprites;

public:
	//!Hud constructor, paramter is an sf::font used for drawing text
	Hud(sf::Font f);

	//!Hud destructor
	~Hud();

	//!Hud update. params are: the player's orientation, whether the player has an active shield, whether the player has an active speed boost
	void Update(float orientationValue, bool shieldActive, bool speedActive);

	//!Hud Update the health indicator, param is the players damage state
	void UpdateHealthIndicator(int damageValue);

	//!Hud draw. draws all elements of the hud. params are a render target and the player's number of available bullets
	void Draw(sf::RenderTarget& window, int availBullets);
};

#endif
