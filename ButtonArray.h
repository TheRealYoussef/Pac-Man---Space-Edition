#include "SFML/Graphics.hpp"

#include <string>
#include <vector>

#include "GlobalVariables.h"

#ifndef ButtonArray_h
#define ButtonArray_h

namespace pac
{
	class ButtonArray
	{

	private:

		//The number of buttons in the array
		int number_of_buttons;

		//The distance between each two buttons
		Distance distance_between_buttons;

		//The starting position of the array of buttons in the SFML window
		Position array_starting_position;

		//sf::texture and sf::sprite of the buttons in their normal state
		std::vector <Image> normal_state_images;

		//sf::texture and sf::sprite of the buttons in their selected state
		std::vector <Image> selected_state_images;

		//The index of the currently selected button
		int selected_button_index;

		//The index of the previously selected button
		int previously_selected_button_index;

		//The type of transition between normal a selected
		TransitionType transition;

		//True if a button is to be scaled, false otherwise or if a button is done scaling
		bool scale_bool;

		//The x and y scale of the button if the TransitionType == SCALE
		Scale scale_amount;

		//The sf::Clock and sf::Time used for scaling the button
		ClockTime scale;

		//The functions of the buttons if they are pressed (0 input arguments)
		std::vector < std::vector <FUNCTIONS> > functions;

		//If the user uses the scale function when the transition type is IMAGE, then an error message is displayed once
		bool error_message_displayed;

		//
		std::vector <int> number_of_functions;

		//Parse file containg the file paths of the images of the buttons in one of their 2 states (normal or selected)
		//Loads the images and sets them to their corresponding sf::Sprite's
		void readFile(std::ifstream & file, std::vector <Image> & images, const int & index);

		//Sets the origin and position in the SFML window of the sprites of the buttons in one of their 2 states (normal or selected)
		void setOriginAndPosition(std::vector <Image> & images, const int & index);

		//Sets some initializations that are repeated in all of the constructors
		void setInitializations(const int & number_of_buttons, const Distance & distance_between_buttons, const Position & array_starting_position, const TransitionType & transition_type);

		//Sets some initializations that are repeated in some of the constructors
		void setScaleInitializations(const Scale & scale_amount, const sf::Time & scale_time);

		//Output an error message if the file fails to open
		void fileFailure(std::ifstream & file, const std::string & file_path);

		//Select the button which the mouse hovers over
		void mouseMoved(const sf::Event & sf_event, sf::RenderWindow & window);

		//Execute the functions of the button which the mouse hovers over while the left mouse button is pressed
		void mouseButtonPressed(const sf::Event & sf_event, sf::RenderWindow & window);

		//Select the button above the currently selected button
		//If no button is currently selected or the first button is selected, then the last button is selected
		void upKeyPressed(const sf::Event & sf_event);

		//Select the button below the currently selected button
		//If no button is currently selected or the last button is selected, then the first button is selected
		void downKeyPressed(const sf::Event & sf_event);

		//Execute the functions of the button currently selected
		//If no button is selected, then nothing will happen
		void returnKeyPressed(const sf::Event & sf_event, sf::RenderWindow & window);

		//Call the corresponding functions of the buttons if they are pressed
		void callFunctions(sf::RenderWindow & window);

		//If the mouse is on a button, returns true, false otherwise
		bool mouseOnButton(const sf::Event & sf_event, sf::RenderWindow & window);

		//Returns true if the mouse is on the selected button, false otherwise
		bool mouseOnSelectedButton(const sf::Event & sf_event, sf::RenderWindow & window);

		//Scales the selected button based on time if the TransitionType == SCALE
		void scaleButton(std::vector <Image> & images);

		//Start scaling the selected button and return the previously selected button to its original size
		void changeScales();

	public:

		//Construct the button array
		//Transition between normal state and selected state is done by displaying the corresponding image
		//int number_of_buttons: number of buttons in the array
		//float distance_between_buttons: distance between each two buttons
		//Position array_starting_position: starting position of the array of buttons relative to the SFML window (position of the first button)
		//std::string normal_state_buttons_file_name: file path for the file containing the file paths of the normal state button images 
		//std::string selected_state_buttons_file_name: file path for the file containing the file paths of the selected state button images
		//File structure: Each file path has to be in a new line on its own
		ButtonArray(const int & number_of_buttons, const Distance & distance_between_buttons, const Position & array_starting_position, const std::string & normal_state_buttons_file_path, const std::string & selected_state_buttons_file_path);

		//Construct the button array
		//Transition between normal state and selected state is done by scaling the image
		//int number_of_buttons: number of buttons in the array
		//float distance_between_buttons: distance between each two buttons
		//Position array_starting_position: starting position of the array of buttons relative to the SFML window (position of the first button)
		//Scale scale_amount: the amount the button scales in the x and y directions respectively if it is selected (scale is a multiplier and not a flat amount)
		//float scale_time: the amount time needed for the button to reach its full selected size in milliseconds
		//std::string normal_state_buttons_file_name: file path for the file containing the file paths of the normal state button images 
		//File structure: Each file path has to be in a new line on its own
		ButtonArray(const int & number_of_buttons, const Distance & distance_between_buttons, const Position & array_starting_position, const Scale & scale_amount, const sf::Time & scale_time, const std::string & normal_state_buttons_file_path);

		//Construct the button array
		//Transition between normal state and selected state is done by displaying the corresponding image and scaling it
		//int number_of_buttons: number of buttons in the array
		//float distance_between_buttons: distance between each two buttons
		//Position array_starting_position: starting position of the array of buttons relative to the SFML window (position of the first button)
		//Scale scale_amount: the amount the button scales in the x and y directions respectively if it is selected (scale is a multiplier and not a flat amount)
		//float scale_time: the amount time needed for the button to reach its full selected size in milliseconds
		//std::string normal_state_buttons_file_name: file path for the file containing the file paths of the normal state button images 
		//std::string selected_state_buttons_file_name: file path for the file containing the file paths of the selected state button images
		//File structure: Each file path has to be in a new line on its own
		ButtonArray(const int & number_of_buttons, const Distance & distance_between_buttons, const Position & array_starting_position, const Scale & scale_amount, const sf::Time & scale_time, const std::string & normal_state_buttons_file_path, const std::string & selected_state_buttons_file_path);

		//Control all events related to the button such as moving the mouse over a button, pressing the left mouse button while the mouse is over a button, up key, down key, and return key while a button is selected 
		void setState(const sf::Event & sf_event, sf::RenderWindow & window);

		//Set what the button of index button_index does if it is pressed
		//void (*function)(): function name which is void and has 0 input arguments
		//Put & followed by the function name only
		//A button can have as many functions as the user wishes
		void setFunction(const int & button_index, void(*function)());

		//Scales the selected button
		void scaleSelectedButton();

		//Display the buttons
		void display(sf::RenderWindow & window) const;

		//Destructor
		~ButtonArray();

	};
}
#endif

