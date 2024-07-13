#include "AIMind.h"
#include <cstdlib>   // ��� std::rand() � std::srand()
#include <random>   
#include <set> 
#include <ranges>

bool AIMind::GuessTry(Hand* leftHand, Hand* rightHand, Hand* myHand, Value& valueForSwap)
{
    Hand* chosenHand = ChooseHand(leftHand, rightHand);
    chosenHand->ChoosedForQuestions();
	Hand* notChosenHand = (chosenHand == leftHand) ? rightHand : leftHand;
    
    Value guessingValue = GuessValue(chosenHand);
    myHand->AddReplica(valueToString(guessingValue));
    // ���������, ���� �� �������� � ��������� ��������� � ��������� ����
    if (!(chosenHand->CheckCards(guessingValue)))
    {
		chosenHand->AddReplica("No");
        SetCurrentInfo(notChosenHand, guessingValue); 
        LearnInGameInfo(notChosenHand, guessingValue, myHand);
        return false;
    }
    else
    {
        chosenHand->AddReplica("Yes");
		SetCurrentInfo(chosenHand, guessingValue); 
        int guessingNum = GuessNum();
        myHand->AddReplica(std::to_string(guessingNum));
        // ���������, ���� �� �������� � ��������� ��������� � ������ � ��������� ����
        if (!(chosenHand->CheckCards(guessingValue, guessingNum)))
        {
           chosenHand->AddReplica("No");
           currentInfo->RemovePossibleNum(guessingNum);
           return false;
        }
		else if (guessingNum == 2 || guessingNum == 1)
        {
            chosenHand->AddReplica("Yes");
			currentInfo->ConfirmPossibleNum(guessingNum); 
            // ������������ ���� ��� ����� ��������
            std::string guessingColor = GuessColor(guessingNum);
            myHand->AddReplica(guessingColor);
            if(!(chosenHand->CheckCards(guessingValue, guessingNum, guessingColor)))
            {
                chosenHand->AddReplica("No");
                currentInfo->RemovePossibleColors(guessingColor); 
                return false;
            }
            else
            {
                chosenHand->AddReplica("Yes");
                if (guessingNum == 2 && guessingColor != "Mixed")
                {
                    valueForSwap = guessingValue; 
					return true;
                }
				currentInfo->ConfirmPossibleColors(guessingColor);
                std::list<Suit> guessingSuits = GuessSuits();
                myHand->AddReplica(suitsToString(guessingSuits));
                if (!(chosenHand->CheckCards(guessingValue, guessingSuits)))
                {
                    chosenHand->AddReplica("No");
                    currentInfo->RemovePossibleSuits(guessingSuits);
                    return false;
                }
                else 
				{
                    chosenHand->AddReplica("Yes");
                    valueForSwap = guessingValue; 
					return true;
				}
            }
        }
        else if (guessingNum == 3)
        {
            chosenHand->AddReplica("Yes");
			currentInfo->ConfirmPossibleNum(guessingNum);
            std::list<Suit> guessingSuits = GuessSuits();
            myHand->AddReplica(suitsToString(guessingSuits));
            if (!(chosenHand->CheckCards(guessingValue, guessingSuits)))
            {
                chosenHand->AddReplica("No");
                currentInfo->RemovePossibleSuits(guessingSuits);
                return false;
            }
            else
            {
                chosenHand->AddReplica("Yes");;
                valueForSwap = guessingValue;
                return true;
            }
        }
    }
}

Hand* AIMind::ChooseHand(Hand* leftHand, Hand* rightHand)
{
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    // ���������, ���� �� ���������� � inGameInfo
   
    if (leftHand->GetCards().size() == 0)
    {
		return rightHand;
    }
    else if (rightHand->GetCards().size() == 0)
    {
		return leftHand;
    }
    else if (!inGameInfo.empty()) {
        std::set<Hand*> hands;
        for (const auto& info : inGameInfo) 
        {
           hands.insert(info.first.first);
        }

        // ���������� ��������� ������ �� ��������� ��������
        std::uniform_int_distribution<int> distrib(0, (int)hands.size() - 1);
        int randomIndex = distrib(gen);

        auto it = hands.begin();
        std::advance(it, randomIndex);

        // ���������� ��������� ��������� �� Hand
        return *it;
    }
    else {
        std::set<Hand*> hands;
		hands.insert(leftHand);
		hands.insert(rightHand); 
        std::uniform_int_distribution<int> distrib(0, 1);
        int randomIndex = distrib(gen); 

        auto it = hands.begin(); 
        std::advance(it, randomIndex); 

        // ���������� ��������� ��������� �� Hand
        return *it;
    }
}

Value AIMind::GuessValue(Hand* owner) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<Value> possibleValues; 
    // ���������, ���� �� ���������� � inGameInfo
    if (!inGameInfo.empty()) {
        // �������� ��� ��������� �������� �� inGameInfo
        for (const auto& info : inGameInfo) {
            if (info.first.first == owner)
            {
                possibleValues.push_back(info.first.second);
            }
        }

        // ���������� ��������� ������ �� ��������� ��������
        std::uniform_int_distribution<int> distrib(0, (int)possibleValues.size() - 1); 
        int randomIndex = distrib(gen); 

        // ���������� ��������� �������� �� inGameInfo
        return possibleValues[randomIndex];
    }
    else {
        // �������� ��� ��������� ��������, ����� ���, ��� � outGameInfo
        for (Value val = Value::A; val <= Value::K; val = static_cast<Value>(static_cast<int>(val) + 1)) {
            bool isInOutGameInfo = false;
            for (const auto& outValue : outGameValue) {
                if (outValue == val) {
                    isInOutGameInfo = true;
                    break;
                }
            }
            if (!isInOutGameInfo) {
                possibleValues.push_back(val);
            }
        }

        // ���������� ��������� ������ �� ��������� ��������
        std::uniform_int_distribution<int> distrib(0, (int)possibleValues.size() - 1);
        int randomIndex = distrib(gen);

        // ���������� ��������� �������� �� ���� ���������, ����� outGameInfo
        return possibleValues[randomIndex];
    }
}

void AIMind::SetCurrentInfo(Hand* owner, Value& value)
{
    std::pair<Hand*, Value> key = std::make_pair(owner, value); 
    // ������� ����� ������� � �������� ������
    auto it = inGameInfo.find(key);

    // ���� ������� �� ������, ������� �����
    if (it == inGameInfo.end()) {
        // ������� ����� �������
        inGameInfo[key] = KnownInfo();
    }

    // ������ ����� ��������� �������� ��������
    currentInfo = &inGameInfo.find(key)->second;
}

int AIMind::GuessNum()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    // ���������, ���� �� ����������
    if (currentInfo->GetPossibleNum().size() != 0) {
        // �������� ��� ��������� �������� �� inGameInfo
        std::vector<int> allPossibleNums;
        for (const auto& numsInfo : currentInfo->GetPossibleNum()) {
            allPossibleNums.push_back(numsInfo);
        }

        // ���������� ��������� ������ �� ��������� ��������
        std::uniform_int_distribution<int> distrib(0, (int)allPossibleNums.size() - 1);
        int randomIndex = distrib(gen);

        // ���������� ��������� �������� �� inGameInfo
        return allPossibleNums[randomIndex];
    }
    else {
        // ���������� ��������� ������ �� ��������� ��������
        std::uniform_int_distribution<int> distrib(1, 3);
        int randomIndex = distrib(gen);

        // ���������� ��������� �������� �� ���� ���������, ����� outGameInfo
        return randomIndex;
    }
}

std::string AIMind::GuessColor(int numCards)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::string> allPossibleColors;
    // ���������, ���� �� ����������
    if (currentInfo->GetPossibleColors().size() != 0) {
        // �������� ��� ��������� �������� �� inGameInfo

        for (const auto& colorsInfo : currentInfo->GetPossibleColors()) {
            allPossibleColors.push_back(colorsInfo);
        }

        // ���������� ��������� ������ �� ��������� ��������
        std::uniform_int_distribution<int> distrib(0, (int)allPossibleColors.size() - 1);
        int randomIndex = distrib(gen);

        // ���������� ��������� �������� �� inGameInfo
        return allPossibleColors[randomIndex];
    }
    else {
        // ���������� ��������� ������ �� ��������� ��������
        int variants = 2;
        if (numCards == 2) { variants = 3; }
        std::uniform_int_distribution<int> distrib(1, variants);
        int randomIndex = distrib(gen);

        if (randomIndex == 1)
        {
            return std::string("Red");

        }
        else if (randomIndex == 2)
        {
			return std::string("Black");
        }
        else if (randomIndex == 3)
        {
			return std::string("Mixed");
        }
    }
}

std::list<Suit> AIMind::GuessSuits()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::list <Suit>> allPossibleSuits;
    // ���������, ���� �� ����������
    if (currentInfo->GetPossibleColors().size() != 0) {
        // �������� ��� ��������� �������� �� inGameInfo

        for (const auto& colorsInfo : currentInfo->GetPossibleSuits()) {
            allPossibleSuits.push_back(colorsInfo);
        }

        // ���������� ��������� ������ �� ��������� ��������
        std::uniform_int_distribution<int> distrib(0, (int)allPossibleSuits.size() - 1);
        int randomIndex = distrib(gen);

        // ���������� ��������� �������� �� inGameInfo
        return allPossibleSuits[randomIndex];
    }
}

void AIMind::LearnInGameInfo(Hand* owner, Value value, Hand* myHand)
{
	int valueOnHands = static_cast<int>(myHand->GetCards().count(value));
    int definetlyNum = 4 - valueOnHands;
    std::list <int> possibleNums;
    possibleNums.push_back(definetlyNum);
    KnownInfo knownInfo(possibleNums);
	currentInfo = &knownInfo;
}

std::string AIMind::valueToString(Value val)
{
    switch (val) {
    case Value::A:
        return "A";
    case Value::Six:
        return "6";
    case Value::Seven:
        return "7";
    case Value::Eight:
        return "8";
    case Value::Nine:
        return "9";
    case Value::Ten:
        return "10";
    case Value::J:
        return "J";
    case Value::Q:
        return "Q";
    case Value::K:
        return "K";
    default:
        return "Unknown";
    }       
}

std::string AIMind::suitsToString(const std::list<Suit>& suits) 
{
    std::string result;
    for (auto it = suits.begin(); it != suits.end(); ++it)
    {
        switch (*it) {
        case Suit::Clubs:
            result += "Clubs";
            if (std::next(it) != suits.end())
            {
                result += ", ";
            }
			break;
        case Suit::Diamonds:
            result += "Diamonds";
            if (std::next(it) != suits.end())
            {
                result += ", ";
            }
			break;
        case Suit::Hearts:
            result += "Hearts";
            if (std::next(it) != suits.end())
            {
                result += ", ";
            }
            break;
        case Suit::Spades:
            result += "Spades";
            if (std::next(it) != suits.end())
            {
                result += ", ";
            }
            break;
        default:
            return "Unknown";
			break;
        }
    }
    return result;
}

KnownInfo::KnownInfo(std::list<int> possibleNums) : possibleNum(possibleNums)
{
    if (std::ranges::find(possibleNum, 1)!= possibleNum.end())
    {
		possibleColors.push_back("Black");
		possibleColors.push_back("Red");
        possibleSuits.push_back({ Suit::Spades });
		possibleSuits.push_back({ Suit::Clubs });
		possibleSuits.push_back({ Suit::Diamonds }); 
		possibleSuits.push_back({ Suit::Hearts }); 
    }
    else if (std::ranges::find(possibleNum, 2) != possibleNum.end())
    {
        possibleColors.push_back("Black");
        possibleColors.push_back("Red");
		possibleColors.push_back("Mixed");
        possibleSuits.push_back({ Suit::Spades, Suit::Diamonds });
        possibleSuits.push_back({ Suit::Spades, Suit::Hearts });
		possibleSuits.push_back({ Suit::Clubs, Suit::Diamonds });
		possibleSuits.push_back({ Suit::Clubs, Suit::Hearts });
    }
    else if (std::ranges::find(possibleNum, 3) != possibleNum.end())
    {
        possibleSuits.push_back({ Suit::Spades, Suit::Clubs, Suit::Diamonds });
        possibleSuits.push_back({ Suit::Spades, Suit::Clubs, Suit::Hearts });
		possibleSuits.push_back({ Suit::Spades, Suit::Diamonds, Suit::Hearts });
		possibleSuits.push_back({ Suit::Clubs, Suit::Diamonds, Suit::Hearts });
    }
}

KnownInfo::KnownInfo()
{
	possibleNum.push_back(1);
	possibleNum.push_back(2);
	possibleNum.push_back(3);
    possibleColors.push_back("Black");
    possibleColors.push_back("Red");
    possibleColors.push_back("Mixed");
    possibleSuits.push_back({ Suit::Spades });
    possibleSuits.push_back({ Suit::Clubs });
    possibleSuits.push_back({ Suit::Diamonds });
    possibleSuits.push_back({ Suit::Hearts });
    possibleSuits.push_back({ Suit::Spades, Suit::Diamonds });
    possibleSuits.push_back({ Suit::Spades, Suit::Hearts });
    possibleSuits.push_back({ Suit::Clubs, Suit::Diamonds });
    possibleSuits.push_back({ Suit::Clubs, Suit::Hearts });
    possibleSuits.push_back({ Suit::Spades, Suit::Clubs, Suit::Diamonds });
    possibleSuits.push_back({ Suit::Spades, Suit::Clubs, Suit::Hearts });
    possibleSuits.push_back({ Suit::Spades, Suit::Diamonds, Suit::Hearts });
    possibleSuits.push_back({ Suit::Clubs, Suit::Diamonds, Suit::Hearts });
}

void KnownInfo::ConfirmPossibleNum(int num)
{
    possibleNum.clear();
    possibleNum.push_back(num);
    if (num == 1)
    {
		RemovePossibleColors("Mixed");
		RemovePossibleSuits({ Suit::Spades, Suit::Diamonds });
		RemovePossibleSuits({ Suit::Spades, Suit::Hearts });
        RemovePossibleSuits({ Suit::Clubs, Suit::Diamonds });
		RemovePossibleSuits({ Suit::Clubs, Suit::Hearts });
        RemovePossibleSuits({ Suit::Spades, Suit::Clubs, Suit::Diamonds });
        RemovePossibleSuits({ Suit::Spades, Suit::Clubs, Suit::Hearts });
        RemovePossibleSuits({ Suit::Spades, Suit::Diamonds, Suit::Hearts });
        RemovePossibleSuits({ Suit::Clubs, Suit::Diamonds, Suit::Hearts });
	} 
    if (num == 2)
    {
        RemovePossibleSuits({ Suit::Spades }); 
        RemovePossibleSuits({ Suit::Clubs }); 
        RemovePossibleSuits({ Suit::Diamonds });
        RemovePossibleSuits({ Suit::Hearts }); 
        RemovePossibleSuits({ Suit::Spades, Suit::Clubs, Suit::Diamonds });
        RemovePossibleSuits({ Suit::Spades, Suit::Clubs, Suit::Hearts });
        RemovePossibleSuits({ Suit::Spades, Suit::Diamonds, Suit::Hearts }); 
        RemovePossibleSuits({ Suit::Clubs, Suit::Diamonds, Suit::Hearts });
    }
	else if (num == 3)
	{
        RemovePossibleSuits({ Suit::Spades });
        RemovePossibleSuits({ Suit::Clubs }); 
        RemovePossibleSuits({ Suit::Diamonds }); 
        RemovePossibleSuits({ Suit::Hearts });
        RemovePossibleSuits({ Suit::Spades, Suit::Diamonds }); 
        RemovePossibleSuits({ Suit::Spades, Suit::Hearts }); 
        RemovePossibleSuits({ Suit::Clubs, Suit::Diamonds }); 
        RemovePossibleSuits({ Suit::Clubs, Suit::Hearts });  
    }
	}

void KnownInfo::ConfirmPossibleColors(std::string color)
{
    possibleColors.clear();
    possibleColors.push_back(color);
    if (color == "Black")
    {
        RemovePossibleSuits({ Suit::Diamonds }); 
        RemovePossibleSuits({ Suit::Hearts }); 
    }
    else if (color == "Red")
    {
        RemovePossibleSuits({ Suit::Spades }); 
        RemovePossibleSuits({ Suit::Clubs });  
    }
}
