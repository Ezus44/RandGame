#include "PlayerHand.h"


void PlayerHand::CheckFours()
{
	for (auto& card : handCards)
	{
		if (handCards.count(card.first) == 4)
		{
			foursCards.insert(card.first);
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

void PlayerHand::DrawHand(sf::RenderWindow& window)
{
	window.draw(table);
	window.draw(info);
	window.draw(replicaText);
	const float scale = 0.9f;  // Scale factor for reducing size
	const float spacing = 10.0f;  // Spacing between cards

	int cardIndex = 0;
	for (auto& pair : handCards)
	{
		Card& card = pair.second;
		float x, y;  // Position of the card on the table
		// Calculate position based on index
		if (cardIndex < 6)
		{
			x = static_cast<float>(cardIndex * (62.f * scale + spacing) + spacing);
			y = 800.0f;  // Adjust the y-coordinate as needed
		}
		else if (cardIndex >= 6 && cardIndex < 12)
		{
			x = static_cast<float>((cardIndex-6) * (62.f * scale + spacing) + spacing);
			y = 900.0f;  // Adjust the y-coordinate as needed
		}
		else if (cardIndex >= 12 && cardIndex < 18)
		{
			x = static_cast<float>(cardIndex * (62.f * scale + spacing) + spacing);
			y = 800.0f;  // Adjust the y-coordinate as needed
		}
		else if (cardIndex >= 18 && cardIndex < 24)
		{
			x = static_cast<float>((cardIndex - 6) * (62.f * scale + spacing) + spacing);
			y = 900.0f;  // Adjust the y-coordinate as needed
		}

		// Draw the card
		card.Draw(window, scale, x, y);

		// Increment card index
		++cardIndex;
	}
}


void PlayerHand::PutOutCards()
{
}




