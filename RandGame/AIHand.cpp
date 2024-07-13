#include "AIHand.h"
#include "Application.h"


void AIHand::DrawHand(sf::RenderWindow& window)
{
	window.draw(table);
	window.draw(info);
	window.draw(replicaText);
}

void AIHand::PutOutCards()
{

}

void AIHand::HandleHandWindowEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		sf::Vector2f mousePosition(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

		if (table.getGlobalBounds().contains(mousePosition))
		{
			choosedForQuestions = true;
			Game& game = Application::Instance().GetGame();
			game.TurnChoiceMenu();
		}
	}
}

void AIHand::CheckFours()
{
	for (auto& card : handCards)
	{
		if (handCards.count(card.first) == 4)
		{
			foursCards.insert(card.first);
			mind.LearnOutGameInfo(card.first);
		}
	}
	for (auto& four : foursCards)
	{
		++foursNum;
		auto range = handCards.equal_range(four);

		handCards.erase(range.first, range.second);
	}
	info.setString(std::to_string(foursNum)); 
}


