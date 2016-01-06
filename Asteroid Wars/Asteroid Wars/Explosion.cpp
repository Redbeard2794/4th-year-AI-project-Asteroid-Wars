#include "stdafx.h"
#include "Explosion.h"

/*
constructor
param is position of explosion
*/
Explosion::Explosion(sf::Vector2f pos)
{
	setPosition(pos);

	if (texture.loadFromFile("Assets/Explosions/explosionSheet.png")) {}
	else texture.loadFromFile("Assets/Debug.png");

	setTexture(texture);

	numFrames = 12;

	finishedAnimation = false;

	animationTime = 0.2f;

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
