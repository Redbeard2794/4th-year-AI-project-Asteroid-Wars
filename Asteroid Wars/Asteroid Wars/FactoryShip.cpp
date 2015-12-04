#include "stdafx.h"
#include "FactoryShip.hpp"

//Default constructor of Factory Ship
FactoryShip::FactoryShip() {
	texture.loadFromFile("Assets/Sprites/Enemies/Factory.png");
	text_size = texture.getSize();
	current_state = WANDER;
	hits_taken = 0;

	setTexture(texture);
	setPosition(0, 0);
}

FactoryShip::~FactoryShip() {	}

//Update modifies Velocity, location and resets the acceleration with the three laws values
void FactoryShip::update(){

}

void FactoryShip::applyForce(sf::Vector2f force) {
	acceleration += force;
}

void FactoryShip::fireInterceptor() {

}

void FactoryShip::Position(sf::Vector2f pos) {

}

void FactoryShip::Evade(sf::Vector2f awayfrom) {

}

void FactoryShip::setCenter(sf::Vector2f center) {
	setPosition(center.x - text_size.x / 2, center.y - text_size.y / 2);
}
sf::Vector2f FactoryShip::getCenter() {
	sf::Vector2f pos = getPosition();
	return sf::Vector2f(pos.x + text_size.x / 2, pos.y + text_size.y / 2);
}
