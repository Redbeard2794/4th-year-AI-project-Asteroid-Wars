#ifndef INTERCEPTORMISSILE_H
#define INTERCEPTORMISSILE_H

class InterceptorMissile : public sf::Sprite
{
private:
	sf::Texture texture;

	sf::Vector2f velocity;

	float maxSpeed;

	float speed;

	sf::Vector2f dirMove;

	float distanceToPlayer;

	int timeAlive;

	sf::Clock lifeClock;

	sf::Texture radarTexture;

	sf::Sprite radarSprite;

public:
	/*Constructor*/
	InterceptorMissile(sf::Vector2f pos);

	/*Deconstructor*/
	~InterceptorMissile();

	/*Check the euclidean distance to the player*/
	void checkRangeToPlayer(sf::Vector2f playerPos);

	/*Update*/
	void Update(sf::Vector2f targetPos);

	/*Seek the position that is passed in(i.e. match this position and face the direction being travelled in)*/
	void Seek(sf::Vector2f targetPos);

	/*Draw the missiles radar icon*/
	void drawRadarIcon(sf::RenderTarget& window);

	/*Get how long the missile has been alive for*/
	float getTimeAlive();
};

#endif
