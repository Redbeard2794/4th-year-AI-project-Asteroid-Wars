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



////////////////////////////////////////////////////////////
///Entrypoint of application 
//////////////////////////////////////////////////////////// 

int main() {
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

	SwarmBoid* testBoid = new SwarmBoid();
	Hud* hud = new Hud(font);

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


		}



		//prepare frame
		window.clear();

		//update sf::View center position
		player_view.setCenter(p->getPosition());
		m_camera.setCenter(m_camera.getPlayerOffset(p->getPosition()));
		
		//set view of window to be player_view
		window.setView(m_camera);

		p->Update(background.getPosition(), backgroundTexture.getSize(), &m_camera);

		testBoid->Update(p->getPosition(), p->getVelocity());//testing only

		window.draw(background);
		//draw frame items
		p->draw(*pWindow);
		
		testBoid->draw(*pWindow);//testing only

		window.setView(window.getDefaultView());
		//draw the hud
		hud->Draw(*pWindow);
		if (p->getHealth() > 70)
			hud->UpdateHealthIndicator(0);
		else if (p->getHealth() > 40 && p->getHealth() < 70)
			hud->UpdateHealthIndicator(1);
		else if (p->getHealth() > 0 && p->getHealth() < 40)
			hud->UpdateHealthIndicator(2);
		hud->Update(p->getRotation());

		//minimap/radar
		window.setView(minimap);
		minimap.setCenter(p->getPosition());
		p->drawRadarIcon(*pWindow);
		testBoid->drawRadarIcon(*pWindow);



		// Finally, display rendered frame on screen 
		window.display();
	} //loop back for next frame

	return EXIT_SUCCESS;
}