/*! \mainpage My Personal Index Page
*
*	\section req_sec Requirements
*
*	Asteroid Wars
*	You must produce the game “Asteroid Wars”. This is a single player shoot 'em up.
*	The aim of the game is to destroy all other ships.
*
*	The player’s ship is controlled using the left, right and up arrows. Left and right
*	arrows cause the chip to turn anticlockwise and clockwise respectively. The up arrow
*	applies thrust to the ship in the direction it is pointing. The space bar fires a bullet.
*	Enemy entities come in different guises and display differing behaviour.
*	
*	Factories
*	These produce predators while actively trying to evade the player. They can survive
*	four hits from the player. When the player’s ship is within range they will take
*	evasive behaviour, otherwise they wander at random. If factories meet they will
*	flock. A factory will fire interceptor guided missiles at the player once the player is
*	within range. Interceptor missiles have the same maximum speed as the player and
*	will explode if they do not reach the player within 10 seconds. A factory can only
*	have five interceptor missiles “live” at a time.
*
*	Swarm Boids
*	These swarm in groups and tend towards the players ship. Once they are within
*	range of the ship they then plot an intercept course for the ship.
*
*	Predator Ships
*	These seek out the player and intelligently engage them. They have the same
*	firepower as the player ship. Predators move in flocks and can also collect “powerups”!
*
*	Obstacles
*	Exactly what it says on the tin.
*
*	Powerups
*	To be decided by individual game designers.
*	Friction operates in the game and any moving object will slow down over time if
*	thrust is not applied to it. All entities try to avoid obstacles. Collision with an obstacle
*	results in death.
*
*	The screen should scroll so that the player’s ship is always in the centre of the
*	screen. The game is wrap around with the total area of play 9 times that of a single
*	screen. Radar will be provided for the player
*
*	\section Beyond_sec Beyond Requirments
*
*	\subsection factory FactoryShip
*	The Factory ships within the current build of the game works on a three state AI state machine:
*	
*	Evade: Causes the factory ship to strategicly retreat from the player. The Factory calculates any 
*	direction thats not towards the player aka in a 180 degree circle away and plots a random path and
*	evades. But if the player is trying to exit the evade circle the factory ship tries its best to keep 
*	them within this radius as it is also the firing range for the Interceptor Missles.
*	
*	Flee: Is purely for self preservation if the player ship gets too close to the factory ship or has 
*	taken enough damage the ship begins to head directly away from the player. Is would be the strategy
*	when the ship is in combat has fired missles and spawned a handful of ships now it must retreat and 
*	save its streght.
*
*	Wander: Plots a random location close to the ships current position and travels there, when enough
*	time has passed or the ship reaches its destination a new point is ploted this is for wandering around
*	the world.
*	The FactoryShip are responsable for there own interceptor missles and must fire them on a cool down time.
*	Each Factory loads 3 missles into memory as that is the maximum that can be active at a time and just 
*	recycles them in game. When the factories are destroyed the Missles they are responsable for are still 
*	updated until they are no longer active. Factories are meant to be large motherships in our game hence 
*	they move slowly but have alot of firepower and Obstacles ricochet off the hull of the ship.
*
*	\subsection predatorController PredatorController
*	The PredatorController is responsible for creating, rendering and updateing all the predators within 
*	the game. This simple Handler class is used to seperate the factory code from the predator. Similary 
*	The predators are responsable for there own bullets as do the Factories.
*
*	\subsection player Player
*	The Player can be controlled using either W,A,S&D or with the arrow keys. Pressing space allows the player to shoot. The player has 10 missiles they 
*	can fire before they have to wait for their guns to reload. As mentioned in the requirements the player accelerates and decelerates.
*
*	\subsection hud Hud
*	The Hud displays several pieces of information to the player. It shows the player's current health status, current orientation, radar, active powerups
*	and current ammunition.
*
*	\subsection swarmboid Swarmboid
*	The SwarmBoid have 3 states, Swarm, Intercept and Evade. 
*	
*	Swarm: The swarmboids spawn in small groups around the world and move towards each other in order to swarm. They loosely swarm so that it doesn't look like
*	one big blob of ufos. The swarm then tends towards the player when the swarm is fully established.
*	
*	Intercept: If the player is near a swarmboid it will break out of the swarm and start to chase the player with the intention of crashing straight into the 
*	player in order to cause damage to the player. If the player gets out of range the swarmboid will return to the swarm.
*	
*	Evade: If a swarmboid notices that there is an obstacle nearby and it is on a collision course with the swarm boid, the swarmboid will try to avoid the collision.
*	They will leave the swarm and try to evade the first obstacle they noticed. They are not always successful and can get hit by a second obstacle while 
*	avoiding the first obstacle.
*
*	\subsection powerup Powerup
*	There are 3 different types of Powerup in this game: Health boost, Shield and Speed boost. Health boost increases the player's health, Shield stops the player taking
*	damage for 6 seconds except for when they crash into an obstacle and Speed boost increases the player's top speed for 6 seconds. A new powerup is spawned by the 
*	powerupController every 5-15 seconds(its a random time between these 2 numbers). There can be a max of 10 powerups on screen at a time. If there is already 10 powerups on 
*	screen then a new powerup will not be spawned. Powerups are spawned in near the player.
*
*	\subsection camera Camera
*	The Games vCamera inherits from an sf::View and has bounds locking for when you begin to reach the edge 
*	of the world the camera ensures the player doesnt see beyond the background image
*
*	\subsection obstacles Obstacles
*	Obstacle are represented by animated asteriod sprites. They move around the world with a random velocity. 
*	If they collide with any object such as the player or a predator, the object is destroyed. The only exception to this is the factory ship.
*	If an obstacle collides with a factory ship it will bounce off of it and cause a small amount of damage to the factory. 
*	This was an intentional change to the requirements as the factory ships are represented by quite a large sprite and it didn't make sense for it to be
*	destroyed by a smaller obstacle.
*
*	\subsection interceptorMissle Interceptor Missle
*	The InterceptorMissile works exactly as is outlined in the requirements.
*
*	\subsection radar Radar
*	The radar was created using an sf::view. The radar shows player, the player's bullets, all enemies(factory, swarmboids, predators), obstacles and also any bullets fired in game.
*	Each object is represented by a small icon on the radar.
*
*	\subsection explosionController ExplosionController
*	The ExplosionController works in much the same way as the powerupController. The difference being that it adds an animated explosion whenever there is a collision.
*	There are 4 types of explosion. The only difference between them is the spritesheet and how fast they are animated. When an explosion has reached the end of its
*	spritesheet it fades out. Once it is fully transparent it is removed from the game.
*
*/
