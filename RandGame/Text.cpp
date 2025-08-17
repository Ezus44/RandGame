#include "Text.h"

sf::Vector2f GetTextOrigin(const sf::Text& text, const sf::Vector2f& relativePosition)
{
    sf::FloatRect bounds = text.getLocalBounds();
    return {
        bounds.left + bounds.width * relativePosition.x,
        bounds.top + bounds.height * relativePosition.y
    };
}

float AlignmentFactor(Alignment align)
{
    switch (align)
    {
    case Alignment::Min: return 0.f;
    case Alignment::Middle: return 0.5f;
    case Alignment::Max: return 1.f;
    default: return 0.f;
    }
}

void DrawTextList(sf::RenderWindow& window, const std::vector<sf::Text*>& items,
    float spacing, Orientation orientation, Alignment alignment,
    const sf::Vector2f& position, const sf::Vector2f& origin)
{
    if (items.empty()) return;

    // Вычисляем общий размер всех текстов
    sf::Vector2f totalSize(0.f, 0.f);
    for (auto it = items.begin(); it != items.end(); ++it)
    {
        sf::FloatRect bounds = (*it)->getLocalBounds();
        if (orientation == Orientation::Horizontal)
        {
            totalSize.x += bounds.width + (std::next(it) != items.end() ? spacing : 0.f);
            totalSize.y = std::max(totalSize.y, bounds.height);
        }
        else
        {
            totalSize.x = std::max(totalSize.x, bounds.width);
            totalSize.y += bounds.height + (std::next(it) != items.end() ? spacing : 0.f);
        }
    }

    // Начальная позиция с учётом origin
    sf::Vector2f currentPos(position.x - origin.x * totalSize.x,
        position.y - origin.y * totalSize.y);

    for (auto& textPtr : items)
    {
        sf::FloatRect bounds = textPtr->getLocalBounds();
        sf::Vector2f itemOrigin(0.f, 0.f);

        if (orientation == Orientation::Horizontal)
        {
            itemOrigin.y = AlignmentFactor(alignment);
            itemOrigin.x = 0.f;
            textPtr->setOrigin(GetTextOrigin(*textPtr, itemOrigin));
            textPtr->setPosition(currentPos.x, currentPos.y + itemOrigin.y * totalSize.y);
            currentPos.x += bounds.width + spacing;
        }
        else
        {
            itemOrigin.x = AlignmentFactor(alignment);
            itemOrigin.y = 0.f;
            textPtr->setOrigin(GetTextOrigin(*textPtr, itemOrigin));
            textPtr->setPosition(currentPos.x + itemOrigin.x * totalSize.x, currentPos.y);
            currentPos.y += bounds.height + spacing;
        }

        window.draw(*textPtr);
    }
}
