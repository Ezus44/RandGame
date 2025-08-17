#include "ChoiceMenu.h"
#include "Application.h"
#include <assert.h>


ChoiceMenu::ChoiceMenu()
    : menu(),
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
    wrongText.setPosition(420.f, 560.f);

    // Создание страниц на куче
    firstPage = std::make_unique<Page>(Orientation::Vertical, Alignment::Middle, 10.f);
    secondPage = std::make_unique<Page>(Orientation::Vertical, Alignment::Middle, 10.f);
    thirdPage = std::make_unique<Page>(Orientation::Vertical, Alignment::Middle, 10.f);
    fourthPage = std::make_unique<Page>(Orientation::Vertical, Alignment::Middle, 10.f);
    fifthPage = std::make_unique<Page>(Orientation::Vertical, Alignment::Middle, 10.f);

    // Связывание страниц
    firstPage->SetNextPage(secondPage.get());
    secondPage->SetNextPage(thirdPage.get());
    thirdPage->SetNextPage(fourthPage.get());
    fourthPage->SetNextPage(fifthPage.get());

    addValueItems(firstPage.get());
    menu.SetCurrentPage(firstPage.get());
    menu.SelectItem();
}

template<typename SuccessCallback>
void ChoiceMenu::handleCheck(bool condition, Page* wrongPage, SuccessCallback onSuccess)
{
    if (condition) {
        onSuccess();
    }
    else {
        addWrongPage(wrongPage);
        menu.SwitchToNextPage();
    }
}

void ChoiceMenu::addValueItems(Page* page) {
    std::vector<std::string> values = { "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
    for (auto& value : values) {
        addMenuItem(page, value, [this, value](MenuItem&) {
            checkValue = getValueFromString(value);
            Game& game = Application::Instance().GetGame();

            handleCheck(game.WithWhomSwap()->CheckCards(checkValue), secondPage.get(), [this]() {
                addNumberItems();
                menu.SwitchToNextPage();
            });
        });
    }
}

void ChoiceMenu::addNumberItems() {
    addMenuItem(secondPage.get(), "One", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        handleCheck(game.WithWhomSwap()->CheckCards(checkValue, 1), thirdPage.get(), [this]() {
            addBlackRedItems();
            menu.SwitchToNextPage();
        });
    });

    addMenuItem(secondPage.get(), "Two", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        handleCheck(game.WithWhomSwap()->CheckCards(checkValue, 2), thirdPage.get(), [this]() {
            addBlackRedItems();
            addBlackAndRedItem();
            isChoosingTwo = true;
            menu.SwitchToNextPage();
        });
    });

    addMenuItem(secondPage.get(), "Three", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        handleCheck(game.WithWhomSwap()->CheckCards(checkValue, 3), thirdPage.get(), [this]() {
            addThreeSuitItems();
            menu.SwitchToNextPage();
            menu.SwitchToNextPage();
        });
    });
}

void ChoiceMenu::addBlackRedItems() {
    addMenuItem(thirdPage.get(), "Black", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        if (isChoosingTwo) {
            handleCheck(game.WithWhomSwap()->CheckCards(checkValue, 2, CardColor::Black), fourthPage.get(), [&]() {
                game.SwapCards(game.GetPlayerHand(), checkValue);
                game.SetPlayerTurn();
            });
        }
        else {
            handleCheck(game.WithWhomSwap()->CheckCards(checkValue, 1, CardColor::Black), fourthPage.get(), [this]() {
                addBlackSuitItems();
                menu.SwitchToNextPage();
            });
        }
    });

    addMenuItem(thirdPage.get(), "Red", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        if (isChoosingTwo) {
            handleCheck(game.WithWhomSwap()->CheckCards(checkValue, 2, CardColor::Red), fourthPage.get(), [&]() {
                game.SwapCards(game.GetPlayerHand(), checkValue);
                game.SetPlayerTurn();
            });
        }
        else {
            handleCheck(game.WithWhomSwap()->CheckCards(checkValue, 1, CardColor::Red), fourthPage.get(), [this]() {
                addRedSuitItems();
                menu.SwitchToNextPage();
            });
        }
    });
}

void ChoiceMenu::addBlackAndRedItem() {
    addMenuItem(thirdPage.get(), "Black & Red", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        handleCheck(game.WithWhomSwap()->CheckCards(checkValue, 2, CardColor::Mixed), fourthPage.get(), [this]() {
            addBlackAndRedSuitItems();
            menu.SwitchToNextPage();
        });
    });
}

void ChoiceMenu::addThreeSuitItems() {
    addMenuItem(fourthPage.get(), "Clubs, Spades & Diamonds", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        handleCheck(game.WithWhomSwap()->CheckCards(checkValue, { Suit::Clubs, Suit::Spades, Suit::Diamonds }), fifthPage.get(), [&]() {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        });
    });

    addMenuItem(fourthPage.get(), "Clubs, Spades & Hearts", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        handleCheck(game.WithWhomSwap()->CheckCards(checkValue, { Suit::Clubs, Suit::Spades, Suit::Hearts }), fifthPage.get(), [&]() {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        });
    });

    addMenuItem(fourthPage.get(), "Clubs, Hearts & Diamonds", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        handleCheck(game.WithWhomSwap()->CheckCards(checkValue, { Suit::Clubs, Suit::Hearts, Suit::Diamonds }), fifthPage.get(), [&]() {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        });
    });

    addMenuItem(fourthPage.get(), "Diamonds, Spades & Hearts", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        handleCheck(game.WithWhomSwap()->CheckCards(checkValue, { Suit::Diamonds, Suit::Spades, Suit::Hearts }), fifthPage.get(), [&]() {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        });
    });
}

void ChoiceMenu::addBlackAndRedSuitItems() {
    addMenuItem(fourthPage.get(), "Diamonds & Spades", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        handleCheck(game.WithWhomSwap()->CheckCards(checkValue, { Suit::Diamonds, Suit::Spades }), fifthPage.get(), [&]() {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        });
    });

    addMenuItem(fourthPage.get(), "Hearts & Clubs", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        handleCheck(game.WithWhomSwap()->CheckCards(checkValue, { Suit::Hearts, Suit::Clubs }), fifthPage.get(), [&]() {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        });
    });

    addMenuItem(fourthPage.get(), "Diamonds & Clubs", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        handleCheck(game.WithWhomSwap()->CheckCards(checkValue, { Suit::Diamonds, Suit::Clubs }), fifthPage.get(), [&]() {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        });
    });

    addMenuItem(fourthPage.get(), "Hearts & Spades", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        handleCheck(game.WithWhomSwap()->CheckCards(checkValue, { Suit::Hearts, Suit::Spades }), fifthPage.get(), [&]() {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        });
    });
}

void ChoiceMenu::addBlackSuitItems() {
    addMenuItem(fourthPage.get(), "Clubs", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        handleCheck(game.WithWhomSwap()->CheckCards(checkValue, { Suit::Clubs }), fifthPage.get(), [&]() {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        });
    });

    addMenuItem(fourthPage.get(), "Spades", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        handleCheck(game.WithWhomSwap()->CheckCards(checkValue, { Suit::Spades }), fifthPage.get(), [&]() {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        });
    });
}

void ChoiceMenu::addRedSuitItems() {
    addMenuItem(fourthPage.get(), "Diamonds", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        handleCheck(game.WithWhomSwap()->CheckCards(checkValue, { Suit::Diamonds }), fifthPage.get(), [&]() {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        });
    });

    addMenuItem(fourthPage.get(), "Hearts", [this](MenuItem&) {
        Game& game = Application::Instance().GetGame();
        handleCheck(game.WithWhomSwap()->CheckCards(checkValue, { Suit::Hearts }), fifthPage.get(), [&]() {
            game.SwapCards(game.GetPlayerHand(), checkValue);
            game.SetPlayerTurn();
        });
    });
}

void ChoiceMenu::addMenuItem(Page* page, const std::string& label, std::function<void(MenuItem&)> callback) {
    auto itemPtr = std::make_unique<MenuItem>(font, label, 38, sf::Color::Black);
    itemPtr->onPressCallback = callback;
    page->AddMenuItem(std::move(itemPtr));
}

void ChoiceMenu::addWrongPage(Page* page)
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
    if (it != valueMap.end()) {
        return it->second;
    }

    throw std::invalid_argument("Unknown value string: " + valueStr);
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

			Orientation orientation = menu.GetCurrentContext()->GetPageOrientation();
            if ((orientation == Orientation::Vertical && event.key.code == sf::Keyboard::W) ||
                (orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::A))
            {
                menu.SwitchToPreviousItem();
            }
            else if ((orientation == Orientation::Vertical && event.key.code == sf::Keyboard::S) ||
                (orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::D))
            {
                menu.SwitchToNextItem();
            }
		}
	}

	void  ChoiceMenu::DrawMenu(sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = (sf::Vector2f)window.getSize();
        if (!wrongText.getString().isEmpty()) {
            window.draw(wrongText);
        }
		menu.Draw(window, viewSize / 2.f, { 0.5f, 0.f });
	}
