#include "stdafx.h"
#include "Obstacle.h"

Obstacle::Obstacle(int type, sf::Vector2f pos)
{
	if(type == 1)
		texture.loadFromFile("Assets/Sprites/Asteriods/cleanAsteriodSheet.png");
	else if(type == 2)
		texture.loadFromFile("Assets/Sprites/Asteriods/metalAsteriodSheet.png");

	setTexture(texture);
	//setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);

	setPosition(pos);

	//set up the texture rectangle
	framePosition = sf::Vector2i(0, 0);
	if(type == 1)
		frameSize = sf::Vector2i(122, 124);
	else if(type == 2)
		frameSize = sf::Vector2i(123, 122);

	frame = sf::IntRect(framePosition, frameSize);
	setTextureRect(frame);

	animationTime = 0.2;
	animationClock.restart();

	boundingBox.setOutlineThickness(2);
	boundingBox.setOutlineColor(sf::Color::White);
	boundingBox.setFillColor(sf::Color::Transparent);

	if (radarIconTexture.loadFromFile("Assets/Sprites/Asteriods/ObstacleRadarIcon.png")) {}
	else radarIconTexture.loadFromFile("Assets/Debug.png");
	radarIconSprite.setTexture(radarIconTexture);
	radarIconSprite.setOrigin(radarIconTexture.getSize().x / 2, radarIconTexture.getSize().y / 2);
	radarIconSprite.setPosition(getPosition());

	velocity = sf::Vector2f(rand() % 3 + 1, rand() % 3 + 1);
}

Obstacle::~Obstacle()
{

}

void Obstacle::Update()
{
	if (animationClock.getElapsedTime().asSeconds() > animationTime)
	{
		if (framePosition.x < texture.getSize().x - frameSize.x)
			framePosition.x += frameSize.x;//move the frame forward

		else
		{
			framePosition.x = 0;
		}

		animationClock.restart();
	}

	//reset the texture rectangle
	frameSize = sf::Vector2i(texture.getSize().x / 5, texture.getSize().y);
	frame = sf::IntRect(framePosition, frameSize);
	setTextureRect(frame);
	setOrigin(frameSize.x / 2, frameSize.y / 2);

	setPosition(getPosition() + velocity);

	BoundaryDetection();
}

void Obstacle::BoundaryDetection()
{
	//right
	if ((getPosition().x + (getTexture()->getSize().x / 2)) > 6400)
	{
		setPosition(sf::Vector2f(0 + 75, getPosition().y));
	}
	//left
	if ((getPosition().x - (getTexture()->getSize().x / 2)) < 0)
	{
		setPosition(sf::Vector2f(6400 - 75, getPosition().y));
	}
	//bottom
	if ((getPosition().y + (getTexture()->getSize().y / 2)) > 4800)
	{
		setPosition(sf::Vector2f(getPosition().x, 0 + 75));
	}
	//top
	if ((getPosition().y - (getTexture()->getSize().y / 2)) < 0)
	{
		setPosition(sf::Vector2f(getPosition().x, 4800 - 75));
	}
}

void Obstacle::DrawBoundingBox(sf::RenderTarget & window)
{
	boundingBox.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
	boundingBox.setSize(sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height));
	window.draw(boundingBox);
}

void Obstacle::DrawRadarIcon(sf::RenderTarget & window)
{
	radarIconSprite.setRotation(getRotation());
	radarIconSprite.setPosition(getPosition());
	window.draw(radarIconSprite);
}

sf::Vector2f Obstacle::GetVelocity()
{
	return velocity;
}
