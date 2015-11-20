#ifndef SWARMBOID_H
#define SWARMBOID_H

class SwarmBoid : public sf::Drawable, public sf::Transformable
{
private:
	sf::Sprite sprite;
	sf::Texture texture;

	enum State
	{
		TEND,
		INTERCEPT
	};
	int currentState;

	float distanceToPlayer;

	sf::Vector2f velocity;
	float speed;
	sf::Vector2f dirMove;

	sf::Texture radarTexture;
	sf::Sprite radarSprite;

public: 
	SwarmBoid();
	~SwarmBoid();
	void Update(sf::Vector2f playerPos, sf::Vector2f playerVel);

	//for the different required behaviours
	void checkDistanceToOtherBoids();
	void tendTowardsPlayer(sf::Vector2f playerPos);

	/*Check the euclidean distance to the player*/
	void checkRangeToPlayer(sf::Vector2f playerPos);

	/*Seek the position that is passed in(i.e. match this position and face the direction being travelled in)*/
	void Seek(sf::Vector2f targetPos);

	/*Plot an intercept course with a predicted position of where the target will be
	(uses seek function to seek this predicted position)*/
	void Pursue(sf::Vector2f targetPos, sf::Vector2f targetVel);

	void interceptPlayer(sf::Vector2f playerPos);

	//swarm up!
	void Swarm(std::vector<SwarmBoid> boids);

	void draw(sf::RenderTarget& window, sf::RenderStates state) const;
	void draw(sf::RenderTarget& window);

	void drawRadarIcon(sf::RenderTarget& window);
};

#endif