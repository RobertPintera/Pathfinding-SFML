#ifndef ASTAR_H
#define ASTAR_H

#include "Global.h"

bool runAstar(
	std::vector<std::pair<TypeCell, Cell>>& cells,
	std::vector<std::pair<float, sf::Vector2i>>& openlist,
	std::vector<sf::Vector2i>& path,
	sf::Vector2i start, sf::Vector2i goal, 
	float(*heuristic)(int, int, int, int), std::vector<sf::Vector2i>& directions);

void reconstructPath(
	std::vector<std::pair<TypeCell, Cell>>& cells,
	std::vector<sf::Vector2i>& path,
	sf::Vector2i& start,sf::Vector2i& goal);

void resetAstar(
	std::vector<std::pair<TypeCell, Cell>>& cells,
	std::vector<std::pair<float, sf::Vector2i>>& openlist,
	std::vector<sf::Vector2i>& path,
	sf::Vector2i& start, sf::Vector2i& goal,
	float(*heuristic)(int, int, int, int));

float calculateManhattan(int x, int y, int goalX, int goalY);
float calculateChebyshev(int x, int y, int goalX, int goalY);
float calculateOctile(int x, int y, int goalX, int goalY);
float calculateEuclidean(int x, int y, int goalX, int goalY);

bool compareFloatVector2i(const std::pair<float, sf::Vector2i>& a, const std::pair<float, sf::Vector2i>& b);
bool isValid(int x, int y);


#endif // !ASTAR_H