#include "stdafx.h"
#include "FactoryShip.hpp"

//Default constructor of Factory Ship
FactoryShip::FactoryShip() {
	loadMedia();
	text_size = texture.getSize();
	current_state = WANDER;
	hits_taken = 0;
	
	setPosition(5000, 200);

	//Initalise Shapes
	evade_circle.setOutlineThickness(2);
	evade_circle.setOutlineColor(sf::Color::Red);
	evade_circle.setFillColor(sf::Color::Transparent);
	evade_circle.setRadius(500);
	evade_circle.setPosition(getPosition());

	missle_circle.setOutlineThickness(2);;
	missle_circle.setOutlineColor(sf::Color::Green);;
	missle_circle.setFillColor(sf::Color::Transparent);
	missle_circle.setRadius(250);
	missle_circle.setPosition(getPosition());

	boundingBox.setOutlineThickness(2);
	boundingBox.setOutlineColor(sf::Color::Yellow);
	boundingBox.setFillColor(sf::Color::Transparent);
}

FactoryShip::~FactoryShip() {	}

void FactoryShip::loadMedia() {
	//Load, set Origin and set the Texture to the sprite for each sprite in the Factory
	texture.loadFromFile("Assets/Sprites/Enemies/Factory.png");
	setOrigin(sf::Vector2f(texture.getSize().x / 2, texture.getSize().y / 2));
	setTexture(texture);

	radarTexture.loadFromFile("Assets/Debug.png");
	radarSprite.setOrigin(sf::Vector2f(radarTexture.getSize().x / 2, radarTexture.getSize().y / 2));
	radarSprite.setTexture(radarTexture);
}

//Update modifies Velocity, location and resets the acceleration with the three laws values
void FactoryShip::update(){


	//Update the Circle Shapes
	evade_circle.setPosition(getPosition() - sf::Vector2f(500,500));
	missle_circle.setPosition(getPosition() - sf::Vector2f(250, 250));
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

void FactoryShip::Evade(sf::Vector2f awayfrom) {
	direction = sf::Vector2f(awayfrom + getPosition());
	float length = sqrtf((direction.x * direction.x) + (direction.y * direction.y));

	direction.x /= length;
	direction.y /= length;

	velocity = direction * speed;
	setPosition(getPosition() + velocity);
}

void FactoryShip::setCenter(sf::Vector2f center) {
	setPosition(center.x - text_size.x / 2, center.y - text_size.y / 2);
}
sf::Vector2f FactoryShip::getCenter() {
	sf::Vector2f pos = getPosition();
	return sf::Vector2f(pos.x + text_size.x / 2, pos.y + text_size.y / 2);
}
