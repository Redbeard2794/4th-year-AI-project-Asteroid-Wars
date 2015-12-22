#include "stdafx.h"
#include "Obstacle.h"

Obstacle::Obstacle(int type, sf::Vector2f pos)
{
	if(type == 1)
		texture.loadFromFile("Assets/Sprites/Asteriods/cleanAsteriodSheet.png");
	else if(type == 2)
		texture.loadFromFile("Assets/Sprites/Asteriods/metalAsteriodSheet.png");

	setTexture(texture);
	setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);

	setPosition(pos);

	//set up the texture rectangle
	framePosition = sf::Vector2i(0, 0);
	if(type == 1)
		frameSize = sf::Vector2i(122, 124);
	else if(type == 2)
		frameSize = sf::Vector2i(123, 122);

	frame = sf::IntRect(framePosition, frameSize);
	setTextureRect(frame);

	animationTime = 0.3;
	animationClock.restart();

	boundingBox.setOutlineThickness(2);
	boundingBox.setOutlineColor(sf::Color::White);
	boundingBox.setFillColor(sf::Color::Transparent);
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
}

void Obstacle::DrawBoundingBox(sf::RenderTarget & window)
{
	boundingBox.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
	boundingBox.setSize(sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height));
	window.draw(boundingBox);
}
