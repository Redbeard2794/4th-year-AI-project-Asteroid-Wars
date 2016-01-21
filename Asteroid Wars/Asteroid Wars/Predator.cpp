#include "stdafx.h"
#include "Predator.hpp"

Predator::Predator() {

}
Predator::Predator(sf::Vector2f pos) {
	shieldBurnDownClock.restart();
	speedBoostBurnDownClock.restart();
	health = 25;
	alive = true;
	can_despawn = true;
	loadMedia();
	text_size = texture.getSize();
	current_state = WANDER;
	maxSpeed = .75f;	//Change this to the players speed
	speed = 0.0f;
	acceleration = sf::Vector2f(0, 0);
	velocity = sf::Vector2f(rand() % 3 - 2, rand() % 3 - 2); // Allows for range of -2 -> 2

	bullets.push_back(new Bullet());
	bullets.push_back(new Bullet());
	bullets.push_back(new Bullet());
	bullets.push_back(new Bullet());
	bullets.push_back(new Bullet());
	bullets.push_back(new Bullet());
	bullet_count = bullets.size();
	next_bullet = 0;

	setPosition(pos);
	seek_circle.setOutlineThickness(2);
	seek_circle.setOutlineColor(sf::Color::Red);
	seek_circle.setFillColor(sf::Color::Transparent);
	seek_circle.setRadius(seek_raduis);
	seek_circle.setPosition(getPosition());

	boundingBox.setOutlineThickness(2);
	boundingBox.setOutlineColor(sf::Color::Yellow);
	boundingBox.setFillColor(sf::Color::Transparent);
	destination = getCenter();

	can_fire = false;
	reload_time = 4;
	fireTime = 4;
	fire_Clock.restart();
}
Predator::~Predator() {	}

void Predator::loadMedia() {
	//Load and set the Texture to the sprite for each sprite in the Predator
	texture.loadFromFile("Assets/Sprites/Enemies/Predator.png");
	setTexture(texture);
	textureImage.loadFromFile("Assets/Sprites/Enemies/Predator.png");

	radarTexture.loadFromFile("Assets/Debug.png");
	radarSprite.setTexture(radarTexture);

	speedBoostTexture.loadFromFile("Assets/Sprites/Enemies/Predator_speed.png");
	shieldActiveTexture.loadFromFile("Assets/Sprites/Enemies/Predator_shield.png");
}
void Predator::update(std::vector<Predator*>* ships, Player *p, ExplosionController * ec, std::vector<Obstacle*> *o) {
	if (alive) {
		if (distanceTo(p->getCenter()) < seek_raduis) current_state = SEEK;
		else if (distanceTo(p->getCenter()) > flock_raduis)	current_state = FLOCK;
		else      current_state = WANDER;

		switch (current_state) {
		case WANDER:
			Wander();
			applyForce(findSeparation(ships));
			break;
		case SEEK:
			Pursue(p->getCenter(), sf::Vector2f(0, 0));
			applyForce(findSeparation(ships));
			break;
		case FLOCK:
			//cout << "Predator FLOCKING" << endl;
			Flock(ships);
			break;
		}
		applyAcceration();
		checkBoundary();
		for (int i = 0; i < o->size(); i++) {
			AvoidCollision(o->at(i)->getPosition(), o->at(i)->GetVelocity());
		}

		if (distanceTo(p->getCenter()) < seek_raduis) current_state = SEEK;
		else if (distanceTo(p->getCenter()) > flock_raduis)	current_state = FLOCK;
		else      current_state = WANDER;

		switch (current_state) {
		case WANDER:
			Wander();
			applyForce(findSeparation(ships));
			break;
		case SEEK:
			Pursue(p->getCenter(), sf::Vector2f(0, 0));
			applyForce(findSeparation(ships));
			break;
		case FLOCK:
			//cout << "Predator FLOCKING" << endl;
			Flock(ships);
			break;
		}
		applyAcceration();
		checkBoundary();

		float dist = distanceTo(p->getCenter());
		if (dist < missle_raduis && fire_Clock.getElapsedTime().asSeconds() > fireTime) {
			can_fire = true;
		}
		if (can_fire) {
			fire_Clock.restart();
			fire();
			cout << "Preditor Fired" << endl;
			can_fire = false;
		}


		//power-up code
		if (shieldBurnDownClock.getElapsedTime().asSeconds() > 6 && shieldActive == true)
		{
			shieldActive = false;
			setTexture(texture);
		}


		if (speedBoostBurnDownClock.getElapsedTime().asSeconds() > 6 && speedBoostActive == true)
		{
			speedBoostActive = false;
			setTexture(texture);
			maxSpeed = 1.5f;
		}
	}

	//Update and handle bullets
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->IsAlive()) {
			bullets[i]->Update();
			//Missle Collide with player check
			if (p->getGlobalBounds().intersects(bullets[i]->getGlobalBounds()) == true) {
				ec->AddExplosion(bullets[i]->getPosition());
				bullets[i]->setPosition(-100, -100);
				bullets[i]->SetAliveStatus(false);
				if (p->IsShieldActive() == false)
					p->setHealth((p->getHealth() - 35));
				std::cout << "Predator missile hit player and dealt 35 damage. Player now has " << p->getHealth() << " health." << std::endl;
			}
			//Missle Collide with obsticles check
			for (int j = 0; j < o->size(); j++) {
				if (bullets[i]->getGlobalBounds().intersects(o->at(j)->getGlobalBounds())) {
					ec->AddExplosion(bullets[i]->getPosition());
					bullets[i]->setPosition(-100, -100);
					bullets[i]->SetAliveStatus(false);
					std::cout << "Obstacle with index " << j << " was hit with Predator missle at index " << i << std::endl;
				}
			}
		}
	}
	if(!can_despawn)
		if (!alive)
			CheckActiveBullets();
}
void Predator::drawRadarIcon(sf::RenderTarget & w) {
	//if (alive)
	//{
		radarSprite.setPosition(getPosition());
		w.draw(radarSprite);
	//}
}
void Predator::drawDebug(sf::RenderTarget & w) {
	//if (alive)
	//{
		boundingBox.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
		boundingBox.setSize(sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height));
		seek_circle.setPosition(getCenter() - sf::Vector2f(seek_raduis, seek_raduis));
		w.draw(boundingBox);
		w.draw(seek_circle);
	//}
}
void Predator::drawBullets(sf::RenderTarget & w) {
	//if (alive)
	//{
		for (int i = 0; i < bullets.size(); i++) {
			if (bullets[i]->IsAlive())
				w.draw(*bullets[i]);
		}
	//}
}

float magnitude(sf::Vector2f vec) {
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}
sf::Vector2f normalize(sf::Vector2f vec) {
	float m = magnitude(vec);

	if (m > 0)	return sf::Vector2f(vec.x / m, vec.y / m);
	else		return sf::Vector2f(vec.x, vec.y);
}

sf::Vector2f Predator::findAlignment(std::vector<Predator*>* ships) {
	float neighbordist = 50;

	sf::Vector2f sum(0, 0);
	int count = 0;
	for (int i = 0; i < ships->size(); i++)
	{
		// Calculate distance from current predator to the predator we're looking at
		float distance = distanceTo(ships->at(i)->getCenter());
		if ((distance > 0) && (distance < flock_ali)) // 0 < d < 50
		{
			sum = sum + ships->at(i)->velocity;
			count++;
		}
	}
	// If there are boids close enough for alignment...
	if (count > 0)
	{
		sum = sf::Vector2f(sum.x * count, sum.y * count);// Divide sum by the number of close boids (average of velocity)
		sum = normalize(sum);	   		// Turn sum into a unit vector, and
		sum = sf::Vector2f(sum.x * maxSpeed, sum.y * maxSpeed);    // Multiply by maxSpeed
																	// Steer = Desired - Velocity
		sf::Vector2f steer;
		steer = sf::Vector2f(sum.x - velocity.x, sum.y - velocity.y);
		double size = magnitude(steer);
		if (size > maxForce)
			steer = sf::Vector2f(steer.x / size, steer.y / size);
		return steer;
	}
	else {
		sf::Vector2f temp(0, 0);
		return temp;
	}
}
sf::Vector2f Predator::findCohesion(std::vector<Predator*>* ships) {
	float neighbordist = 50;

	sf::Vector2f sum(0, 0);
	int count = 0;
	for (int i = 0; i < ships->size(); i++)
	{
		float distance = distanceTo(ships->at(i)->getCenter());
		if ((distance > 0) && (distance < flock_coh))
		{
			sum = sum + ships->at(i)->getCenter();
			count++;
		}
	}
	if (count > 0)
	{
		sum = sf::Vector2f(sum.x / count, sum.y / count);
		return Seek(sum);
	}
	else {
		sf::Vector2f temp(0, 0);
		return temp;
	}
}
sf::Vector2f Predator::findSeparation(std::vector<Predator*>* ships) {
	// Distance of field of vision for separation between predator
	float desiredseparation = 20;

	sf::Vector2f steer(0, 0);
	int count = 0;
	// For every predator in the system, check if it's too close
	for (int i = 0; i < ships->size(); i++)	{
		// Calculate distance from current predator to the predator we're looking at
		float distance = distanceTo(ships->at(i)->getCenter());

		// If this is a fellow predator and it's too close, move away from it
		if ((distance > 0) && (distance < flock_sep))
		{
			sf::Vector2f diff(0, 0);
			sf::Vector2f pos = getPosition();
			diff = sf::Vector2f(pos.x - ships->at(i)->getPosition().x, pos.y - ships->at(i)->getPosition().y);
			diff = normalize(diff);
			diff = sf::Vector2f(diff.x / distance, diff.y / distance);	//Weight by distance
			steer = steer + diff;
			count++;
		}
	}
	// Adds average difference of location to acceleration
	if (count > 0) {
		steer = sf::Vector2f(steer.x / count, steer.y / count);
	}
	if (magnitude(steer) > 0)
	{
		// Steering = Desired - Velocity
		steer = normalize(steer);
		steer = sf::Vector2f(steer.x * maxSpeed, steer.y * maxSpeed);
		steer = sf::Vector2f(steer.x - velocity.x, steer.y - velocity.y);
		double size = magnitude(steer);
		if (size > maxForce)
			steer = sf::Vector2f(steer.x / size, steer.y / size);
	}
	return steer;
}
void Predator::Wander() {
	//If we are at our wander destination get another one.
	if (reachDestination()) {
		while (checkWithinBounds(getCenter() + getRandomPoint(wander_distance, wander_distance, wander_distance / 2, wander_distance / 2))) {}
		destination = random_point + getCenter();
	}
	direction = sf::Vector2f(destination - getCenter());
	float length = sqrtf((direction.x * direction.x) + (direction.y * direction.y));
	direction.x /= length;
	direction.y /= length;
	//velocity = direction * maxSpeed;
	sf::Vector2f force = direction * maxSpeed;

	acceleration = sf::Vector2f(force.x - velocity.x, force.y - velocity.y);
	double size = magnitude(acceleration);
	if (size > maxForce)
		acceleration = sf::Vector2f(acceleration.x / size, acceleration.y / size);  // Limit to maximum steering force

	//setCenter(getCenter() + velocity);
	applyForce(force);
}
sf::Vector2f Predator::Seek(sf::Vector2f targetPos) {
	sf::Vector2f desired;
	desired = desired - targetPos;	// A vector pointing from the location to the target
						   // Normalize desired and scale to maximum speed
	desired = normalize(desired);
	desired = sf::Vector2f(desired.x * maxSpeed, desired.y * maxSpeed);
	// Steering = Desired minus Velocity
	acceleration = sf::Vector2f(desired.x - velocity.x, desired.y - velocity.y);
	double size = magnitude(acceleration);
	if (size > maxForce)
		acceleration = sf::Vector2f(acceleration.x / size, acceleration.y / size);  // Limit to maximum steering force
	return acceleration;
}
void Predator::Flock(std::vector<Predator*>* ships) {
	sf::Vector2f sep = findSeparation(ships);
	sf::Vector2f ali = findAlignment(ships);
	sf::Vector2f coh = findCohesion(ships);

	sep = sf::Vector2f(sep.x * 1.0, sep.y * 1.0);
	ali = sf::Vector2f(ali.x * 1.0, ali.y * 1.0); // Might need to alter weights for different characteristics
	coh = sf::Vector2f(coh.x * 1.0, coh.y * 1.0);

	applyForce(sep);
	applyForce(ali);
	applyForce(coh);
}
void Predator::Pursue(sf::Vector2f targetPos, sf::Vector2f targetVel) {
	direction = sf::Vector2f(targetPos - getPosition());
	float length = sqrtf((direction.x * direction.x) + (direction.y * direction.y));

	direction.x /= length;
	direction.y /= length;

	sf::Vector2f force = direction * maxSpeed;
	applyForce(force);

	//sort out the orientation
	float angle;

	angle = acos(direction.x);
	angle *= (180 / 3.14);
	if (getPosition().y < targetPos.y)
		setRotation(angle);
	else setRotation(-angle);
}

//Avoidence
void Predator::Flee(sf::Vector2f targetPos) {
	avoid_direction = sf::Vector2f(targetPos - getCenter());
	float length = sqrtf((avoid_direction.x * avoid_direction.x) + (avoid_direction.y * avoid_direction.y));

	avoid_direction.x /= length;
	avoid_direction.y /= length;
	speed = 3.5;

	velocity = avoid_direction*speed;//Remove this?
	setPosition(getCenter() - velocity);

	//sort out the orientation
	float angle;

	angle = acos(direction.x);
	angle *= (180 / 3.14);
	if (getPosition().y < targetPos.y)
		setRotation(angle);
	else setRotation(-angle);
}
void Predator::Evade(sf::Vector2f targetPos, sf::Vector2f targetVel, float distanceToObstacle) {
	//std::cout << "Evade() called" << std::endl;
	avoid_direction = sf::Vector2f(targetPos - getCenter());
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
void Predator::AvoidCollision(sf::Vector2f targetPos, sf::Vector2f targetVel) {
	avoid_direction = sf::Vector2f(targetPos - getCenter());
	float length = sqrtf((avoid_direction.x * avoid_direction.x) + (avoid_direction.y * avoid_direction.y));

	avoid_direction.x /= length;
	avoid_direction.y /= length;

	sf::Vector2f myOrientation = sf::Vector2f(cos(getRotation()), sin(getRotation()));

	float dotProd = (avoid_direction.x * myOrientation.x) + (avoid_direction.y * myOrientation.y);

	float distance = sqrtf((((targetPos.x - getCenter().x)*(targetPos.x - getCenter().x)) + ((targetPos.y - getCenter().y)*(targetPos.y - getCenter().y))));

	if (dotProd < (90 / 2))
	{
		if (distance < 400)
		{
			current_state = EVADE;
			Evade(targetPos, targetVel, distance);
		}
		else if (distance > 400)
			current_state = WANDER;
	}
}

sf::Vector2f Predator::getRandomPoint(int maxX, int maxY, int minX, int minY) {
	bool coin_flip;

	coin_flip = rand() % 2;		//Between 0 & 1
	random_point.x = rand() % minX + maxX;
	if (coin_flip)	random_point.x *= -1;

	coin_flip = rand() % 2;
	random_point.y = rand() % minY + maxY;
	if (coin_flip)	random_point.y *= -1;

	return random_point;
}
bool Predator::reachDestination() {
	sf::Vector2f center = getCenter();
	if (center == destination)
		return true;
	else if (center.x <= destination.x + 5 && center.x >= destination.x - 5 &&
		center.y <= destination.y + 5 && center.y >= destination.y - 5)
		return true;
	return false;
}
void Predator::checkBoundary() {
	sf::Vector2f pos = getPosition();

	if (pos.x > 6400)
		setPosition(0, pos.y);
	else if (pos.x < 0)
		setPosition(6400, pos.y);
	else if (pos.y > 4800)
		setPosition(pos.x, 0);
	else if (pos.y < 0)
		setPosition(pos.x, 4800);
}
bool Predator::checkWithinBounds(sf::Vector2f point) {
	if (point.x > 0 && point.x < 6400 &&
		point.y > 0 && point.y < 4800)
		return false;
	return true;
}
float Predator::distanceTo(sf::Vector2f point) {
	sf::Vector2f center = getCenter();
	return sqrtf((((center.x - point.x)*(center.x - point.x)) + ((center.y - point.y)*(center.y - point.y))));
}
void Predator::applyForce(sf::Vector2f force) {
	acceleration += force;
}
void Predator::applyAcceration() {
	setCenter(getCenter() + acceleration);
	acceleration = sf::Vector2f(0, 0);		//Reset acceleration after applying it.
}
void Predator::fire() {
	bullets[next_bullet]->Reset(direction, 5, getPosition(), getRotation());

	next_bullet++;
	if (!(next_bullet < bullet_count))
		next_bullet = 0;
}

void Predator::setCenter(sf::Vector2f center) {
	setPosition(center.x - text_size.x / 2, center.y - text_size.y / 2);
}
sf::Vector2f Predator::getCenter() {
	sf::Vector2f pos = getPosition();
	return sf::Vector2f(pos.x + text_size.x / 2, pos.y + text_size.y / 2);
}

void Predator::Destroy() {
	alive = false;
	can_despawn = true;
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets.at(i)->IsAlive()) {
			can_despawn = false;
			break;
		}
	}
}

void Predator::CheckActiveBullets() {
	can_despawn = true;
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets.at(i)->IsAlive()) {
			can_despawn = false;
			break;
		}
	}
}

void Predator::SetShieldActive(bool sa)
{
	shieldBurnDownClock.restart();
	shieldActive = sa;
	setTexture(shieldActiveTexture);
}
void Predator::SetSpeedBoostActive(bool sba)
{
	speedBoostBurnDownClock.restart();
	maxSpeed = 3.0f;
	speedBoostActive = sba;
	setTexture(speedBoostTexture);
}
