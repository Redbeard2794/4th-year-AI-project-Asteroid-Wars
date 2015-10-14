#ifndef SWARMBOID_H
#define SWARMBOID_H

class SwarmBoid : public sf::Drawable, public sf::Transformable
{
private:
	sf::Sprite sprite;
	sf::Texture texture;

public: 
	SwarmBoid();
	~SwarmBoid();
	void Update();

	//for the different required behaviours
	void checkDistanceToOtherBoids();
	void tendTowardsPlayer(sf::Vector2f playerPos);
	void checkRangeToPlayer(sf::Vector2f playerPos);
	void interceptPlayer(sf::Vector2f playerPos);

	void draw(sf::RenderTarget& window, sf::RenderStates state) const;
	void draw(sf::RenderTarget& window);
};

#endif
