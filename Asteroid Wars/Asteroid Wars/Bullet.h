#ifndef BULLET_H
#define BULLET_H

class Bullet : public sf::Sprite
{
private:
	sf::Texture texture;
	sf::Image textureImage;
	float speed;
	sf::Vector2f direction;

	sf::Clock lifeClock;

	int timeToLive;

	bool alive;

	sf::RectangleShape boundingBox;

	sf::Texture radarIconTexture;
	sf::Sprite radarIconSprite;
	sf::Vector2f default_position;
public:

	//!default constructor
	Bullet();

	//!constructor. params are the direction the bullet is to travel in, it's max time to live and its starting position
	Bullet(sf::Vector2f dir, int t, sf::Vector2f pos, float angle);

	//!Destructor
	~Bullet();

	//!Update. mov the bullet in the direction it was fired
	void Update();

	//!Reset the bullet. params are: a direction for the bullet to travel in, time to live, position to start at, and angle to set its rotation to
	void Reset(sf::Vector2f dir, int t, sf::Vector2f pos, float angle);

	//!Draw the bounding box. param is a renderTarget
	void DrawBoundingBox(sf::RenderTarget& window);

	//!Draw the bullet on the radar. param is a renderTarget
	void DrawRadarIcon(sf::RenderTarget& window);

	/*get and sets*/

	//!Is the bullet alive?
	bool IsAlive();

	//!Set the alive status of the bullet
	void SetAliveStatus(bool a);

	//!Get the textureImage
	sf::Image GetTextureImage() { return textureImage; }
};

#endif
