#ifndef INTERCEPTORMISSILE_H
#define INTERCEPTORMISSILE_H

class InterceptorMissile : public sf::Sprite
{
private:
	sf::Texture texture;

	sf::Vector2f velocity;

	float maxSpeed;

	float speed;

	sf::Vector2f dirMove;

	float distanceToPlayer;

	int timeAlive;

	sf::Clock lifeClock;

	sf::Texture radarTexture;

	sf::Sprite radarSprite;

	sf::RectangleShape boundingBox;

	bool alive;
	bool active;
	sf::Vector2f default_spawn;

	sf::Image textureImage;
public:
	/*Constructor*/
	InterceptorMissile(sf::Vector2f pos);

	/*Deconstructor*/
	~InterceptorMissile();

	/*Check the euclidean distance to the player*/
	void checkRangeToPlayer(sf::Vector2f playerPos);

	/*Update*/
	void Update(sf::Vector2f targetPos);

	/*Seek the position that is passed in(i.e. match this position and face the direction being travelled in)*/
	void Seek(sf::Vector2f targetPos);

	/*Draw the missiles radar icon*/
	void drawRadarIcon(sf::RenderTarget& window);

	/*Get how long the missile has been alive for*/
	float getTimeAlive();

	/*Check if the missile is still alive*/
	bool CheckIfAlive();
	bool CheckIfActive();

	/*set the alive status of the missile*/
	void SetAliveStatus(bool a);
	void SetActiveStatus(bool a);

	/*Draw the bounding box*/
	void DrawBoundingBox(sf::RenderTarget& window);

	/*Reset the Missle to (-100, -100)*/
	void Reset();
	void Launch(sf::Vector2f launch_pos);

	sf::Image GetTextureImage() { return textureImage; }
};

#endif
