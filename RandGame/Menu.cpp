#include "Menu.h"
#include <assert.h>



MenuItem::MenuItem(sf::Font& font, const std::string& title, int fontSize, sf::Color color)
    : deselectedColor(color)
{
    text.setString(title);
    text.setFont(font);
    text.setCharacterSize(fontSize);
    text.setFillColor(deselectedColor);
}


void Menu::SelectItem() {
    if (!currentPage || currentPage->GetMenuItems().empty())
        return;

    SelectMenuItem(currentPage->GetMenuItems().front().get());
}

void Menu::Draw(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin) {
    if (!currentPage) return;

    std::vector<sf::Text*> texts;
    for (const auto& itemPtr : currentPage->GetMenuItems()) {
        MenuItem* item = itemPtr.get();
        if (item->IsEnabled())
            texts.push_back(item->GetText());
    }

    DrawTextList(window, texts, 10.f, Orientation::Vertical, Alignment::Middle, position, origin);
}

void Menu::PressOnSelectedItem() {
    if (selectedItem && selectedItem->onPressCallback)
        selectedItem->onPressCallback(*selectedItem);
}

void Menu::SwitchToPreviousItem() {
    if (!selectedItem || !currentPage) return;

    auto& items = currentPage->GetMenuItems();
    auto it = std::find_if(items.begin(), items.end(), [this](const std::unique_ptr<MenuItem>& ptr) {
        return ptr.get() == selectedItem;
    });

    if (it != items.begin() && it != items.end()) {
        SelectMenuItem((it - 1)->get());
    }
}

void Menu::SwitchToNextItem() {
    if (!selectedItem || !currentPage) return;

    auto& items = currentPage->GetMenuItems();
    auto it = std::find_if(items.begin(), items.end(), [this](const std::unique_ptr<MenuItem>& ptr) {
        return ptr.get() == selectedItem;
    });

    if (it != items.end() && (it + 1) != items.end()) {
        SelectMenuItem((it + 1)->get());
    }
}

void Menu::SwitchToNextPage() {
    if (currentPage && currentPage->GetNextPage()) {
        currentPage = currentPage->GetNextPage();
        SelectItem();
    }
}

void Menu::SelectMenuItem(MenuItem* item) {
    if (!item || !item->IsEnabled()) return;

    if (selectedItem) {
        selectedItem->GetText()->setFillColor(selectedItem->GetDeselectedColor());
    }

    selectedItem = item;
    selectedItem->GetText()->setFillColor(selectedItem->GetSelectedColor());
}
