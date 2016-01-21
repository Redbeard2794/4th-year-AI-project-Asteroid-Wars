#ifndef _PREDATOR_CONTROLLER_HPP
#define _PREDATOR_CONTROLLER_HPP

#include "Predator.hpp"
#include "Player.h"
#include "ExplosionController.h"
#include "PowerupController.h"

class PredatorController {
private:
	int num_active;
	std::vector<Predator*> container;
public:
	PredatorController();
	~PredatorController();

	void update(Player *p, ExplosionController * ec, std::vector<Obstacle*> *o);
	void Add(sf::Vector2f position);
	void Cull();
	void DrawShips(sf::RenderWindow& w, bool debug);
	void DrawRadar(sf::RenderWindow& w);
	void CheckPowerUp(PowerupController *pu);
};

#endif
