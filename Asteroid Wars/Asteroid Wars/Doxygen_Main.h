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
*	Power-ups
*	To be decided by individual game designers.
*	Friction operates in the game and any moving object will slow down over time if
*	thrust is not applied to it. All entities try to avoid obstacles. Collision with an obstacle
*	results in death.
*
*	The screen should scroll so that the player’s ship is always in the centre of the
*	screen. The game is wrap around with the total area of play 9 times that of a single
*	screen. Radar will be provided for the player
*
*	\section Beyond_sec <b>Beyond Requirments<b>
*
*	\subsection factory Factory
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
*
*	The factorys are responsable for there own interceptor missles and must fire them on a cool down time.
*	Each Factory loads 3 missles into memory as that is the maximum that can be active at a time and just 
*	recycles them in game. When the factories are destroyed the Missles they are responsable for are still 
*	updated until they are no longer active. Factories are meant to be large motherships in our game hence 
*	they move slowly but have alot of firepower and Obstacles ricochet off the hull of the ship.
*
*	\subsection predatorController PredatorController
*	The Predator Controller is responsible for creating, rendering and updateing all the predators within 
*	the game. This simple Handler class is used to seperate the factory code from the predator. Similary 
*	The predators are responsable for there own bullets as do the Factories.
*
*	\subsection player Player
*	\subsection swarmboid Swarmboid
*	\subsection powerup Powerup
*	\subsection camera Camera
*	The Games camera inherits from an sf::View and has bounds locking for when you begin to reach the edge 
*	of the world the camera ensures the player doesnt see beyond the background image
*
*	\subsection obstacles Obstacles
*	\subsection interceptorMissle Interceptor Missle
*	\subsection radar Radar
*	\subsection collisionAvoidence Collision Avoidence
*	\subsection explosionController ExplosionController
*
*/
