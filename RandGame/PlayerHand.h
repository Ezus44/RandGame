#pragma once
#include "Hand.h"
class PlayerHand : public Hand
{
public:
	using Hand::Hand;

	void CheckFours() override;

	void DrawHand(sf::RenderWindow& window) override;

};