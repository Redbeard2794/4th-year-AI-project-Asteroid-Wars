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
void SwarmBoid::Swarm(std::vector<SwarmBoid*> boids)
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
		R = sf::Vector2f(getPosition().x - boids.at(i)->getPosition().x, getPosition().y - boids.at(i)->getPosition().y);

		//Magnitude of vector formula
		float D = sqrtf((R.x*R.x) + (R.y*R.y));

		if (D != 0)
		{
			float U = -A / pow(D, N) + B / pow(D, M);

			//normalise R
			R = sf::Vector2f(R.x / D, R.y / D);

			R = sf::Vector2f(R.x*U, R.y*U);

			sum = sf::Vector2f(sum.x + R.x, sum.y + R.y);
		}
	}

	ApplyForce(sum);
	UpdateInSwarm();
	BoundaryDetection();
}

void SwarmBoid::ApplyForce(sf::Vector2f force)
{
	acceleration = sf::Vector2f(acceleration.x + force.x, acceleration.y + force.y);
}

void SwarmBoid::UpdateInSwarm()
{
	//To make the slow down not as abrupt
	acceleration = sf::Vector2f(acceleration.x*.4f, acceleration.y*.4f);

	// Update velocity
	velocity = sf::Vector2f(velocity.x + acceleration.x, velocity.y + acceleration.y);

	// Limit speed
	float velMag = sqrtf((velocity.x*velocity.x) + (velocity.y*velocity.y));
	if (velMag > 1.5f)
		velocity = sf::Vector2f(velocity.x / velMag, velocity.y / velMag);

	//update position
	setPosition(sf::Vector2f(getPosition().x + velocity.x, getPosition().y + velocity.y));

	float angle = acos(velocity.x);
	angle *= (180 / 3.14);
	//if (getPosition().y < targetPos.y)
	//	setRotation(angle);
	//else setRotation(-angle);
	//setRotation(angle);

	// Reset accelertion to 0 each cycle
	acceleration = sf::Vector2f(0, 0);
}

void SwarmBoid::BoundaryDetection()
{
	if (getPosition().x > 6400)
		setPosition(0, getPosition().y);
	else if (getPosition().x < 0)
		setPosition(6400, getPosition().y);
	else if (getPosition().y > 4800)
		setPosition(getPosition().x, 0);
	else if (getPosition().y < 0)
		setPosition(getPosition().x, 4800);
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
