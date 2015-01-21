#include "SFML/Graphics.hpp"

#include <fstream>
#include <vector>

#include "Point.h"
#include "TeleportationPair.h"
#include "Tile.h"
#include "GlobalVariables.h"

#ifndef Map_h
#define Map_h

namespace pac
{
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
		std::vector < std::vector < std::vector <bool> > > tile_boxes;

		//All the tiles that make up the map
		std::vector < std::vector <Tile> > tile_array;

		//All the teleportation pairs information
		std::vector <TeleportationPairInfo> teleportation_pairs_info;

		//All the teleportation pairs in the map
		std::vector <TeleportationPair> teleportation_pairs;

		//Points in the map
		std::vector < std::vector <Point> > points;

		//Number of point file paths
		int number_of_point_files;

		//Textures of the points
		std::vector <sf::Texture> point_textures;

		//Types of the points
		std::vector <PointType> point_types;

		//Values of the points
		std::vector <int> point_values;

		//The player's starting position in the map
		Position player_starting_position;

		//The ghosts' starting positions in the map
		Position ghosts_starting_positions[4];

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

		//Read point file paths, types and values
		void readPointFilesAndTypes(std::ifstream & input);

		//Read and create point map
		void readAndCreatePoints(std::ifstream & input);

		//Get a point in the specified coordinate and return true if successful
		bool getPoint(const Coordinate & coordinate, Point *&surrounding_points);

	public:

		//Default constructor
		Map();

		//Initialize map
		//Position position: the starting position of the map relative to the SFML window
		//std::string map_file_path: the file path of the map txt file
		void init(const Position & position, const std::string & map_file_path);

		//Display the whole map except the tiles of TELEPORTATION type
		void display(sf::RenderWindow & window) const;

		//Display the tiles of TELEPORTATION type
		void displayTeleportationTiles(sf::RenderWindow & window) const;

		//Display the points
		void displayPoints(sf::RenderWindow & window) const;

		//Get the coordinate of an object on the map
		Coordinate getCoordinate(const Position & position) const;

		//Get the position of an object on the map
		Position getPosition(const Coordinate & coordinate) const;

		//Get the map's position in the window
		Position getMapPosition() const;

		//Get the map's size
		GridSize getMapSize() const;

		//Return pointers to the 3 tiles colliding with an object in the direction the object is moving
		bool getCollidingTiles(const Coordinate & coordinate, const Direction & direction, Tile *tiles[DISTINCT_COLLIDING_TILES]);

		//Return pointers to all the teleportation pairs in the map
		void getTeleportationPairs(std::vector <TeleportationPair*> & teleportation_pair);

		//Return a pointer to the point in front of the player in the direction he is moving
		bool getPoint(const Coordinate & coordinate, const Direction & direction, Point *&surrounding_points);

		//Get the player's starting position in the map
		Position getPlayerStartingPosition();

		//Get the type of the tile of the specified coordinate
		TileType getTileType(const Coordinate & coordinate) const;

		//Destructor
		~Map();

	};
}

#endif
