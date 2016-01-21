#include "stdafx.h"
#include "SwarmBoid.h"

SwarmBoid::SwarmBoid(sf::Vector2f startPos)
{
	//load the correct texture or load the debug texture if something is wrong
	if (texture.loadFromFile("Assets/Sprites/Enemies/swarmBoid/ufoSprite.png")) {}
	else texture.loadFromFile("Assets/Debug.png");	//if it fails load placeholder
	setOrigin(sf::Vector2f(texture.getSize().x / 2, texture.getSize().y / 2));
	setTexture(texture);
	setPosition(startPos);//for testing it out only

	textureImage.loadFromFile("Assets/Sprites/Enemies/swarmBoid/ufoSprite.png");

	int shrinkCoin = rand() % 100 + 2;
	if (shrinkCoin % 7)
		scale(0.75, 0.75);
	else if (shrinkCoin % 10)
		scale(0.5, 0.5);

	if (radarTexture.loadFromFile("Assets/Sprites/Enemies/swarmBoid/swarmBoidRadarIcon2.png")) {}
	else radarTexture.loadFromFile("Assets/Debug.png");
	radarSprite.setOrigin(sf::Vector2f(radarTexture.getSize().x / 2, radarTexture.getSize().y / 2));
	radarSprite.setTexture(radarTexture);

	currentState = SWARM;
	previousState = SWARM;
	speed = 0.4f;
	velocity = sf::Vector2f(1, 1);

	boundingBox.setOutlineThickness(2);
	boundingBox.setOutlineColor(sf::Color::Magenta);
	boundingBox.setFillColor(sf::Color::Transparent);

	alive = true;
}

SwarmBoid::~SwarmBoid() {}

void SwarmBoid::Update(sf::Vector2f playerPos, sf::Vector2f playerVel, std::vector<SwarmBoid*> boids, std::vector<Obstacle*> obstacles)
{
	if (alive)
	{
		checkRangeToPlayer(playerPos);

		for (int i = 0; i < obstacles.size(); i++)
		{
			AvoidCollision(obstacles.at(i)->getPosition(), obstacles.at(i)->GetVelocity());
		}
		if (currentState == SWARM)
		{
			Swarm(boids, playerPos);
		}

		else if (currentState == INTERCEPT)
		{
			Pursue(playerPos, playerVel);
		}

		else if (currentState == EVADE)
		{
			//std::cout << "Evading something" << std::endl;
		}

		//std::cout << "Current state: " << currentState << std::endl;
	}
	
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
	{
		if(currentState != INTERCEPT)
			ChangeStateTo(INTERCEPT);
	}
	else
	{
		if(currentState != SWARM)
			ChangeStateTo(SWARM);
	}
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

	float A = 30;//25
	float B = 4000;//5000
	float N = 1;//1
	float M = 2;//2

	sf::Vector2f R;
	sf::Vector2f sum;

	//get the direction towards the player
	dirMove = sf::Vector2f(playerPos.x - getPosition().x, playerPos.y - getPosition().y);
	float length = sqrtf((dirMove.x * dirMove.x) + (dirMove.y * dirMove.y));

	dirMove.x /= length;
	dirMove.y /= length;

	for (int i = 0; i < boids.size(); i++)
	{
		if (boids.at(i)->currentState == SWARM)
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

				R = sf::Vector2f(R.x + (dirMove.x/1000), R.y +(dirMove.y/1000));//this line make the swarm tend towards the player. Seems a bit fast though. /500 was used after messing with numbers for a while

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

	// Reset accelertion to 0 each cycle
	acceleration = sf::Vector2f(0, 0);
}

//Flee to the position passed in
void SwarmBoid::Flee(sf::Vector2f targetPos)
{
	dirMove = sf::Vector2f(targetPos - getPosition());
	float length = sqrtf((dirMove.x * dirMove.x) + (dirMove.y * dirMove.y));

	dirMove.x /= length;
	dirMove.y /= length;
	speed = 3.5;

	velocity = dirMove*speed;//Remove this?
	setPosition(getPosition() - velocity);
}

//Like pursue but in reverse. params are obstacle position, velocity and distance to the obstacle
void SwarmBoid::Evade(sf::Vector2f targetPos, sf::Vector2f targetVel, float distanceToObstacle)
{
	//std::cout << "Evade() called" << std::endl;
	dirMove = sf::Vector2f(targetPos - getPosition());
	float velLength = sqrtf((velocity.x * velocity.x) + (velocity.y * velocity.y));
	speed = velLength;

	sf::Vector2f newTargetPos;
	float maxTimePrediction = 60;//fiddle with this
	float timePrediction;

	if (speed >= (distanceToObstacle / maxTimePrediction))//>= not <=(in the notes)?
	{
		timePrediction = maxTimePrediction;
	}
	else
	{
		timePrediction = distanceToObstacle / speed;
		newTargetPos = targetPos + sf::Vector2f(targetVel.x*timePrediction, targetVel.y*timePrediction);
	}
	//std::cout << "TargetPos: " << targetPos.x << ", " << targetPos.y << std::endl;
	//std::cout << "NewTargetPos: " << newTargetPos.x << ", " << newTargetPos.y << std::endl;
	Flee(newTargetPos);
}

//Check whether we need to take avoiding action, params are the obstacles position and velocity
void SwarmBoid::AvoidCollision(sf::Vector2f targetPos, sf::Vector2f targetVel)
{
	dirMove = sf::Vector2f(targetPos - getPosition());
	float length = sqrtf((dirMove.x * dirMove.x) + (dirMove.y * dirMove.y));

	dirMove.x /= length;
	dirMove.y /= length;

	sf::Vector2f myOrientation = sf::Vector2f(cos(getRotation()), sin(getRotation()));

	float dotProd = (dirMove.x * myOrientation.x) + (dirMove.y * myOrientation.y);

	float distance = sqrtf((((targetPos.x - getPosition().x)*(targetPos.x - getPosition().x)) + ((targetPos.y - getPosition().y)*(targetPos.y - getPosition().y))));

	if (dotProd < (90 / 2))
	{
		if (distance < 200)
		{
			ChangeStateTo(EVADE);
			Evade(targetPos, targetVel, distance);
		}
		else if(distance > 200)
			ChangeStateTo(previousState);
	}
	
}

//Change the current state to the passed in state. Also updates the previous state
void SwarmBoid::ChangeStateTo(int stateToChangeTo)
{
	if (currentState != stateToChangeTo)
	{
		//std::cout << "Changing state from " << currentState << " to " << stateToChangeTo << std::endl;
		previousState = currentState;
		currentState = stateToChangeTo;
	}
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
