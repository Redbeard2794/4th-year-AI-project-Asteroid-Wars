#ifndef EXPLOSION_H
#define EXPLOSION_H

class Explosion : public sf::Sprite
{
private:
	sf::Texture texture;//The texture

	bool finishedAnimation;//Is the animation finished or not

	int numFrames;//The total number of frames in the sheet

	/*How long does it take to move through each frame*/
	float animationTime;

	/*how fast does the explosion fade out when its finished*/
	float fadeOutSpeed;

	/*clock for moving through the sheet*/
	sf::Clock animationClock;

	/*texture rectangle stuff*/
	sf::Vector2i framePosition;
	sf::Vector2i frameSize;
	sf::IntRect frame;

	/*Can we remove the explosion yet*/
	bool removable;

	int type;

public:

	//!	constructor. param is position of explosion
	Explosion(sf::Vector2f pos);

	//!destructor
	~Explosion();

	//! update. animate through the spritesheet and fade out when finished
	void Update();

	//! can the explosion be removed?
	bool IsRemoveable();
};

#endif
