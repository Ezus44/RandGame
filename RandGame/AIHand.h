#pragma once
#include "Hand.h"
#include "AIMind.h"

class AIHand : public Hand
{
public:
	AIHand(int numPlace) : Hand(numPlace)
	{
	}
	void DrawHand(sf::RenderWindow& window) override;
	void PutOutCards() override;
	void HandleHandWindowEvent(const sf::Event& event);
	void CheckFours() override;
	AIMind& GetMind() { return mind; }
private:
	AIMind mind;
}; 
