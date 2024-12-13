#ifndef APP_H
#define APP_H

#include "imgui.h"
#include "imgui-SFML.h"
#include "AStar.h"

class App
{
public:
	App();
	virtual ~App();

	void updateMousePosition();
	void updateEvents();
	void updateEditTilemap();
	void updateWidgets();
	void update();

	void render();

	void run();
private:
	sf::RenderWindow window;
	sf::Vector2f mousePositionWindow;
	sf::Clock deltaTime;
	bool isFocus;
	
	bool isRun;
	bool isClear;

	TypeCell selectedTypeCell;
	Metric selectedMetric;
	float(*heuristic)(int, int, int, int);
	bool direction;
	
	bool isStart;
	bool isGoal;

	sf::Vector2i startPosition;
	sf::Vector2i goalPosition;

	std::vector<sf::RectangleShape> tilemap;

	std::vector<std::pair<TypeCell, Cell>> cells;
	std::vector<std::pair<float, sf::Vector2i>> openlist;
	std::vector<sf::Vector2i> path;
};

#endif // !APP_H