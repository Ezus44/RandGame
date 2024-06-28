#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Menu.h"


class ChoiceMenu
	{
	public:
		ChoiceMenu();
		void HandleGameStateMainMenuWindowEvent(const sf::Event& event);
		void UpdateGameStateMainMenu(float timeDelta);
		void DrawGameStateMainMenu(sf::RenderWindow& window);

	private:
		sf::Font font;
		Menu menu;
		sf::SoundBuffer menuBuffer;
		sf::Sound menuSound;
	};

