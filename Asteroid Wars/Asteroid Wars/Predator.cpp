#include "stdafx.h"
#include "Predator.hpp"

Predator::Predator() {

}
Predator::Predator(sf::Vector2f pos) {
	alive = true;
	loadMedia();
	text_size = texture.getSize();
	current_state = WANDER;
	speed = 0.4f;	//Change this to the players speed

	setPosition(pos);
	seek_circle.setOutlineThickness(2);
	seek_circle.setOutlineColor(sf::Color::Red);
	seek_circle.setFillColor(sf::Color::Transparent);
	seek_circle.setRadius(seek_raduis);
	seek_circle.setPosition(getPosition());

	boundingBox.setOutlineThickness(2);
	boundingBox.setOutlineColor(sf::Color::Yellow);
	boundingBox.setFillColor(sf::Color::Transparent);
}
Predator::~Predator()	{	}

void Predator::loadMedia() {
	//Load, set Origin and set the Texture to the sprite for each sprite in the Factory
	texture.loadFromFile("Assets/Sprites/Enemies/Predator.png");
	setTexture(texture);

	radarTexture.loadFromFile("Assets/Debug.png");
	radarSprite.setTexture(radarTexture);
}
void Predator::update() {

}
void Predator::drawRadarIcon(sf::RenderTarget & w) {

}
void Predator::drawDebug(sf::RenderTarget & w) {

}

sf::Vector2f Predator::findAlignment(std::vector<Predator*>* ships) {

	return sf::Vector2f();
}
sf::Vector2f Predator::findCohesion(std::vector<Predator*>* ships) {

	return sf::Vector2f();
}
sf::Vector2f Predator::findSeparation(std::vector<Predator*>* ships) {

	return sf::Vector2f();
}
void Predator::Wander() {

}
void Predator::Seek(sf::Vector2f targetPos) {

}
void Predator::Flee(sf::Vector2f awayfrom) {

}
void Predator::Pursue(sf::Vector2f targetPos, sf::Vector2f targetVel) {

}

sf::Vector2f Predator::getRandomPoint(int maxX, int maxY, int minX, int minY) {

	return sf::Vector2f();
}
bool Predator::reachDestination() {

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

	return false;
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

}

void Predator::setCenter(sf::Vector2f center) {
	setPosition(center.x - text_size.x / 2, center.y - text_size.y / 2);
}
sf::Vector2f Predator::getCenter() {
	sf::Vector2f pos = getPosition();
	return sf::Vector2f(pos.x + text_size.x / 2, pos.y + text_size.y / 2);
}
