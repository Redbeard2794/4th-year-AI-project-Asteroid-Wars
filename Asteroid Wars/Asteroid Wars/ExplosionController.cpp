#include "stdafx.h"
#include "ExplosionController.h"

/*Constructor*/
ExplosionController::ExplosionController()
{
	activeExplosions = 0;
}

/*destructor*/
ExplosionController::~ExplosionController()
{
	//blank for the moment
}

/*Update all active explosions and check for removeable ones*/
void ExplosionController::Update()
{
	activeExplosions = explosions.size();

	for (int i = 0; i < explosions.size(); i++)
	{
		explosions.at(i)->Update();
	}

	RemoveExplosion();
}

/*
Add a new explosion
param is the position for the explosion
*/
void ExplosionController::AddExplosion(sf::Vector2f pos)
{
	Explosion* e = new Explosion(pos);
	explosions.push_back(e);
	//std::cout << "Added a new explosion." << std::endl;
}

/*Remove an explosion that has finished its animation and faded out*/
void ExplosionController::RemoveExplosion()
{
	for (auto it = explosions.begin(); it != explosions.end();)
	{
		if ((*it)->IsRemoveable())//if the explosion can be removed
		{
			delete * it;//delete the pointer
			it = explosions.erase(it);//erase the object(calls the objects destructor)
			//std::cout << "Removed an explosion." << std::endl;
		}
		else ++it;
	}
}

/*Draw Explosions*/
void ExplosionController::DrawExplosions(sf::RenderTarget& window)
{
	for (int i = 0; i < explosions.size(); i++)
	{
		window.draw(*explosions.at(i));
	}
}

int ExplosionController::CheckActiveExplosionCount()
{
	return activeExplosions;
}
