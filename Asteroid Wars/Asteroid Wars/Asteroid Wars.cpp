//////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
#include "stdafx.h" 
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 

#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#include <iostream> 
#define _USE_MATH_DEFINES
#include <math.h>
#include "vCamera.hpp"
#include "FactoryShip.hpp"
#include "Predator.hpp"


////////////////////////////////////////////////////////////
///Entrypoint of application 
//////////////////////////////////////////////////////////// 

//convert a sf::FloatRect to an sf::IntRect
sf::IntRect FToIRect(const sf::FloatRect& f)
{
	return sf::IntRect((int)f.left, (int)f.top, (int)f.width, (int)f.height);
}

//pixel perfect collision detection(sprite1, sprite2, texture1, texture2)
bool PixelPerfectCollision(const sf::Sprite& spriteA, const sf::Sprite& spriteB, const sf::Image& imgA, const sf::Image& imgB)
{
	//https://gist.github.com/ernestoalejo/3033095

	sf::IntRect boundsA(FToIRect(spriteA.getGlobalBounds()));//convert spriteA's global bounds to an sf::IntRect

	sf::IntRect boundsB(FToIRect(spriteB.getGlobalBounds()));//convert spriteB's global bounds to an sf::IntRect

	sf::IntRect intersection;//An sf::intRect representing the intersection area

	if (boundsA.intersects(boundsB, intersection))//if boundsA intersects boundsB, with the intersection area stored in intersection
	{
		const sf::Transform& inverseA(spriteA.getInverseTransform());//get spriteA's inverse transform

		const sf::Transform& inverseB(spriteB.getInverseTransform());//get spriteB's inverse transform

		const sf::Vector2u& sizeA(imgA.getSize());//Get the size of imgA

		const sf::Vector2u& sizeB(imgB.getSize());//get the size of imgB

		const sf::Uint8* pixA = imgA.getPixelsPtr();//Get imgA's pixel pointer which is a read-only pointer to its array of pixels

		const sf::Uint8* pixB = imgB.getPixelsPtr();//Get imgB's pixel pointer which is a read-only pointer to its array of pixels

		sf::Vector2f vecA, vecB;

		int xMax = intersection.left + intersection.width;//max x value of intersection

		int yMax = intersection.top + intersection.height;//max y value of intersection

		for (int x = intersection.left; x < xMax; x++)
		{
			for (int y = intersection.top; y < yMax; y++)
			{

				vecA = inverseA.transformPoint(x, y);

				vecB = inverseB.transformPoint(x, y);

				int idxA = ((int)vecA.x + ((int)vecA.y)*sizeA.x) * 4 + 3;

				int idxB = ((int)vecB.x + ((int)vecB.y)*sizeB.x) * 4 + 3;

				if (vecA.x > 0 && vecA.y > 0 &&
					vecB.x > 0 && vecB.y > 0 &&
					vecA.x < sizeA.x && vecA.y < sizeA.y &&
					vecB.x < sizeB.x && vecB.y < sizeB.y &&
					pixA[idxA] > 0 &&
					pixB[idxB] > 0)
				{
					return true;
				}
			}
		}

	}


	return false;
}

int main() {
	/* initialize random seed: */
	srand(time(NULL));//Don't do this anywhere else!

	// Create the main window 
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Josh + Jason Asteroid Wars");
	sf::RenderWindow *pWindow = &window;
	
	//create sf::View
	sf::View player_view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
	vCamera m_camera = vCamera(sf::Vector2f(800, 600), sf::FloatRect(0,0,6400, 4800));

	//minimap
	unsigned int size = 130;//100
	sf::View minimap(sf::FloatRect(player_view.getCenter().x, player_view.getCenter().y, size, window.getSize().y*size / window.getSize().x));
	//change the viewport to change the maps size
	minimap.setViewport(sf::FloatRect(0.6f - (1.f*minimap.getSize().x) / window.getSize().x - 0.10f, 1.f - (1.f*minimap.getSize().y) / window.getSize().y - 0.004f, (2.0f*minimap.getSize().x) / window.getSize().x, (1.f*minimap.getSize().y) / (window.getSize().y)));
	minimap.zoom(8.f);//4

	//load a font
	sf::Font font;
	font.loadFromFile("Assets/space age.ttf");

	Player* p = new Player();

	sf::Sprite background;
	sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile("Assets/Backgrounds/giantBackground.png");
	background.setTexture(backgroundTexture);
	background.setPosition(sf::Vector2f(0, 0));


	window.setFramerateLimit(60);


	sf::Image icon;
	icon.loadFromFile("Assets/icon2.png");
	window.setIcon(32, 32, icon.getPixelsPtr());
	////////////////////////////////////////////////////////////////////////////
	//Create Entites Here
	std::vector<SwarmBoid*> boids;
	sf::Vector2f boidStartPos = sf::Vector2f(rand() % 6200 + 200, rand() % 4600 + 200);
	for (int i = 0; i < 60; i++)
	{
		if (i % 10)
		{
			boidStartPos = sf::Vector2f(rand() % 6200 + 200, rand() % 4600 + 200);
		}
		boids.push_back(new SwarmBoid(boidStartPos));
	}
	std::vector<FactoryShip*> factories;
	factories.push_back(new FactoryShip());
	//factories.push_back(new FactoryShip(sf::Vector2f(5000, 550)));
	
	std::vector<Predator*> predators;
	predators.push_back(new Predator(sf::Vector2f(5100, 400)));
	predators.push_back(new Predator(sf::Vector2f(5000, 400)));
	predators.push_back(new Predator(sf::Vector2f(5200, 400)));
	//Predator predator = Predator(sf::Vector2f(5000, 400));
	////////////////////////////////////////////////////////////////////////////
	Hud* hud = new Hud(font);

	InterceptorMissile* testMissile = new InterceptorMissile(sf::Vector2f(0, 0));

	bool debugMode = false;

	std::vector<Obstacle*> obstacles;
	for (int i = 0; i < 40; i++)
	{
		obstacles.push_back(new Obstacle(rand() % 2 + 1, sf::Vector2f(rand() % 6200 + 200, rand() % 4600 + 200)));
	}

	ExplosionController explosionController = ExplosionController();

	PowerupController powerupController = PowerupController();

	// Start game loop 
	while (window.isOpen())
	{
		// Process events 
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			// Close window : exit 
			if (Event.type == sf::Event::Closed)
				window.close();

			// Escape key : exit 
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
				window.close();

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::M))
				debugMode = !debugMode;

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Space))
				p->Shoot();
		}

		//prepare frame
		window.clear();

		//update sf::View center position
		player_view.setCenter(p->getPosition());
		m_camera.setCenter(m_camera.getPlayerOffset(p->getPosition()));
		//m_camera.setCenter(factories.at(0)->getPosition());
		
		//set view of window to be player_view
		window.setView(m_camera);

		//update the player
		p->Update(background.getPosition(), backgroundTexture.getSize(), &m_camera);

		//draw the background
		window.draw(background);

		explosionController.Update();
		explosionController.DrawExplosions(window);

		//draw and update the test missile
		testMissile->Update(p->getPosition());
		if (testMissile->CheckIfAlive() == true)
		{
			window.draw(*testMissile);
			if(debugMode)
				testMissile->DrawBoundingBox(window);
		}

		for (int i = 0; i < obstacles.size(); i++)
		{
			window.draw(*obstacles.at(i));
			if(debugMode)
				obstacles.at(i)->DrawBoundingBox(window);
			obstacles.at(i)->Update();
		}

		//draw the player and their bullets
		p->DrawBullets(window, debugMode);
		//p->DrawThruster(window);
		window.draw(*p);
		if(debugMode)
			p->DrawBoundingBox(window);

		powerupController.Update(p->getPosition());
		powerupController.DrawPowerups(window, debugMode);

		if (powerupController.CheckCollisionsWithPlayer(p->getGlobalBounds()))//if the player collided with a powerup
		{
			if (powerupController.getPrevCollectedPowerupType() == 1)
			{
				std::cout << "Player got a health powerup." << std::endl;
				p->setHealth(p->getHealth() + 25);
			}
			else if (powerupController.getPrevCollectedPowerupType() == 2)
			{
				std::cout << "Player got a shield powerup." << std::endl;
				p->SetShieldActive(true);
			}
			else if (powerupController.getPrevCollectedPowerupType() == 3)
			{
				std::cout << "Player got a speed powerup." << std::endl;
				p->SetSpeedBoostActive(true);
			}
		}
		
		////////////////////////////////////////////////////////////////////////////
		//Draw and Update Entites Here
		//draw and update the swarm boids
		for (int i = 0; i < boids.size(); i++)
		{
			if (boids.at(i)->CheckIfAlive() == true)
			{
				boids.at(i)->Update(p->getPosition(), p->getVelocity(), boids, obstacles);
				window.draw(*boids.at(i));
				if (debugMode)
					boids.at(i)->DrawBoundingBox(window);
			}
		}

		//Update and Draw Factory
		for (int i = 0; i < factories.size(); i++)
		{
			factories.at(i)->update(p, &factories, &explosionController, &obstacles);
			window.draw(*factories.at(i));
			if(debugMode)
				factories.at(i)->drawDebug(window);
			factories.at(i)->drawMissles(window);
		}
		

		//Update and Draw predator
		for (int i = 0; i < predators.size(); i++)
		{
			predators.at(i)->update(&predators, p, &explosionController, &obstacles);
			window.draw(*predators.at(i));
			if(debugMode)
				predators.at(i)->drawDebug(window);
			predators.at(i)->drawBullets(window);
		}
		////////////////////////////////////////////////////////////////////////////

		//collision detection(basic bounding box collision detection to start with)
		//missile and player
		if (p->getGlobalBounds().intersects(testMissile->getGlobalBounds()) == true)
		{
			explosionController.AddExplosion(testMissile->getPosition());
			testMissile->setPosition(0, 0);//just temporarily
			testMissile->SetAliveStatus(false);

			if(p->IsShieldActive() ==false)
				p->setHealth((p->getHealth() - 35));
			std::cout << "Missile hit player and dealt 35 damage. Player now has " << p->getHealth() << " health." << std::endl;
		}
		//player and obstacles
		for (int i = 0; i < obstacles.size(); i++)
		{
			if (p->getGlobalBounds().intersects(obstacles.at(i)->getGlobalBounds()))
			{
				if (PixelPerfectCollision(*p, *obstacles.at(i), p->getTextureImage(), obstacles.at(i)->getTextureImage()))
				{
					explosionController.AddExplosion(p->getPosition());
					p->setHealth((p->getHealth() - 100));
					std::cout << "Obstacle with index " << i << " hit the player and demolished the player's ship" << std::endl;
				}
			}
		}
		//player's bullets and swarmboids
		for (int i = 0; i < boids.size(); i++)
		{
			if (p->CheckBulletsCollision(boids.at(i)->getGlobalBounds()) == true)
			{
					explosionController.AddExplosion(boids.at(i)->getPosition());
					boids.at(i)->SetAliveStatus(false);
					boids.at(i)->setPosition(-1000, -1000);//just temporarily
			}
		}
		//player and swarmboids
		for (int i = 0; i < boids.size(); i++)
		{
			if (p->getGlobalBounds().intersects(boids.at(i)->getGlobalBounds()))
			{
				if (PixelPerfectCollision(*p, *boids.at(i), p->getTextureImage(), boids.at(i)->getTextureImage()))
				{
					explosionController.AddExplosion(boids.at(i)->getPosition());
					boids.at(i)->SetAliveStatus(false);
					boids.at(i)->setPosition(0, 0);//just temporarily

					if (p->IsShieldActive() == false)
						p->setHealth((p->getHealth() - 35));
					std::cout << "Swarmboid with index " << i << " hit the player." << std::endl;
				}
			}
		}
		//swarmboids and obstacles
		for (int i = 0; i < obstacles.size(); i++)
		{
			for (int j = 0; j < boids.size(); j++)
			{
				if (boids.at(j)->getGlobalBounds().intersects(obstacles.at(i)->getGlobalBounds()))
				{
					if (PixelPerfectCollision(*boids.at(j), *obstacles.at(i), boids.at(j)->getTextureImage(), obstacles.at(i)->getTextureImage()))
					{
						explosionController.AddExplosion(boids.at(j)->getPosition());
						boids.at(j)->SetAliveStatus(false);
						boids.at(j)->setPosition(0, 0);//just temporarily
					}
				}
			}
		}
		//player bullets and obstacles
		for (int i = 0; i < obstacles.size(); i++)
		{
			if (p->CheckBulletsCollision(obstacles.at(i)->getGlobalBounds()) == true)
			{
				explosionController.AddExplosion(obstacles.at(i)->getPosition());
				std::cout << "Player bullet hit obstacle " << i << "." << std::endl;
			}
		}

		//player and predators
		//for (int i = 0; i < predators.size(); i++)
		//{
		//	if (p->getGlobalBounds().intersects(predators.at(i)->getGlobalBounds()))
		//	{
		//		if (PixelPerfectCollision(*p, *predators.at(i), p->getTextureImage(), predators.at(i)->getTextureImage()))
		//		{
		//			explosionController.AddExplosion(predators.at(i)->getPosition());
		//			predators.at(i)->SetAlive(false);
		//			predators.at(i)->setPosition(-1000, -1000);//just temporarily

		//			if (p->IsShieldActive() == false)
		//				p->setHealth((p->getHealth() - 35));
		//			std::cout << "Predator with index " << i << " hit the player." << std::endl;
		//		}
		//	}
		//}

		//player and factories
		for (int i = 0; i < factories.size(); i++)
		{
			if (p->getGlobalBounds().intersects(factories.at(i)->getGlobalBounds()))
			{
				if (PixelPerfectCollision(*p, *factories.at(i), p->getTextureImage(), factories.at(i)->GetTextureImage()))
				{
					explosionController.AddExplosion(p->getPosition());

					if (factories.at(i)->getHealth() > 25)
						factories.at(i)->setHealth(factories.at(i)->getHealth() - 25);
					else
					{
						factories.at(i)->SetAlive(false);
						factories.at(i)->setPosition(-1000, -1000);
					}

					p->setHealth((p->getHealth() - 100));
					std::cout << "Factory with index " << i << " hit the player." << std::endl;
				}
			}
		}
		//predators and obstacles
		//for (int i = 0; i < predators.size(); i++)
		//{
		//	for (int j = 0; j < obstacles.size(); j++)
		//	{
		//		
		//	}
		//}

		//factories and obstacles
		for (int i = 0; i < factories.size(); i++)
		{
			for (int j = 0; j < obstacles.size(); j++)
			{
				if (factories.at(i)->getGlobalBounds().intersects(obstacles.at(j)->getGlobalBounds()))
				{
					if (PixelPerfectCollision(*factories.at(i), *obstacles.at(j), factories.at(i)->GetTextureImage(), obstacles.at(j)->getTextureImage()))
					{
						explosionController.AddExplosion(obstacles.at(j)->getPosition());

						if (factories.at(i)->getHealth() > 10)
							factories.at(i)->setHealth(factories.at(i)->getHealth() - 10);
						else
						{
							factories.at(i)->SetAlive(false);
							factories.at(i)->setPosition(-1000, -1000);
						}

						obstacles.at(j)->ReverseVelocity();
					}
				}
			}
		}

		//player bullet and factories

		//factory bullet and player

		//respawn the player if they managed to be blown to bits
		if (p->getHealth() <= 0)
		{
			p->Respawn();
			//need to reset the bullets?
			std::cout << "Player was blown to bits and is now respawning." << std::endl;
		}

		//draw the hud
		window.setView(window.getDefaultView());

		if (p->IsReusingBullets() == false)
			hud->Draw(window, p->GetInactiveBullets());
		else hud->Draw(*pWindow, p->CheckInactiveBullets());

		if (p->getHealth() > 100)
			hud->UpdateHealthIndicator(0);
		else if (p->getHealth() > 70)//can tweak these values later
			hud->UpdateHealthIndicator(1);
		else if (p->getHealth() > 40 && p->getHealth() < 70)
			hud->UpdateHealthIndicator(2);
		else if (p->getHealth() > 0 && p->getHealth() < 40)
			hud->UpdateHealthIndicator(3);
		hud->Update(p->getRotation(), p->IsShieldActive(), p->IsSpeedBoostActive());


		//minimap/radar
		window.setView(minimap);
		minimap.setCenter(p->getPosition());
		
		////////////////////////////////////////////////////////////////////////////
		//Draw Radar Icons of Entites Here
		//player
		p->drawRadarIcon(*pWindow);

		//boids
		for (int i = 0; i < boids.size(); i++)
		{
			boids.at(i)->drawRadarIcon(*pWindow);
		}
		
		for (int i = 0; i < factories.size(); i++) {
			factories.at(i)->drawRadarIcon(window);
		}
		
		for (int i = 0; i < predators.size(); i++)		{
			predators.at(i)->drawRadarIcon(window);
		}
	
		for (int i = 0; i < obstacles.size(); i++)
		{
			obstacles.at(i)->DrawRadarIcon(*pWindow);
		}

		powerupController.DrawRadarIcons(window);

		//interceptor missile
		if(testMissile->CheckIfAlive() == true)
			testMissile->drawRadarIcon(*pWindow);
		////////////////////////////////////////////////////////////////////////////

		// Finally, display rendered frame on screen 
		window.display();
	} //loop back for next frame

	return EXIT_SUCCESS;
}

