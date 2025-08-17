#pragma once
#include "Card.h"
#include <map>
#include <set>
#include <list>
#include <deque>
class Hand
{
public:
	Hand(int num); 
	std::multimap <Value, Card>& GetCards() { return handCards; }
	int& GetFoursNum() { return foursNum; }
	std::deque<std::string>& GetDialog() { return dialog; }
	void NotChoosedForQuestions() { choosedForQuestions = false; }
	void ChoosedForQuestions() { choosedForQuestions = true; }
	bool isChoosedForQuestions() const { return choosedForQuestions; }
	virtual void CheckFours() = 0;
	bool CheckCards(Value value);
	bool CheckCards(Value value, int numCards);
	bool CheckCards(Value value, int numCards, CardColor color);
	bool CheckCards(Value value, std::list <Suit> suits);
	void AddCard(Card card)
	{
		handCards.insert({ card.GetValue(), card });
	}
	void SetPlaceNum(int num) { placeNum = num; }
	virtual void DrawHand(sf::RenderWindow& window) = 0;
	void DrawDialog();
	void ClearDialog();
	virtual ~Hand() = default;
	void AddReplica(std::string str) { dialog.push_back(str); }
	void TakeCards(Hand& otherHand, Value cardsValue);
protected:
	std::multimap <Value, Card> handCards;
	std::set<Value> foursCards;
	int foursNum;
	std::string replica;
	sf::Font font; 
	sf::Text info;
	sf::Text replicaText;
	sf::RectangleShape table;
	bool choosedForQuestions = false;
	int placeNum;
	std::deque<std::string> dialog;

};