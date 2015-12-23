#ifndef _FACTORY_SHIP_HPP
#define _FACTORY_SHIP_HPP
#include "stdafx.h"
#include "Player.h"

class FactoryShip : public sf::Sprite {
private:
	sf::Texture texture;
	sf::Vector2u text_size;
	sf::Vector2f acceleration;
	sf::Vector2f velocity;
	sf::Vector2f direction;
	sf::Vector2f destination;
	sf::Vector2f random_point;

	sf::CircleShape evade_circle;
	sf::CircleShape missle_circle;
	sf::RectangleShape boundingBox;

	const float evade_raduis = 500.0f;
	const float missle_raduis = 250.0f;
	const float wander_distance = 100;

	float speed;

	float maxSpeed;
	const int max_hits = 4;
	const int max_num_missiles = 5;
	int hits_taken;

	enum State{ EVADE, WANDER, FLOCK, FLEE };
	State current_state;

	sf::Texture radarTexture;
	sf::Sprite radarSprite;
public:
	FactoryShip();
	~FactoryShip();

	void loadMedia();

	void update(Player *p);
	float distanceTo(sf::Vector2f point);
	void applyForce(sf::Vector2f force);
	void fireInterceptor();
	void Position(sf::Vector2f pos);

	void checkBoundary();
	bool checkWithinBounds(sf::Vector2f point);
	void drawRadarIcon(sf::RenderTarget& w);	//Jasons method for drawing the enemies radar sprite
	void drawDebug(sf::RenderTarget& w);	//For drawing the bounds of the sprites

	sf::Vector2f getRandomPoint(int maxX, int maxY, int minX, int minY);
	bool reachDestination();

	//AI
	void Wander();
	void Evade(sf::Vector2f awayfrom, float dist);	//Evade in the opposite direction of the player but keep in striking distance
	void Flee(sf::Vector2f awayfrom);	//Run in the exact opposite direction from the player

	void setCenter(sf::Vector2f center);
	sf::Vector2f getCenter();
	
	// Three Laws that boids follow
	//Pvector Separation(vector<Boid> Boids);
	//Pvector Alignment(vector<Boid> Boids);
	//Pvector Cohesion(vector<Boid> Boids);
	
	//Functions involving SFML and visualisation linking
	//Pvector seek(Pvector v);
	//void run(vector <Boid> v);
	//void flock(vector <Boid> v);
	//void borders();
	//float angle(Pvector v);
	//void swarm(vector <Boid> v);
	//Pvector CalculateLJ(Boid v);
};

#endif