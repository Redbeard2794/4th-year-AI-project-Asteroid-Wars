#ifndef PLAYER_H
#define PLAYER_H

class Player : public sf::Drawable, public sf::Transformable
{
private:
	sf::Vector2f velocity;
	sf::Sprite mSprite;

	sf::Texture mTexture;

	float speed;

	float health;
	int score;

	int lives = 3;
	sf::Vector2f direction;

public:
	Player();
	~Player();
	void Update();
	void Move();
	void Turn(float a);
	void draw(sf::RenderTarget& window, sf::RenderStates state) const;
	void Player::draw(sf::RenderTarget& window);

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
	void boundary();
	void setVelocity(sf::Vector2f vel){ velocity = vel; }
	void setSpeed(float s){ speed = s; }
	//end sets
#pragma endregion

};

#endif