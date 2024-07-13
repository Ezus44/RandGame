#include "ChoiceMenu.h"
#include "Application.h"
#include <assert.h>


ChoiceMenu::ChoiceMenu() : firstPage(Orientation::Vertical, Alignment::Middle, 10.f),
secondPage(Orientation::Vertical, Alignment::Middle, 10.f),
thirdPage(Orientation::Vertical, Alignment::Middle, 10.f),
fourthPage(Orientation::Vertical, Alignment::Middle, 10.f),
fifthPage(Orientation::Vertical, Alignment::Middle, 10.f),
menu(firstPage),
isChoosingTwo(false),
checkValue(Value::A)
{
    assert(font.loadFromFile("Resources/Roboto-Black.ttf"));
    assert(menuBuffer.loadFromFile("Resources/Theevilsocks__menu-hover.wav"));
    menuSound.setBuffer(menuBuffer);

    wrongText.setFont(font);
    wrongText.setCharacterSize(24);
    wrongText.setFillColor(sf::Color::Red);
    wrongText.setString("");
    wrongText.setPosition(320.f, 460.f);

    firstPage.SetNextPage(secondPage); 
    secondPage.SetNextPage(thirdPage); 
    thirdPage.SetNextPage(fourthPage); 
	fourthPage.SetNextPage(fifthPage);
    // Добавление элементов на первую страницу
    addValueItems(firstPage);


    // Выбор первого элемента в меню
    menu.SelectItem();
}

void ChoiceMenu::addValueItems(Page& page) {
    std::vector<std::string> values = { "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
    for (auto& value : values) {
        addMenuItem(page, value, [this, value](MenuItem&) {
            checkValue = getValueFromString(value);
            Game& game = Application::Instance().GetGame();
            if (game.WithWhomSwap()->CheckCards(checkValue))
            {
                // Добавление элементов на вторую страницу
                addNumberItems();
                menu.SwitchToNextPage();
            }
            else
            {
				addWrongPage(secondPage);
                menu.SwitchToNextPage();
            }
            });
    }
}



void ChoiceMenu::addNumberItems() {
    // Добавление элемента "One"
    addMenuItem(secondPage, "One", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        if (game.WithWhomSwap()->CheckCards(checkValue, 1))
        {
            addBlackRedItems();
            menu.SwitchToNextPage();
        }
        else
        {
            addWrongPage(thirdPage);
            menu.SwitchToNextPage();
        }
        });

    // Добавление элемента "Two"
    addMenuItem(secondPage, "Two", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        if (game.WithWhomSwap()->CheckCards(checkValue, 2))
        {
            addBlackRedItems(); 
            addBlackAndRedItem(); 
            isChoosingTwo = true; 
            menu.SwitchToNextPage(); 
        }
        else
        {
            addWrongPage(thirdPage);  
            menu.SwitchToNextPage(); 
        }
        });

    // Добавление элемента "Three"
    addMenuItem(secondPage, "Three", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        if (game.WithWhomSwap()->CheckCards(checkValue, 3))
        {
			addThreeSuitItems();
            menu.SwitchToNextPage(); 
            menu.SwitchToNextPage();  
        }
        else
        {
            addWrongPage(thirdPage); 
            menu.SwitchToNextPage();
        }
        });
}

void ChoiceMenu::addBlackRedItems() {
    // Добавление элемента "Black"
    addMenuItem(thirdPage, "Black", [this](MenuItem&) {
        if (isChoosingTwo) {
            Game& game = Application::Instance().GetGame();
            if (game.WithWhomSwap()->CheckCards(checkValue, 2, "Black"))
            {
                game.SwapCards(game.GetPlayerHand(), checkValue); 
                game.SetPlayerTurn();
            }
            else
            {
                addWrongPage(fourthPage);
                menu.SwitchToNextPage();
            }
            
        }
        else {
            Game& game = Application::Instance().GetGame();
            if (game.WithWhomSwap()->CheckCards(checkValue, 1, "Black"))
            {
                addBlackSuitItems();
                menu.SwitchToNextPage(); 
            }
            else
            {
                addWrongPage(fourthPage); 
                menu.SwitchToNextPage(); 
            }
        }
        });

    // Добавление элемента "Red"
    addMenuItem(thirdPage, "Red", [this](MenuItem&) {
        if (isChoosingTwo) {
            Game& game = Application::Instance().GetGame();
            if (game.WithWhomSwap()->CheckCards(checkValue, 2, "Red"))
            {
                game.SwapCards(game.GetPlayerHand(), checkValue);
                game.SetPlayerTurn();
            }
            else
            {
                addWrongPage(fourthPage);
                menu.SwitchToNextPage();
            }
        }
        else {
            Game& game = Application::Instance().GetGame();
            if (game.WithWhomSwap()->CheckCards(checkValue, 1, "Red"))
            {
				addRedSuitItems();
                menu.SwitchToNextPage(); 
            }
            else
            {
                addWrongPage(fourthPage);
                menu.SwitchToNextPage();
            }
        }
        });
}

void ChoiceMenu::addBlackAndRedItem() {
    // Добавление элемента "Black & Red"
    addMenuItem(thirdPage, "Black & Red", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
		if (game.WithWhomSwap()->CheckCards(checkValue, 2, "Mixed"))
        {
			addBlackAndRedSuitItems();
            menu.SwitchToNextPage();
        }
        else
        {
            addWrongPage(fourthPage);
            menu.SwitchToNextPage();
        }
        });
}

void ChoiceMenu::addThreeSuitItems() {
    addMenuItem(fourthPage, "Clubs, Spades & Diamonds", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        if (game.WithWhomSwap()->CheckCards(checkValue, { Suit::Clubs, Suit::Spades, Suit::Diamonds }))
        {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        }
        else
        {
            addWrongPage(fifthPage);
            menu.SwitchToNextPage();
        }
        });

    addMenuItem(fourthPage, "Clubs, Spades & Hearts", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        if (game.WithWhomSwap()->CheckCards(checkValue, { Suit::Clubs, Suit::Spades, Suit::Hearts }))
        {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        }
        else
        {
            addWrongPage(fifthPage);
            menu.SwitchToNextPage();
        }
        });
    
    addMenuItem(fourthPage, "Clubs, Hearts & Diamonds", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        if (game.WithWhomSwap()->CheckCards(checkValue, { Suit::Clubs, Suit::Hearts, Suit::Diamonds }))
        {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        }
        else
        {
            addWrongPage(fifthPage);
            menu.SwitchToNextPage();
        }
        });

    addMenuItem(fourthPage, "Diamonds, Spades & Hearts", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        if (game.WithWhomSwap()->CheckCards(checkValue, { Suit::Diamonds, Suit::Spades, Suit::Hearts }))
        {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        }
        else
        {
            addWrongPage(fifthPage);
            menu.SwitchToNextPage();
        }
        });
}

void ChoiceMenu::addBlackAndRedSuitItems() {
        addMenuItem(fourthPage, "Diamonds & Spades", [this](MenuItem&) {
            Game& game = Application::Instance().GetGame();
            if (game.WithWhomSwap()->CheckCards(checkValue, { Suit::Diamonds, Suit::Spades }))
            {
                game.SwapCards(game.GetPlayerHand(), checkValue);
                game.SetPlayerTurn();
            }
            else
            {
                addWrongPage(fifthPage);
                menu.SwitchToNextPage();
            }
            });

    addMenuItem(fourthPage,  "Hearts & Clubs", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        if (game.WithWhomSwap()->CheckCards(checkValue, { Suit::Hearts, Suit::Clubs }))
        {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        }
        else
        {
            addWrongPage(fifthPage);
            menu.SwitchToNextPage();
        }
        });

    addMenuItem(fourthPage, "Diamonds & Clubs", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        if (game.WithWhomSwap()->CheckCards(checkValue, { Suit::Diamonds, Suit::Clubs }))
        {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        }
        else
        {
            addWrongPage(fifthPage);
            menu.SwitchToNextPage();
        }
        });

    addMenuItem(fourthPage, "Hearts & Spades", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        if (game.WithWhomSwap()->CheckCards(checkValue, { Suit::Hearts, Suit::Spades }))
        {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        }
        else
        {
            addWrongPage(fifthPage);
            menu.SwitchToNextPage();
        }
        });
}


void ChoiceMenu::addBlackSuitItems() {
    addMenuItem(fourthPage, "Clubs", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        if (game.WithWhomSwap()->CheckCards(checkValue, { Suit::Clubs }))
        {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        }
        else
        {
            addWrongPage(fifthPage);
            menu.SwitchToNextPage();
        }
        });

    addMenuItem(fourthPage, "Spades", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        if (game.WithWhomSwap()->CheckCards(checkValue, { Suit::Spades }))
        {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        }
        else
        {
            addWrongPage(fifthPage);
            menu.SwitchToNextPage();
        }
        });

}

void ChoiceMenu::addRedSuitItems() {
    addMenuItem(fourthPage, "Diamonds", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        if (game.WithWhomSwap()->CheckCards(checkValue, { Suit::Diamonds }))
        {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        }
        else
        {
            addWrongPage(fifthPage);
            menu.SwitchToNextPage();
        }
        });

    addMenuItem(fourthPage, "Hearts", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        if (game.WithWhomSwap()->CheckCards(checkValue, { Suit::Hearts }))
        {
            game.SwapCards(game.GetPlayerHand(), checkValue);
			game.SetPlayerTurn();
        }
        else
        {
            addWrongPage(fifthPage);
            menu.SwitchToNextPage();
        }
        });

}

void ChoiceMenu::addMenuItem(Page& page, const std::string& label, std::function<void(MenuItem&)> callback) {
    MenuItem item(font, label, 38, sf::Color::Black);
    item.onPressCallback = callback;
    page.AddMenuItem(item);
}

void ChoiceMenu::addWrongPage(Page& page)
{
    wrongText.setString("You are wrong!");
    addMenuItem(page, "Continue", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        game.WithWhomSwap()->NotChoosedForQuestions(); 
        game.SetKingSlayerTurn();
        });
}

Value ChoiceMenu::getValueFromString(const std::string& valueStr) {
    static const std::unordered_map<std::string, Value> valueMap = {
        {"A", Value::A},
        {"6", Value::Six},
        {"7", Value::Seven},
        {"8", Value::Eight},
        {"9", Value::Nine},
        {"10", Value::Ten},
        {"J", Value::J},
        {"Q", Value::Q},
        {"K", Value::K}
    };

    auto it = valueMap.find(valueStr);
    return it->second;
}


	void  ChoiceMenu::HandleMenuWindowEvent(const sf::Event& event)
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

	void  ChoiceMenu::DrawMenu(sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = (sf::Vector2f)window.getSize();
		window.draw(wrongText);

		menu.Draw(window, viewSize / 2.f, { 0.5f, 0.f });
	}
