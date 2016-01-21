#include "stdafx.h"
#include "FactoryShip.hpp"

#include <iostream>
using namespace std;

//Default constructor of Factory Ship
FactoryShip::FactoryShip() {
	alive = true;
	loadMedia();
	text_size = texture.getSize();
	current_state = WANDER;
	speed = 0.2f;
	hits_taken = 0;
	flock_raduis = 600.0;
	
	setPosition(5000, 400);
	
	//missle_container.reserve(3);
	missle_container.push_back(new InterceptorMissile(sf::Vector2f(-100, -100)));
	missle_container.push_back(new InterceptorMissile(sf::Vector2f(-100, -100)));
	missle_container.push_back(new InterceptorMissile(sf::Vector2f(-100, -100)));

	missle_count = missle_container.size();
	next_missle = 0;
	fire_Clock.restart();
	fireTime = 8;
	fire_reload = sf::Time::Zero;
	can_fire = false;

	//Initalise Shapes
	evade_circle.setOutlineThickness(2);
	evade_circle.setOutlineColor(sf::Color::Red);
	evade_circle.setFillColor(sf::Color::Transparent);
	evade_circle.setRadius(evade_raduis);
	evade_circle.setPosition(getPosition());

	missle_circle.setOutlineThickness(2);;
	missle_circle.setOutlineColor(sf::Color::Green);;
	missle_circle.setFillColor(sf::Color::Transparent);
	missle_circle.setRadius(missle_raduis);
	missle_circle.setPosition(getPosition());

	boundingBox.setOutlineThickness(2);
	boundingBox.setOutlineColor(sf::Color::Yellow);
	boundingBox.setFillColor(sf::Color::Transparent);

	destination = getCenter();

	health = 100;
}

FactoryShip::FactoryShip(sf::Vector2f position) {
	alive = true;
	loadMedia();
	text_size = texture.getSize();
	current_state = WANDER;
	speed = 0.2f;
	hits_taken = 0;

	setPosition(position.x, position.y);

	//missle_container.reserve(3);
	missle_container.push_back(new InterceptorMissile(sf::Vector2f(-100, -100)));
	missle_container.push_back(new InterceptorMissile(sf::Vector2f(-100, -100)));
	missle_container.push_back(new InterceptorMissile(sf::Vector2f(-100, -100)));

	missle_count = missle_container.size();
	next_missle = 0;
	fire_Clock.restart();
	fireTime = 15;
	fire_reload = sf::Time::Zero;
	can_fire = false;

	//Initalise Shapes
	evade_circle.setOutlineThickness(2);
	evade_circle.setOutlineColor(sf::Color::Red);
	evade_circle.setFillColor(sf::Color::Transparent);
	evade_circle.setRadius(evade_raduis);
	evade_circle.setPosition(getPosition());

	missle_circle.setOutlineThickness(2);;
	missle_circle.setOutlineColor(sf::Color::Green);;
	missle_circle.setFillColor(sf::Color::Transparent);
	missle_circle.setRadius(missle_raduis);
	missle_circle.setPosition(getPosition());

	boundingBox.setOutlineThickness(2);
	boundingBox.setOutlineColor(sf::Color::Yellow);
	boundingBox.setFillColor(sf::Color::Transparent);

	destination = getCenter();
	health = 100;
}

FactoryShip::~FactoryShip() {	}

void FactoryShip::loadMedia() {
	//Load, set Origin and set the Texture to the sprite for each sprite in the Factory
	texture.loadFromFile("Assets/Sprites/Enemies/Factory.png");
	//setOrigin(sf::Vector2f(texture.getSize().x / 2, texture.getSize().y / 2));
	setTexture(texture);
	textureImage.loadFromFile("Assets/Sprites/Enemies/Factory.png");

	radarTexture.loadFromFile("Assets/Debug.png");
	//radarSprite.setOrigin(sf::Vector2f(radarTexture.getSize().x / 2, radarTexture.getSize().y / 2));
	radarSprite.setTexture(radarTexture);
}

//Update modifies Velocity, location and resets the acceleration with the three laws values
void FactoryShip::update(Player *p, std::vector<FactoryShip*> *ships, ExplosionController * ec, std::vector<Obstacle*> *o){
	#pragma region Player AI
	float dist = distanceTo(p->getCenter());
	//Large Circle
	if (dist < evade_raduis) {
		//Small Circle
		if (dist < missle_raduis) {
			//Evasive Behaviour
			current_state = FLEE;
			//cout << "Factory FLEE" << endl;
		}
		//Evade
		//Fire @ Player
		else {
			current_state = EVADE;
			//cout << "Factory EVADE" << endl;
		}
	}
	else {
		for (int i = 0; i < ships->size(); i++)
		{
			float distance = distanceTo(ships->at(i)->getCenter());
			if ((distance < flock_raduis) && (distance > flock_ali)) {// 0 < d < flocking Raduis
				current_state = FLOCK;
				break;
			}
		}
		current_state = WANDER;
		//cout << "Factory WANDER" << endl;
	}
	//current_state = EVADE;
	//AI Switch
	switch (current_state) {
	case WANDER:
		Wander();
		break;
	case EVADE:
		Evade(p->getCenter(), dist);
		break;
	case FLEE:
		Flee(p->getCenter());
		break;
	case FLOCK:
		//cout << "Factory FLOCKING" << endl;
		sf::Vector2f alignment = findAlignment(ships);
		sf::Vector2f cohesion = findCohesion(ships);
		sf::Vector2f separation = findSeparation(ships);

		//Weight the Values
		alignment = sf::Vector2f(alignment.x * 1.0, alignment.y * 1.0);
		cohesion = sf::Vector2f(cohesion.x * 0.8, cohesion.y * 0.8);
		separation = sf::Vector2f(separation.x * 1.0, separation.y * 1.0);

		// Add the force vectors to acceleration
		applyForce(alignment);
		applyForce(cohesion);
		applyForce(separation);
		break;
	}
	#pragma endregion

	checkBoundary();

	//Firing sequence for the factory ship
	dist = distanceTo(p->getCenter());
	fire_reload += fire_Clock.getElapsedTime();
	int num = fire_reload.asSeconds();
	if (dist < missle_raduis && fire_Clock.getElapsedTime().asSeconds() > fireTime) {
		can_fire = true;
	}
	if (can_fire)	{
		fire_reload = sf::Time::Zero;
		fire_Clock.restart();
		fireInterceptor();
		cout << "Interceptor Fired" << endl;
		can_fire = false;
	}

	//Collision for the Inceptor Missles
	for (int i = 0; i < missle_count; i++) {
		if (missle_container[i]->CheckIfActive()) {
			//Update the Missle
			missle_container[i]->Update(p->getCenter());

			//Missle Collide with player check
			if (p->getGlobalBounds().intersects(missle_container[i]->getGlobalBounds()) == true) {
				ec->AddExplosion(missle_container[i]->getPosition());
				missle_container[i]->Reset();
				if (p->IsShieldActive() == false)
					p->setHealth((p->getHealth() - 35));
				std::cout << "Factory Interceptor Missile hit player and dealt 35 damage. Player now has " << p->getHealth() << " health." << std::endl;
			}

			//Missle Collide with obsticles check
			for (int j = 0; j < o->size(); j++) {
				if (missle_container[i]->getGlobalBounds().intersects(o->at(j)->getGlobalBounds()))		{
					ec->AddExplosion(missle_container[i]->getPosition());
					missle_container[i]->Reset();
					std::cout << "Obstacle with index " << j << " was hit with Factory ship missle at index " << i<< std::endl;
				}
			}
		}
	}

	//Update the Circle Shapes
	evade_circle.setPosition(getCenter() - sf::Vector2f(evade_raduis, evade_raduis));
	missle_circle.setPosition(getCenter() - sf::Vector2f(missle_raduis, missle_raduis));

	applyAcceration();
}

float FactoryShip::distanceTo(sf::Vector2f point) {
	sf::Vector2f center = getCenter();
	return sqrtf((((center.x - point.x)*(center.x - point.x)) + ((center.y - point.y)*(center.y - point.y))));
}

//Applies the passed in force to the acceleration
void FactoryShip::applyForce(sf::Vector2f force) {
	acceleration += force;
}

void FactoryShip::applyAcceration() {
	setCenter(getCenter() + acceleration);
	acceleration = sf::Vector2f(0,0);		//Reset acceleration after applying it.
}

void FactoryShip::fireInterceptor() {
	missle_container[next_missle]->Launch(getCenter());
	
	next_missle++;
	if (!(next_missle < missle_count))
		next_missle = 0;
}

void FactoryShip::Position(sf::Vector2f pos) {

}

//Simple wrap screen method 
void FactoryShip::checkBoundary() {
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

bool FactoryShip::checkWithinBounds(sf::Vector2f point) {
	if (point.x > 0 && point.x < 6400 &&
		point.y > 0 && point.y < 4800)
		return false;
	return true;
}

void FactoryShip::drawRadarIcon(sf::RenderTarget & w) {
	radarSprite.setPosition(getPosition());
	w.draw(radarSprite);
	for (int i = 0; i < missle_count; i++) {
		if (missle_container[i]->CheckIfActive())
			missle_container[i]->drawRadarIcon(w);
	}
}

void FactoryShip::drawDebug(sf::RenderTarget & w) {
	boundingBox.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
	boundingBox.setSize(sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height));
	w.draw(boundingBox);
	w.draw(evade_circle);
	w.draw(missle_circle);
	for (int i = 0; i < missle_count; i++) {
		if (missle_container[i]->CheckIfActive())
			missle_container[i]->DrawBoundingBox(w);
	}
}

void FactoryShip::drawMissles(sf::RenderTarget & w) {
	for (int i = 0; i < missle_count; i++) {
		if (missle_container[i]->CheckIfActive())
			w.draw(*missle_container[i]);
	}
}

sf::Vector2f divideVector(sf::Vector2f v, float amount) {
	return sf::Vector2f(v.x / amount, v.y / amount);
}

sf::Vector2f FactoryShip::findAlignment(std::vector<FactoryShip*> *ships) {
	sf::Vector2f ali(0, 0);
	int count = 0;
	flock_raduis = 600.0;
		
	for (int i = 0; i < ships->size(); i++)
	{
		float distance = distanceTo(ships->at(i)->getCenter());
		if ((distance > 0) && (distance < flock_ali)) // 0 < d < flocking Raduis
		{
			ali.x += ships->at(i)->velocity.x;
			ali.y += ships->at(i)->velocity.y;
			count++;
		}
	}

	// If there are boids close enough for alignment...
	if (count > 0)
	{
		ali = divideVector(ali, (float)count);// Divide sum by the number of close boids (average of velocity)
		//Normalise Alignment Vector
		float mag = sqrt(ali.x * ali.x + ali.y * ali.y);
		if (mag > 0)	ali = sf::Vector2f(ali.x / mag, ali.y / mag);
		ali.x *= speed;
		ali.y *= speed;

		return ali;
	}
	else {
		return sf::Vector2f(0,0);
	}
}
sf::Vector2f FactoryShip::findCohesion(std::vector<FactoryShip*> *ships) {
	sf::Vector2f coh(0, 0);
	float flock_cohesion = 400.0f;

	int count = 0;
	for (int i = 0; i < ships->size(); i++)
	{
		float distance = distanceTo(ships->at(i)->getCenter());
		if ((distance > 0) && (distance < flock_coh)) // 0 < d < flocking cohesion
		{
			coh += ships->at(i)->getCenter();
			count++;
		}
	}

	if (count > 0) 	{
		coh = divideVector(coh, (float)count);
		//Normalise Cohesion Vector
		float mag = sqrt(coh.x * coh.x + coh.y * coh.y);
		if (mag > 0)	coh = sf::Vector2f(coh.x / mag, coh.y / mag);
		coh.x *= speed;
		coh.y *= speed;
		return coh;
	}
	else {
		return sf::Vector2f(0, 0);
	}

}
sf::Vector2f FactoryShip::findSeparation(std::vector<FactoryShip*> *ships) {
	// Distance of field of vision for separation between boids
	float flock_separation = 10.0f;

	sf::Vector2f sep(0, 0);
	int count = 0;
	// For every boid in the system, check if it's too close
	for (int i = 0; i < ships->size(); i++)	{
		float distance = distanceTo(ships->at(i)->getCenter());
		// If this is a fellow boid and it's too close, move away from it
		if ((distance > 0) && (distance < flock_sep))
		{
			sep = sf::Vector2f(getCenter().x - ships->at(i)->getCenter().x, getCenter().y - ships->at(i)->getCenter().y);
			//Normalise Cohesion Vector
			float mag = sqrt(sep.x * sep.x + sep.y * sep.y);
			if (mag > 0)	sep = sf::Vector2f(sep.x / mag, sep.y / mag);
			sep.x *= speed;
			sep.y *= speed;
			sep = divideVector(sep, (float)count);
			count++;
		}
	}

	return sep;

}

sf::Vector2f FactoryShip::getRandomPoint(int maxX, int maxY, int minX, int minY) {
	bool coin_flip;

	coin_flip = rand() % 2;		//Between 0 & 1
	random_point.x = rand() % minX + maxX ;
	if (coin_flip)	random_point.x *= -1;

	coin_flip = rand() % 2;
	random_point.y = rand() % minY +maxY;
	if (coin_flip)	random_point.y *= -1;

	return random_point;
}

bool FactoryShip::reachDestination() {
	sf::Vector2f center = getCenter();
	if (center == destination)
		return true;
	else if (center.x <= destination.x + 5 && center.x >= destination.x - 5 &&
		center.y <= destination.y + 5 && center.y >= destination.y - 5)
		return true;
	return false;
}

void FactoryShip::Wander() {
	//If we are at our wander destination get another one.
	if (reachDestination()) {
		while (checkWithinBounds(getCenter() + getRandomPoint(wander_distance, wander_distance, wander_distance / 2, wander_distance / 2))) {}
		destination = random_point + getCenter();
	}
	direction = sf::Vector2f(destination - getCenter());
	float length = sqrtf((direction.x * direction.x) + (direction.y * direction.y));
	direction.x /= length;
	direction.y /= length;
	velocity = direction * speed;

	//setCenter(getCenter() + velocity);
	applyForce(velocity);
}
void FactoryShip::Evade(sf::Vector2f awayfrom, float dist) {
	//Wander away in a direction while trying to keep the player in the Evade Raduis
	sf::Vector2u other_direction = sf::Vector2u(awayfrom - getPosition());
	
	//Flip either X or Y
	int new_direction;
	bool coin_flip;
	coin_flip = rand() % 2;		//Between 0 & 1
	if (coin_flip) {
		//Invert X
		new_direction = other_direction.x * -1;
		//Find a new X between the orignal X and the fliped one.
		//int randomNum = rand() % 19 + (-9); To find between -9 & 9
		if (other_direction.x > 0)	other_direction.x = rand() % other_direction.x + new_direction;
		if (new_direction > 0)	other_direction.x = rand() % new_direction + other_direction.x;
	}
	else {
		//Invert Y
		new_direction = other_direction.y * -1;
		//Find a new Y between the orignal X and the fliped one.
		if (other_direction.y > 0)	other_direction.y = rand() % other_direction.y + new_direction;
		if (new_direction > 0)	other_direction.y = rand() % new_direction + other_direction.y;
	}
	direction = sf::Vector2f((float)other_direction.x, (float)other_direction.y);
	float length = sqrtf((direction.x * direction.x) + (direction.y * direction.y));

	direction.x /= length;
	direction.y /= length;

	velocity = direction * speed;
	if (dist > evade_raduis - 40)
		applyForce(velocity);
		//setPosition(getPosition() + velocity);
	else
		applyForce(-velocity);
		//setPosition(getPosition() - velocity);
}
void FactoryShip::Flee(sf::Vector2f awayfrom) {
	direction = sf::Vector2f(awayfrom - getPosition());
	float length = sqrtf((direction.x * direction.x) + (direction.y * direction.y));

	direction.x /= length;
	direction.y /= length;

	velocity = direction * speed;
	//setPosition(getPosition() - velocity);
	applyForce(velocity);
}

void FactoryShip::setCenter(sf::Vector2f center) {
	setPosition(center.x - text_size.x / 2, center.y - text_size.y / 2);
}
sf::Vector2f FactoryShip::getCenter() {
	sf::Vector2f pos = getPosition();
	return sf::Vector2f(pos.x + text_size.x / 2, pos.y + text_size.y / 2);
}

float FactoryShip::getHealth()
{
	return health;
}

void FactoryShip::setHealth(float h)
{
	health = h;
}

bool FactoryShip::IsAlive()
{
	return alive;
}

void FactoryShip::SetAlive(bool a)
{
	alive = a;
}
