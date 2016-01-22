#ifndef _FACTORY_SHIP_HPP
#define _FACTORY_SHIP_HPP
#include "stdafx.h"
#include "Player.h"
#include "InterceptorMissile.h"
#include "PredatorController.hpp"
#include "Predator.hpp"
#include "Obstacle.h"
#include <vector>
using std::vector;

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

	vector<InterceptorMissile *> missle_container;

	float flock_raduis = 500.0f;
	const float evade_raduis = 600.0f;
	const float flee_raduis = 250.0f;
	const float missle_raduis = 450.0f;
	const float wander_distance = 100;

	const float flock_coh = 200.0f;
	const float flock_sep = 150.0f;
	const float flock_ali = 150.0f;

	float speed;

	int missle_count;
	int next_missle;
	float maxSpeed;
	const int max_hits = 4;
	const int max_num_missiles = 5;
	int hits_taken;

	enum State{ EVADE, WANDER, FLOCK, FLEE };
	State current_state;

	sf::Texture radarTexture;
	sf::Sprite radarSprite;
	bool alive;
	bool can_despawn;
	sf::Image textureImage;
	float health;
public:
	FactoryShip();
	FactoryShip(sf::Vector2f position);
	~FactoryShip();

	void loadMedia();

	void update(Player *p, std::vector<FactoryShip*> *ships, ExplosionController * ec, std::vector<Obstacle*> *o, PredatorController *pc);
	float distanceTo(sf::Vector2f point);
	void applyForce(sf::Vector2f force);
	void applyAcceration();
	void fireInterceptor();
	void spawnPredator();
	void CheckActiveMissles();
	void Position(sf::Vector2f pos);

	void checkBoundary();
	bool checkWithinBounds(sf::Vector2f point);
	void drawRadarIcon(sf::RenderTarget& w);	//Jasons method for drawing the enemies radar sprite
	void drawDebug(sf::RenderTarget& w);	//For drawing the bounds of the sprites
	void drawMissles(sf::RenderTarget& w);

	sf::Vector2f findAlignment(std::vector<FactoryShip*> *ships);
	sf::Vector2f findCohesion(std::vector<FactoryShip*> *ships);
	sf::Vector2f findSeparation(std::vector<FactoryShip*> *ships);

	sf::Vector2f getRandomPoint(int maxX, int maxY, int minX, int minY);
	bool reachDestination();
	//Fire Clock Data
	sf::Clock fire_clock;
	sf::Time fire_time;
	int reload;
	bool can_fire;

	//Spawn Clock Data
	sf::Clock spawn_clock;
	sf::Time spawn_time;
	int respawn;
	bool can_spawn;

	//AI
	void Wander();
	void Evade(sf::Vector2f awayfrom, float dist);	//Evade in the opposite direction of the player but keep in striking distance
	void Flee(sf::Vector2f awayfrom);	//Run in the exact opposite direction from the player
	void Damage();
	void CheckFire();
	void CheckSpawn();

	void setCenter(sf::Vector2f center);
	sf::Vector2f getCenter();
	
	//Functions involving SFML and visualisation linking
	//Pvector seek(Pvector v);
	//void run(vector <Boid> v);
	//void flock(vector <Boid> v);
	//void borders();
	//float angle(Pvector v);
	//void swarm(vector <Boid> v);
	//Pvector CalculateLJ(Boid v);
	sf::Image GetTextureImage() {return textureImage;}
	float getHealth();
	void setHealth(float h);
	bool canDespawn() { return can_despawn; }
	bool IsAlive();
	void SetAlive(bool a);
};

#endif