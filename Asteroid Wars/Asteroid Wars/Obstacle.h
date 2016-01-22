#ifndef OBSTACLE_H
#define OBSTACLE_H

class Obstacle : public sf::Sprite
{
private:
	sf::Texture texture;

	sf::Clock animationClock;
	double animationTime;
	sf::Vector2i framePosition;
	sf::Vector2i frameSize;
	sf::IntRect frame;

	sf::RectangleShape boundingBox;

	sf::Texture radarIconTexture;
	sf::Sprite radarIconSprite;

	sf::Vector2f velocity;

	sf::Image textureImage;

public:
	//!Constructor. params are: type of obstacle(should be either 1 or 2) and position to start at
	Obstacle(int type, sf::Vector2f pos);

	//!Destructor
	~Obstacle();

	//!Update. this moves through the spritesheet and also moves the obstacle
	void Update();

	//!Detects if the obstacle goes off edge of screen and resets to other side of screen
	void BoundaryDetection();

	//!Draw the bounding box
	void DrawBoundingBox(sf::RenderTarget& window);

	//!Draw the radar icon
	void DrawRadarIcon(sf::RenderTarget& window);

	//!For when the obstacle hit a factory, send the obstacle back the opposite way it was travelling in orignally with a little variation
	void ReverseVelocity();

	//!get the current velocity
	sf::Vector2f GetVelocity();

	//!Get the textureImage
	sf::Image getTextureImage() { return textureImage; }
};

#endif
