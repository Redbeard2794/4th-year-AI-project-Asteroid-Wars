#ifndef SWARMBOID_H
#define SWARMBOID_H

#include "Obstacle.h"

class SwarmBoid : public sf::Sprite
{
private:
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Image textureImage;
	sf::Vector2f acceleration;

	enum State
	{
		SWARM,
		INTERCEPT,
		EVADE
	};
	int currentState;
	int previousState;

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
	//!Constructor. param is the starting position for the swarm boid
	SwarmBoid(sf::Vector2f startPos);

	//!Destructor
	~SwarmBoid();

	//!Update. call the appropriate behaviour based on the boids current state. params are: player's position, player's velocity, vector of all other boids and vector of obstacles
	void Update(sf::Vector2f playerPos, sf::Vector2f playerVel, std::vector<SwarmBoid*> boids, std::vector<Obstacle*> obstacles);

	//!Check how far this boid is from all other boids
	sf::Vector2f checkDistanceToOtherBoids(std::vector<SwarmBoid*> boids);

	void tendTowardsPlayer(sf::Vector2f playerPos);

	//!Check the euclidean distance to the player. param is palyer's position
	void checkRangeToPlayer(sf::Vector2f playerPos);

	//!Seek the position that is passed in(i.e. match this position and face the direction being travelled in)
	void Seek(sf::Vector2f targetPos);

	//!Plot an intercept course with a predicted position of where the target will be(uses seek function to seek this predicted position)
	void Pursue(sf::Vector2f targetPos, sf::Vector2f targetVel);

	//!swarm up! params are vector of other boids and player's position
	void Swarm(std::vector<SwarmBoid*> boids, sf::Vector2f playerPos);

	//!Apply force generates from the Swarm method
	void ApplyForce(sf::Vector2f force);

	//!Update the boid while they are in the swarm(change speed etc). param is player's position
	void UpdateInSwarm(sf::Vector2f playerPos);

	//!Flee to the position passed in
	void Flee(sf::Vector2f targetPos);

	//!Like pursue but in reverse. params are obstacle position, velocity and distance to the obstacle
	void Evade(sf::Vector2f targetPos, sf::Vector2f targetVel, float distanceToObstacle);

	//!Check whether we need to take avoiding action, params are the obstacles position and velocity
	void AvoidCollision(sf::Vector2f targetPos, sf::Vector2f targetVel);

	//!Change the current state to the passed in state. Also updates the previous state
	void ChangeStateTo(int stateToChangeTo);

	//!detect the edge of the screen
	void BoundaryDetection();

	//!Draw the boids radar icon. param is a renderTarget
	void drawRadarIcon(sf::RenderTarget& window);

	//!Draw the bounding box. param is the renderTarget
	void DrawBoundingBox(sf::RenderTarget& window);

	//!Check if the boid is still alive
	bool CheckIfAlive();

	//!set the alive status of the boid
	void SetAliveStatus(bool a);

	//! Get the texture image
	sf::Image getTextureImage() { return textureImage; }
};

#endif
