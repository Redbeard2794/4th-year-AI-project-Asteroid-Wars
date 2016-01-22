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
	~PerPixelCollisionManager();

	static PerPixelCollisionManager* GetInstance();

	sf::IntRect FToIRect(const sf::FloatRect& f);

	bool PixelPerfectCollision(const sf::Sprite& spriteA, const sf::Sprite& spriteB, const sf::Image& imgA, const sf::Image& imgB);
};

#endif