#include "App.h"

App::App() :
	window(sf::VideoMode(1040, 800), "Pathfinding", sf::Style::Titlebar | sf::Style::Close),
	mousePositionWindow(0.f, 0.f), isFocus(true), isRun(false), isGoal(false),
	selectedTypeCell(TypeCell::None), selectedMetric(Metric::Manhattan), heuristic(calculateManhattan),
	direction(false), isStart(false),isClear(true)
{
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window);

	tilemap.resize(WIDTH_TILEMAP * HEIGHT_TILEMAP);
	cells.resize(WIDTH_TILEMAP * HEIGHT_TILEMAP);

	float startX = 16.f;
	float startY = 16.f;
	float tileWidth = 16.f;
	float tileHeight = 16.f;

	for (size_t y = 0; y < HEIGHT_TILEMAP; y++)
	{
		for (size_t x = 0; x < WIDTH_TILEMAP; x++)
		{
			auto& element = tilemap[(y * WIDTH_TILEMAP) + x];
			element.setSize(sf::Vector2f(tileWidth, tileHeight));
			element.setPosition(startX + static_cast<float>(x) * (tileWidth + 2.f), startY + static_cast<float>(y) * (tileHeight + 2.f));
			element.setFillColor(COLOR_TILE_NONE);
		}
	}
}

App::~App()
{
	ImGui::SFML::Shutdown();
}

void App::updateMousePosition()
{
	mousePositionWindow = window.mapPixelToCoords(sf::Mouse::getPosition(window));
}

void App::updateEvents()
{
	sf::Event sfEvent;
	while (window.pollEvent(sfEvent))
	{
		ImGui::SFML::ProcessEvent(sfEvent);
		if (sfEvent.type == sf::Event::Closed)
		{
			window.close();
		}
		else if (sfEvent.type == sf::Event::GainedFocus)
		{
			isFocus = true;
		}
		else if (sfEvent.type == sf::Event::LostFocus)
		{
			isFocus = false;
		}
	}
}

void App::updateEditTilemap()
{
	for (size_t i = 0; i < HEIGHT_TILEMAP; i++)
	{
		for (size_t j = 0; j < WIDTH_TILEMAP; j++)
		{
			auto& tile = tilemap[(WIDTH_TILEMAP * i) + j];
			auto& cell = cells[(WIDTH_TILEMAP * i) + j];

			if (tile.getGlobalBounds().contains(mousePositionWindow))
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if (!isClear)
					{
						for (size_t i = 0; i < tilemap.size(); i++)
						{
							if (cells[i].first == TypeCell::None)
							{
								tilemap[i].setFillColor(COLOR_TILE_NONE);
							}
						}
					}

					if (cell.first == TypeCell::None)
					{
						if (selectedTypeCell == TypeCell::Start)
						{
							cell.first = TypeCell::Start;
							tile.setFillColor(COLOR_TILE_START);
							isStart = true;
							sf::Vector2i currentlyStartPosition = sf::Vector2i(j, i);
							if (startPosition != currentlyStartPosition)
							{
								auto& previousTile = tilemap[(WIDTH_TILEMAP * startPosition.y) + startPosition.x];
								auto& previousCell = cells[(WIDTH_TILEMAP * startPosition.y) + startPosition.x];
								previousTile.setFillColor(COLOR_TILE_NONE);
								previousCell.first = TypeCell::None;
							}
							startPosition = sf::Vector2i(j, i);
							
						}
						else if (selectedTypeCell == TypeCell::Wall)
						{
							cell.first = TypeCell::Wall;
							tile.setFillColor(COLOR_TILE_WALL);
						}
						else if (selectedTypeCell == TypeCell::End)
						{
							cell.first = TypeCell::End;
							tile.setFillColor(COLOR_TILE_END);
							isGoal = true;
							sf::Vector2i currentlyGoalPosition = sf::Vector2i(j, i);
							if (goalPosition != currentlyGoalPosition)
							{
								auto& previousTile = tilemap[(WIDTH_TILEMAP * goalPosition.y) + goalPosition.x];
								auto& previousCell = cells[(WIDTH_TILEMAP * goalPosition.y) + goalPosition.x];
								previousTile.setFillColor(COLOR_TILE_NONE);
								previousCell.first = TypeCell::None;
							}
							goalPosition = sf::Vector2i(j, i);
						}
					}
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				{
					if (!isClear)
					{
						for (size_t i = 0; i < tilemap.size(); i++)
						{
							if (cells[i].first == TypeCell::None)
							{
								tilemap[i].setFillColor(COLOR_TILE_NONE);
							}
						}
					}

					if (cell.first == TypeCell::Start && selectedTypeCell == TypeCell::Start)
					{
						cell.first = TypeCell::None;
						tile.setFillColor(COLOR_TILE_NONE);
						isStart = false;
					}
					else if (cell.first == Wall && selectedTypeCell == TypeCell::Wall)
					{
						cell.first = TypeCell::None;
						tile.setFillColor(COLOR_TILE_NONE);
					}
					else if (cell.first == End && selectedTypeCell == TypeCell::End)
					{
						cell.first = TypeCell::None;
						tile.setFillColor(COLOR_TILE_NONE);
						isGoal = false;
					}
				}
			}
		}
		
	}
}


void App::updateWidgets()
{
	ImGui::SetNextWindowPos(ImVec2(800.f, 50.f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(0, 0));
	ImGui::Begin("Frame", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
	ImGui::SetWindowFontScale(2.f);

	ImGui::BeginDisabled(isRun);
	ImGui::Text("Type Cells");
	ImGui::Spacing();
	if (ImGui::RadioButton("Start Cell", selectedTypeCell == TypeCell::Start)) {
		selectedTypeCell = (selectedTypeCell == TypeCell::Start) ? TypeCell::None : TypeCell::Start;
	}
	if (ImGui::RadioButton("Obstacle Cell", selectedTypeCell == TypeCell::Wall)) {
		selectedTypeCell = (selectedTypeCell == TypeCell::Wall) ? TypeCell::None : TypeCell::Wall;
	}
	if (ImGui::RadioButton("Goal Cell", selectedTypeCell == TypeCell::End)) {
		selectedTypeCell = (selectedTypeCell == TypeCell::End) ? TypeCell::None : TypeCell::End;
	}
	ImGui::Spacing();
	ImGui::Text("Directions");
	ImGui::Spacing();
	bool isFour = !direction;
	bool isEight = direction;
	if (ImGui::Checkbox("Four", &isFour)) {
		direction = false;
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Eight", &isEight)) {
		direction = true;
	}
	ImGui::Spacing();
	ImGui::Text("Metrics");
	ImGui::Spacing();
	if (ImGui::RadioButton("Manhattan", selectedMetric == Metric::Manhattan)) {
		selectedMetric = Metric::Manhattan;
		heuristic = calculateManhattan;
	}
	if (ImGui::RadioButton("Octile", selectedMetric == Metric::Octile)) {
		selectedMetric = Metric::Octile;
		heuristic = calculateOctile;
	}
	if (ImGui::RadioButton("Chebyshev", selectedMetric == Metric::Chebyshev)) {
		selectedMetric = Metric::Chebyshev;
		heuristic = calculateChebyshev;
	}
	if (ImGui::RadioButton("Euclidean", selectedMetric == Metric::Euclidean)) {
		selectedMetric = Metric::Euclidean;
		heuristic = calculateEuclidean;
	}
	ImGui::EndDisabled();

	ImGui::Dummy(ImVec2(0.f, 80.f));
	ImGui::Text("Activities");
	ImGui::Spacing();


	if (ImGui::Button("Start", ImVec2(200.f, 50.f)))
	{
		if (isStart && isGoal)
		{
			system("cls");
			isRun = true;
			resetAstar(cells, openlist, path, startPosition, goalPosition, heuristic);
			for (size_t i = 0; i < tilemap.size(); i++)
			{
				if (cells[i].first == TypeCell::None)
				{
					tilemap[i].setFillColor(COLOR_TILE_NONE);
				}
			}
		}
	}

	if (ImGui::Button("Stop", ImVec2(200.f, 50.f)))
	{
		isRun = false;
		for (size_t i = 0; i < tilemap.size(); i++)
		{
			if (cells[i].first == TypeCell::None)
			{
				tilemap[i].setFillColor(sf::Color(COLOR_TILE_NONE));
			}
		}
	}
	if (ImGui::Button("Clear", ImVec2(200.f, 50.f)))
	{
		for (size_t i = 0; i < cells.size(); i++)
		{
			isRun = false;
			isStart = false;
			isGoal = false;
			cells[i].first = TypeCell::None;
			tilemap[i].setFillColor(sf::Color(COLOR_TILE_NONE));
		}
	}
	ImGui::End();
}

void App::update()
{
	updateEvents();
	updateMousePosition();

	ImGui::SFML::Update(window,deltaTime.restart());
	updateWidgets();

	if (isRun)
	{
		if (!direction) 
		{
			std::vector<sf::Vector2i> directions = { sf::Vector2i(-1, 0), sf::Vector2i(1, 0), sf::Vector2i(0, -1),sf::Vector2i(0, 1) };
			isRun = !runAstar(cells, openlist, path, startPosition, goalPosition, heuristic, directions);
		}
		else 
		{
			std::vector<sf::Vector2i> directions = { 
				sf::Vector2i(-1, 0), sf::Vector2i(1, 0), sf::Vector2i(0, -1),sf::Vector2i(0, 1),
				sf::Vector2i(-1, -1), sf::Vector2i(1, 1), sf::Vector2i(1, -1),sf::Vector2i(-1, 1) };
			isRun = !runAstar(cells, openlist, path, startPosition, goalPosition, heuristic, directions);
		}

		for (size_t i = 0; i < HEIGHT_TILEMAP; i++)
		{
			for (size_t j = 0; j < WIDTH_TILEMAP; j++)
			{
				int temp = (WIDTH_TILEMAP * i) + j;
				if (cells[temp].first == None)
				{
					if (cells[temp].second.isAnalyse == true)
					{
						tilemap[temp].setFillColor(COLOR_TILE_ANALYSIS);
					}
					else if (cells[temp].second.isVisit == true)
					{
						tilemap[temp].setFillColor(COLOR_TILE_VISIT);
					}
				}
				
			}
		}
		if (!isRun) {
			isClear = false;

			for (auto& i : path)
			{
				int temp = (WIDTH_TILEMAP * i.y) + i.x;
				if (cells[temp].first == None)
				{
					tilemap[temp].setFillColor(COLOR_TILE_PATH);
				}
			}
		}
	}
	else if(isFocus)
	{
		updateEditTilemap();
	}
}

void App::render()
{
	window.clear();

	for (auto& i : tilemap)
	{
		window.draw(i);
	}

	ImGui::SFML::Render(window);

	window.display();
}

void App::run()
{
	while (window.isOpen())
	{
		update();
		render();
	}
}

