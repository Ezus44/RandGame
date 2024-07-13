#include "GameOverMenu.h"
#include "Application.h"
#include <assert.h>



GameOverMenu::GameOverMenu() : firstPage(Orientation::Horizontal, Alignment::Middle, 10.f), menu(firstPage)

{
	assert(font.loadFromFile("Resources/Roboto-Black.ttf"));
	assert(menuBuffer.loadFromFile("Resources/Theevilsocks__menu-hover.wav"));
	menuSound.setBuffer(menuBuffer);
	

	MenuItem restart(font, "Restart", 18, sf::Color::Black);
	restart.onPressCallback = [this](MenuItem&)
		{

		};
	firstPage.AddMenuItem(restart);

	MenuItem exit(font, "Exit", 18, sf::Color::Black);
	exit.onPressCallback = [this](MenuItem&)
		{
		
		};
	firstPage.AddMenuItem(exit); 

	menu.SelectItem();

	Game& game = Application::Instance().GetGame();
	int plScore = game.GetPlayerHand().GetFoursNum();
	int ksScore = game.GetKingSlayerHand().GetFoursNum();
	int hmScore = game.GetHigherMindHand().GetFoursNum();
	std::map<std::string, int >scores = { {"Player: ", plScore}, {"KingSlayer: ", ksScore}, {"HigherMind: ", hmScore}};
	finalScore.setFont(font);
	finalScore.setCharacterSize(24);
	finalScore.setFillColor(sf::Color::White);
	for (auto it = scores.rbegin(); it != scores.rend(); ++it)
	{
		std::string text = it->first + std::to_string(it->second); 
		finalScore.setString(finalScore.getString() + text + "\n" + "\n"); 
	}

	
	finalScore.setPosition(310.f, 460.f);
}

void  GameOverMenu::HandleMenuWindowEvent(const sf::Event& event)
{

	if (event.type == sf::Event::KeyPressed)
	{

		menuSound.play();

		if (event.key.code == sf::Keyboard::Enter)
		{
			menu.PressOnSelectedItem();
		}

		Orientation orientation = menu.GetCurrentContext().GetPageOrientation();
		if (orientation == Orientation::Vertical && event.key.code == sf::Keyboard::W ||
			orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::A)
		{
			menu.SwitchToPreviousItem();
		}
		else if (orientation == Orientation::Vertical && event.key.code == sf::Keyboard::S ||
			orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::D)
		{
			menu.SwitchToNextItem();
		}
	}
}

void  GameOverMenu::DrawMenu(sf::RenderWindow& window)
{
	sf::Vector2f viewSize = (sf::Vector2f)window.getSize();

	window.draw(finalScore);
	menu.Draw(window, viewSize / 2.f, { 0.5f, 0.f });
}