#pragma once
#include <SFML/Graphics.hpp>
#include "Text.h"
#include <list>
#include <functional>



class MenuItem
{
public:
	MenuItem(sf::Font& font, const std::string& title, int fontSize, sf::Color color);

	std::function<void(MenuItem& item)> onPressCallback;
	sf::Text* GetText() { return &text; }
	sf::Color GetSelectedColor() { return selectedColor; }
	sf::Color GetDeselectedColor() { return deselectedColor; }
	bool IsEnabled() const { return enabled; }

private:
	sf::Text text;
	sf::Color selectedColor = sf::Color::Green;
	sf::Color deselectedColor;
	bool enabled = true;

};

class Page {
public:
	Page(Orientation orientation, Alignment alignment, float spacing) :
		pageOrientation(orientation), pageAlignment(alignment), pageSpacing(spacing), nextPage(nullptr) {
	}
	void AddMenuItem(std::unique_ptr<MenuItem> menuItem) {
		menuItems.push_back(std::move(menuItem));
	}
	void SetNextPage(Page* nextPage) { this->nextPage = nextPage; }

	const std::vector<std::unique_ptr<MenuItem>>& GetMenuItems() const { return menuItems; }
	Page* GetNextPage() const { return nextPage; }
	Orientation GetPageOrientation() const { return pageOrientation; }
	Alignment GetPageAlignment() const { return pageAlignment; }
	float GetPageSpacing() const { return pageSpacing; }

private:
	std::vector<std::unique_ptr<MenuItem>> menuItems;
	Page* nextPage{ nullptr };
	Orientation pageOrientation;
	Alignment pageAlignment;
	float pageSpacing;
};

class Menu {
public:
	Menu() : selectedItem(nullptr) {}
	void SelectItem();
	void Draw(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin);
	void PressOnSelectedItem();
	void SwitchToPreviousItem();
	void SwitchToNextItem();
	void SwitchToNextPage();
	void SelectMenuItem(MenuItem* item);
	Page* GetCurrentContext() { return currentPage;	}
	void SetCurrentPage(Page* page) { currentPage = page; }

private:
	Page* currentPage{ nullptr };
	MenuItem* selectedItem{ nullptr };
};



