#ifndef HUD_H
#define HUD_H

class Hud
{
private:
	sf::Texture overlayTexture;
	sf::Sprite overlaySprite;

	sf::Texture healthyIndicatorTexture;
	sf::Texture damagedIndicatorTexture;
	sf::Texture nearlyDeadIndicatorTexture;

	sf::Sprite healthIndicatorSprite;

	enum damageStates
	{
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
	Hud(sf::Font f);
	~Hud();

	void Update(float orientationValue);

	void UpdateHealthIndicator(int damageValue);

	void Draw(sf::RenderTarget& window, int availBullets);
};

#endif
