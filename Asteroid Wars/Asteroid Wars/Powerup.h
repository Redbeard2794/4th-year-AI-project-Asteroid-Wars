#ifndef POWERUP_H
#define POWERUP_H

class Powerup : public sf::Sprite
{
private:

	/*power up types*/
	enum PowerupType
	{
		HEALTH = 1,
		SHIELD = 2,
		SPEED = 3
	};

	//the type of this powerup
	int type;

	/*the texture*/
	sf::Texture texture;

	/*radar icon texture*/
	sf::Texture radarTexture;

	/*sprite for the radar*/
	sf::Sprite radarSprite;

	/*rectangle shape to represent the bounding box*/
	sf::RectangleShape boundingBox;

	/*can the powerup be removed*/
	bool removeable;

public:

	//!constructor. param is the position to start at
	Powerup(sf::Vector2f playerPos);

	//!destructor
	~Powerup();

	//!update spins the powerup
	void Update();

	//!draw the bounding box representation. param is a renderTarget
	void DrawBoundingBox(sf::RenderTarget & window);

	//!draw the radar icon. param is a renderTarget
	void DrawRadarIcon(sf::RenderTarget& window);

	//!get the type of this powerup
	int getType();

	//!Check if the powerup can be removed
	bool isRemoveable();

	//!set whether the powerup should be removed
	void SetRemoveableStatus(bool r);

};

#endif
