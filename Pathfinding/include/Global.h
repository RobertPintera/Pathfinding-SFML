#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>
#include <iostream>
#include <memory>
#include "SFML/Graphics.hpp"

#define WIDTH_TILEMAP 42
#define HEIGHT_TILEMAP 42

#define COLOR_TILE_NONE sf::Color(255,255,255,255)
#define COLOR_TILE_START sf::Color(0, 219, 32, 255)
#define COLOR_TILE_WALL sf::Color(128, 128, 128, 255)
#define COLOR_TILE_END sf::Color(250, 114, 0, 255)

#define COLOR_TILE_ANALYSIS sf::Color(0, 0, 255,255)
#define COLOR_TILE_VISIT sf::Color(10, 147, 252,255)
#define COLOR_TILE_PATH sf::Color(255, 213, 77, 255)

struct Cell
{
	int xParent = 0;
	int yParent = 0;
	float g = std::numeric_limits<float>::infinity();
	float f = std::numeric_limits<float>::infinity();
	bool isVisit = false;
	bool isAnalyse = false;
};

enum TypeCell
{
	None,
	Start,
	Wall,
	End,
};

enum Metric
{
	Manhattan,
	Octile,
	Chebyshev,
	Euclidean,
};

#endif // !GLOBAL_H
