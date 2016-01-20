#include "stdafx.h"
#include "PowerupController.h"

/*Constructor*/
PowerupController::PowerupController()
{
	activePowerups = 0;

	addPowerupClock.restart();

	timeBeforeAddPowerup = rand() % 15 + 5;

	prevCollectedPowerupType = 0;
}

/*destructor*/
PowerupController::~PowerupController()
{
	//blank for the moment
}

/*Update all active powerups and check for removeable ones*/
void PowerupController::Update(sf::Vector2f playerPos)
{
	activePowerups = powerups.size();

	for (int i = 0; i < powerups.size(); i++)
	{
		powerups.at(i)->Update();
	}

	if (addPowerupClock.getElapsedTime().asSeconds() > timeBeforeAddPowerup)
	{
		if(activePowerups < 10)
			AddPowerup(playerPos);
		timeBeforeAddPowerup = rand() % 15 + 5;
		addPowerupClock.restart();
	}

	RemovePowerup();
}

/*Add a new powerup at a random position*/
void PowerupController::AddPowerup(sf::Vector2f playerPos)
{
	Powerup* p = new Powerup(playerPos);
	powerups.push_back(p);
	std::cout << "Added a new powerup." << std::endl;
}

/*Remove a powerup that has been collected*/
void PowerupController::RemovePowerup()
{
	for (auto it = powerups.begin(); it != powerups.end();)
	{
		if ((*it)->isRemoveable())//if the explosion can be removed
		{
			delete * it;//delete the pointer
			it = powerups.erase(it);//erase the object(calls the objects destructor)
			std::cout << "Removed a powerup." << std::endl;
		}
		else ++it;
	}
}

bool PowerupController::CheckCollisionsWithPlayer(sf::FloatRect playerBoundingBox)
{
	for (int i = 0; i < powerups.size(); i++)
	{
		if (powerups.at(i)->getGlobalBounds().intersects(playerBoundingBox))
		{
			prevCollectedPowerupType = powerups.at(i)->getType();
			powerups.at(i)->SetRemoveableStatus(true);
			return true;
		}
	}
	return false;
}

/*Draw Powerups*/
void PowerupController::DrawPowerups(sf::RenderTarget& window, bool debugMode)
{
	for (int i = 0; i < powerups.size(); i++)
	{
		window.draw(*powerups.at(i));
		if(debugMode)
			powerups.at(i)->DrawBoundingBox(window);
	}
}

/*Draw the radar icons*/
void PowerupController::DrawRadarIcons(sf::RenderTarget & window)
{
	for (int i = 0; i < powerups.size(); i++)
	{
		powerups.at(i)->DrawRadarIcon(window);
	}
}

/*Check the number of active powerups*/
int PowerupController::CheckActivePowerupsCount()
{
	return activePowerups;
}

/*get the previously collected powerup type*/
int PowerupController::getPrevCollectedPowerupType()
{
	return prevCollectedPowerupType;
}
