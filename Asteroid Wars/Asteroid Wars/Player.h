#ifndef PLAYER_H
#define PLAYER_H

#include "vCamera.hpp"
#include "Bullet.h"

class Player : public sf::Sprite
{
private:
	sf::Vector2f velocity;
	sf::Sprite mSprite;

	sf::Texture mTexture;

	sf::Texture radarTexture;
	sf::Sprite radarSprite;

	float speed;

	float health;
	int score;

	int lives = 3;
	sf::Vector2f direction;

	sf::RectangleShape boundingBox;

	std::vector<Bullet*> bullets;

	int inactiveBullets;

	bool reusingBullets;

	//for the thruster
	sf::Texture thrusterTexture;
	sf::Sprite thrusterSprite;
	sf::IntRect thrusterTextureRect;

public:
	Player();
	~Player();
	void Update(sf::Vector2f backgroundPos, sf::Vector2u bGroundSize, vCamera *cam);
	bool boundary(sf::Vector2f backgroundPos, sf::Vector2u bGroundSize);
	void Move();

	void Shoot();

	//respawn the player if they died
	void Respawn();

	/*Check the amount of currently inactive bullets*/
	int CheckInactiveBullets();

	/*
	Check if a bullet has collided with something. 
	Param is the bounds of the object you want to check
	*/
	bool CheckBulletsCollision(sf::FloatRect boundsOfObjectToCheck);

	void Turn(float a);

	void drawRadarIcon(sf::RenderTarget& window);

	/*Draw the bounding box*/
	void DrawBoundingBox(sf::RenderTarget& window);

	void DrawBullets(sf::RenderTarget& window, bool debugMode);

	//draw the thruster
	void DrawThruster(sf::RenderTarget& window);

	//start gets
	sf::Vector2f getVelocity(){ return velocity; }
	sf::Sprite getSprite(){ return mSprite; }
	float getSpeed(){ return speed; }
	int getScore(){ return score; }
	float getHealth(){ return health; }
	int getLives(){ return lives; }

	sf::Vector2f getDirection();

	bool IsReusingBullets();
	int GetInactiveBullets();

	//end gets

	//start sets
	void setVelocity(sf::Vector2f vel){ velocity = vel; }
	void setSpeed(float s){ speed = s; }
	sf::Vector2f getCenter();
	void setHealth(float h);
	//end sets

};

#endif