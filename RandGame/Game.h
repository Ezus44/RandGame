#pragma once
#include <SFML/Graphics.hpp>
#include "PlayerHand.h"
#include "AIHand.h"
#include "ChoiceMenu.h"
#include "GameOverMenu.h"
#include <list>

enum class GameStateType
{
	PlayerTurn = 0,
	KingSlayerTurn,
	HigherMindTurn,
	ChoiceMenu,
	GameOver,
	ShowDialog
};

class Game
{
public:
	Game();
	~Game() = default;
	void HandleWindowEvents(sf::RenderWindow& window);
	bool Update(float timeDelta); // Return false if game should be closed
	void Draw(sf::RenderWindow& window);
	void Shutdown();
	void SwapCards(Hand& receivingHand, Value& cardsValue);
	void isGameOver();
	void SetPlayerTurn() { if (gameState!=GameStateType::GameOver) { gameState = GameStateType::PlayerTurn; } }
	void SetKingSlayerTurn() { if (gameState != GameStateType::GameOver) { gameState = GameStateType::KingSlayerTurn; } }
	void SetHigherMindTurn() { if (gameState != GameStateType::GameOver) { gameState = GameStateType::HigherMindTurn; } }
	void TurnChoiceMenu() { gameState = GameStateType::ChoiceMenu; } 
	void TurnShowDialog() { gameState = GameStateType::ShowDialog; }

	Hand* WithWhomSwap();

	PlayerHand& GetPlayerHand() { return playerHand; }
	AIHand& GetKingSlayerHand() { return kingSlayerHand; }
	AIHand& GetHigherMindHand() { return higherMindHand; }
	 
private:
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;

	sf::RectangleShape background;
	std::vector <Hand*>  hands;
	PlayerHand playerHand; 
	AIHand kingSlayerHand; 
	AIHand higherMindHand; 
	GameStateType gameState;
	std::unique_ptr<BaseRealisationMenu> choiceMenu;
	std::unique_ptr<BaseRealisationMenu> gameOverMenu;
	AIHand* currentAIHand = nullptr; 
	Hand* targetHand = nullptr; 
	float delay = 0.5f;
	size_t dialogSize = 0;
	size_t currentDialogIndex = 0;
	float timeForDelay = 0.f;
	bool isRight = false;
	Value valueForSwap = Value::A;
};