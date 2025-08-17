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
    // Подсчёт количества карт каждого типа
    std::unordered_map<Value, int> counts;
    for (const auto& card : handCards)
        ++counts[card.first];

    // Сохранение найденных "четвёрок"
    for (const auto& [value, count] : counts)
    {
        if (count == 4)
        {
            foursCards.insert(value);
            mind.LearnOutGameInfo(value);
        }
    }

    // Удаление всех "четвёрок"
    for (const auto& four : foursCards)
        handCards.erase(four);

    // Обновление числа четвёрок
    foursNum = static_cast<int>(foursCards.size());
    info.setString(std::to_string(foursNum));
}


