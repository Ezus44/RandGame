#include "Hand.h"
#include <set>
#include <assert.h>
void Hand::DrawHand(sf::RenderWindow& window, int placeNum)
{
	table.setSize(sf::Vector2f(200,200));
	table.setFillColor(sf::Color::Red);

	assert(font.loadFromFile("Resources/Roboto-Black.ttf")); 
	info.setFont(font); 
	info.setCharacterSize(24); 
	info.setFillColor(sf::Color::White); 
	info.setString(std::to_string(foursNum)); 

	switch (placeNum)
	{
	case 0:
		table.setPosition(300.f, 600.f);
		info.setPosition(310.f, 610.f);
		break;
	case 1:
		table.setPosition(0.f, 0.f);
		info.setPosition(10.f, 10.f);
		break;
	case 2:
		table.setPosition(600.f, 0.f);
		info.setPosition(610.f, 10.f);
		break;
	default:
		break;
	}
	window.draw(table);
	window.draw(info);
}

void Hand::CheckFours()
{
	std::set<Value> fours;
	for (auto& card : handCards)
	{
		if (handCards.count(card.first) == 4)
		{
			fours.insert(card.first);
		}
	}
	for (auto& four : fours)
	{
		++foursNum;
		auto range = handCards.equal_range(four);

		handCards.erase(range.first, range.second);
	}

	
	
}