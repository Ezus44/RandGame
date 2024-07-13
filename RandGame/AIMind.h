#pragma once
#include "Card.h"
#include <list>
#include "Hand.h"
#include <map>

class KnownInfo 
{
	std::list <int> possibleNum;
	std::list<std::string> possibleColors;
	std::list <std::list <Suit> > possibleSuits;

public: 
	KnownInfo(std::list <int> possibleNums);
	KnownInfo();
	std::list <int> GetPossibleNum() const{ return possibleNum; }
	void RemovePossibleNum(int num) { possibleNum.remove(num); }
	void ConfirmPossibleNum(int num);
	void RemovePossibleColors(std::string color) { possibleColors.remove(color); } 
	void ConfirmPossibleColors(std::string color);
	void RemovePossibleSuits(std::list <Suit> suit) { possibleSuits.remove(suit); } 
	std::list <std::list <Suit> > GetPossibleSuits() const {return possibleSuits; }
	std::list<std::string> GetPossibleColors() const {return possibleColors; } 
};

class AIMind
{
public:
	AIMind() : currentInfo{ nullptr } {}
	bool GuessTry(Hand* leftHand, Hand* rightHand, Hand* myHand, Value& valueForSwap);
	Hand* ChooseHand(Hand* leftHand, Hand* rightHand);
	Value GuessValue(Hand* owner);
	void SetCurrentInfo(Hand* owner, Value& value);
	int GuessNum();
	std::string GuessColor(int numCards);
	std::list <Suit> GuessSuits();
	void LearnInGameInfo(Hand* owner, Value falseValue, Hand* myHand);
	void LearnOutGameInfo(Value value) { outGameValue.push_back(value); }
	std::string valueToString(Value val);
	std::string suitsToString(const std::list<Suit>& suits);

private:
	std::map <std::pair<Hand*, Value>, KnownInfo> inGameInfo;
	KnownInfo* currentInfo;
	std::vector <Value> outGameValue;
};

