#ifndef PERPIXELCOLLISIONMANAGER_H
#define PERPIXELCOLLISIONMANAGER_H

class PerPixelCollisionManager
{
private:
	static bool instanceFlag;
	static PerPixelCollisionManager* instance;

	PerPixelCollisionManager()
	{

	}
public:
	//!destructor
	~PerPixelCollisionManager();

	//!Get the instance
	static PerPixelCollisionManager* GetInstance();

	//!Convert a sf::FloatRect to an sf::IntRect
	sf::IntRect FToIRect(const sf::FloatRect& f);

	//!Carry out per pixel collision. return true or false. params are: sprite of object1, sprite of object2, textureImage of object1, textureImage of object2
	bool PixelPerfectCollision(const sf::Sprite& spriteA, const sf::Sprite& spriteB, const sf::Image& imgA, const sf::Image& imgB);
};

#endif