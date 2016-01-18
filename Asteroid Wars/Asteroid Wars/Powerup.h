#ifndef POWERUP_H
#define POWERUP_H

class Powerup : sf::Sprite
{
private:

	enum PowerupType
	{
		HEALTH,
		SHIELD,
		SPEED
	};

	int type;

	sf::Texture texture;

	sf::Texture radarTexture;

	sf::Sprite radarSprite;

	sf::RectangleShape boundingBox;
public:
	Powerup();

	~Powerup();

	void Update();

	void DrawBoundingBox(sf::RenderTarget & window);

	void DrawRadarIcon(sf::RenderTarget& window);

	int getType();
};

#endif
