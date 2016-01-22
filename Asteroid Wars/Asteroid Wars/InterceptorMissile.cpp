#include "stdafx.h"
#include "InterceptorMissile.h"

/*Constructor*/
InterceptorMissile::InterceptorMissile(sf::Vector2f pos)	{
	default_spawn = sf::Vector2f(-100, -100);
	//load the correct texture or load the debug texture if something is wrong
	if (texture.loadFromFile("Assets/Sprites/missileC.png")) { }
	else texture.loadFromFile("Assets/Debug.png");	//if it fails load placeholder
	textureImage.loadFromFile("Assets/Sprites/missileC.png");
	setTexture(texture);

	setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
	setPosition(pos);
	speed = 1.5f;

	if (radarTexture.loadFromFile("Assets/Sprites/missileRadarIcon.png")) {}
	else radarTexture.loadFromFile("Assets/Debug.png");	//if it fails load placeholder
	radarSprite.setTexture(radarTexture);
	radarSprite.setOrigin(radarTexture.getSize().x / 2, radarTexture.getSize().y / 2);
	radarSprite.setPosition(pos);

	lifeClock.restart();
	boundingBox.setOutlineThickness(2);
	boundingBox.setOutlineColor(sf::Color::Yellow);
	boundingBox.setFillColor(sf::Color::Transparent);

	alive = true;
}

/*Deconstructor*/
InterceptorMissile::~InterceptorMissile()
{

}

/*Check the euclidean distance to the player*/
void InterceptorMissile::checkRangeToPlayer(sf::Vector2f playerPos)
{
	distanceToPlayer = sqrtf((((playerPos.x - getPosition().x)*(playerPos.x - getPosition().x)) + ((playerPos.y - getPosition().y)*(playerPos.y - getPosition().y))));
}

/*Update*/
void InterceptorMissile::Update(sf::Vector2f targetPos)
{
	if (lifeClock.getElapsedTime().asSeconds() < 10)	{
		Seek(targetPos);
	}
	else	{
		Reset();
	}

	timeAlive = lifeClock.getElapsedTime().asSeconds();
}

/*Seek the position that is passed in(i.e. match this position and face the direction being travelled in)*/
void InterceptorMissile::Seek(sf::Vector2f targetPos) {
	dirMove = sf::Vector2f(targetPos - getPosition());
	float length = sqrtf((dirMove.x * dirMove.x) + (dirMove.y * dirMove.y));

	dirMove.x /= length;
	dirMove.y /= length;

	velocity = dirMove*speed;
	setPosition(getPosition() + velocity);

	//sort out the missiles orientation
	float angle;

	angle = acos(dirMove.x);
	angle *= (180 / 3.14);
	if (getPosition().y < targetPos.y)
		setRotation(angle);
	else setRotation(-angle);
}

/*Draw the missiles radar icon*/
void InterceptorMissile::drawRadarIcon(sf::RenderTarget & window)
{
	radarSprite.setPosition(getPosition());
	radarSprite.setRotation(getRotation());
	window.draw(radarSprite);
}

/*Get how long the missile has been alive for*/
float InterceptorMissile::getTimeAlive()
{
	return timeAlive;
}

/*Check if the missile is still alive*/
bool InterceptorMissile::CheckIfAlive()
{
	return alive;
}
bool InterceptorMissile::CheckIfActive()
{
	return active;
}

/*set the alive status of the missile*/
void InterceptorMissile::SetAliveStatus(bool a)
{
	alive = a;
}
void InterceptorMissile::SetActiveStatus(bool a)
{
	active = a;
}
void InterceptorMissile::DrawBoundingBox(sf::RenderTarget & window)
{
	boundingBox.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
	boundingBox.setSize(sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height));
	window.draw(boundingBox);
}

void InterceptorMissile::Reset() {
	setPosition(default_spawn);
	alive = false;
	active = false;
}
void InterceptorMissile::Launch(sf::Vector2f launch_pos) {
	setPosition(launch_pos);
	alive = true;
	active = true;
	timeAlive = 0;
	lifeClock.restart();
}

