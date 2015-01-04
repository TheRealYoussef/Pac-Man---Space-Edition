#include <SFML/Graphics.hpp>

#include <string>
#include <fstream>
#include <iostream>

#include "Point.h"
#include "TeleportationPair.h"
#include "Player.h"
#include "Tile.h"
#include "Map.h"
#include "GlobalVariables.h"

Map::Map(const Position & position, const std::string & map_file_path)
{
	this->position.x = position.x;
	this->position.y = position.y;
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
	}
	else
		std::cerr << "Failed to open " << map_file_path << std::endl;
}

void Map::readImagePathsAndTileTypes(std::ifstream & input)
{
	char file_path_char;
	std::string file_path, tile_type;
	sf::Texture texture;
	number_of_files = 0;
	TileType temp;
	do
	{
		file_path = "";
		do
		{
			input.get(file_path_char);
			if (file_path_char != '%' && file_path_char != '\n' && file_path_char != '*')
				file_path += file_path_char;
		} while (file_path_char != '%' && file_path_char != '*');
		if (file_path_char != '*')
		{
			textures.push_back(texture);
			textures[number_of_files].loadFromFile(file_path);
			number_of_files++;
			input >> tile_type;
			temp = (tile_type == "WALL") ? WALL : (tile_type == "NORMAL") ? NORMAL : (tile_type == "INTERSECTION") ? INTERSECTION : (tile_type == "TELEPORTATION") ? TELEPORTATION : DOOR;
			tile_types.push_back(temp);
		}
	} while (file_path_char != '*');
}

void Map::readTileBoxesInfo(std::ifstream & input)
{
	char tile_box_wall_char;
	int tile_box_wall_int;
	int row = 0;
	int col = 0;
	int index = 0;
	tile_boxes.resize(number_of_files);
	for (int i = 0; i < number_of_files; i++)
	{
		tile_boxes[i].resize(TILE_BOXES_ROWS);
		for (int j = 0; j < TILE_BOXES_ROWS; j++)
			tile_boxes[i][j].resize(TILE_BOXES_COLS);
	}
	do
	{
		input >> tile_box_wall_char;
		tile_box_wall_int = tile_box_wall_char - ASCII_CHAR_TO_INT_DIFF;
		if (index < number_of_files)
			tile_boxes[index][row][col].wall = (tile_box_wall_int == 0) ? false : true;
		col++;
		if (col == TILE_BOXES_COLS)
		{
			col = 0;
			row++;
			if (row == TILE_BOXES_ROWS)
			{
				row = 0;
				index++;
			}
		}
	} while (tile_box_wall_char != '{');
}

void Map::readAndCreateMap(std::ifstream & input)
{
	int num;
	char trash;
	TileBox tile_box[TILE_BOXES_ROWS][TILE_BOXES_COLS];
	tile_array.resize(size.row);
	for (int i = 0; i < size.row; i++)
		tile_array[i].resize(size.col);
	for (int i = 0; i < size.row; i++)
	{
		for (int j = 0; j < size.col; j++)
		{
			input >> num;
			input >> trash;
			if (num != 0)
			{
				for (int k = 0; k < TILE_BOXES_ROWS; k++)
				{
					for (int l = 0; l < TILE_BOXES_COLS; l++)
					{
						tile_box[k][l].wall = tile_boxes[num - 1][k][l].wall;
						tile_box[k][l].position.x = position.x + TILE_SIZE.width * TILE_SCALE.x * j + TILE_BOX.width * l;
						tile_box[k][l].position.y = position.y + TILE_SIZE.height * TILE_SCALE.y * i + TILE_BOX.height * k;
					}
				}
				Tile temp(Position{ position.x + TILE_SIZE.width * TILE_SCALE.x * j, position.y + TILE_SIZE.height * TILE_SCALE.y * i }, tile_types[num - 1], tile_box, textures[num - 1]);
				tile_array[i][j] = temp;
			}
		}
	}
}

void Map::readAndCreateTeleportationPairs(std::ifstream & input)
{
	char trash;
	int num1, num2, num3, num4;
	std::string direction1str, direction2str;
	Direction direction1, direction2;
	do 
	{
		input >> num1 >> trash >> num2 >> trash >> direction1str >> trash >> num3 >> trash >> num4 >> trash >> direction2str >> trash;
		direction1 = (direction1str == "RIGHT") ? RIGHT : (direction1str == "LEFT") ? LEFT : (direction1str == "UP") ? UP : DOWN;
		direction2 = (direction2str == "RIGHT") ? RIGHT : (direction2str == "LEFT") ? LEFT : (direction2str == "UP") ? UP : DOWN;
		TeleportationPair temp(Position{ position.x + num1 * TILE_SIZE.width * TILE_SCALE.x, position.y + num2 * TILE_SIZE.height * TILE_SCALE.y }, direction1, Position{ position.x + num3 * TILE_SIZE.width * TILE_SCALE.x, position.y + num4 * TILE_SIZE.height * TILE_SCALE.y }, direction2);
		teleportation_pairs.push_back(temp);
	} while (trash != '$');
}

void Map::readPointFilesAndTypes(std::ifstream & input)
{
	char trash;
	char file_path_char;
	std::string file_path, point_type;
	int point_value;
	sf::Texture texture;
	PointType temp;
	number_of_point_files = 0;
	do
	{
		input.get(trash);
		file_path = "";
		do
		{
			input.get(file_path_char);
			if (file_path_char != '%' && file_path_char != '\n' && file_path_char != '{')
				file_path += file_path_char;
		} while (file_path_char != '%' && file_path_char != '{');
		if (file_path_char != '{')
		{
			point_textures.push_back(texture);
			point_textures[number_of_point_files].loadFromFile(file_path);
			number_of_point_files++;
			input >> point_type;
			temp = (point_type == "NORMAL") ? NORMAL_POINT : POWER_POINT;
			point_types.push_back(temp);
			input >> point_value;
			point_values.push_back(point_value);
		}
	} while (file_path_char != '{');
}

void Map::readAndCreatePoints(std::ifstream & input)
{
	int num;
	char trash;
	points.resize(size.row);
	for (int i = 0; i < size.row; i++)
		points[i].resize(size.col);
	for (int i = 0; i < size.row; i++)
	{
		for (int j = 0; j < size.col; j++)
		{
			input >> num;
			input >> trash;
			if (num != 0)
			{
				Point temp(Position{ position.x + TILE_SIZE.width * TILE_SCALE.x * j, position.y + TILE_SIZE.height * TILE_SCALE.y * i }, point_types[num - number_of_files - 1], point_values[num - number_of_files - 1], point_textures[num - number_of_files - 1]);
				points[i][j] = temp;
			}
		}
	} 
}

void Map::display(sf::RenderWindow & window) const
{
	for (int i = 0; i < size.row; i++)
	{
		for (int j = 0; j < size.col; j++)
		{
			if (tile_array[i][j].tile_type != TELEPORTATION)
				tile_array[i][j].display(window);
		}
	}
}

void Map::displayTeleportationTiles(sf::RenderWindow & window) const
{
	for (int i = 0; i < size.row; i++)
	{
		for (int j = 0; j < size.col; j++)
		{
			if (tile_array[i][j].tile_type == TELEPORTATION)
				tile_array[i][j].display(window);
		}
	}
}

void Map::displayPoints(sf::RenderWindow & window) const
{
	for (int i = 0; i < size.row; i++)
		for (int j = 0; j < size.col; j++)
			points[i][j].display(window);
}

bool Map::isCollidingRight(Player & object) const
{
	int x = (object.getPosition().x - position.x) / (TILE_SIZE.width * TILE_SCALE.x);
	int y = (object.getPosition().y - position.y) / (TILE_SIZE.height * TILE_SCALE.y);
	for (int i = y; i < y + 3; i++)
	{
		int j = x + 2;
		if (i >= 0 && i < size.row && j >= 0 && j < size.col)
		{
			if (tile_array[i][j].isCollidingRight(object))
				return true;
		}
	}
	return false;
}

bool Map::isCollidingLeft(Player & object) const
{
	int x = (object.getPosition().x - position.x) / (TILE_SIZE.width * TILE_SCALE.x);
	int y = (object.getPosition().y - position.y) / (TILE_SIZE.height * TILE_SCALE.y);
	for (int i = y; i < y + 3; i++)
	{
		int j = x;
		if (i >= 0 && i < size.row && j >= 0 && j < size.col)
		{
			if (tile_array[i][j].isCollidingLeft(object))
				return true;
		}
	}
	return false;
}

bool Map::isCollidingUp(Player & object) const
{
	int x = (object.getPosition().x - position.x) / (TILE_SIZE.width * TILE_SCALE.x);
	int y = (object.getPosition().y - position.y) / (TILE_SIZE.height * TILE_SCALE.y);
	int i = y;
	for (int j = x; j < x + 3; j++)
	{
		if (i >= 0 && i < size.row && j >= 0 && j < size.col)
		{
			if (tile_array[i][j].isCollidingUp(object))
				return true;
		}
	}
	return false;
}

bool Map::isCollidingDown(Player & object) const
{
	int x = (object.getPosition().x - position.x) / (TILE_SIZE.width * TILE_SCALE.x);
	int y = (object.getPosition().y - position.y) / (TILE_SIZE.height * TILE_SCALE.y);
	int i = y + 2;
	for (int j = x; j < x + 3; j++)
	{
		if (i >= 0 && i < size.row && j >= 0 && j < size.col)
		{
			if (tile_array[i][j].isCollidingDown(object))
				return true;
		}
	}
	return false;
}

void Map::teleport(Player & object) const
{
	for (unsigned int i = 0; i < teleportation_pairs.size(); i++)
		teleportation_pairs[i].teleport(object);
}

void Map::eatPoint(const Player & player)
{
	int x = (player.getPosition().x - position.x) / (TILE_SIZE.width * TILE_SCALE.x);
	int y = (player.getPosition().y - position.y) / (TILE_SIZE.height * TILE_SCALE.y);
	for (int i = y + 1; i < y + 3; i++)
	{
		for (int j = x + 1; j < x + 3; j++)
		{
			if (i >= 0 && i < size.row && j >= 0 && j < size.col)
			{
				points[i][j].eat(player);
			}
		}
	}
}
