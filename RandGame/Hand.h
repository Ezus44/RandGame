#pragma once
#include "Card.h"
#include <map>
class Hand
{
public:
	virtual void TakeCards(std::multimap <Value, Card>& otherHandCards) = 0 ;
	virtual void PutOutCards() = 0;
	virtual std::multimap <Value, Card> GetCards() = 0;
	virtual void AddCard(Card card) = 0;
	void CheckFours();
	void DrawHand(sf::RenderWindow& window, int placeNum);
	virtual ~Hand() = default;
protected:
	std::multimap <Value, Card> handCards;
	int foursNum;
	sf::Font font; 
	sf::Text info;
	sf::RectangleShape table;

};