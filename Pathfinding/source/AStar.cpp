#include "AStar.h"

bool runAstar(std::vector<std::pair<TypeCell, Cell>>& cells,
	std::vector<std::pair<float, sf::Vector2i>>& openlist,
	std::vector<sf::Vector2i>& path,
	sf::Vector2i start, sf::Vector2i goal,
	float(*heuristic)(int, int, int, int), std::vector<sf::Vector2i>& directions)
{
	if (openlist.empty())
		return true;
	
	std::sort(openlist.begin(), openlist.end(), compareFloatVector2i);
	std::pair<float, sf::Vector2i> current = openlist.back();
	openlist.pop_back();

	cells[(WIDTH_TILEMAP * current.second.y) + current.second.x].second.isAnalyse = true;
	
	if (current.second == goal)
	{
		reconstructPath(cells,path,start, goal);
		return true;
	}

	for (auto& i : directions)
	{
		sf::Vector2i neighbour = sf::Vector2i(current.second.x + i.x, current.second.y + i.y);

		if (!isValid(neighbour.x, neighbour.y))
			continue;

		auto& neighbourCell = cells[(WIDTH_TILEMAP * neighbour.y) + neighbour.x];
		auto& neighbourCellData = neighbourCell.second;

		if (neighbourCell.first == Wall || neighbourCellData.isAnalyse == true)
			continue;

		float temp = 1.f;
		if (i.x != 0 && i.y != 0)
		{
			temp = 1.414f;

			int adjacentX = i.x > 0 ? -1 : 1;
			int adjacentY = i.y > 0 ? -1 : 1;

			if(!(isValid(neighbour.x + adjacentX, neighbour.y) && cells[(WIDTH_TILEMAP * neighbour.y) + neighbour.x + adjacentX].first != Wall)
				|| !(isValid(neighbour.x, neighbour.y + adjacentY) && cells[(WIDTH_TILEMAP * (neighbour.y + adjacentY)) + neighbour.x].first != Wall))
				continue;
		}
			
		float tentativeGScore = cells[(WIDTH_TILEMAP * current.second.y) + current.second.x].second.g + temp;

		if (neighbourCellData.isVisit == false ||
			tentativeGScore < neighbourCellData.g)
		{
			neighbourCellData.xParent = current.second.x;
			neighbourCellData.yParent = current.second.y;
			neighbourCellData.g = tentativeGScore;
			neighbourCellData.f = neighbourCellData.g + heuristic(neighbour.x, neighbour.y, goal.x, goal.y);
			neighbourCellData.isVisit = true;

			auto it = std::find_if(openlist.begin(), openlist.end(),
				[neighbour](const std::pair<float, sf::Vector2i>& element) {
					return element.second == neighbour;
				});

			if (it != openlist.end())
				it->first = neighbourCellData.f;
			else
				openlist.push_back(std::make_pair(neighbourCellData.f, neighbour));
		}
	}

	return false;
}

void reconstructPath(std::vector<std::pair<TypeCell, Cell>>& cells,
	std::vector<sf::Vector2i>& path,
	sf::Vector2i& start, sf::Vector2i& goal)
{
	path.push_back(goal);
	auto element = cells[(WIDTH_TILEMAP * goal.y) + goal.x].second;

	while (path.back() != start)
	{
		path.emplace_back(element.xParent, element.yParent);
		auto& elementPath = cells[(WIDTH_TILEMAP * element.yParent) + element.xParent];
		element = cells[(WIDTH_TILEMAP * element.yParent) + element.xParent].second;
	}
}

void resetAstar(std::vector<std::pair<TypeCell, Cell>>& cells,
	std::vector<std::pair<float, sf::Vector2i>>& openlist,
	std::vector<sf::Vector2i>& path,
	sf::Vector2i& start, sf::Vector2i& goal,
	float(*heuristic)(int, int, int, int))
{
	Cell defaultCell;
	for (auto& i : cells) 
	{
		i.second = defaultCell;
	}

	auto& startCell = cells[(WIDTH_TILEMAP * start.y) + start.x];
	startCell.second.xParent = start.x;
	startCell.second.yParent = start.y;
	startCell.second.g = 0.f;
	startCell.second.f = heuristic(start.x, start.y, goal.x, goal.y);
	startCell.second.isVisit = false;
	startCell.second.isAnalyse = false;

	openlist.clear();
	openlist.push_back(std::make_pair(0.f, start));
	path.clear();
}

float calculateManhattan(int x, int y, int goalX, int goalY)
{
	return std::abs(static_cast<float>(x - goalX)) + std::abs(static_cast<float>(y - goalY));
}

float calculateChebyshev(int x, int y, int goalX, int goalY)
{
	float dx = std::abs(static_cast<float>(x - goalX));
	float dy = std::abs(static_cast<float>(y - goalY));
	return (dx + dy) + (- 1.f) * std::min(dx, dy);
}

float calculateOctile(int x, int y, int goalX, int goalY)
{
	float dx = std::abs(static_cast<float>(x - goalX));
	float dy = std::abs(static_cast<float>(y - goalY));
	return (dx + dy) + (std::sqrtf(2.f) - 2.f) * std::min(dx, dy);
}

float calculateEuclidean(int x, int y, int goalX, int goalY)
{
	return std::sqrtf(std::powf(static_cast<float>(x - goalX), 2) + std::powf(static_cast<float>(y - goalY), 2));
}

bool compareFloatVector2i(const std::pair<float, sf::Vector2i>& a, const std::pair<float, sf::Vector2i>& b)
{
	return a.first > b.first;
}

bool isValid(int x, int y)
{
	return 0 <= x && x < WIDTH_TILEMAP && 0 <= y && y < HEIGHT_TILEMAP;
}
