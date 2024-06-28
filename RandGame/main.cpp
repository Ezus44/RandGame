#include <SFML/Graphics.hpp>
#include  "Game.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Randomizing Game");
	sf::Clock game_clock;
	sf::Time lastTime = game_clock.getElapsedTime();
	Game game;
	// Game loop
	while (window.isOpen())
	{
		game.HandleWindowEvents(window);

		if (!window.isOpen())
		{
			break;
		}

		// Calculate time delta
		sf::Time currentTime = game_clock.getElapsedTime();
		float timeDelta = currentTime.asSeconds() - lastTime.asSeconds();
		lastTime = currentTime;
		if (game.Update(timeDelta))
		{
			// Draw everything here
		// Clear the window first
			window.clear();

			game.Draw(window);

			// End the current frame, display window contents on screen
			window.display();
		}
		else
		{
			window.close();
		}
	}

    return 0;
}