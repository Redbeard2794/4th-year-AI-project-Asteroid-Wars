#ifndef BULLET_H
#define BULLET_H

class Bullet : public sf::Sprite
{
private:
	sf::Texture texture;

	float speed;
	sf::Vector2f direction;

	sf::Clock lifeClock;

	int timeToLive;

	bool alive;

	sf::RectangleShape boundingBox;

	sf::Texture radarIconTexture;
	sf::Sprite radarIconSprite;

public:

	/*
	constructor
	params are the direction the bullet is to travel in, it's max time to live and its starting position
	*/
	Bullet(sf::Vector2f dir, int t, sf::Vector2f pos, float angle);

	/*Destructor*/
	~Bullet();

	void Update();

	void Reset(sf::Vector2f dir, int t, sf::Vector2f pos, float angle);

	/*Draw the bounding box*/
	void DrawBoundingBox(sf::RenderTarget& window);

	/*Draw the bullet on the radar*/
	void DrawRadarIcon(sf::RenderTarget& window);

	/*get and sets*/

	bool IsAlive();

	void SetAliveStatus(bool a);
};

#endif
