#pragma once
#include "BaseRealisationMenu.h"


class ChoiceMenu : public BaseRealisationMenu
	{
	public:
		ChoiceMenu();
		void addValueItems(Page* page);
		void addNumberItems();
		void addBlackRedItems();
		void addBlackAndRedItem();
		void addThreeSuitItems();
		void addBlackSuitItems();
		void addRedSuitItems();
		void addBlackAndRedSuitItems();

		void addMenuItem(Page* page, const std::string& label, std::function<void(MenuItem&)> callback);
		void addWrongPage(Page* page);
		Value getValueFromString(const std::string& valueStr);
		void HandleMenuWindowEvent(const sf::Event& event) override;
		void DrawMenu(sf::RenderWindow& window) override;

	private:
		sf::Font font;
		sf::Text wrongText;

		// Страницы на куче, управляемые unique_ptr
		std::unique_ptr<Page> firstPage;
		std::unique_ptr<Page> secondPage;
		std::unique_ptr<Page> thirdPage;
		std::unique_ptr<Page> fourthPage;
		std::unique_ptr<Page> fifthPage;

		Menu menu;
		bool isChoosingTwo;
		Value checkValue; 

		sf::SoundBuffer menuBuffer;
		sf::Sound menuSound;

		template<typename SuccessCallback>
		void handleCheck(bool condition, Page* wrongPage, SuccessCallback onSuccess);
};

