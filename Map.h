#include "SFML/Graphics.hpp"

#include <fstream>
#include <vector>

#include "TeleportationPair.h"
#include "Tile.h"
#include "GlobalVariables.h"

#ifndef Map_h
#define Map_h

class Player;

class Map
{

private:

	//The starting position of the map relative to the SFML window
	Position position;

	//The length and width of the map (in number of tiles)
	GridSize size;

	//The textures of the images that make up the map 
	std::vector <sf::Texture> textures;

	//The types of all the tiles used to make the map 
	std::vector <TileType> tile_types;

	//The number of image files used to make the map
	int number_of_files;

	//All the tile boxes of the unique tiles in the map
	//Each tile has 4 tile boxes
	std::vector < std::vector < std::vector <TileBox> > > tile_boxes;

	//All the tiles that make up the map
	std::vector < std::vector <Tile> > tile_array;

	//All the teleportation pairs in the map
	std::vector < TeleportationPair > teleportation_pairs;

	//Reads all the image file paths
	//Loads the image files to their corresponding sf::Texture's
	//Reads all the tile types
	void readImagePathsAndTileTypes(std::ifstream & input);

	//Read the tile boxes' wall bools (whether the tile box is a wall or not)
	void readTileBoxesInfo(std::ifstream & input);

	//Reads the tile numbers and creates the tiles which make up the map
	void readAndCreateMap(std::ifstream & input);

	//Read teleportaion pair tile position and their directions of entry
	void readAndCreateTeleportationPairs(std::ifstream & input);

public:

	//Map constructor
	//Position position: the starting position of the map relative to the SFML window
	//std::string map_file_path: the file path of the map txt file
	Map(const Position & position, const std::string & map_file_path);

	//Display the whole map except the tiles of TELEPORTATION type
	void display(sf::RenderWindow & window) const;

	//Display the tiles of TELEPORTATION type
	void displayTeleportationTiles(sf::RenderWindow & window) const;

	//True if the object of position object_position is colliding with a tile to its right
	bool isCollidingRight(Player & object) const;

	//True if the object of position object_position is colliding with a tile to its left
	bool isCollidingLeft(Player & object) const;

	//True if the object of position object_position is colliding with a tile above it
	bool isCollidingUp(Player & object) const;

	//True if the object of position object_position is colliding with a tile below it
	bool isCollidingDown(Player & object) const;

	//Teleport the object if it enters a teleporter to its linked teleporter
	void teleport(Player & object) const;
};

#endif
