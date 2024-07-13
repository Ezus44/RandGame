#include "Card.h"
#include <assert.h>


Card::Card(int s, int v) 
    : suit{}, value{}, x_shift{}, y_shift{}, sprite{}, texture{}
{
    
    switch (s) {
    case 0: 
        suit = Suit::Clubs;
        y_shift = 0;
        break;
    case 1: 
        suit = Suit::Diamonds;
        y_shift = 89;
        break;
    case 2: 
        suit = Suit::Hearts;
        y_shift = 178;
        break;
    case 3: 
        suit = Suit::Spades;
        y_shift = 268;
        break;
    default:
        assert(0);
    }

    switch (v) {
    case 0:
        value = Value::A;
        x_shift = 0;
        break;
    case 1:
        value = Value::Six;
        x_shift = 307;
        break;
    case 2:
        value = Value::Seven;
        x_shift = 369;
        break;
    case 3:
        value = Value::Eight;
        x_shift = 430;
        break;
    case 4:
        value = Value::Nine;
        x_shift = 492;
        break;
    case 5:
        value = Value::Ten;
        x_shift = 553;
        break;
    case 6:
        value = Value::J;
        x_shift = 615;
        break;
    case 7:
        value = Value::Q;
        x_shift = 676;
        break;
    case 8:
        value = Value::K;
        x_shift = 738;
        break;
    default:
        assert(0);
    }
	
}
void Card::Draw(sf::RenderWindow& window, const float scale, float& positionX, float& positionY)
{
    assert(texture.loadFromFile("Resources/Tset.png"));
    sprite.setTextureRect(sf::IntRect(x_shift, y_shift, 62, 90));
    sprite.setTexture(texture);
    sprite.setScale(scale, scale); 
    sprite.setPosition(positionX, positionY); 
	window.draw(sprite);
}