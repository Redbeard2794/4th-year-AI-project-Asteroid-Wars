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
	Obstacle(int type, sf::Vector2f pos);

	~Obstacle();

	void Update();

	void BoundaryDetection();

	/*Draw the bounding box*/
	void DrawBoundingBox(sf::RenderTarget& window);

	void DrawRadarIcon(sf::RenderTarget& window);

	/*For when the obstacle hit a factory, send the obstacle back the opposite way it was travelling in orignally*/
	void ReverseVelocity();

	sf::Vector2f GetVelocity();

	sf::Image getTextureImage() { return textureImage; }
};

#endif
