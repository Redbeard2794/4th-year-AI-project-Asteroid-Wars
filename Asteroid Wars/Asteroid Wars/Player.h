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

	float maxSpeed;

	float health;
	int score;

	int lives = 3;
	sf::Vector2f direction;

	sf::RectangleShape boundingBox;

	

	int inactiveBullets;

	bool reusingBullets;

	//for the thruster
	sf::Texture thrusterTexture;
	sf::Sprite thrusterSprite;
	sf::IntRect thrusterTextureRect;

	bool shieldActive;
	sf::Texture shieldActiveTexture;
	bool speedBoostActive;
	sf::Texture speedBoostTexture;

	sf::Clock shieldBurnDownClock;
	sf::Clock speedBoostBurnDownClock;

	sf::Image playerTextureImage;
	std::vector<Bullet*> bullets;
public:
	//!Constructor
	Player();

	//!Destructor
	~Player();

	//!Update, handles active powerups, calls move and calculates direction. params are the backgrounds position, the background size and a pointer to the camera
	void Update(sf::Vector2f backgroundPos, sf::Vector2u bGroundSize, vCamera *cam);

	//!Check if the player has gone off the edge of the world and reset them to the other side. params are: background's position and background size
	bool boundary(sf::Vector2f backgroundPos, sf::Vector2u bGroundSize);

	//!Move. handles keyboard input
	void Move();

	//!Shoot
	void Shoot();

	//!respawn the player if they died
	void Respawn();

	//!Check the amount of currently inactive bullets
	int CheckInactiveBullets();

	//!Check if a bullet has collided with something. Param is the bounds of the object you want to check, the object to check against and the object to check againsts textureImage
	bool CheckBulletsCollision(sf::FloatRect boundsOfObjectToCheck, const sf::Sprite& spriteB, const sf::Image& imgB);

	//!Turn the player. param is the angle to turn with
	void Turn(float a);

	//!Draw the radar icon. param is a renderTarget
	void drawRadarIcon(sf::RenderTarget& window);

	//!Draw the bounding box. param is a renderTarget
	void DrawBoundingBox(sf::RenderTarget& window);

	//!Draw the player's active bullets. params are a renderTarget and whether we are in debug mode or not
	void DrawBullets(sf::RenderTarget& window, bool debugMode);

	//draw the thruster
	void DrawThruster(sf::RenderTarget& window);

	//start gets

	//!Get the current velocity
	sf::Vector2f getVelocity(){ return velocity; }

	sf::Sprite getSprite(){ return mSprite; }

	//!Get the current speed
	float getSpeed(){ return speed; }
	int getScore(){ return score; }
	//!Get the current health
	float getHealth(){ return health; }
	int getLives(){ return lives; }

	//!Get the current direction
	sf::Vector2f getDirection();

	//!Are we reusing an old bullet
	bool IsReusingBullets();
	//!Get the number of inactive bullets
	int GetInactiveBullets();

	//!Do we have a shield active?
	bool IsShieldActive() { return shieldActive; }
	//!Do we have a speed boost active?
	bool IsSpeedBoostActive() { return speedBoostActive; }

	//!Get the textureImage
	sf::Image getTextureImage() { return playerTextureImage; }

	//end gets

	//start sets
	
	//!Set the velocity
	void setVelocity(sf::Vector2f vel){ velocity = vel; }

	//!set the speed
	void setSpeed(float s){ speed = s; }

	//!Get the players center
	sf::Vector2f getCenter();

	//!Set the players health
	void setHealth(float h);

	//!Set the player to have an active shield
	void SetShieldActive(bool sa);

	//!Set the player to have an active speed boost
	void SetSpeedBoostActive(bool sba);

	//end sets
};

#endif