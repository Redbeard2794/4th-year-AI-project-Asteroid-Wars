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
public:
	Hud(sf::Font f);
	~Hud();

	void Update();

	void UpdateHealthIndicator(int damageValue);

	void Draw(sf::RenderTarget& window);
};

#endif
