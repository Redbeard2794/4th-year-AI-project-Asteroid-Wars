#include "stdafx.h"
#include "SwarmBoid.h"

SwarmBoid::SwarmBoid()
{
	//load the correct texture or load the debug texture if something is wrong
	if (texture.loadFromFile("Assets/Sprites/Enemies/SwarmBoid.png")) {}
	else texture.loadFromFile("Assets/Debug.png");	//if it fails load placeholder
	sprite.setOrigin(sf::Vector2f(texture.getSize().x / 2, texture.getSize().y / 2));
	sprite.setTexture(texture);
	setPosition(5800, 350);//for testing it out only
}

SwarmBoid::~SwarmBoid() {}

void SwarmBoid::Update()
{

}

void SwarmBoid::checkDistanceToOtherBoids()
{

}

void SwarmBoid::tendTowardsPlayer(sf::Vector2f playerPos)
{

}

void SwarmBoid::checkRangeToPlayer(sf::Vector2f playerPos)
{

}

void SwarmBoid::interceptPlayer(sf::Vector2f playerPos)
{

}

void SwarmBoid::draw(sf::RenderTarget& window, sf::RenderStates state) const{}

void SwarmBoid::draw(sf::RenderTarget& window)
{
	window.draw(sprite, getTransform());
}