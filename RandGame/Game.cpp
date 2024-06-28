#include "Game.h"
#include <random>
#include <ranges>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>


Game::Game()
{

	hands.push_back(&playerHand);   
	hands.push_back(&kingSlayerHand);  
	hands.push_back(&higherMindHand);
	
	std::vector <Card> cardsDeck;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			Card card(i, j);
			cardsDeck.push_back(card);
			
		}
	}
	std::random_device rd; 
	std::mt19937 g(rd()); 
	std::shuffle(cardsDeck.begin(), cardsDeck.end(), g);

	for (auto& currentHand : hands)
	{
		for (int i = 0; i < 12; ++i)
		{
			currentHand->AddCard(cardsDeck.back());
			cardsDeck.pop_back();
		}
		currentHand->CheckFours(); 
	}


	background.setSize(sf::Vector2f(800, 800));
	background.setFillColor(sf::Color::Cyan);
	background.setPosition(0.f, 0.f);

}



void Game::HandleWindowEvents(sf::RenderWindow& window)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		// Close window if close button or Escape key pressed
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
	}
}

bool Game::Update(float timeDelta)
{
	return true;
	
}

void Game::Draw(sf::RenderWindow& window)
{
	window.draw(background);
	for (int i = 0; i < hands.size(); ++i)
	{
		hands[i]->DrawHand(window, i);
	}
	/*for (auto& card : playerHand.GetCards()) {
		card.second.Draw(window);
	}*/
}

void Game::Shutdown()
{
	
}