#ifndef POWERUPCONTROLLER_H
#define POWERUPCONTROLLER_H

#include "Powerup.h"

class PowerupController
{
private:

	std::vector<Powerup*> powerups;

	int activePowerups;

	sf::Clock addPowerupClock;

	int timeBeforeAddPowerup;

	int prevCollectedPowerupType;

public:

	//!Constructor
	PowerupController();

	//!destructor
	~PowerupController();

	//!Update all active powerups and check for removeable ones. param is the player's position
	void Update(sf::Vector2f playerPos);

	//!Add a new powerup at a random position relative to the player's position. param is: the player's position
	void AddPowerup(sf::Vector2f playerPos);

	//!Remove a powerup that has been collected
	void RemovePowerup();

	//! Check to see if the player has collided with a powerup. param is: the player's bounding box
	bool CheckCollisionsWithPlayer(sf::FloatRect playerBoundingBox);

	//!Draw Powerups. params are: a renderTarget and whether we are in debug mode or not
	void DrawPowerups(sf::RenderTarget& window, bool debugMode);

	//!Draw the radar icons. param is a renderTarget
	void DrawRadarIcons(sf::RenderTarget& window);

	//!Check the number of active powerups
	int CheckActivePowerupsCount();

	//!get the previously collected powerup type
	int getPrevCollectedPowerupType();

};

#endif
