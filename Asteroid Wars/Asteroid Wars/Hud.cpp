#include "stdafx.h"
#include "Hud.h"

Hud::Hud(sf::Font f) : font(f)
{
	mapRect.setSize(sf::Vector2f(255, 97));
	mapRect.setOrigin(127.5f, 48.5f);
	mapRect.setFillColor(sf::Color(0,127,50,255));
	mapRect.setOutlineColor(sf::Color::Cyan);
	mapRect.setOutlineThickness(1);
	mapRect.setPosition(SCREENWIDTH/2, SCREENHEIGHT-50);

	//load the correct texture or load the debug texture if something is wrong
	if (overlayTexture.loadFromFile("Assets/HUD/hudOverlay.png")) {}
	else overlayTexture.loadFromFile("Assets/Debug.png");	//if it fails load placeholder
	overlaySprite.setTexture(overlayTexture);
	overlaySprite.setOrigin(overlayTexture.getSize().x / 2, overlayTexture.getSize().y / 2);
	overlaySprite.setPosition(SCREENWIDTH / 2, SCREENHEIGHT / 2);

	if (healthyIndicatorTexture.loadFromFile("Assets/HUD/healthGoodIndicator.png")) {}
	else healthyIndicatorTexture.loadFromFile("Assets/Debug.png");	//if it fails load placeholder

	if (damagedIndicatorTexture.loadFromFile("Assets/HUD/slightDamageIndicator.png")) {}
	else damagedIndicatorTexture.loadFromFile("Assets/Debug.png");	//if it fails load placeholder

	if (nearlyDeadIndicatorTexture.loadFromFile("Assets/HUD/heavyDamageIndicator.png")) {}
	else nearlyDeadIndicatorTexture.loadFromFile("Assets/Debug.png");	//if it fails load placeholder

	healthIndicatorSprite.setTexture(healthyIndicatorTexture);
	healthIndicatorSprite.setOrigin(healthyIndicatorTexture.getSize().x / 2, healthyIndicatorTexture.getSize().y / 2);
	healthIndicatorSprite.setPosition(sf::Vector2f(50, 485));

	currentDamageState = NODAMAGE;
	healthFlashClock = sf::Clock();

	healthIndText.setFont(font);
	healthIndText.setCharacterSize(15);
	healthIndText.setString("Damage\n  status");
	healthIndText.setStyle(sf::Text::Bold);
	healthIndText.setPosition(25, 500);

	orientationText.setFont(font);
	orientationText.setCharacterSize(15);
	orientationText.setString("Orientation");
	orientationText.setStyle(sf::Text::Bold);
	orientationText.setPosition(680, 580);

	if (orientationBackgroundTexture.loadFromFile("Assets/HUD/OrientationBackground.png")) {}
	else orientationBackgroundTexture.loadFromFile("Assets/Debug.png");	//if it fails load placeholder
	orientationBackgroundSprite.setTexture(orientationBackgroundTexture);
	orientationBackgroundSprite.setOrigin(orientationBackgroundTexture.getSize().x / 2, orientationBackgroundTexture.getSize().y / 2);
	orientationBackgroundSprite.setPosition(720, 540);

	if (orientationIndicatorTexture.loadFromFile("Assets/HUD/orientationIndicator.png")) {}
	else orientationIndicatorTexture.loadFromFile("Assets/Debug.png");	//if it fails load placeholder
	orientationIndicatorSprite.setTexture(orientationIndicatorTexture);
	orientationIndicatorSprite.setOrigin(orientationIndicatorTexture.getSize().x / 2, orientationIndicatorTexture.getSize().y / 2);
	orientationIndicatorSprite.setPosition(721, 540);
}

Hud::~Hud()
{

}

void Hud::Update(float orientationValue)
{
	orientationIndicatorSprite.setRotation(orientationValue);
}

void Hud::UpdateHealthIndicator(int damageValue)
{
	if (damageValue == NODAMAGE && currentDamageState != damageValue)
	{
		healthIndicatorSprite.setTexture(healthyIndicatorTexture);
	}
	else if (damageValue == MILDDAMAGE && currentDamageState != damageValue)
	{
		healthIndicatorSprite.setTexture(damagedIndicatorTexture);
	}
	else if (damageValue == HEAVYDAMAGE && currentDamageState != damageValue)
	{
		healthIndicatorSprite.setTexture(nearlyDeadIndicatorTexture);
	}
	currentDamageState = damageValue;
	if (healthFlashClock.getElapsedTime().asSeconds() > 0 && healthFlashClock.getElapsedTime().asSeconds() < 1)
	{
		healthIndicatorSprite.setColor(sf::Color::White);
	}
	else if (healthFlashClock.getElapsedTime().asSeconds() > 1 && healthFlashClock.getElapsedTime().asSeconds() < 2)
	{
		healthIndicatorSprite.setColor(sf::Color::Transparent);
	}
	else healthFlashClock.restart();
}

void Hud::Draw(sf::RenderTarget& window)
{
	window.draw(overlaySprite);
	window.draw(mapRect);
	window.draw(healthIndicatorSprite);
	window.draw(healthIndText);
	window.draw(orientationBackgroundSprite);
	window.draw(orientationIndicatorSprite);
	window.draw(orientationText);
}