#pragma once
#include <SFML/Graphics.hpp>
#include "Card.h"


class ActionCard : public Card
{
public:

	void Draw(sf::RenderWindow& window) override;

private:
	sf::Sprite sprite;
	sf::Texture texture;

};
