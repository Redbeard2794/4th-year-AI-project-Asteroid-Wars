#include "stdafx.h"
#include "vCamera.hpp"

vCamera::vCamera() {

}
vCamera::vCamera(sf::Vector2f screen_size) {
	reset(sf::FloatRect{ 0, 0, screen_size.x, screen_size.y });
	setViewport(sf::FloatRect{ 0, 0, 1.0f, 1.0f });
	x_bounds = false;
	y_bounds = false;
}
vCamera::vCamera(sf::Vector2f screen_size, sf::FloatRect bounds) {
	reset(sf::FloatRect{ 0, 0, screen_size.x, screen_size.y });
	setViewport(sf::FloatRect{ 0, 0, 1.0f, 1.0f });
	m_bounds = bounds;
	x_bounds = false;
	y_bounds = false;
}
vCamera::~vCamera() {

}

sf::Vector2f vCamera::getPlayerOffset(sf::Vector2f player_center) {
	checkBounds(player_center);
	if (x_bounds)
		player_center.x = XAxisOffset;

	if(y_bounds)
		player_center.y = YAxisOffset;

	return player_center;
}
sf::FloatRect vCamera::checkBounds(sf::Vector2f player_center) {
	sf::FloatRect viewport = getRectViewport();

	if (viewport.left < m_bounds.left) {
		x_bounds = true;
		viewport.left = m_bounds.left;

		XAxisOffset = viewport.left;
		XAxisOffset += viewport.width / 2;
	}
	if (viewport.left > m_bounds.width - viewport.width) {
		x_bounds = true;
		viewport.left = m_bounds.width - viewport.width;

		XAxisOffset = m_bounds.width;
		XAxisOffset -= viewport.width / 2;
	}

	if (viewport.top < m_bounds.top) {
		y_bounds = true;
		viewport.top = m_bounds.top;

		YAxisOffset = viewport.top;
		YAxisOffset += viewport.height / 2;
	}
	if (viewport.top > m_bounds.height - viewport.height) {
		y_bounds = true;
		viewport.top = m_bounds.height - viewport.height;

		YAxisOffset = m_bounds.height;
		YAxisOffset -= viewport.height / 2;
	}

	if (player_center.x > viewport.width / 2 && player_center.x < m_bounds.width - (viewport.width / 2))
		x_bounds = false;
	if (player_center.y > viewport.height / 2 && player_center.y < m_bounds.height - (viewport.height / 2))
		y_bounds = false;
	return viewport;
}
sf::FloatRect vCamera::getRectViewport() {
	sf::Vector2f l_size, l_center;
	l_size = getSize();
	l_center = getCenter();
	sf::FloatRect temp = sf::FloatRect((float)(l_center.x - l_size.x / 2),
		(float)(l_center.y - l_size.y / 2),
		(float)(l_size.x),
		(float)(l_size.y));
	return temp;
}

void vCamera::setDefaults() {
	x_bounds = false;
	y_bounds = false;

	XAxisOffset = 0;
	YAxisOffset = 0;
}



