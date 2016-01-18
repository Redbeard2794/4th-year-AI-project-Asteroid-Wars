#include "stdafx.h"
#include "Powerup.h"

Powerup::Powerup()
{
	type = rand() % 3 + 1;

	if (type == HEALTH)
	{
		texture.loadFromFile("");
	}
	else if (type == SHIELD)
	{
		texture.loadFromFile("");
	}
	else if (type == SPEED)
	{
		texture.loadFromFile("");
	}

	setTexture(texture);
	setOrigin(sf::Vector2f(texture.getSize().x / 2, texture.getSize().y / 2));

	boundingBox.setOutlineThickness(2);
	boundingBox.setOutlineColor(sf::Color(20, 69, 247, 255));
	boundingBox.setFillColor(sf::Color::Transparent);
}

Powerup::~Powerup()
{

}

void Powerup::Update()
{

}

void Powerup::DrawBoundingBox(sf::RenderTarget& window)
{
	boundingBox.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
	boundingBox.setSize(sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height));
	window.draw(boundingBox);
}

void Powerup::DrawRadarIcon(sf::RenderTarget& window)
{

}

int Powerup::getType()
{
	return type;
}
