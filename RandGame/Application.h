#pragma once

#include "Game.h"
#include <random>


class Application
{
public:
	Application(const Application& app) = delete;
	Application& operator= (const Application&) = delete;
	static Application& Instance();

	void Run();

	Game& GetGame() { return game; }

private:
	Application();
	~Application() = default;

private:
	Game game;
	sf::RenderWindow window;
	// Генератор случайных чисел и распределение
	std::mt19937 gen;

};

