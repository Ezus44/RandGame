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
	replica = ""; 
	replicaText.setString(replica);
}

void Hand::TakeCards(Hand& otherHand, Value cardsValue)
{
	auto cardsToTake = otherHand.GetCards().equal_range(cardsValue);

	for (auto it = cardsToTake.first; it != cardsToTake.second; ) {
		handCards.insert(*it++);
	}

	otherHand.GetCards().erase(cardsToTake.first, cardsToTake.second);
}

Hand::Hand(int num)
{
	foursNum = 0;
	replica = "";
	SetPlaceNum(num);
	table.setSize(sf::Vector2f(200, 200));
	table.setFillColor(sf::Color::Yellow);

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
	return std::distance(cardsToTake.first, cardsToTake.second) == numCards;
}

bool Hand::CheckCards(Value value, int numCards, CardColor color)
{
	auto cardsToTake = handCards.equal_range(value);

	int redCardCount = static_cast<int>(std::count_if(cardsToTake.first, cardsToTake.second, [](auto& pair) {
		Suit s = pair.second.GetSuit();
		return s == Suit::Diamonds || s == Suit::Hearts;
	}));

	int blackCardCount = static_cast<int>(std::distance(cardsToTake.first, cardsToTake.second) - redCardCount);
		
	if (numCards == 1)
	{
		if (color == CardColor::Red && redCardCount == 1 && blackCardCount == 0)
		{
			return true;
		}
		else if (color == CardColor::Black && redCardCount == 0 && blackCardCount == 1)
		{
			return true;
		}
	}
	else if (numCards == 2)
	{
		if (color == CardColor::Red && redCardCount == 2 && blackCardCount == 0)
		{
			return true;
		}
		else if (color == CardColor::Black && redCardCount == 0 && blackCardCount == 2)
		{
			return true;
		}
		else if (color == CardColor::Mixed && redCardCount == 1 && blackCardCount == 1)
		{
			return true;
		}
	}
	return false;
}

bool Hand::CheckCards(Value value, std::list<Suit> suits)
{
	auto cardsToTake = handCards.equal_range(value);
	auto cardCount = std::distance(cardsToTake.first, cardsToTake.second);

	if (cardCount == suits.size())
	{
		return true;
	}
	return false;
}