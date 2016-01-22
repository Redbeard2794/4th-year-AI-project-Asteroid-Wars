#include "stdafx.h"
#include "PerPixelCollisionManager.h"

bool PerPixelCollisionManager::instanceFlag = false;
PerPixelCollisionManager* PerPixelCollisionManager::instance = NULL;

PerPixelCollisionManager* PerPixelCollisionManager::GetInstance()
{
	if (!instanceFlag)
	{
		instance = new PerPixelCollisionManager();
		instanceFlag = true;
		return instance;
	}
	else return instance;
}

PerPixelCollisionManager::~PerPixelCollisionManager()
{
	instanceFlag = false;
}

//convert a sf::FloatRect to an sf::IntRect
sf::IntRect PerPixelCollisionManager::FToIRect(const sf::FloatRect& f)
{
	return sf::IntRect((int)f.left, (int)f.top, (int)f.width, (int)f.height);
}

//pixel perfect collision detection(sprite1, sprite2, texture1, texture2)
bool PerPixelCollisionManager::PixelPerfectCollision(const sf::Sprite& spriteA, const sf::Sprite& spriteB, const sf::Image& imgA, const sf::Image& imgB)
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
