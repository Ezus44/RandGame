#include "ChoiceMenu.h"
#include <assert.h>


	ChoiceMenu::ChoiceMenu()
	{
		assert(font.loadFromFile("Resources/Roboto-Black.ttf"));
		assert(menuBuffer.loadFromFile("Resources/Theevilsocks__menu-hover.wav"));
		menuSound.setBuffer(menuBuffer);

		MenuItem number;

		startGame.SetTextParameters(font, "Start Game", 24);
		startGame.onPressCallback = [](MenuItem&)
		{
			Application::Instance().GetGame().SwitchStateTo(GameStateType::Playing);
		};

		MenuItem recordsTable;
		recordsTable.SetTextParameters(font, "Records Table", 24);
		recordsTable.onPressCallback = [](MenuItem&)
		{
			Application::Instance().GetGame().PushState(GameStateType::RecordsTable, true);
		};

		const bool isSoundOn = Application::Instance().GetGame().IsEnableOptions(GameOptions::Sound);
		MenuItem optionsSound;
		optionsSound.SetTextParameters(font, "Sound: " + std::string(isSoundOn ? "On" : "Off"), 24);
		optionsSound.onPressCallback = [this](MenuItem& item)
		{
			Game& game = Application::Instance().GetGame();
			bool newOptionValue = !game.IsEnableOptions(GameOptions::Sound);
			game.SetOption(GameOptions::Sound, newOptionValue);
			item.SetTextParameters(font, "Sound: " + std::string(newOptionValue ? "On" : "Off"), 24);
		};
		const bool isMusicOn = Application::Instance().GetGame().IsEnableOptions(GameOptions::Music);
		MenuItem optionsMusic;
		optionsMusic.SetTextParameters(font, "Music: " + std::string(isMusicOn ? "On" : "Off"), 24);
		optionsMusic.onPressCallback = [this](MenuItem& item)
		{
			Game& game = Application::Instance().GetGame();
			bool newOptionValue = !game.IsEnableOptions(GameOptions::Music);
			game.SetOption(GameOptions::Music, newOptionValue);
			item.SetTextParameters(font, "Music: " + std::string(newOptionValue ? "On" : "Off"), 24);
		};
		MenuItem options;
		options.SetTextParameters(font, "Options", 24);
		options.SetHintTextParameters(font, "Options", 48, sf::Color::Red);
		options.SetChildrenParameters(Orientation::Vertical, Alignment::Middle, 10.f);
		options.AddChild(optionsSound);
		options.AddChild(optionsMusic);

		MenuItem yes;
		yes.SetTextParameters(font, "Yes", 24);
		yes.onPressCallback = [](MenuItem&) {
			Application::Instance().GetGame().SwitchStateTo(GameStateType::None);
		};
		MenuItem no;
		no.SetTextParameters(font, "No", 24);
		no.onPressCallback = [this](MenuItem&) {
			menu.GoBack();
		};
		MenuItem exitGame;
		exitGame.SetTextParameters(font, "Exit Game", 24);
		exitGame.SetHintTextParameters(font, "Are you sure?", 48, sf::Color::Red);
		exitGame.SetChildrenParameters(Orientation::Horizontal, Alignment::Middle, 10.f);
		exitGame.AddChild(yes);
		exitGame.AddChild(no);

		MenuItem mainMenu;
		mainMenu.SetTextParameters(font, "Snake Game", 48, sf::Color::Red);
		mainMenu.SetChildrenParameters(Orientation::Vertical, Alignment::Middle, 10.f);
		mainMenu.AddChild(startGame);
		mainMenu.AddChild(recordsTable);
		mainMenu.AddChild(options);
		mainMenu.AddChild(exitGame);

		menu.Init(mainMenu);
	}

	void  GameStateMainMenuData::HandleGameStateMainMenuWindowEvent(const sf::Event& event)
	{

		if (event.type == sf::Event::KeyPressed)
		{
			if (Application::Instance().GetGame().IsEnableOptions(GameOptions::Sound))
			{
				menuSound.play();
			}
			if (event.key.code == sf::Keyboard::B)
			{
				menu.GoBack();
			}
			else if (event.key.code == sf::Keyboard::Enter)
			{
				menu.PressOnSelectedItem();
			}

			Orientation orientation = menu.GetCurrentContext().GetChildrenOrientation();
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

	void  GameStateMainMenuData::UpdateGameStateMainMenu(float timeDelta)
	{

	}

	void  GameStateMainMenuData::DrawGameStateMainMenu(sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = (sf::Vector2f)window.getSize();

		sf::Text* hintText = menu.GetCurrentContext().GetHintText();
		if (hintText) {
			hintText->setOrigin(GetTextOrigin(*hintText, { 0.5f, 0.f }));
			hintText->setPosition(viewSize.x / 2.f, 150.f);
			window.draw(*hintText);
		}

		menu.Draw(window, viewSize / 2.f, { 0.5f, 0.f });
	}
