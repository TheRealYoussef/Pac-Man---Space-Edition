#include <vector>

#include "Character.h"
#include "Map.h"
#include "GlobalVariables.h"

#ifndef Enemy_h
#define Enemy_h

namespace pac
{

	class Player;

	class Enemy : public Character
	{
	private:

		//The images of the ghost in frightened mode (state 1)
		Image frightened_images_state_1[NUMBER_OF_SPRITES_PER_CHARACTER];

		//The images of the ghost in frightened mode (state 2)
		Image frightened_images_state_2[NUMBER_OF_SPRITES_PER_CHARACTER];

		//The images of the enemy when it is returning to the ghost house
		Image eaten_images[NUMBER_OF_SPRITES_PER_CHARACTER];

		//The coordinate of the enemy's center point
		Coordinate center_coordinate;

		//The distances between the centers 4 tiles around the enemy and the target position
		std::vector <float> distances;

		//Did the enemy change its direction?
		bool same_direction;

		//Frightened animation state
		AnimationState frightened_animation_state;

		//Is the enemy forced to move in the opposite direction?
		bool forced_switch;

		//The ghost's previous mode
		GhostMode previous_mode;

		//The timer of the previous mode
		sf::Time previous_time;

		//If the tile is not a wall tile, then calculate the distance from its center to the target position and store that distance in the distances vector
		//Otherwise, store -1
		void calculateDistance(const Coordinate & coordinate, const Direction & direction, Map & map);

		//Update the previous directions
		void updatePreviousDirections(const Direction & direction);

		//Compare distances between surrounding tiles and target position with that of the tile in the enemy's current direction
		void compareWithCurrentDirection(const bool & same_direction);

		//If another direction wasn't chosen, then compare distances between surrounding tiles and target position with that of another tile
		void compareWithAnotherDirection(Map & map);

		//Choose the other direction
		void getOtherDirection();
		
		//Return true if the direction passed to the function is the same as any of the previous directions, false otherwise
		bool checkPreviousDirections(const Direction & direction);

		//Return true if the direction passed to the function is the opposite of any of the previous directions, false otherwise
		bool checkPreviousDirectionsOpposites(const Direction & direction);

		//Control when the chasing will end
		void chase();

		//Control when the scattering will end
		void scatter();

		//Choose a random target position and control when the frightened mode ends
		void run(Map & map);

		//Control the movement in the ghost house
		void house(Map & map);

		//Leave the ghost house
		void leaveHouse(const Map & map);

		//Switch between chase and scatter modes
		void switchMode(const GhostMode & mode, const sf::Time & time, const bool & switch_direction, const bool & scatter_position);

	protected:

		//Mode clock and time
		ClockTime mode_clock_time;

		//The enemy's current state (scatter, chase, or frightened)
		GhostMode mode;

		//The pac::NUMBER_OF_SAFETY_FRAMES previous directions of the enemy
		std::vector <Direction> previous_directions;

		//The enemy's target position
		Position target_position;

		//The enemy's scatter position
		Position scatter_position;

		//The number of points the player has to eat for the enemy to get out of the ghost house
		int get_out_of_ghost_house_points;

	public:

		//Default constructor
		Enemy();

		//Set the enemy's scatter position
		void setScatterPosition(const Map & map, const Coordinate & coordinate);

		//Load the images of the enemy in frightened and eaten modes
		void loadImages(const std::string & frightened_file_path_1, const std::string & frightened_file_path_2, const std::string & eaten_file_path);

		//Control the enemy's current mode/state
		void changeMode(pac::Map & map);

		//Target a position depending on the enemy's personality
		virtual void targetPosition(const Player & player);

		//Choose the next direction to take
		void chooseBestDirection(Map & map);

		//Frighten the enemy
		void frighten();

		//Move the frightened sprites
		void moveFrightened();

		//Play the frightened state animation
		void frightenedAnimation();

		//Set the position of the frightened sprites
		void setFrightenedPosition(const Position & position);

		//Display the enemy
		void display(sf::RenderWindow & window) const;

	};
}
#endif