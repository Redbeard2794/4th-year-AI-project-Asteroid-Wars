#include "stdafx.h"
#include "SwarmBoid.h"

SwarmBoid::SwarmBoid()
{
	//load the correct texture or load the debug texture if something is wrong
	if (texture.loadFromFile("Assets/Sprites/Enemies/swarmBoid/SwarmBoid.png")) {}
	else texture.loadFromFile("Assets/Debug.png");	//if it fails load placeholder
	sprite.setOrigin(sf::Vector2f(texture.getSize().x / 2, texture.getSize().y / 2));
	sprite.setTexture(texture);
	setPosition(5800, 350);//for testing it out only

	if (radarTexture.loadFromFile("Assets/Sprites/Enemies/swarmBoid/swarmBoidRadarIcon.png")) {}
	else radarTexture.loadFromFile("Assets/Debug.png");
	radarSprite.setOrigin(sf::Vector2f(radarTexture.getSize().x / 2, radarTexture.getSize().y / 2));
	radarSprite.setTexture(radarTexture);

	currentState = TEND;
	speed = 0.4f;
	velocity = sf::Vector2f(1, 1);
}

SwarmBoid::~SwarmBoid() {}

void SwarmBoid::Update(sf::Vector2f playerPos, sf::Vector2f playerVel)
{
	checkRangeToPlayer(playerPos);

	if (currentState == TEND)
	{
		tendTowardsPlayer(playerPos);
	}

	else if (currentState == INTERCEPT)
	{
		//interceptPlayer(playerPos);
		Pursue(playerPos, playerVel);
	}

	//std::cout << "Current state: " << currentState << std::endl;
}

void SwarmBoid::checkDistanceToOtherBoids()
{

}

void SwarmBoid::tendTowardsPlayer(sf::Vector2f playerPos)
{

}

/*Check the euclidean distance to the player*/
void SwarmBoid::checkRangeToPlayer(sf::Vector2f playerPos)
{
	distanceToPlayer = sqrtf((((playerPos.x - getPosition().x)*(playerPos.x - getPosition().x)) + ((playerPos.y - getPosition().y)*(playerPos.y - getPosition().y))));
	
	if (distanceToPlayer < 300)
		currentState = INTERCEPT;
	else currentState = TEND;
}

/*Seek the position that is passed in(i.e. match this position and face the direction being travelled in)*/
void SwarmBoid::Seek(sf::Vector2f targetPos)
{
	dirMove = sf::Vector2f(targetPos - getPosition());
	float length = sqrtf((dirMove.x * dirMove.x) + (dirMove.y * dirMove.y));

	dirMove.x /= length;
	dirMove.y /= length;

	velocity = dirMove*speed;//Remove this?
	setPosition(getPosition() + velocity);


	//sort out the swarmBoids orientation
	float angle;

	angle = acos(dirMove.x);
	angle *= (180 / 3.14);
	if (getPosition().y < targetPos.y)
		setRotation(angle);
	else setRotation(-angle);
}

/*Plot an intercept course with a predicted position of where the target will be
(uses seek function to seek this predicted position)*/
void SwarmBoid::Pursue(sf::Vector2f targetPos, sf::Vector2f targetVel)
{
	dirMove = sf::Vector2f(targetPos - getPosition());
	float velLength = sqrtf((velocity.x * velocity.x) + (velocity.y * velocity.y));
	speed = velLength;

	sf::Vector2f newTargetPos;
	float maxTimePrediction = 15;//fiddle with this
	float timePrediction;

	if (speed >= (distanceToPlayer / maxTimePrediction))//>= not <=(in the notes)?
	{
		timePrediction = maxTimePrediction;
	}
	else
	{
		timePrediction = distanceToPlayer / speed;
		newTargetPos = targetPos + sf::Vector2f(targetVel.x*timePrediction, targetVel.y*timePrediction);
	}
	std::cout << "TargetPos: " << targetPos.x << ", " << targetPos.y << std::endl;
	std::cout << "NewTargetPos: " << newTargetPos.x << ", " << newTargetPos.y << std::endl;
	Seek(newTargetPos);
}

void SwarmBoid::interceptPlayer(sf::Vector2f playerPos)
{

}

//swarm up!
void SwarmBoid::Swarm(std::vector<SwarmBoid> boids)
{
	/*Lenard-Jones Potential function
	Vector R = me.position - you.position
	Real D = R.magnitude()
	Real U = -A / pow(D, N) + B / pow(D, M)
	R.normalise()
	force = force + R*U
	*/

	float A = 50;//100
	float B = 5000;//5000
	float N = 1;//10
	float M = 2;//5

	sf::Vector2f R;
	sf::Vector2f sum;

	for (int i = 0; i < boids.size(); i++)
	{
		//R = R.subTwoVector(location, v.at(i).location);

		//float D;//= R.magnitude();

		//if (D != 0)
		//{

		//}
	}


	//applyForce(sum);
	//update();
	//borders();

}

void SwarmBoid::draw(sf::RenderTarget& window, sf::RenderStates state) const{}

void SwarmBoid::draw(sf::RenderTarget& window)
{
	window.draw(sprite, getTransform());
}

void SwarmBoid::drawRadarIcon(sf::RenderTarget& window)
{
	window.draw(radarSprite, getTransform());
}
