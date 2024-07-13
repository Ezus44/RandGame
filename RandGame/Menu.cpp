#include "Menu.h"
#include <assert.h>



MenuItem::MenuItem(sf::Font& font, const std::string& title, int fontSize, sf::Color color) : deselectedColor(color)
{
    text.setString(title);
    text.setFont(font);
    text.setCharacterSize(fontSize);
  
    text.setFillColor(deselectedColor);
   
}


void Page::AddMenuItem(MenuItem& menuItem)
{
    menuItems.push_back(menuItem);
}

void Page::SetNextPage(Page& nextPage)
{
    this->nextPage = &nextPage;
}


void Menu::SelectItem()
{
    const std::vector<MenuItem>& items = currentPage.GetMenuItems();
    if (!items.empty())
    {
        SelectMenuItem(items.front());
    }
}

void Menu::Draw(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin)
{

    std::vector<sf::Text*> texts;
    texts.reserve(currentPage.GetMenuItems().size());
    for (auto& item : currentPage.GetMenuItems())
    {
        if (item.IsEnabled())
        {
            texts.push_back(item.GetText());
        }
    }

    DrawTextList(window, texts, currentPage.GetPageSpacing(), currentPage.GetPageOrientation(), currentPage.GetPageAlignment(), position, origin);
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
}



void Menu::SwitchToPreviousItem()
{
    if (!selectedItem)
    {
        return;
    }
    std::vector<MenuItem>& items = currentPage.GetMenuItems();
    auto it = std::find_if(items.begin(), items.end(), [this](const MenuItem& item) {
        return selectedItem == &item;
        });

    if (it != items.begin())
    {
        SelectMenuItem(*std::prev(it));
    }
}

void Menu::SwitchToNextItem()
{
    if (!selectedItem)
    {
        return;
    }



    const std::vector<MenuItem>& items = currentPage.GetMenuItems();
    auto it = std::find_if(items.begin(), items.end(), [this](const MenuItem& item) {
        return selectedItem == &item;
        });

    ++it;
    if (it != items.end())
    {
        SelectMenuItem(*it);
    }
}

void Menu::SwitchToNextPage()
{
    if (currentPage.GetNextPage())
    { 
        currentPage = *(currentPage.GetNextPage());
        SelectItem();
                
    }
}

void Menu::SelectMenuItem(const MenuItem& item)
{

   
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

    selectedItem = const_cast<MenuItem*>(&item); 

    if (selectedItem)
    {
        sf::Text* text = selectedItem->GetText();
        text->setFillColor(selectedItem->GetSelectedColor());
    }
}
