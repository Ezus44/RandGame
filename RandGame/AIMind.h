#pragma once
#include "Card.h"
#include <list>
#include "Hand.h"
#include <map>

class KnownInfo
{
	std::set<int> possibleNum;
	std::set<CardColor> possibleColors;
	std::list<std::list<Suit>> possibleSuits;
	bool priority;

public:
	KnownInfo(const std::set<int>& possibleNums);
	KnownInfo();

	const std::set<int>& GetPossibleNum() const { return possibleNum; }
	void RemovePossibleNum(int num);
	void ConfirmPossibleNum(int num);

	const std::set<CardColor>& GetPossibleColors() const { return possibleColors; }
	void RemovePossibleColors(const CardColor& color);
	void ConfirmPossibleColors(const CardColor& color);

	std::list<std::list<Suit>>& GetPossibleSuits() { return possibleSuits; }
	void RemovePossibleSuits(const std::list<Suit>& suit);
	void ConfirmPossibleSuits(const std::list<Suit>& suit);

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
	CardColor GuessColor(int numCards);
	std::list <Suit> GuessSuits();
	static void LearnOutGameInfo(Value value);
	std::string valueToString(Value val);
	std::string suitsToString(const std::list<Suit>& suits);
	std::string cardColorToString(CardColor color);
	void CheckForPriority();
	void SetPrivateInfo(Hand* leftHand, Hand* rightHand, Hand* myHand);

private:
	void ProcessNotFourCase(Hand* myHand, Hand* otherHand, const Value& value, int guessingNum, const CardColor& color,
							const std::list<Suit>& suits);
	void RemoveOrConfirmSuits(Hand* hand, const Value& value, const std::list<Suit>& suits, bool confirm);
	void UpdateInfoColor(Hand* hand, const Value& value, const CardColor& color, bool confirm);
	void UpdateInfoNum(Hand* hand, const Value& value, int num, bool confirm);
	void AddYesNoReplica(Hand* hand, bool answer);
	void UpdatePrivateInfoForHand(Hand* hand, Hand* otherHand, Hand* myHand, Value v, int blackNum, int redNum, 
								 const std::list<Suit>& mySuits);

	static std::map <std::pair<Hand*, Value>, KnownInfo> inGameInfo;
	std::map <std::pair<Hand*, Value>, KnownInfo> privateInfo;
	KnownInfo* currentInfo;
	static std::vector <Value> outGameValue;
};
