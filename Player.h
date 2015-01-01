#include "SFML/Graphics.hpp"

#include <string>

#include "Map.h"
#include "GlobalVariables.h"

#ifndef Player_h
#define Player_h

class Player
{
private:
	
	//The player's spawn position
	Position spawn;

	//The player's current position
	Position position;

	//The player's sf::Texture 's and sf::Sprite 's
	TextureSprite image[8];	

	//The player's current direction
	Direction player_direction;

	//The player's next direction
	Direction next_direction;

	//The sf::Clock and sf::Time used to control the player's animation
	ClockTime animation;

	//The player's current animation state
	AnimationState animation_state;

	//Did the movement clock equal or exceed the movement time makeing the player move?
	bool moved;

	//The time per frame in milliseconds (longer on slower computers and shorter on faster computers)
	float time_per_frame;

	//The sf::Clock and sf::Time used to control the player's movement
	ClockTime movement;

	//The distance moved by the player per millisecond
	float move_speed_const;

	//The modified distance moved by the player based on the time per frame 
	float move_speed;

	//False until the player starts moving
	bool started_moving;

	//The player's velocity
	Velocity velocity;

	//Constructor initializations
	void setInitializations(const Position & spawn_position, const sf::Time & animation_time, const float & move_speed);

	//Load player textures from the sprite sheet
	void loadTextures(const std::string & player_file_path);

	//Set the sprite textures
	void setSprites();

	//Movement at the very start of the game
	void initialMovement(const sf::Event & sf_event);

	//User presses arrow keys to choose the direction the player moves in (only if the player is not colliding in that direction)
	void pressToChooseDirection(const sf::Event & sf_event, const Map & map);

	//Calculate time per frame and movement time and move speed based on the time per frame
	void setMoveTimeAndSpeed();

	//Control the storage of a direction to move next
	void setPlayerDirection(const Map & map);

	//Set the player's velocity based on direcion and collision
	void setPlayerVelocity(const Map & map);

	//Move the player based on time
	void movePlayer();

public:

	//Player constructor
	//const Position & spawn_position: the player's spawning position
	//const sf::Time & animation_time: the time between each animation state
	//const float & move_speed: the player's distance moved in one second
	//const std::string & player_file_path: the file path for the player's sprite sheet
	Player(const Position & spawn_position, const sf::Time & animation_time, const float & move_speed, const std::string & player_file_path);

	//Play the player's animation
	void playAnimation();

	//Get input from the keyboard (up, down, right, and left keys) to control the player's movement
	void initializeMovement(const sf::Event & event, const Map & map);

	//Move the player
	void move(const Map & map);

	//Get the player's current position
	Position getPosition() const;

	//Get the time taken to go through the current frame
	float getTimePerFrame() const;

	//Set the player's position
	void setPosition(const Position & position);

	//Set the player's direction
	void setDirection(const Direction & direction);

	//Display the player
	void display(sf::RenderWindow & window) const;

};

#endif
