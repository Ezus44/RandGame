#pragma once
#include "Hand.h"
class AIHand : public Hand
{
public:
	void AddCard(Card card) override {
		handCards.insert({ card.GetValue(), card });
	}
	void TakeCards(std::multimap <Value, Card>& otherHandCards) override;
	void PutOutCards() override;
	std::multimap <Value, Card> GetCards() override {
		return handCards;
	}




}; 
