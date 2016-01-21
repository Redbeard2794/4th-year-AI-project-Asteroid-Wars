#include "stdafx.h"
#include "PredatorController.hpp"

PredatorController::PredatorController() {

}
PredatorController::~PredatorController() {

}

void PredatorController::update(Player *p, ExplosionController * ec, std::vector<Obstacle*> *o) {
	Cull();
	for (int i = 0; i < container.size(); i++) {
		container.at(i)->update(&container, p, ec, o);
	}

	//Check Predators for Colisions
	for (int i = 0; i < container.size(); i++) {
		if (container[i]->isAlive()) {
			//Preditor Collide with Player Check
			if (p->getGlobalBounds().intersects(container[i]->getGlobalBounds()) == true) {
				ec->AddExplosion(container[i]->getPosition());
				container[i]->Destroy();
				if (p->IsShieldActive() == false)
					p->setHealth((p->getHealth() - 35));
				std::cout << "Predator Colided with player and dealt 35 damage. Player now has " << p->getHealth() << " health." << std::endl;
			}
			//Preditor Collide with Player Bullets
			if (p->CheckBulletsCollision(container.at(i)->getGlobalBounds()) == true)
			{
				ec->AddExplosion(container.at(i)->getPosition());
				container.at(i)->Destroy();
				std::cout << "Players Bullet destroyed predator at index" << i << std::endl;
			}
			//Preditor Collide with obsticles check
			for (int j = 0; j < o->size(); j++) {
				if (container[i]->getGlobalBounds().intersects(o->at(j)->getGlobalBounds())) {
					ec->AddExplosion(container[i]->getPosition());
					container[i]->Destroy();
					std::cout << "Obstacle with index " << j << " was hit with Predator at index " << i << std::endl;
				}
			}
		}
	}
}

void PredatorController::Add(sf::Vector2f position) {
	container.push_back(new Predator(position));
}

void PredatorController::Cull() {
	//Loop through all of the Predators
	for (auto it = container.begin(); it != container.end();)	{
		//if the predator is not alive
		if (!(*it)->isAlive() && (*it)->canDespawn()) {
			delete * it;				//delete the pointer
			it = container.erase(it);	//erase the object(calls the objects destructor)
			std::cout << "Removed a Predator." << std::endl;
		}
		else ++it;
	}
}

void PredatorController::DrawShips(sf::RenderWindow & w, bool debug) {
	for (int i = 0; i < container.size(); i++)	{
		w.draw(*container.at(i));
		if (debug)
			container.at(i)->drawDebug(w);
		container.at(i)->drawBullets(w);		//draw each ships bullets, I know its not efficent but it works !
	}
}

void PredatorController::DrawRadar(sf::RenderWindow & w) {
	for (int i = 0; i < container.size(); i++) {
		container.at(i)->drawRadarIcon(w);
	}
}

void PredatorController::CheckPowerUp(PowerupController * pu) {
	for (int i = 0; i < container.size(); i++)	{
		if (pu->CheckCollisionsWithPlayer(container[i]->getGlobalBounds())) {
			if (pu->getPrevCollectedPowerupType() == 1)
			{
				std::cout << "Predator stole a health powerup." << std::endl;
				container[i]->setHealth(container[i]->getHealth() + 25);
			}
			else if (pu->getPrevCollectedPowerupType() == 2)
			{
				std::cout << "Predator stole a shield powerup." << std::endl;
				container[i]->SetShieldActive(true);
			}
			else if (pu->getPrevCollectedPowerupType() == 3)
			{
				std::cout << "Predator stole a speed powerup." << std::endl;
				container[i]->SetSpeedBoostActive(true);
			}
		}
	}
}

