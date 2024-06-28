#pragma once
#include <SFML/Graphics.hpp>
#include "Text.h"
#include <list>
#include <functional>



	class MenuItem
	{

	public:
		void InitMenuItem();
		void SetTextParameters(sf::Font& font, const std::string& title, int fontSize, sf::Color color = sf::Color::Transparent);
		void SetHintTextParameters(sf::Font& font, const std::string& title, int fontSize, sf::Color color = sf::Color::Transparent);
		void SetChildrenParameters(Orientation orientation, Alignment alignment, float spacing);
		void AddChild(MenuItem& child);

		std::function<void(MenuItem& item)> onPressCallback;
		sf::Text* GetHintText() { return &hintText; }
		sf::Text* GetText() { return &text; }
		sf::Color GetSelectedColor() { return selectedColor; }
		sf::Color GetDeselectedColor() { return deselectedColor; }
		MenuItem* GetParent() { return parent; }
		bool IsEnabled() const { return enabled; }
		Orientation GetChildrenOrientation() const { return childrenOrientation; }
		Alignment GetChildrenAlignment() const { return childrenAlignment; }
		float GetChildrenSpacing() const { return childrenSpacing; }
		std::vector<MenuItem>& GetChildren() { return children; }

	private:
		sf::Text text;
		sf::Text hintText; // Visible when child item is selected
		Orientation childrenOrientation = Orientation::Vertical;
		Alignment childrenAlignment = Alignment::Min;
		float childrenSpacing = 0.f;

		sf::Color selectedColor = sf::Color::Green;
		sf::Color deselectedColor = sf::Color::White;

		bool enabled = true;
		std::vector<MenuItem> children;


		MenuItem* parent = nullptr;
	};

	class Menu
	{

	public:

		void Init(const MenuItem& item);
		void Draw(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin);
		void PressOnSelectedItem();
		void GoBack();
		void SelectMenuItem(const MenuItem& item);
		void SwitchToPreviousItem();
		void SwitchToNextItem();

		MenuItem& GetCurrentContext();


	private:
		MenuItem rootItem;
		MenuItem* selectedItem = nullptr;

	};



