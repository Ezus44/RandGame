#include "WinningCard.h"


void WinningCard::Draw(sf::RenderWindow& window)
{
	
	rect.setPosition(230.f, 240.f);
	Card::Draw(window);
}