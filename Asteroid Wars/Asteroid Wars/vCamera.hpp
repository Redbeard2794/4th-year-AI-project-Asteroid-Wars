#ifndef _VIEW_CAMERA_HPP
#define _VIEW_CAMERA_HPP
#include "stdafx.h"
#include "SFML\Main.hpp"
#include "SFML\Graphics.hpp"
#include "SFML\Graphics\Shape.hpp"
#include "SFML\Graphics\RectangleShape.hpp"
#include "SFML\Graphics\View.hpp"

class vCamera : public sf::View {
private:
	bool x_bounds, y_bounds;
	float XAxisOffset, YAxisOffset;
	sf::FloatRect m_bounds;
public:
	vCamera();
	vCamera(sf::Vector2f screen_size);
	vCamera(sf::Vector2f screen_size, sf::FloatRect bounds);
	~vCamera();

	sf::Vector2f getPlayerOffset(sf::Vector2f player_center);

	sf::FloatRect checkBounds(sf::Vector2f player_center);
	sf::FloatRect getRectViewport();
	void addBounds(sf::FloatRect bounds) { m_bounds = bounds; }
	void setDefaults();

};

#endif