#ifndef _FACTORY_SHIP_HPP
#define _FACTORY_SHIP_HPP
#include "stdafx.h"

class FactoryShip : public sf::Sprite {
private:
	sf::Texture texture;
	sf::Vector2u text_size;
	sf::Vector2f acceleration;
	sf::Vector2f velocity;
	sf::Vector2f dirMove;
	float speed;

	float maxSpeed;
	const int max_hits = 4;
	const int max_num_missiles = 5;
	int hits_taken;

	enum State{ EVADE, WANDER, FLOCK };
	State current_state;

	sf::Texture radarTexture;
	sf::Sprite radarSprite;
public:
	FactoryShip();
	~FactoryShip();

	void update();
	void applyForce(sf::Vector2f force);
	void fireInterceptor();
	void Position(sf::Vector2f pos);

	void Evade(sf::Vector2f awayfrom);	//The Enemy can flee from the target passed in
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