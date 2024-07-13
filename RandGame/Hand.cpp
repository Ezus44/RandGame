#include "Hand.h"
#include <set>
#include <assert.h>
#include <ranges>

void Hand::DrawDialog()
{
	replica = dialog.front();
	replicaText.setString(replica);
	dialog.pop_front();
}

void Hand::ClearDialog()
{
	dialog.clear();
	replica = "Cleared"; 
	replicaText.setString(replica);
}

void Hand::TakeCards(Hand& otherHand, Value cardsValue)
{
	auto cardsToTake = otherHand.GetCards().equal_range(cardsValue);

	for (auto it = cardsToTake.first; it != cardsToTake.second; ++it) {
		handCards.insert(*it);
	}

	otherHand.GetCards().erase(cardsToTake.first, cardsToTake.second);
}

Hand::Hand(int num)
{
	foursNum = 0;
	replica = "Chat";
	SetPlaceNum(num);
	table.setSize(sf::Vector2f(200, 200));
	table.setFillColor(sf::Color::Red);

	assert(font.loadFromFile("Resources/Roboto-Black.ttf"));
	info.setFont(font);
	info.setCharacterSize(24);
	info.setFillColor(sf::Color::White);
	info.setString(std::to_string(foursNum));

	replicaText.setFont(font);
	replicaText.setCharacterSize(24);
	replicaText.setFillColor(sf::Color::White);
	replicaText.setString(replica);

	switch (placeNum)
	{
	case 0:
		table.setPosition(400.f, 800.f);
		info.setPosition(410.f, 810.f);
		replicaText.setPosition(440.f, 840.f);
		break;
	case 1:
		table.setPosition(0.f, 0.f);
		info.setPosition(10.f, 10.f);
		replicaText.setPosition(40.f, 40.f);
		break;
	case 2:
		table.setPosition(800.f, 0.f);
		info.setPosition(810.f, 10.f);
		replicaText.setPosition(840.f, 40.f);
		break;
	default:
		break;
	}


}

bool Hand::CheckCards(Value value)
{
	auto cardsToTake = handCards.equal_range(value);
	if (cardsToTake.first != cardsToTake.second)
	{
		return true;
	}
	return false;
}

bool Hand::CheckCards(Value value, int numCards)
{
	auto cardsToTake = handCards.equal_range(value);
	auto numTakenCards = std::distance(cardsToTake.first, cardsToTake.second);
	if (numTakenCards == numCards)
	{
		return true;
	}
	return false;
}

bool Hand::CheckCards(Value value, int numCards, std::string color)
{
	auto cardsToTake = handCards.equal_range(value); 
	std::multimap<Value, Card> checkTheseCards; 
	for (auto it = cardsToTake.first; it != cardsToTake.second; ++it) {
		checkTheseCards.insert(*it);
	}
	int redCardCount = 0;
	int blackCardCount = 0;
	for (auto& pair : checkTheseCards) {
		Suit cardSuit = pair.second.GetSuit();
		if (cardSuit == Suit::Diamonds || cardSuit == Suit::Hearts) {
			++redCardCount;
		}
		else if (cardSuit == Suit::Clubs || cardSuit == Suit::Spades) {
			++blackCardCount;
		}
	}
		
	if (numCards == 1)
	{
		if (color == "Red" && redCardCount == 1 && blackCardCount == 0)
		{
			return true;
		}
		else if (color == "Black" && redCardCount == 0 && blackCardCount == 1)
		{
			return true;
		}
	}
	else if (numCards == 2)
	{
		if (color == "Red" && redCardCount == 2 && blackCardCount == 0)
		{
			return true;
		}
		else if (color == "Black" && redCardCount == 0 && blackCardCount == 2)
		{
			return true;
		}
		else if (color == "Mixed" && redCardCount == 1 && blackCardCount == 1)
		{
			return true;
		}
	}
	return false;
}

bool Hand::CheckCards(Value value, std::list<Suit> suits)
{
	auto cardsToTake = handCards.equal_range(value);
	std::multimap<Value, Card> checkTheseCards;
	for (auto it = cardsToTake.first; it != cardsToTake.second; ++it) {
		checkTheseCards.insert(*it);
	}
	int cardCount = 0;
	for (auto& checkCard : checkTheseCards)
	{
		if (std::ranges::find(suits, checkCard.second.GetSuit()) != suits.end())
		{
			++cardCount;
		}
	}
	if (cardCount == suits.size())
	{
		return true;
	}
	return false;
}