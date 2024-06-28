#include "ActionCard.h"


void ActionCard::Draw(sf::RenderWindow& window)
{

	rect.setPosition(430.f, 440.f);
	Card::Draw(window);
}