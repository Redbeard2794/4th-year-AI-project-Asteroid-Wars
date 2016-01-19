#include "stdafx.h"
#include "Powerup.h"

/*constructor*/
Powerup::Powerup(sf::Vector2f playerPos)
{
	type = rand() % 3 + 1;

	if (type == HEALTH)
	{
		if (texture.loadFromFile("Assets/powerups/healthPowerup.png")) {}
		else texture.loadFromFile("Assets/Debug.png");
	}
	else if (type == SHIELD)
	{
		if (texture.loadFromFile("Assets/powerups/shieldPowerup.png")) {}
		else texture.loadFromFile("Assets/Debug.png");
	}
	else if (type == SPEED)
	{
		if (texture.loadFromFile("Assets/powerups/speedPowerup.png")) {}
		else texture.loadFromFile("Assets/Debug.png");
	}

	setTexture(texture);
	setOrigin(sf::Vector2f(texture.getSize().x / 2, texture.getSize().y / 2));

	//setPosition(rand() % 6200 + 200, rand() % 4600 + 200);

	float xChange = rand() % 500 + 100;
	float yChange = rand() % 500 + 100;

	//throw in something different(make a distance to wander negative to go in the opposite direction some times)
	int negCoin = rand() % 2;
	if (yChange == 0 && negCoin == 1)
		xChange = -xChange;
	else if (xChange == 0 && negCoin == 0)
		yChange = -yChange;

	//set the next position to wander to
	//wanderPos = sf::Vector2f(getPosition().x + xChange, getPosition().y + yChange);

	setPosition(playerPos.x + xChange, playerPos.y + yChange);

	scale(1.5, 1.5);

	if (radarTexture.loadFromFile("Assets/powerups/powerupRadarIcon.png")) {}
	else radarTexture.loadFromFile("Assets/Debug.png");
	radarSprite.setTexture(radarTexture);
	radarSprite.setOrigin(radarTexture.getSize().x / 2, radarTexture.getSize().y / 2);

	boundingBox.setOutlineThickness(2);
	boundingBox.setOutlineColor(sf::Color::Black);
	boundingBox.setFillColor(sf::Color::Transparent);

	removeable = false;
}

/*destructor*/
Powerup::~Powerup()
{
	texture.~Texture();//destruct the texture
}

/*update spins the powerup*/
void Powerup::Update()
{
	rotate(0.7);
}

/*draw the bounding box representation*/
void Powerup::DrawBoundingBox(sf::RenderTarget& window)
{
	boundingBox.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
	boundingBox.setSize(sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height));
	window.draw(boundingBox);
}

/*draw the radar icon*/
void Powerup::DrawRadarIcon(sf::RenderTarget& window)
{
	radarSprite.setPosition(getPosition());
	radarSprite.setRotation(getRotation());
	window.draw(radarSprite);
}

/*get the type of this powerup*/
int Powerup::getType()
{
	return type;
}

/*Check if the powerup can be removed*/
bool Powerup::isRemoveable()
{
	return removeable;
}

/*set whether the powerup should be removed*/
void Powerup::SetRemoveableStatus(bool r)
{
	removeable = r;
}
