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
	bool priority;

public: 
	KnownInfo(std::list <int> possibleNums);
	KnownInfo();
	std::list <int> GetPossibleNum() const{ return possibleNum; }
	void RemovePossibleNum(int num);
	void ConfirmPossibleNum(int num);
	void RemovePossibleColors(std::string color);
	void ConfirmPossibleColors(std::string color);
	void RemovePossibleSuits(std::list <Suit> suit);
	void ConfirmPossibleSuits(std::list<Suit> suit);
	std::list <std::list <Suit>>& GetPossibleSuits() {return possibleSuits; }
	std::list<std::string> GetPossibleColors() const {return possibleColors; } 
	void SetPriority() { priority = true; }
	bool IsPriority() const { return priority; }
};

class AIMind
{
public:
	AIMind() : currentInfo{ nullptr } {}
	bool GuessTry(Hand* leftHand, Hand* rightHand, Hand* myHand, Value& valueForSwap);
	Hand* ChooseHand(Hand* leftHand, Hand* rightHand);
	Value GuessValue(Hand* owner);
	void SetCurrentInfo(std::pair<Hand*, Value> key, Hand* leftHand, Hand* rightHand, Hand* myHand);
	int GuessNum();
	std::string GuessColor(int numCards);
	std::list <Suit> GuessSuits();
	static void LearnOutGameInfo(Value value);
	std::string valueToString(Value val);
	std::string suitsToString(const std::list<Suit>& suits);
	void CheckForPriority();
	void SetPrivateInfo(Hand* leftHand, Hand* rightHand, Hand* myHand);

private:
	static std::map <std::pair<Hand*, Value>, KnownInfo> inGameInfo;
	std::map <std::pair<Hand*, Value>, KnownInfo> privateInfo;
	KnownInfo* currentInfo;
	static std::vector <Value> outGameValue;
};
