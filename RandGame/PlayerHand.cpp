#include "PlayerHand.h"
#include "AIMind.h"


void PlayerHand::CheckFours()
{
	std::set<Value> uniqueValues;
	for (auto& pair : handCards) {
		uniqueValues.insert(pair.first);
	}

	for (auto val : uniqueValues) {
		if (handCards.count(val) == 4) {
			AIMind::LearnOutGameInfo(val);
			foursCards.insert(val);
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
	const float foursScale = 0.6f;
	int cardIndex = 0;
	int foursIndex = 0;
	for (auto& pair : handCards)
	{
		Card& card = pair.second;
		float x, y;
		// Calculate position based on index
		int row = cardIndex / 6;
		int col = cardIndex % 6;
		x = col * (62.f * scale + spacing) + spacing + (row / 2) * 200.f;
		y = 800.f + (row % 2) * 100.f;

		card.Draw(window, scale, x, y);

		++cardIndex;
	}
	for (auto& four : foursCards)
	{
		Card card(four);
		float x, y;
		x = static_cast<float>(foursIndex * (62.f * foursScale + spacing) + spacing);
		y = 700.0f; 
		
		card.Draw(window, foursScale, x, y);
		++foursIndex;
	}
}




