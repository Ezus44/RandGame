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

class Page
{
public:
	Page(Orientation orientation, Alignment alignment, float spacing) : 
		pageOrientation( orientation ), pageAlignment( alignment ), pageSpacing( spacing ), nextPage( nullptr ) {}
	void AddMenuItem(MenuItem& menuItem);
	void SetNextPage(Page& nextPage);
	Orientation GetPageOrientation() const { return pageOrientation; }
	Alignment GetPageAlignment() const { return pageAlignment; }
	float GetPageSpacing() const { return pageSpacing; }
	std::vector<MenuItem>& GetMenuItems() { return menuItems; }
	Page* GetNextPage() { return nextPage; }

private:
	Orientation pageOrientation;
	Alignment pageAlignment;
	float pageSpacing;
	std::vector<MenuItem> menuItems;
	Page* nextPage;

};

class Menu
{
public:
	Menu(Page& page): currentPage(page), selectedItem(nullptr){}
	void Draw(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin);
	void PressOnSelectedItem();
	void SelectMenuItem(const MenuItem& item);
	void SwitchToPreviousItem();
	void SwitchToNextItem();
	Page& GetCurrentContext() { return currentPage; }
	void SwitchToNextPage();
	void SelectItem();

private:
	MenuItem* selectedItem = nullptr;
	Page& currentPage;
};



