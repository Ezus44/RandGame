#include "Menu.h"
#include <assert.h>


	void Menu::Init(const MenuItem& item)
	{
		rootItem = item;

		rootItem.InitMenuItem();
		if (!rootItem.GetChildren().empty())
		{
			SelectMenuItem(rootItem.GetChildren().front());
		}
	}

	void MenuItem::InitMenuItem()
	{
		for (auto& child : children)
		{
			child.parent = this;
			child.InitMenuItem();
		}
	}

	void MenuItem::SetTextParameters(sf::Font& font, const std::string& title, int fontSize, sf::Color color)
	{
		text.setString(title);
		text.setFont(font);
		text.setCharacterSize(fontSize);
		if (color != sf::Color::Transparent)
		{
			text.setFillColor(color);
		}
	}

	void MenuItem::SetHintTextParameters(sf::Font& font, const std::string& title, int fontSize, sf::Color color)
	{
		hintText.setString(title);
		hintText.setFont(font);
		hintText.setCharacterSize(fontSize);
		if (color != sf::Color::Transparent)
		{
			hintText.setFillColor(color);
		}
	}

	void MenuItem::SetChildrenParameters(Orientation orientation, Alignment alignment, float spacing)
	{
		childrenOrientation = orientation;
		childrenAlignment = alignment;
		childrenSpacing = spacing;
	}

	void MenuItem::AddChild(MenuItem& child)
	{
		children.push_back(child);
	}


	void Menu::Draw(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin)
	{
		MenuItem& expandedItem = GetCurrentContext();

		std::vector<sf::Text*> texts;
		texts.reserve(expandedItem.GetChildren().size());
		for (auto& child : expandedItem.GetChildren())
		{
			if (child.IsEnabled())
			{
				sf::Text* text = child.GetText();
				texts.push_back(text);
			}
		}

		DrawTextList(window, texts, expandedItem.GetChildrenSpacing(), expandedItem.GetChildrenOrientation(), expandedItem.GetChildrenAlignment(), position, origin);
	}

	void Menu::PressOnSelectedItem()
	{
		if (!selectedItem)
		{
			return;
		}

		if (selectedItem->onPressCallback)
		{
			selectedItem->onPressCallback(*selectedItem);
			return;
		}

		if (!selectedItem->GetChildren().empty())
		{
			SelectMenuItem(selectedItem->GetChildren().front());
		}
	}

	void Menu::GoBack()
	{
		MenuItem& parent = GetCurrentContext();

		if (&parent != &rootItem)
		{
			SelectMenuItem(parent);
		}
	}

	void Menu::SwitchToPreviousItem()
	{
		if (!selectedItem)
		{
			return;
		}
		MenuItem* parent = selectedItem->GetParent();
		assert(parent); // There always should be parent

		auto it = std::find_if(parent->GetChildren().begin(), parent->GetChildren().end(), [this](const auto& item)
		{
			return selectedItem == &item;
		});
		if (it != parent->GetChildren().begin())
		{
			SelectMenuItem(*std::prev(it));
		}
	}

	void Menu::SwitchToNextItem()
	{
		if (!selectedItem) {
			return;
		}

		MenuItem* parent = selectedItem->GetParent();
		assert(parent); // There always should be parent

		auto it = std::find_if(parent->GetChildren().begin(), parent->GetChildren().end(), [this](const auto& item) {
			return selectedItem == &item;
		});
		it = std::next(it);
		if (it != parent->GetChildren().end()) {
			SelectMenuItem(*it);
		}
	}

	void Menu::SelectMenuItem(const MenuItem& item)
	{
		assert(&item != &rootItem);

		if (selectedItem == &item)
		{
			return;
		}

		if (!item.IsEnabled())
		{
			// Don't allow to select disabled item
			return;
		}

		if (selectedItem)
		{
			sf::Text* text = selectedItem->GetText();
			text->setFillColor(selectedItem->GetDeselectedColor());
		}

		selectedItem = const_cast<MenuItem*>(&item); // Убираем константность временного объекта

		if (selectedItem)
		{
			sf::Text* text = selectedItem->GetText();
			text->setFillColor(selectedItem->GetSelectedColor());
		}
	}

	MenuItem& Menu::GetCurrentContext()
	{
		return selectedItem ? *(selectedItem->GetParent()) : rootItem;
	}
