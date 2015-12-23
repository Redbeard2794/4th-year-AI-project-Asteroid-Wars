#include "stdafx.h"
#include "SwarmBoid.h"

SwarmBoid::SwarmBoid()
{
	//load the correct texture or load the debug texture if something is wrong
	if (texture.loadFromFile("Assets/Sprites/Enemies/swarmBoid/ufoSprite.png")) {}
	else texture.loadFromFile("Assets/Debug.png");	//if it fails load placeholder
	setOrigin(sf::Vector2f(texture.getSize().x / 2, texture.getSize().y / 2));
	setTexture(texture);
	setPosition(5800, 350);//for testing it out only

	if (radarTexture.loadFromFile("Assets/Sprites/Enemies/swarmBoid/swarmBoidRadarIcon2.png")) {}
	else radarTexture.loadFromFile("Assets/Debug.png");
	radarSprite.setOrigin(sf::Vector2f(radarTexture.getSize().x / 2, radarTexture.getSize().y / 2));
	radarSprite.setTexture(radarTexture);

	currentState = TEND;
	speed = 0.4f;
	velocity = sf::Vector2f(1, 1);

	boundingBox.setOutlineThickness(2);
	boundingBox.setOutlineColor(sf::Color::Magenta);
	boundingBox.setFillColor(sf::Color::Transparent);

	alive = true;
}

SwarmBoid::~SwarmBoid() {}

void SwarmBoid::Update(sf::Vector2f playerPos, sf::Vector2f playerVel, std::vector<SwarmBoid*> boids)
{
	if (alive)
	{
		checkRangeToPlayer(playerPos);

		if (currentState == TEND)
		{
			//tendTowardsPlayer(playerPos);
			Swarm(boids, playerPos);
		}

		else if (currentState == INTERCEPT)
		{
			//interceptPlayer(playerPos);
			Pursue(playerPos, playerVel);
		}
	}
	//std::cout << "Current state: " << currentState << std::endl;
}

sf::Vector2f SwarmBoid::checkDistanceToOtherBoids(std::vector<SwarmBoid*> boids)
{
	// Distance of field of vision for separation between boids
	float desiredSeparation = 0.5;

	sf::Vector2f steeringForce(0, 0);
	int count = 0;

	// For every boid in the system, check if it's too close
	for (int i = 0; i < boids.size(); i++)
	{
		sf::Vector2f pos = getPosition();
		// Calculate distance from current boid to boid we're looking at
		float d = sqrtf((((pos.x - boids.at(i)->getPosition().x)*(pos.x - boids.at(i)->getPosition().x)) +
			((pos.y - boids.at(i)->getPosition().y)*(pos.y - boids.at(i)->getPosition().y))));


		// If this is a fellow boid and it's too close, move away from it
		if ((d > 0) && (d < desiredSeparation))
		{
			sf::Vector2f diff;
			diff = sf::Vector2f(getPosition().x - boids.at(i)->getPosition().x, getPosition().y - boids.at(i)->getPosition().y);

			//normalise
			float length = sqrtf((diff.x * diff.x) + (diff.y * diff.y));

			diff.x /= length;
			diff.y /= length;

			diff = sf::Vector2f(diff.x / d, diff.y / d);    // Weight by distance

			//add difference to steeringForce
			steeringForce = sf::Vector2f(steeringForce.x + diff.x, steeringForce.y + diff.y);
			count++;
		}
	}
	// Adds average difference of location to acceleration
	if (count > 0)
		steeringForce = sf::Vector2f(steeringForce.x/(float)count, steeringForce.y/(float)count);

	float magnitude = sqrt((steeringForce.x * steeringForce.x) + (steeringForce.y * steeringForce.y)); //Magnitude of vector formula

	if (magnitude > 0)
	{
		// Steering = Desired - Velocity

		//normalise
		float length = sqrtf((steeringForce.x * steeringForce.x) + (steeringForce.y * steeringForce.y));

		steeringForce.x /= length;
		steeringForce.y /= length;

		steeringForce = sf::Vector2f(steeringForce.x * 0.4, steeringForce.y * 0.4);//max speed

		steeringForce = sf::Vector2f(steeringForce.x - velocity.x, steeringForce.y - velocity.y);

		if (magnitude > 0.5)//max force
		{
			steeringForce = sf::Vector2f(steeringForce.x / magnitude, steeringForce.y / magnitude);
		}
	}
	return steeringForce;
}

void SwarmBoid::tendTowardsPlayer(sf::Vector2f playerPos)
{

}

/*Check the euclidean distance to the player*/
void SwarmBoid::checkRangeToPlayer(sf::Vector2f playerPos) {
	distanceToPlayer = sqrtf((((playerPos.x - getPosition().x)*(playerPos.x - getPosition().x)) + ((playerPos.y - getPosition().y)*(playerPos.y - getPosition().y))));
	
	if (distanceToPlayer < 200)
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
	if (getPosition().y < targetPos.y)//maybe remove rotation as they are flying saucers
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
	//std::cout << "TargetPos: " << targetPos.x << ", " << targetPos.y << std::endl;
	//std::cout << "NewTargetPos: " << newTargetPos.x << ", " << newTargetPos.y << std::endl;
	Seek(newTargetPos);
}

//swarm up!
void SwarmBoid::Swarm(std::vector<SwarmBoid*> boids, sf::Vector2f playerPos)
{
	/*Lenard-Jones Potential function
	Vector R = me.position - you.position
	Real D = R.magnitude()
	Real U = -A / pow(D, N) + B / pow(D, M)
	R.normalise()
	force = force + R*U
	*/

	float A = 25;//100....50
	float B = 5000;//5000....5000
	float N = 1;//10....1
	float M = 2;//5....2

	sf::Vector2f R;
	sf::Vector2f sum;

	//get the direction towards the player
	dirMove = sf::Vector2f(playerPos.x - getPosition().x, playerPos.y - getPosition().y);
	float length = sqrtf((dirMove.x * dirMove.x) + (dirMove.y * dirMove.y));

	dirMove.x /= length;
	dirMove.y /= length;

	for (int i = 0; i < boids.size(); i++)
	{
		if (boids.at(i)->currentState == TEND)
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
				//sum = sf::Vector2f(sum.x + R.x + dirMove.x, sum.y + R.y + dirMove.y);
			}
		}
	}

	ApplyForce(sum);
	//sf::Vector2f sep = checkDistanceToOtherBoids(boids);
	//ApplyForce(sep);
	UpdateInSwarm(playerPos);
	BoundaryDetection();
}

void SwarmBoid::ApplyForce(sf::Vector2f force)
{
	acceleration = sf::Vector2f(acceleration.x + force.x, acceleration.y + force.y);
}

void SwarmBoid::UpdateInSwarm(sf::Vector2f playerPos)
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

	//float angle = acos(acceleration.x);
	//angle *= (180 / 3.14);
	//if (getPosition().y < playerPos.y)
	//	setRotation(angle);
	//else setRotation(-angle);

	// Reset accelertion to 0 each cycle
	acceleration = sf::Vector2f(0, 0);
}

//detect the edge of the screen
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

void SwarmBoid::drawRadarIcon(sf::RenderTarget& window){
	radarSprite.setPosition(getPosition());
	radarSprite.setRotation(getRotation());
	window.draw(radarSprite);
}

void SwarmBoid::DrawBoundingBox(sf::RenderTarget & window)
{
	boundingBox.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
	boundingBox.setSize(sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height));
	window.draw(boundingBox);
}

/*Check if the boid is still alive*/
bool SwarmBoid::CheckIfAlive()
{
	return alive;
}

/*set the alive status of the boid*/
void SwarmBoid::SetAliveStatus(bool a)
{
	alive = a;
}
