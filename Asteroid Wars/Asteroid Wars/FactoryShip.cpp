#include "stdafx.h"
#include "FactoryShip.hpp"

#include <iostream>
using namespace std;

//Default constructor of Factory Ship
FactoryShip::FactoryShip() {
	loadMedia();
	text_size = texture.getSize();
	current_state = WANDER;
	speed = 0.2f;
	hits_taken = 0;
	
	setPosition(5000, 400);

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
}

FactoryShip::~FactoryShip() {	}

void FactoryShip::loadMedia() {
	//Load, set Origin and set the Texture to the sprite for each sprite in the Factory
	texture.loadFromFile("Assets/Sprites/Enemies/Factory.png");
	//setOrigin(sf::Vector2f(texture.getSize().x / 2, texture.getSize().y / 2));
	setTexture(texture);

	radarTexture.loadFromFile("Assets/Debug.png");
	//radarSprite.setOrigin(sf::Vector2f(radarTexture.getSize().x / 2, radarTexture.getSize().y / 2));
	radarSprite.setTexture(radarTexture);
}

//Update modifies Velocity, location and resets the acceleration with the three laws values
void FactoryShip::update(Player *p){
	float dist = distanceTo(p->getCenter());
	//Large Circle
	if (dist < evade_raduis) {
		//Small Circle
		if (dist < missle_raduis) {
			//Evasive Behaviour
			current_state = FLEE;
			cout << "Factory FLEE" << endl;
		}
		//Evade
		//Fire @ Player
		else {
			current_state = EVADE;
			cout << "Factory EVADE" << endl;
		}
	}
	else {
		current_state = WANDER;
		cout << "Factory WANDER" << endl;
	}
	current_state = EVADE;
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
	}

	checkBoundary();

	//Update the Circle Shapes
	evade_circle.setPosition(getCenter() - sf::Vector2f(evade_raduis, evade_raduis));
	missle_circle.setPosition(getCenter() - sf::Vector2f(missle_raduis, missle_raduis));
}

float FactoryShip::distanceTo(sf::Vector2f point) {
	sf::Vector2f center = getCenter();
	return sqrtf((((center.x - point.x)*(center.x - point.x)) + ((center.y - point.y)*(center.y - point.y))));
}

//Applies the passed in force to the acceleration
void FactoryShip::applyForce(sf::Vector2f force) {
	acceleration += force;
}

void FactoryShip::fireInterceptor() {

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
}

void FactoryShip::drawDebug(sf::RenderTarget & w) {
	boundingBox.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
	boundingBox.setSize(sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height));
	w.draw(boundingBox);
	w.draw(evade_circle);
	w.draw(missle_circle);
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

	setCenter(getCenter() + velocity);
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
		setPosition(getPosition() + velocity);
	else
		setPosition(getPosition() - velocity);
}
void FactoryShip::Flee(sf::Vector2f awayfrom) {
	direction = sf::Vector2f(awayfrom - getPosition());
	float length = sqrtf((direction.x * direction.x) + (direction.y * direction.y));

	direction.x /= length;
	direction.y /= length;

	velocity = direction * speed;
	setPosition(getPosition() - velocity);
}

void FactoryShip::setCenter(sf::Vector2f center) {
	setPosition(center.x - text_size.x / 2, center.y - text_size.y / 2);
}
sf::Vector2f FactoryShip::getCenter() {
	sf::Vector2f pos = getPosition();
	return sf::Vector2f(pos.x + text_size.x / 2, pos.y + text_size.y / 2);
}
