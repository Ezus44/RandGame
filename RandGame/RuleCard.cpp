#include "RuleCard.h"


void RuleCard::Draw(sf::RenderWindow& window)
{
	
	rect.setPosition(330.f, 340.f);
	Card::Draw(window);
}