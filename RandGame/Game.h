#pragma once
#include <SFML/Graphics.hpp>
#include "PlayerHand.h"
#include "AIHand.h"
#include <list>
class Game
{
public:
	Game();
	~Game() = default;
	void HandleWindowEvents(sf::RenderWindow& window);
	bool Update(float timeDelta); // Return false if game should be closed
	void Draw(sf::RenderWindow& window);
	void Shutdown();

private:
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;

	sf::RectangleShape background;
	PlayerHand playerHand;
	AIHand kingSlayerHand;
	AIHand higherMindHand;
	std::vector <Hand*>  hands;



};