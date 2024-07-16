#include "AIHand.h"
#include "Application.h"


void AIHand::DrawHand(sf::RenderWindow& window)
{
	window.draw(table);
	window.draw(info);
	window.draw(replicaText);
	const float spacing = 10.0f;  // Spacing between cards
	const float foursScale = 0.6f;
	int foursIndex = 0;
	for (auto& four : foursCards)
	{
		Card card(four);
		float x, y;  // Position of the card on the table
		// Calculate position based on index
		if (placeNum == 1)
		{
			x = static_cast<float>(foursIndex * (62.f * foursScale + spacing) + spacing);
			y = 250.0f;  // Adjust the y-coordinate as needed
		}
		else if (placeNum == 2)
		{
			x = static_cast<float>(foursIndex * (62.f * foursScale + spacing) + 600);
			y = 250.0f;  // Adjust the y-coordinate as needed
		}

		// Draw the card
		card.Draw(window, foursScale, x, y);

		// Increment card index
		++foursIndex;
	}
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
	foursNum = 0;
	for (auto& four : foursCards)
	{
		++foursNum;
		auto range = handCards.equal_range(four);

		handCards.erase(range.first, range.second);
	}
	info.setString(std::to_string(foursNum)); 
}


