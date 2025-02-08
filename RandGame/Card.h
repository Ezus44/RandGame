#pragma once
#include <SFML/Graphics.hpp>

enum class Suit { Clubs, Diamonds, Hearts, Spades };
enum class Value { A, Six, Seven, Eight, Nine, Ten, J, Q, K };

	class Card 
	{
		Suit suit;
		Value value;
		int x_shift;
		int y_shift;
		sf::Sprite sprite;
		sf::Texture texture;
	public:
	
		Card(int s, int v);
		Card(Value v);
		const Suit& GetSuit() const { return suit; }
		const Value& GetValue() const {return value; }
		void Draw(sf::RenderWindow& window, const float scale, float& positionX, float& positionY);
		
	};
