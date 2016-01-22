#include "stdafx.h"
#include "Player.h"
#include "PixelPerfect.hpp"

Player::Player() : speed(0.0f), score(0), velocity(sf::Vector2f(1, 1)), health(100)
{
	//load the correct texture or load the debug texture if something is wrong
	if (mTexture.loadFromFile("Assets/Sprites/Player/player.png")){}
	else mTexture.loadFromFile("Assets/Debug.png");	//if it fails load placeholder
	setOrigin(sf::Vector2f(mTexture.getSize().x / 2, mTexture.getSize().y / 2));
	setTexture(mTexture);
	setPosition(3200, 2400);

	playerTextureImage.loadFromFile("Assets/Sprites/Player/player.png");

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
	reusingBullets = false;

	shieldActive = false;
	speedBoostActive = false;

	if (shieldActiveTexture.loadFromFile("Assets/Sprites/Player/playerShieldActive.png"));
	else shieldActiveTexture.loadFromFile("Assets/Debug.png");
	if (speedBoostTexture.loadFromFile("Assets/Sprites/Player/playerSpeedBoostActive.png"));
	else speedBoostTexture.loadFromFile("Assets/Debug.png");

	shieldBurnDownClock.restart();
	speedBoostBurnDownClock.restart();

	maxSpeed = 1.5f;

	//thruster stuff
	if (thrusterTexture.loadFromFile("Assets/Sprites/Player/playerThruster.png")) {}
	else thrusterTexture.loadFromFile("Assets/Debug.png");
	thrusterSprite.setTexture(thrusterTexture);
	thrusterSprite.setOrigin(thrusterTexture.getSize().x / 2, thrusterSprite.getOrigin().y);
	thrusterSprite.setPosition(getPosition().x - (mTexture.getSize().x / 2), getPosition().y);

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

	if(inactiveBullets <= 0)
		reusingBullets = true;

	if (shieldBurnDownClock.getElapsedTime().asSeconds() > 6 && shieldActive == true)
	{
		shieldActive = false;
		setTexture(mTexture);
	}
	

	if (speedBoostBurnDownClock.getElapsedTime().asSeconds() > 6 && speedBoostActive == true)
	{
		speedBoostActive = false;
		setTexture(mTexture);
		maxSpeed = 1.5f;
	}

}//end Update()

void Player::Move()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (speed < maxSpeed)
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		Turn(-1);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
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
		inactiveBullets -= 1;
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

//respawn the player if they died
void Player::Respawn()
{
	health = 100;
	setPosition(3200, 2400);//or whatever position we decide the player should spawn at
	speed = 0;
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

/*
Check if a bullet has collided with something.
Param is the bounds of the object you want to check
*/
bool Player::CheckBulletsCollision(sf::FloatRect boundsOfObjectToCheck, const sf::Sprite& spriteB, const sf::Image& imgB)
{
	for (int i = 0; i < bullets.size(); i++)
	{
		if (boundsOfObjectToCheck.intersects(bullets.at(i)->getGlobalBounds()))
		{
			//do per pixel detection here
			if (PerPixelCollisionManager::GetInstance()->PixelPerfectCollision(*bullets.at(i), spriteB, bullets.at(i)->GetTextureImage(), imgB))
			{
				bullets.at(i)->SetAliveStatus(false);
				return true;
			}
		}
	}

	return false;
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
	for (int i = 0; i < bullets.size(); i++)
	{
		bullets.at(i)->DrawRadarIcon(window);
	}
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

void Player::DrawThruster(sf::RenderTarget & window)
{
	thrusterSprite.setPosition(getPosition().x - (mTexture.getSize().x / 2), getPosition().y);
	//thrusterSprite.setRotation(getRotation());
	window.draw(thrusterSprite);
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

bool Player::IsReusingBullets()
{
	return reusingBullets;
}

int Player::GetInactiveBullets()
{
	return inactiveBullets;
}

sf::Vector2f Player::getCenter() {
	sf::Vector2f pos = getPosition();
	sf::Vector2u size = getTexture()->getSize();
	
	sf::Vector2f center = sf::Vector2f(pos.x + size.x /2, pos.y + size.y /2);

	return center;
}

void Player::setHealth(float h)
{
	health = h;
}

void Player::SetShieldActive(bool sa)
{
	shieldBurnDownClock.restart();
	shieldActive = sa;
	setTexture(shieldActiveTexture);
}

void Player::SetSpeedBoostActive(bool sba)
{
	speedBoostBurnDownClock.restart();
	maxSpeed = 3.0f;
	speedBoostActive = sba; 
	setTexture(speedBoostTexture);
}

