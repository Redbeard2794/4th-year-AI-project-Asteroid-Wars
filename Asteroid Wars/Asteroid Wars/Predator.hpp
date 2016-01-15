#ifndef _PREDATOR_HPP
#define _PREDATOR_HPP
#include "stdafx.h"
#include <vector>
#include "Player.h"

class Predator : public sf::Sprite {
private:
	sf::Texture texture;
	sf::Vector2u text_size;
	sf::Vector2f acceleration;
	sf::Vector2f velocity;
	sf::Vector2f direction;
	sf::Vector2f random_point;
	sf::Vector2f destination;
	float speed;

	sf::Clock fire_Clock;
	int reload_time;
	std::vector<Bullet*> bullets;
	int next_bullet;
	int bullet_count;
	const float missle_raduis = 300.0f;

	float maxSpeed;
	float maxForce;

	sf::CircleShape seek_circle;
	sf::RectangleShape boundingBox;
	const float flock_raduis = 500.0f;
	const float flock_coh = 165.0f;
	const float flock_sep = 100.0f;
	const float flock_ali = 150.0f;
	const float seek_raduis = 750.0f;
	const float wander_distance = 100;

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
	void update(std::vector<Predator*>* ships, Player *p);
	void drawRadarIcon(sf::RenderTarget& w);	//Jasons method for drawing the enemies radar sprite
	void drawDebug(sf::RenderTarget& w);	//For drawing the bounds of the sprites

	//AI
	void AI();
	sf::Vector2f findAlignment(std::vector<Predator*> *ships);
	sf::Vector2f findCohesion(std::vector<Predator*> *ships);
	sf::Vector2f findSeparation(std::vector<Predator*> *ships);
	void Wander();
	sf::Vector2f Seek(sf::Vector2f targetPos);
	void Flock(std::vector<Predator*> *ships);
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