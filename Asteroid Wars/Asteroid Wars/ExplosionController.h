#ifndef EXPLOSIONCONTROLLER_H
#define EXPLOSIONCONTROLLER_H

#include "Explosion.h"

class ExplosionController
{
private:
	std::vector<Explosion*> explosions;

	int activeExplosions;

public:
	/*Constructor*/
	ExplosionController();

	/*destructor*/
	~ExplosionController();

	/*Update all active explosions and check for removeable ones*/
	void Update();

	/*
	Add a new explosion
	param is the position for the explosion
	*/
	void AddExplosion(sf::Vector2f pos);

	/*Remove an explosion that has finished its animation and faded out*/
	void RemoveExplosion();

	/*Draw Explosions*/
	void DrawExplosions(sf::RenderTarget& window);

	int CheckActiveExplosionCount();
};

#endif
