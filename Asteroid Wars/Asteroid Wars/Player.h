#ifndef PLAYER_H
#define PLAYER_H

#include "vCamera.hpp"

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

public:
	Player();
	~Player();
	void Update(sf::Vector2f backgroundPos, sf::Vector2u bGroundSize, vCamera *cam);
	bool boundary(sf::Vector2f backgroundPos, sf::Vector2u bGroundSize);
	void Move();
	void Turn(float a);

	void drawRadarIcon(sf::RenderTarget& window);
	/*Draw the bounding box*/
	void DrawBoundingBox(sf::RenderTarget& window);

#pragma region Properties
	//start gets
	sf::Vector2f getVelocity(){ return velocity; }
	sf::Sprite getSprite(){ return mSprite; }
	float getSpeed(){ return speed; }
	int getScore(){ return score; }
	float getHealth(){ return health; }
	int getLives(){ return lives; }
	//end gets
	//start sets
	void setVelocity(sf::Vector2f vel){ velocity = vel; }
	void setSpeed(float s){ speed = s; }
	sf::Vector2f getCenter();
	void setHealth(float h);
	//end sets
#pragma endregion

};

#endif