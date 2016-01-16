#include "stdafx.h"
#include "Explosion.h"

/*
constructor
param is position of explosion
*/
Explosion::Explosion(sf::Vector2f pos)
{
	setPosition(pos);

	type = rand() % 4 + 1;
	std::cout << "Type: " << type << std::endl;

	if (texture.loadFromFile("Assets/Explosions/explosionSheet" + std::to_string(type) + ".png")) {}
	else texture.loadFromFile("Assets/Debug.png");

	setTexture(texture);

	if (type == 1)
	{
		numFrames = 12;
		animationTime = 0.2f;
	}
	else if (type == 2)
	{
		numFrames = 12;
		animationTime = 0.05f;
	}
	else if (type == 3)
	{
		numFrames = 9;
		animationTime = 0.15f;
	}
	else if (type == 4)
	{
		numFrames = 17;
		animationTime = 0.12f;
	}


	finishedAnimation = false;

	

	fadeOutSpeed = 5;

	animationClock.restart();

	//set up the texture rectangle
	framePosition = sf::Vector2i(0, 0);

	frameSize = sf::Vector2i(192, 116);

	frame = sf::IntRect(framePosition, frameSize);

	setTextureRect(frame);

	removable = false;//this explosion is not ready to remove
}

/*destructor*/
Explosion::~Explosion()
{
	texture.~Texture();//destruct the texture
}

void Explosion::Update()
{
	if (animationClock.getElapsedTime().asSeconds() > animationTime && finishedAnimation == false)
	{
		if (framePosition.x < texture.getSize().x - frameSize.x)
			framePosition.x += frameSize.x;//move the frame forward

		else//the animation is over
		{
			finishedAnimation = true;
		}

		animationClock.restart();
	}

	if (finishedAnimation == true)//if the animation is over
	{
		if (getColor().a > 0)//fade out
		{
			setColor(sf::Color(getColor().r, getColor().g, getColor().b, getColor().a - fadeOutSpeed));
		}
		else removable = true;//the explosion can now be safely removed
	}

	//reset the texture rectangle
	frameSize = sf::Vector2i(texture.getSize().x / numFrames, texture.getSize().y);
	frame = sf::IntRect(framePosition, frameSize);
	setTextureRect(frame);
	setOrigin(frameSize.x / 2, frameSize.y / 2);
}

bool Explosion::IsRemoveable()
{
	return removable;
}
