#include "stdafx.h"
#include "Player.h"

Player::Player() : speed(0.0f), score(0), velocity(sf::Vector2f(1, 1)), health(100)
{
	//load the correct texture or load the debug texture if something is wrong
	if (mTexture.loadFromFile("Assets/Sprites/Player/player.png")){}
	else mTexture.loadFromFile("Assets/Debug.png");	//if it fails load placeholder
	setOrigin(sf::Vector2f(mTexture.getSize().x / 2, mTexture.getSize().y / 2));
	setTexture(mTexture);
	setPosition(5000, 200);

	if (radarTexture.loadFromFile("Assets/Sprites/Player/playerRadarIcon4.png")) {}
	else radarTexture.loadFromFile("Assets/Debug.png");
	radarSprite.setOrigin(sf::Vector2f(radarTexture.getSize().x / 2, radarTexture.getSize().y / 2));
	radarSprite.setTexture(radarTexture);

	health = 100;
	boundingBox.setOutlineThickness(2);
	boundingBox.setOutlineColor(sf::Color(20, 69, 247, 255));
	boundingBox.setFillColor(sf::Color::Transparent);

	bullets.reserve(10);//can have a max of 10 bullets on the go
	inactiveBullets = 10;
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
}

void Player::Shoot()
{
	std::cout << "Player Shoot() called" << std::endl;

	if (bullets.size() < 10)//if we have not used 10 bullets yet
	{
		Bullet* b = new Bullet(direction, 10, getPosition(), getRotation());//create a new one
		bullets.push_back(b);
	}
	else//reuse an old bullet
	{
		for (int i = 0; i < bullets.size(); i++)
		{
			if (bullets.at(i)->IsAlive() == false)
			{
				bullets.at(i)->Reset(direction, 10, getPosition(), getRotation());
				break;
			}
		}
	}

}

/*Check the amount of currently inactive bullets*/
int Player::CheckInactiveBullets()
{
	int total = 0;

	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets.at(i)->IsAlive() == false)
			total += 1;
	}

	return total;
}

void Player::Turn(float a)
{
	rotate(a);
}

void Player::drawRadarIcon(sf::RenderTarget& window)
{
	radarSprite.setRotation(getRotation());
	radarSprite.setPosition(getPosition());
	window.draw(radarSprite);
}

void Player::DrawBoundingBox(sf::RenderTarget & window)
{
	boundingBox.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
	boundingBox.setSize(sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height));
	window.draw(boundingBox);
}

void Player::DrawBullets(sf::RenderTarget & window, bool debugMode)
{
	for (int i = 0; i < bullets.size(); i++)
	{
		bullets.at(i)->Update();
		window.draw(*bullets.at(i));
		if (debugMode)
			bullets.at(i)->DrawBoundingBox(window);
	}
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

sf::Vector2f Player::getDirection()
{
	return direction;
}

sf::Vector2f Player::getCenter() {
	sf::Vector2f pos = getPosition();
	sf::Vector2u size = radarTexture.getSize();
	
	sf::Vector2f center = sf::Vector2f(pos.x + size.x, pos.y + size.y);

	return center;
}

void Player::setHealth(float h)
{
	health = h;
}

