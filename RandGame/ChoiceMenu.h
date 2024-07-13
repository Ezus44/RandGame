#pragma once
#include "BaseRealisationMenu.h"


class ChoiceMenu : public BaseRealisationMenu
	{
	public:
		ChoiceMenu();
		void addValueItems(Page& page);
		void addNumberItems();
		void addBlackRedItems();
		void addBlackAndRedItem();
		void addThreeSuitItems();
		void addBlackSuitItems();
		void addRedSuitItems();
		void addBlackAndRedSuitItems();

		void addMenuItem(Page& page, const std::string& label, std::function<void(MenuItem&)> callback);
		void addWrongPage(Page& page);
		Value getValueFromString(const std::string& valueStr);
		void HandleMenuWindowEvent(const sf::Event& event) override;
		void DrawMenu(sf::RenderWindow& window) override;

	private:
		sf::Font font;
		sf::Text wrongText;
		Page firstPage;
		Page secondPage;
		Page thirdPage;
		Page fourthPage;
		Page fifthPage;
		Menu menu;
		bool isChoosingTwo;
		Value checkValue; 
		sf::SoundBuffer menuBuffer;
		sf::Sound menuSound;
	};

