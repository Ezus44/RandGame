#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Menu.h"
#include "Card.h"


class BaseRealisationMenu
{
public:
	virtual void HandleMenuWindowEvent(const sf::Event& event) = 0;
	virtual void DrawMenu(sf::RenderWindow& window) = 0;

\
};
