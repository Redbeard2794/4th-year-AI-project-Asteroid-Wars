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

	/*Constructor*/
	PowerupController();

	/*destructor*/
	~PowerupController();

	/*Update all active powerups and check for removeable ones*/
	void Update(sf::Vector2f playerPos);

	/*Add a new powerup at a random position*/
	void AddPowerup(sf::Vector2f playerPos);

	/*Remove a powerup that has been collected*/
	void RemovePowerup();

	bool CheckCollisionsWithPlayer(sf::FloatRect playerBoundingBox);

	/*Draw Powerups*/
	void DrawPowerups(sf::RenderTarget& window);

	/*Draw the radar icons*/
	void DrawRadarIcons(sf::RenderTarget& window);

	/*Check the number of active powerups*/
	int CheckActivePowerupsCount();

	/*get the previously collected powerup type*/
	int getPrevCollectedPowerupType();

};

#endif
