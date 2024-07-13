#include "Game.h"
#include <random>
#include <ranges>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>


Game::Game() : gameState(GameStateType::PlayerTurn), playerHand(0), kingSlayerHand(1), higherMindHand(2)
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


	background.setSize(sf::Vector2f(1000, 1000));
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
		
		switch (gameState)
		{
		case GameStateType::ChoiceMenu:
			if (!choiceMenu)
			{
				choiceMenu = std::make_unique<ChoiceMenu>();
			}
			choiceMenu->HandleMenuWindowEvent(event);
			break;
		case GameStateType::GameOver:
			if (!gameOverMenu)
			{
				gameOverMenu = std::make_unique<GameOverMenu>();
			}
			gameOverMenu->HandleMenuWindowEvent(event);
			break;
		case GameStateType::PlayerTurn:
			choiceMenu.reset();
			kingSlayerHand.HandleHandWindowEvent(event); 
			higherMindHand.HandleHandWindowEvent(event);
			break;
		case GameStateType::KingSlayerTurn:
			break;
		case GameStateType::HigherMindTurn:
			break;
		case GameStateType::ShowDialog:
			break;
		default:
			break;
		}

	}
	
}

bool Game::Update(float timeDelta)
{
	switch (gameState)
	{
	case GameStateType::KingSlayerTurn:
		currentAIHand = &kingSlayerHand;
		isRight = currentAIHand->GetMind().GuessTry(hands[2], hands[0], hands[1], valueForSwap);
		targetHand = WithWhomSwap();
		if (isRight)
		{
			dialogSize = currentAIHand->GetDialog().size(); 
			currentDialogIndex = 0; 
			TurnShowDialog(); 
		}
		else
		{
			WithWhomSwap()->NotChoosedForQuestions();
			dialogSize = currentAIHand->GetDialog().size(); 
			currentDialogIndex = 0;
			TurnShowDialog();
		}
		break;	
	case GameStateType::HigherMindTurn:
		currentAIHand = &higherMindHand;
		isRight = currentAIHand->GetMind().GuessTry(hands[0], hands[1], hands[2], valueForSwap);
		targetHand = WithWhomSwap();
		if (isRight)
		{ 
			dialogSize = currentAIHand->GetDialog().size(); 
			currentDialogIndex = 0; 
			TurnShowDialog(); 
		}
		else
		{
			WithWhomSwap()->NotChoosedForQuestions(); 
			dialogSize = currentAIHand->GetDialog().size(); 
			currentDialogIndex = 0;
			TurnShowDialog(); 
		} 
		break;
	case GameStateType::ShowDialog:
		if (timeForDelay > delay)
		{
			if (currentDialogIndex >= dialogSize)
			{
				// Clear dialog messages
				currentAIHand->ClearDialog();
				targetHand->ClearDialog();

				if (currentAIHand == &kingSlayerHand)
				{
					if (isRight)
					{
						SwapCards(kingSlayerHand, valueForSwap);
						SetKingSlayerTurn();
					}
					else
					{
						SetHigherMindTurn();
					}
				}
				else if (currentAIHand == &higherMindHand)
				{
					if (isRight)
					{
						SwapCards(kingSlayerHand, valueForSwap);
						SetHigherMindTurn();
					}
					else
					{
						SetPlayerTurn();
					}
				}
			}
			else
			{
				currentAIHand->DrawDialog();
				targetHand->DrawDialog();
				++currentDialogIndex;
			}
			timeForDelay = 0.f;
		}
		else
		{
			timeForDelay += timeDelta;
		}		
		break;
	default:
		break; 
	}
	return true; // Return based on your logic
}
void Game::Draw(sf::RenderWindow& window)
{
	window.draw(background);
	for (auto& currentHand : hands)
	{
		currentHand->DrawHand(window);
	}
	switch (gameState)
	{
	case GameStateType::ChoiceMenu:
		if (!choiceMenu)
		{
			choiceMenu = std::make_unique<ChoiceMenu>();
		}
		choiceMenu->DrawMenu(window); 
		break;
	case GameStateType::GameOver:
		if (!gameOverMenu)
		{
			gameOverMenu = std::make_unique<GameOverMenu>();
		}
		gameOverMenu->DrawMenu(window);
		break;
	case GameStateType::PlayerTurn:
			break;
	case GameStateType::KingSlayerTurn:
			break;
		
	case GameStateType::HigherMindTurn:
			break;
	default:
		break;
	}
}

void Game::Shutdown()
{
	
}

void Game::SwapCards(Hand& receivingHand, Value& cardsValue)
{
	receivingHand.TakeCards(*(WithWhomSwap()), cardsValue);
	receivingHand.CheckFours();
	WithWhomSwap()->NotChoosedForQuestions(); 
	isGameOver();
}

void Game::isGameOver()
{
	if (hands[0]->GetCards().size() == 0 && hands[1]->GetCards().size() == 0 && hands[2]->GetCards().size() == 0)
	{
		gameState = GameStateType::GameOver;
	}
}


Hand* Game::WithWhomSwap()
{
	for (auto& currentHand : hands)
	{
		if (currentHand->isChoosedForQuestions())
		{
			return currentHand;
		}
	}
	return nullptr;
}
