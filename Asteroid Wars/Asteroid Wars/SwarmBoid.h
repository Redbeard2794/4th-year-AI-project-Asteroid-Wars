#ifndef SWARMBOID_H
#define SWARMBOID_H

class SwarmBoid : public sf::Sprite
{
private:
	sf::Sprite sprite;
	sf::Texture texture;

	sf::Vector2f acceleration;

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

	bool isLeader;

	sf::RectangleShape boundingBox;

	bool alive;
public: 
	SwarmBoid();
	~SwarmBoid();
	void Update(sf::Vector2f playerPos, sf::Vector2f playerVel, std::vector<SwarmBoid*> boids);

	//for the different required behaviours
	sf::Vector2f checkDistanceToOtherBoids(std::vector<SwarmBoid*> boids);
	void tendTowardsPlayer(sf::Vector2f playerPos);

	/*Check the euclidean distance to the player*/
	void checkRangeToPlayer(sf::Vector2f playerPos);

	/*Seek the position that is passed in(i.e. match this position and face the direction being travelled in)*/
	void Seek(sf::Vector2f targetPos);

	/*Plot an intercept course with a predicted position of where the target will be
	(uses seek function to seek this predicted position)*/
	void Pursue(sf::Vector2f targetPos, sf::Vector2f targetVel);

	//swarm up!
	void Swarm(std::vector<SwarmBoid*> boids, sf::Vector2f playerPos);

	void ApplyForce(sf::Vector2f force);

	void UpdateInSwarm(sf::Vector2f playerPos);

	//detect the edge of the screen
	void BoundaryDetection();

	void drawRadarIcon(sf::RenderTarget& window);

	/*Draw the bounding box*/
	void DrawBoundingBox(sf::RenderTarget& window);

	/*Check if the boid is still alive*/
	bool CheckIfAlive();

	/*set the alive status of the boid*/
	void SetAliveStatus(bool a);
};

#endif
