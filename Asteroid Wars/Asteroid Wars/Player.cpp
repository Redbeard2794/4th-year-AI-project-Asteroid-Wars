#include "stdafx.h"
#include "Player.h"



Player::Player() :speed(0.0f), score(0), velocity(sf::Vector2f(1, 1)), health(100)
{
	//load the correct texture or load the debug texture if something is wrong
	if (mTexture.loadFromFile("Assets/Sprites/Player/player.png")){}
	else mTexture.loadFromFile("Assets/Debug.png");	//if it fails load placeholder
	mSprite.setOrigin(sf::Vector2f(mTexture.getSize().x / 2, mTexture.getSize().y / 2));
	mSprite.setTexture(mTexture);
	setPosition(5000, 200);

	if (radarTexture.loadFromFile("Assets/Sprites/Player/playerRadarIcon4.png")) {}
	else radarTexture.loadFromFile("Assets/Debug.png");
	radarSprite.setOrigin(sf::Vector2f(radarTexture.getSize().x / 2, radarTexture.getSize().y / 2));
	radarSprite.setTexture(radarTexture);

	health = 100;

}//end constructor


Player::~Player()
{
	//blank destructor for now
}

void Player::Update(sf::Vector2f backgroundPos, sf::Vector2u bGroundSize, vCamera *cam)
{
	Move();
	//Get direction based on the rotation of the sprite(in radians because sin and cos need radians and sfml rotations work in degrees)
	direction = sf::Vector2f(cos(getRotation()*(3.14 / 180.0)), sin(getRotation()*(3.14 / 180.0)));
	//normalise the direction vector by dividing it by its own length
	direction /= std::sqrt((direction.x * direction.x) + (direction.y * direction.y));
	//set velocity to be direction by speed
	velocity = direction*speed;

	if (boundary(backgroundPos, bGroundSize))
		cam->setDefaults();

}//end Update()

void Player::Move()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		if (speed < 1.5f)
		{
			speed += 0.1f;
		}
	}
	else
	{
		if (speed > 0)
			speed -= 0.008f;//fiddle with this number to change deceleration
		else if (speed < 0)
			speed = 0;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		Turn(-1);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		Turn(1);
	}

	setPosition(getPosition() + (velocity));
}//end Move()

void Player::Turn(float a)
{
	rotate(a);
}



void Player::draw(sf::RenderTarget& window, sf::RenderStates state) const
{

}//end Draw()

void Player::draw(sf::RenderTarget& window)
{
	window.draw(mSprite, getTransform());
}

void Player::drawRadarIcon(sf::RenderTarget& window)
{
	window.draw(radarSprite, getTransform());
}

bool Player::boundary(sf::Vector2f backgroundPos, sf::Vector2u bGroundSize)
{
	bool trip = false;
	//right
	if ((getPosition().x + (mTexture.getSize().x/2)) > bGroundSize.x)
	{
		setPosition(sf::Vector2f(backgroundPos.x + 75, getPosition().y));
		trip = true;
	}
	//left
	if ((getPosition().x - (mTexture.getSize().x / 2)) < backgroundPos.x)
	{
		setPosition(sf::Vector2f(bGroundSize.x - 75, getPosition().y));
		trip = true;
	}
	//bottom
	if ((getPosition().y + (mTexture.getSize().y/2)) > bGroundSize.y)
	{
		setPosition(sf::Vector2f(getPosition().x, backgroundPos.y+75));
		trip = true;
	}
	//top
	if ((getPosition().y - (mTexture.getSize().y/2)) < backgroundPos.y)
	{
		setPosition(sf::Vector2f(getPosition().x, bGroundSize.y - 75));
		trip = true;
	}

	return trip;
}

sf::Vector2f Player::getCenter() {
	sf::Vector2f pos = getPosition();
	sf::Vector2u size = radarTexture.getSize();
	
	sf::Vector2f center = sf::Vector2f(pos.x + size.x, pos.y + size.y);

	return center;
}

