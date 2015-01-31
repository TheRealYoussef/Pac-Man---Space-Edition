#include <SFML/Graphics.hpp>

#include <string>
#include <fstream>
#include <iostream>

#include "Point.h"
#include "TeleportationPair.h"
#include "Tile.h"
#include "Map.h"
#include "GlobalVariables.h"
#include "GlobalFunctions.h"

pac::Map::Map()
{
}

void pac::Map::init(const pac::Position & position, const std::string & map_file_path)
{
	this->position = position;
	std::ifstream input(map_file_path.c_str());
	if (!input.fail())
	{
		char trash;
		input >> size.col >> trash >> size.row;
		readImagePathsAndTileTypes(input);
		readTileBoxesInfo(input);
		readAndCreateMap(input);
		readAndCreateTeleportationPairs(input);
		readPointFilesAndTypes(input);
		readAndCreatePoints(input);
		input >> player_starting_position.x >> player_starting_position.y;
		input >> ghost_house.x >> ghost_house.y;
		input.close();
	}
	else
	{
		std::cerr << "Failed to open " << map_file_path << std::endl;
	}
}

void pac::Map::readImagePathsAndTileTypes(std::ifstream & input)
{
	char file_path_char;
	std::string file_path, tile_type;
	sf::Texture texture;
	number_of_files = 0;
	pac::TileType temp;
	do
	{
		file_path = "";
		do
		{
			input.get(file_path_char);
			if (file_path_char != '?' && file_path_char != '\n' && file_path_char != '*')
			{
				file_path += file_path_char;
			}
		} while (file_path_char != '?' && file_path_char != '*');
		if (file_path_char != '*')
		{
			textures.push_back(texture);
			textures[number_of_files].loadFromFile(file_path);
			number_of_files++;
			input >> tile_type;
			temp = (tile_type == "WALL") ? pac::WALL : (tile_type == "NORMAL") ? pac::NORMAL : pac::TELEPORTATION;
			tile_types.push_back(temp);
		}
	} while (file_path_char != '*');
}

void pac::Map::readTileBoxesInfo(std::ifstream & input)
{
	char tile_box_wall_char;
	int tile_box_wall_int;
	int row = 0;
	int col = 0;
	int index = 0;
	tile_boxes.resize(number_of_files);
	for (int i = 0; i < number_of_files; i++)
	{
		tile_boxes[i].resize(pac::TILE_BOXES_ROWS);
		for (int j = 0; j < pac::TILE_BOXES_ROWS; j++)
		{
			tile_boxes[i][j].resize(pac::TILE_BOXES_COLS);
		}
	}
	do
	{
		input >> tile_box_wall_char;
		tile_box_wall_int = tile_box_wall_char - pac::ASCII_CHAR_TO_INT_DIFF;
		if (index < number_of_files)
		{
			tile_boxes[index][row][col] = (tile_box_wall_int == 0) ? false : true;
		}
		col++;
		if (col == pac::TILE_BOXES_COLS)
		{
			col = 0;
			row++;
			if (row == pac::TILE_BOXES_ROWS)
			{
				row = 0;
				index++;
			}
		}
	} while (tile_box_wall_char != '{');
}

void pac::Map::readAndCreateMap(std::ifstream & input)
{
	int num;
	char trash;
	pac::TileBox tile_box[pac::TILE_BOXES_ROWS][pac::TILE_BOXES_COLS];
	tile_array.resize(size.row);
	for (int i = 0; i < size.row; i++)
	{
		tile_array[i].resize(size.col);
	}
	for (int i = 0; i < size.row; i++)
	{
		for (int j = 0; j < size.col; j++)
		{
			input >> num;
			input >> trash;
			if (num != 0)
			{
				for (int k = 0; k < pac::TILE_BOXES_ROWS; k++)
				{
					for (int l = 0; l < pac::TILE_BOXES_COLS; l++)
					{
						tile_box[k][l].wall = tile_boxes[num - 1][k][l];
						tile_box[k][l].position.x = position.x + pac::TILE_SIZE.width * j + pac::TILE_BOX.width * l;
						tile_box[k][l].position.y = position.y + pac::TILE_SIZE.height * i + pac::TILE_BOX.height * k;
					}
				}
				tile_array[i][j].init(pac::Position( position.x + pac::TILE_SIZE.width * j, position.y + pac::TILE_SIZE.height * i ), tile_types[num - 1], tile_box, textures[num - 1]);
			}
		}
	}
}

void pac::Map::readAndCreateTeleportationPairs(std::ifstream & input)
{
	char trash;
	pac::Coordinate coordinate_1, coordinate_2;
	std::string direction_1_str, direction_2_str;
	pac::Direction direction_1, direction_2;
	do 
	{
		input >> coordinate_1.x >> trash >> coordinate_1.y >> trash >> direction_1_str >> trash >> coordinate_2.x >> trash >> coordinate_2.y >> trash >> direction_2_str >> trash;
		direction_1 = (direction_1_str == "RIGHT") ? pac::RIGHT : (direction_1_str == "LEFT") ? pac::LEFT : (direction_1_str == "UP") ? pac::UP : pac::DOWN;
		direction_2 = (direction_2_str == "RIGHT") ? pac::RIGHT : (direction_2_str == "LEFT") ? pac::LEFT : (direction_2_str == "UP") ? pac::UP : pac::DOWN;
		pac::TeleportationPairInfo temp_info( coordinate_1, direction_1, coordinate_2, direction_2 );
		teleportation_pairs_info.push_back(temp_info);
	} while (trash != '$');
	teleportation_pairs.resize(teleportation_pairs_info.size());
	for (unsigned int i = 0; i < teleportation_pairs_info.size(); i++)
	{
		teleportation_pairs[i].init(pac::Position( position.x + teleportation_pairs_info[i].teleporter_1_c.x * pac::TILE_SIZE.width, position.y + teleportation_pairs_info[i].teleporter_1_c.y * pac::TILE_SIZE.height ), teleportation_pairs_info[i].teleporter_1_d, pac::Position( position.x + teleportation_pairs_info[i].teleporter_2_c.x * pac::TILE_SIZE.width, position.y + teleportation_pairs_info[i].teleporter_2_c.y * pac::TILE_SIZE.height ), teleportation_pairs_info[i].teleporter_2_d);
	}
}

void pac::Map::readPointFilesAndTypes(std::ifstream & input)
{
	char trash;
	char file_path_char;
	std::string file_path, point_type;
	int point_value;
	sf::Texture texture;
	pac::PointType temp;
	number_of_point_files = 0;
	do
	{
		input.get(trash);
		file_path = "";
		do
		{
			input.get(file_path_char);
			if (file_path_char != '?' && file_path_char != '\n' && file_path_char != '{')
			{
				file_path += file_path_char;
			}
		} while (file_path_char != '?' && file_path_char != '{');
		if (file_path_char != '{')
		{
			point_textures.push_back(texture);
			point_textures[number_of_point_files].loadFromFile(file_path);
			number_of_point_files++;
			input >> point_type;
			temp = (point_type == "NORMAL") ? pac::NORMAL_POINT : pac::POWER_POINT;
			point_types.push_back(temp);
			input >> point_value;
			point_values.push_back(point_value);
		}
	} while (file_path_char != '{');
}

void pac::Map::readAndCreatePoints(std::ifstream & input)
{
	int num;
	char trash;
	points.resize(size.row);
	number_of_points = 0;
	number_of_eaten_points = 0;
	for (int i = 0; i < size.row; i++)
	{
		points[i].resize(size.col);
	}
	for (int i = 0; i < size.row; i++)
	{
		for (int j = 0; j < size.col; j++)
		{
			input >> num;
			input >> trash;
			if (num != 0)
			{
				points[i][j].init(Position( position.x + pac::TILE_SIZE.width * j, position.y + pac::TILE_SIZE.height * i ), point_types[num - number_of_files - 1], point_values[num - number_of_files - 1], point_textures[num - number_of_files - 1]);
				number_of_points++;
			}
		}
	} 
}

void pac::Map::display(sf::RenderWindow & window) const
{
	for (int i = 0; i < size.row; i++)
	{
		for (int j = 0; j < size.col; j++)
		{
			if (tile_array[i][j].getTileType() != pac::TELEPORTATION)
			{
				tile_array[i][j].display(window);
			}
			points[i][j].display(window);
		}
	}
}

void pac::Map::displayTeleportationTiles(sf::RenderWindow & window) const
{
	for (int i = 0; i < size.row; i++)
	{
		for (int j = 0; j < size.col; j++)
		{
			if (tile_array[i][j].getTileType() == pac::TELEPORTATION)
			{
				tile_array[i][j].display(window);
			}
		}
	}

}

pac::Coordinate pac::Map::getCoordinate(const pac::Position & position) const
{
	return pac::Coordinate((position.x - this->position.x) / (pac::TILE_SIZE.width), (position.y - this->position.y) / (pac::TILE_SIZE.height));
}

pac::Position pac::Map::getPosition(const pac::Coordinate & coordinate) const
{
	return pac::Position( position.x + coordinate.x * TILE_SIZE.width, position.y + coordinate.y * TILE_SIZE.height );
}

pac::Position pac::Map::getMapPosition() const
{
	return position;
}

pac::GridSize pac::Map::getMapSize() const
{
	return size;
}

bool pac::Map::getCollidingTiles(const pac::Coordinate & coordinate, const pac::Direction & direction, pac::Tile *tiles[pac::DISTINCT_COLLIDING_TILES])
{
	bool collided = false;

	switch (direction)
	{
	case pac::RIGHT:
		for (int i = 0; i < pac::DISTINCT_COLLIDING_TILES; i++)
		{
			if (lessThan(coordinate.y + i, size.row) && lessThan(coordinate.x + 2, size.col))
			{
				tiles[i] = &(tile_array[coordinate.y + i][coordinate.x + 2]);
				collided = true;
			}
		}
		break;
	case pac::LEFT:
		for (int i = 0; i < pac::DISTINCT_COLLIDING_TILES; i++)
		{
			if (lessThan(coordinate.y + i, size.row) && lessThan(coordinate.x, size.col))
			{
				tiles[i] = &(tile_array[coordinate.y + i][coordinate.x]);
				collided = true;
			}
		}
		break;
	case pac::UP:
		for (int i = 0; i < pac::DISTINCT_COLLIDING_TILES; i++)
		{
			if (lessThan(coordinate.y, size.row) && lessThan(coordinate.x + i, size.col))
			{
				tiles[i] = &(tile_array[coordinate.y][coordinate.x + i]);
				collided = true;
			}
		}
		break;
	case pac::DOWN:
		for (int i = 0; i < pac::DISTINCT_COLLIDING_TILES; i++)
		{
			if (lessThan(coordinate.y + 2, size.row) && lessThan(coordinate.x + i, size.col))
			{
				tiles[i] = &(tile_array[coordinate.y + 2][coordinate.x + i]);
				collided = true;
			}
		}
		break;
	}

	return collided;
}

void pac::Map::getTeleportationPairs(std::vector <pac::TeleportationPair*> & teleportation_pairs)
{
	pac::TeleportationPair *temp;

	for (unsigned int i = 0; i < this->teleportation_pairs.size(); i++)
	{
		temp = &(this->teleportation_pairs[i]);
		teleportation_pairs.push_back(temp);
	}
}

bool pac::Map::getPoint(const pac::Coordinate & coordinate, const pac::Direction & direction, pac::Point *&surrounding_points)
{
	switch (direction)
	{
	case pac::RIGHT:
		return getPoint(pac::Coordinate( coordinate.x + 2, coordinate.y + 1 ), surrounding_points);
	case pac::LEFT:
		return getPoint(pac::Coordinate( coordinate.x, coordinate.y + 1 ), surrounding_points);
	case pac::UP:
		return getPoint(pac::Coordinate( coordinate.x + 1, coordinate.y ), surrounding_points);
	case pac::DOWN:
		return getPoint(pac::Coordinate( coordinate.x + 1, coordinate.y + 2 ), surrounding_points);
	}
}

bool pac::Map::getPoint(const Coordinate & coordinate, pac::Point *&surrounding_points)
{
	if (lessThan(coordinate.y, size.row) && biggerThanOrEqual(coordinate.y, 0) && lessThan(coordinate.x, size.col) && biggerThanOrEqual(coordinate.x, 0))
	{
		surrounding_points = &(points[coordinate.y][coordinate.x]);
		return true;
	}
	return false;
}

pac::Position pac::Map::getPlayerStartingPosition() const
{
	return player_starting_position + position;
}

pac::Position pac::Map::getBlinkyStartingPosition() const
{
	return position + pac::Position(pac::TILE_SIZE.width * (ghost_house.x + 3.f), pac::TILE_SIZE.height * (ghost_house.y - 1.5f));
}

pac::Position pac::Map::getPinkyStartingPosition() const
{
	return position + pac::Position(pac::TILE_SIZE.width * (ghost_house.x + 3.f), pac::TILE_SIZE.height * (ghost_house.y + 1.5f));
}

pac::Position pac::Map::getInkyStartingPosition() const
{
	return position + pac::Position(pac::TILE_SIZE.width * (ghost_house.x + 1.f), pac::TILE_SIZE.height * (ghost_house.y + 1.5f));
}

pac::Position pac::Map::getClydeStartingPosition() const
{
	return position + pac::Position(pac::TILE_SIZE.width * (ghost_house.x + 5.f), pac::TILE_SIZE.height * (ghost_house.y + 1.5f));
}

pac::Position pac::Map::getGhostHouseCenterPosition() const
{
	return position + pac::Position(pac::TILE_SIZE.width * (ghost_house.x + 3.f), pac::TILE_SIZE.height * (ghost_house.y + 1.5f));
}

pac::Position pac::Map::getOutsideGhostHousePosition() const
{
	return position + pac::Position(pac::TILE_SIZE.width * (ghost_house.x + 3.f), pac::TILE_SIZE.height * (ghost_house.y - 1.5f));
}

pac::TileType pac::Map::getTileType(const pac::Coordinate & coordinate) const
{
	return tile_array[coordinate.y][coordinate.x].getTileType();
}

void pac::Map::decrementPoints()
{
	number_of_points--;
	number_of_eaten_points++;
}

int pac::Map::getEatenPoints() const
{
	return number_of_eaten_points;
}

pac::Map::~Map()
{
}