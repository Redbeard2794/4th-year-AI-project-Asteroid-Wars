#ifndef _PREDATOR_HPP
#define _PREDATOR_HPP
#include "stdafx.h"
#include <vector>

class Predator : public sf::Sprite {
private:
	sf::Texture texture;
	sf::Vector2u text_size;
	sf::Vector2f acceleration;
	sf::Vector2f velocity;
	sf::Vector2f direction;
	sf::Vector2f random_point;
	float speed;

	sf::CircleShape seek_circle;
	sf::RectangleShape boundingBox;
	const float flock_raduis = 500.0f;
	const float seek_raduis = 750.0f;

	sf::Texture radarTexture;
	sf::Sprite radarSprite;
	bool alive;

	enum State{ WANDER, FLEE, SEEK };
	State current_state;
public:
	Predator();
	Predator(sf::Vector2f pos);
	~Predator();

	void loadMedia();
	void update();
	void drawRadarIcon(sf::RenderTarget& w);	//Jasons method for drawing the enemies radar sprite
	void drawDebug(sf::RenderTarget& w);	//For drawing the bounds of the sprites

	//AI
	sf::Vector2f findAlignment(std::vector<Predator*> *ships);
	sf::Vector2f findCohesion(std::vector<Predator*> *ships);
	sf::Vector2f findSeparation(std::vector<Predator*> *ships);
	void Wander();
	void Seek(sf::Vector2f targetPos);
	void Flee(sf::Vector2f awayfrom);
	/*Plot an intercept course with a predicted position of where the target will be
	(uses seek function to seek this predicted position)*/
	void Pursue(sf::Vector2f targetPos, sf::Vector2f targetVel);

	sf::Vector2f getRandomPoint(int maxX, int maxY, int minX, int minY);
	bool reachDestination();
	void checkBoundary();
	bool checkWithinBounds(sf::Vector2f point);
	float distanceTo(sf::Vector2f point);
	void applyForce(sf::Vector2f force);
	void applyAcceration();
	void fire();
	void setCenter(sf::Vector2f center);
	sf::Vector2f getCenter();
};

#endif