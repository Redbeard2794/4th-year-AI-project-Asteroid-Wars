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
	for (int i = 0; i < 20; i++)
	{
		boids.push_back(new SwarmBoid());
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

	bool debugMode = true;

	std::vector<Obstacle*> obstacles;
	for (int i = 0; i < 30; i++)
	{
		obstacles.push_back(new Obstacle(rand() % 2 + 1, sf::Vector2f(rand() % 6200 + 200, rand() % 4600 + 200)));
	}

	ExplosionController explosionController = ExplosionController();

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
			obstacles.at(i)->DrawBoundingBox(window);
			obstacles.at(i)->Update();
		}

		//draw the player and their bullets
		p->DrawBullets(window, debugMode);
		//p->DrawThruster(window);
		window.draw(*p);
		if(debugMode)
			p->DrawBoundingBox(window);
		/*
		////////////////////////////////////////////////////////////////////////////
		//Draw and Update Entites Here
		//draw and update the swarm boids
		for (int i = 0; i < boids.size(); i++)
		{
			if (boids.at(i)->CheckIfAlive() == true)
			{
				boids.at(i)->Update(p->getPosition(), p->getVelocity(), boids);
				window.draw(*boids.at(i));
				if (debugMode)
					boids.at(i)->DrawBoundingBox(window);
			}
		}*/

		//Update and Draw Factory
		for (int i = 0; i < factories.size(); i++)
		{
			factories.at(i)->update(p, &factories);
			window.draw(*factories.at(i));
			factories.at(i)->drawDebug(window);
			factories.at(i)->drawMissles(window);
		}
		

		//Update and Draw predator
		for (int i = 0; i < predators.size(); i++)
		{
			predators.at(i)->update(&predators, p);
			window.draw(*predators.at(i));
			predators.at(i)->drawDebug(window);
		}
		////////////////////////////////////////////////////////////////////////////

		//collision detection(basic bounding box collision detection to start with)
		//missile and player
		if (p->getGlobalBounds().intersects(testMissile->getGlobalBounds()) == true)
		{
			explosionController.AddExplosion(testMissile->getPosition());
			testMissile->setPosition(0, 0);//just temporarily
			testMissile->SetAliveStatus(false);
			p->setHealth((p->getHealth() - 35));
			std::cout << "Missile hit player and dealt 35 damage. Player now has " << p->getHealth() << " health." << std::endl;
		}
		//player and obstacles
		for (int i = 0; i < obstacles.size(); i++)
		{
			if (p->getGlobalBounds().intersects(obstacles.at(i)->getGlobalBounds()))
			{
				explosionController.AddExplosion(p->getPosition());
				p->setHealth((p->getHealth() - 100));
				std::cout << "Obstacle with index " << i << " hit the player and demolished the player's ship" << std::endl;
			}
		}
		//swarmboids and obstacles
		for (int i = 0; i < obstacles.size(); i++)
		{
			for (int j = 0; j < boids.size(); j++)
			{
				if (boids.at(j)->getGlobalBounds().intersects(obstacles.at(i)->getGlobalBounds()))
				{
					explosionController.AddExplosion(boids.at(j)->getPosition());
					boids.at(j)->SetAliveStatus(false);
					boids.at(j)->setPosition(0, 0);//just temporarily
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

		if (p->getHealth() > 70)//can tweak these values later
			hud->UpdateHealthIndicator(0);
		else if (p->getHealth() > 40 && p->getHealth() < 70)
			hud->UpdateHealthIndicator(1);
		else if (p->getHealth() > 0 && p->getHealth() < 40)
			hud->UpdateHealthIndicator(2);
		hud->Update(p->getRotation());


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
		//interceptor missile
		if(testMissile->CheckIfAlive() == true)
			testMissile->drawRadarIcon(*pWindow);
		////////////////////////////////////////////////////////////////////////////

		// Finally, display rendered frame on screen 
		window.display();
	} //loop back for next frame

	return EXIT_SUCCESS;
}