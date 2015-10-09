#include "stdafx.h"
#include "Player.h"



Player::Player() :speed(0.0f), score(0), velocity(sf::Vector2f(1, 1)), health(100)
{
	//load the correct texture or load the debug texture if something is wrong
	if (mTexture.loadFromFile("Assets/Sprites/Player/player.png")){}
	else mTexture.loadFromFile("Assets/Debug.png");	//if it fails load placeholder
	mSprite.setOrigin(sf::Vector2f(mTexture.getSize().x / 2, mTexture.getSize().y / 2));
	mSprite.setTexture(mTexture);
	setPosition(200, 200);

}//end constructor


Player::~Player()
{
	//blank destructor for now
}

void Player::Update()
{
	Move();
	//Get direction based on the rotation of the sprite(in radians because sin and cos need radians and sfml rotations work in degrees)
	direction = sf::Vector2f(cos(getRotation()*(3.14 / 180.0)), sin(getRotation()*(3.14 / 180.0)));
	//normalise the direction vector by dividing it by its own length
	direction /= std::sqrt((direction.x * direction.x) + (direction.y * direction.y));
	//set velocity to be direction by speed
	velocity = direction*speed;

	//boundary();

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
void Player::boundary()
{
	if (getPosition().x > SCREENWIDTH)
	{
		setPosition(sf::Vector2f(20, getPosition().y));
	}
	if (getPosition().x < 0)
	{
		setPosition(sf::Vector2f(SCREENWIDTH - 20, getPosition().y));
	}
	if (getPosition().y > SCREENHEIGHT)
	{
		setPosition(sf::Vector2f(getPosition().x, 20));
	}
	if (getPosition().y < 0)
	{
		setPosition(sf::Vector2f(getPosition().x, SCREENHEIGHT - 20));
	}
}
