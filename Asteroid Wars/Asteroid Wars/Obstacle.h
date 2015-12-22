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

public:
	Obstacle(int type, sf::Vector2f pos);

	~Obstacle();

	void Update();

	/*Draw the bounding box*/
	void DrawBoundingBox(sf::RenderTarget& window);
};

#endif
