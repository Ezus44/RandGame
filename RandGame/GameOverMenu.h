#pragma once
#include "BaseRealisationMenu.h"


class GameOverMenu : public BaseRealisationMenu
{
public:
	GameOverMenu();
	void HandleMenuWindowEvent(const sf::Event& event) override;
	void DrawMenu(sf::RenderWindow& window) override;

private:
	sf::Font font;
	sf::Text finalScore;
	std::unique_ptr<Page>  firstPage;
	Menu menu;
	sf::SoundBuffer menuBuffer;
	sf::Sound menuSound;
};
