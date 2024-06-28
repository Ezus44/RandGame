#include "ValueCard.h"


void ValueCard::Draw(sf::RenderWindow& window)
{

	rect.setPosition(630.f, 640.f);
	Card::Draw(window);
}