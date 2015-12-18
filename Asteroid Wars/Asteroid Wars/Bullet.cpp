#include "stdafx.h"
#include "Bullet.h"

/*
constructor
params are the direction the bullet is to travel in, it's max time to live and its starting position
*/
Bullet::Bullet(sf::Vector2f dir, int t, sf::Vector2f pos, float angle)
{
	if (texture.loadFromFile("Assets/Sprites/Player/playerMissile2.png")) {}
	else texture.loadFromFile("Assets/Debug.png");
	setTexture(texture);
	setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);

	direction = dir;
	timeToLive = t;

	speed = 2.5f;

	lifeClock.restart();

	setPosition(pos);

	setRotation(angle);

	alive = true;

	boundingBox.setOutlineThickness(2);
	boundingBox.setOutlineColor(sf::Color::Green);
	boundingBox.setFillColor(sf::Color::Transparent);
}

/*Destructor*/
Bullet::~Bullet()
{

}

void Bullet::Update()
{
	if(lifeClock.getElapsedTime().asSeconds() < timeToLive)//update the position if the bullet is still alive
		setPosition(getPosition() + (direction*speed));

	else alive = false;

	if (!alive)
	{
		setPosition(-4000, -4000);
	}
}

void Bullet::Reset(sf::Vector2f dir, int t, sf::Vector2f pos, float angle)
{
	direction = dir;
	timeToLive = t;
	setPosition(pos);
	setRotation(angle);
	alive = true;
	lifeClock.restart();
}

void Bullet::DrawBoundingBox(sf::RenderTarget & window)
{
	boundingBox.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
	boundingBox.setSize(sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height));
	window.draw(boundingBox);
}

bool Bullet::IsAlive()
{
	return alive;
}
