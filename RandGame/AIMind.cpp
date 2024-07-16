#include "AIMind.h"
#include <cstdlib>   
#include <random>   
#include <set> 
#include <ranges>

std::map <std::pair<Hand*, Value>, KnownInfo> AIMind::inGameInfo;
std::vector <Value> AIMind::outGameValue;
bool AIMind::GuessTry(Hand* leftHand, Hand* rightHand, Hand* myHand, Value& valueForSwap)
{
    SetPrivateInfo(leftHand, rightHand, myHand);
    CheckForPriority();
    Hand* chosenHand = ChooseHand(leftHand, rightHand);
    chosenHand->ChoosedForQuestions();
	Hand* notChosenHand = (chosenHand == leftHand) ? rightHand : leftHand;
    Value guessingValue = GuessValue(chosenHand);
    myHand->AddReplica(valueToString(guessingValue));
 
    if (!(chosenHand->CheckCards(guessingValue)))
    {
		chosenHand->AddReplica("No");
        std::pair<Hand*, Value> key = std::make_pair(notChosenHand, guessingValue);
        SetCurrentInfo(key, leftHand, rightHand, myHand);
        return false;
    }
    else
    {
        chosenHand->AddReplica("Yes");
        std::pair<Hand*, Value> key = std::make_pair(chosenHand, guessingValue);
        SetCurrentInfo(key, leftHand, rightHand, myHand);

        int guessingNum = GuessNum();
        myHand->AddReplica(std::to_string(guessingNum));
    
        if (!(chosenHand->CheckCards(guessingValue, guessingNum)))
        {
           chosenHand->AddReplica("No");
           inGameInfo[key].RemovePossibleNum(guessingNum);
           return false;
        }
		else if (guessingNum == 2 || guessingNum == 1)
        {
            chosenHand->AddReplica("Yes");
            inGameInfo[key].ConfirmPossibleNum(guessingNum);
            SetCurrentInfo(key, leftHand, rightHand, myHand); 
       
            std::string guessingColor = GuessColor(guessingNum);
            myHand->AddReplica(guessingColor);
            if(!(chosenHand->CheckCards(guessingValue, guessingNum, guessingColor)))
            {
                chosenHand->AddReplica("No");
                inGameInfo[key].RemovePossibleColors(guessingColor);
                return false;
            }
            else
            {
                chosenHand->AddReplica("Yes");
                if (guessingNum == 2 && guessingColor != "Mixed")
                {
                    valueForSwap = guessingValue; 
                    inGameInfo.erase(key);
                    if (myHand->GetCards().count(guessingValue) + guessingNum != 4)
                    {
                        inGameInfo[std::make_pair(notChosenHand, guessingValue)].SetPriority();
                        inGameInfo[std::make_pair(notChosenHand, guessingValue)].ConfirmPossibleNum(4 - (myHand->GetCards().count(guessingValue) + guessingNum));
                        inGameInfo[std::make_pair(myHand, guessingValue)].RemovePossibleNum(1);
                        if (guessingColor == "Red") 
                        { 
                            inGameInfo[std::make_pair(myHand, guessingValue)].ConfirmPossibleColors("Red");
                            inGameInfo[std::make_pair(myHand, guessingValue)].GetPossibleSuits().remove({ Suit::Spades, Suit::Clubs, Suit::Diamonds });
                            inGameInfo[std::make_pair(myHand, guessingValue)].GetPossibleSuits().remove({ Suit::Spades, Suit::Clubs, Suit::Hearts });
                        }
                        else if (guessingColor == "Black") 
                        { 
                            inGameInfo[std::make_pair(myHand, guessingValue)].ConfirmPossibleColors("Black");
                            inGameInfo[std::make_pair(myHand, guessingValue)].GetPossibleSuits().remove({ Suit::Spades, Suit::Diamonds, Suit::Hearts });
                            inGameInfo[std::make_pair(myHand, guessingValue)].GetPossibleSuits().remove({ Suit::Clubs, Suit::Diamonds, Suit::Hearts });
                        }
                    }
					return true;
                }
                inGameInfo[key].ConfirmPossibleColors(guessingColor);
                SetCurrentInfo(key, leftHand, rightHand, myHand);
                std::list<Suit> guessingSuits = GuessSuits();
                myHand->AddReplica(suitsToString(guessingSuits));
                if (!(chosenHand->CheckCards(guessingValue, guessingSuits)))
                {
                    chosenHand->AddReplica("No");
                    inGameInfo[key].GetPossibleSuits().remove(guessingSuits);
                    return false;
                }
                else 
				{
                    chosenHand->AddReplica("Yes");
                    valueForSwap = guessingValue; 
                    inGameInfo.erase(key);
                    if (myHand->GetCards().count(guessingValue) + guessingNum != 4)
                    {
                        inGameInfo[std::make_pair(notChosenHand, guessingValue)].SetPriority();
                        inGameInfo[std::make_pair(notChosenHand, guessingValue)].ConfirmPossibleNum(4 - (myHand->GetCards().count(guessingValue) + guessingNum));
                        inGameInfo[std::make_pair(myHand, guessingValue)].GetPossibleSuits().remove({ Suit::Clubs, Suit::Diamonds, Suit::Hearts });
                        if (guessingColor == "Red")
                        {
                            inGameInfo[std::make_pair(myHand, guessingValue)].RemovePossibleColors("Black");
                            inGameInfo[std::make_pair(myHand, guessingValue)].ConfirmPossibleSuits(guessingSuits);
                        }
                        else if (guessingColor == "Black")
                        {
                            inGameInfo[std::make_pair(myHand, guessingValue)].RemovePossibleColors("Red");
                            inGameInfo[std::make_pair(myHand, guessingValue)].ConfirmPossibleSuits(guessingSuits); 
                        }
                        if (guessingNum == 2) { inGameInfo[std::make_pair(myHand, guessingValue)].RemovePossibleNum(1); }
                    }
					return true;
				}
            }
        }
        else
        {
            chosenHand->AddReplica("Yes");
            inGameInfo[key].ConfirmPossibleNum(guessingNum);
            SetCurrentInfo(key, leftHand, rightHand, myHand);
            std::list<Suit> guessingSuits = GuessSuits();
            myHand->AddReplica(suitsToString(guessingSuits));
            if (!(chosenHand->CheckCards(guessingValue, guessingSuits)))
            {
                chosenHand->AddReplica("No");
                inGameInfo[key].GetPossibleSuits().remove(guessingSuits); 
                return false;
            }
            else
            {
                chosenHand->AddReplica("Yes");;
                valueForSwap = guessingValue;
                inGameInfo.erase(key);
                if (myHand->GetCards().count(guessingValue) + guessingNum != 4)
                {
                    inGameInfo[std::make_pair(notChosenHand, guessingValue)].SetPriority();
                    inGameInfo[std::make_pair(notChosenHand, guessingValue)].ConfirmPossibleNum(1);
                    inGameInfo[std::make_pair(myHand, guessingValue)].ConfirmPossibleNum(3); 
                    inGameInfo[std::make_pair(myHand, guessingValue)].ConfirmPossibleSuits(guessingSuits); 
                }
                return true;
            }
        }
    }
}

Hand* AIMind::ChooseHand(Hand* leftHand, Hand* rightHand)
{
    std::random_device rd; 
    std::mt19937 gen(rd()); 

   
    if (leftHand->GetCards().size() == 0)
    {
		return rightHand;
    }
    else if (rightHand->GetCards().size() == 0)
    {
		return leftHand;
    }
    else {
        std::set<Hand*> hands;

        if (!privateInfo.empty()) 
        {
            for (auto& info : privateInfo)
            {
                if (info.second.IsPriority())
                {
                    if (info.first.first == leftHand) { hands.insert(leftHand); }
                    else if (info.first.first == rightHand) { hands.insert(rightHand); }
                }
            }
        }
        if (hands.size() == 0)
        {
            hands.insert(leftHand);
            hands.insert(rightHand);
        }
        std::uniform_int_distribution<int> distrib(0, (int) hands.size() -1);

        int randomIndex = distrib(gen); 

        auto it = hands.begin(); 
        std::advance(it, randomIndex); 


        return *it;
    }
}

Value AIMind::GuessValue(Hand* owner) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<Value> possibleValues; 
   
    if (!privateInfo.empty()) {
        for (auto& info : privateInfo)
        {
            if (info.second.IsPriority())
            {
                if (info.first.first == owner)
                {
                    if (std::ranges::find(outGameValue, info.first.second) == outGameValue.end())
                    {
                        possibleValues.push_back(info.first.second);
                    }
                }
            }
        }
        if (possibleValues.empty())
        {
            for (const auto& info : privateInfo)
            {
                if (info.first.first == owner)
                {
                    if (std::ranges::find(outGameValue, info.first.second) == outGameValue.end()) 
                    {
                        possibleValues.push_back(info.first.second); 
                    }
                }
            }
        }

    }

    if (!possibleValues.empty())
    {
        std::uniform_int_distribution<int> distrib(0, (int)possibleValues.size() - 1);
        int randomIndex = distrib(gen);

        return possibleValues[randomIndex];
    }
    else
    {
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

        std::uniform_int_distribution<int> distrib(0, (int)possibleValues.size() - 1);
        int randomIndex = distrib(gen);

        return possibleValues[randomIndex];
    }
}

void AIMind::SetCurrentInfo(std::pair<Hand*, Value> key, Hand* leftHand, Hand* rightHand, Hand* myHand)
{
    auto it = inGameInfo.find(key);
   
    if (it == inGameInfo.end()) {
        
        inGameInfo[key] = KnownInfo();
    }
    SetPrivateInfo(leftHand, rightHand, myHand);
    
    currentInfo = &privateInfo.find(key)->second;
} 

int AIMind::GuessNum()
{
    std::random_device rd;
    std::mt19937 gen(rd());

   
    if (currentInfo->GetPossibleNum().size() != 0) {
       
        std::vector<int> allPossibleNums;
        for (const auto& numsInfo : currentInfo->GetPossibleNum()) {
            allPossibleNums.push_back(numsInfo);
        }

        
        std::uniform_int_distribution<int> distrib(0, (int)allPossibleNums.size() - 1);
        int randomIndex = distrib(gen);

       
        return allPossibleNums[randomIndex];
    }
    else {
       
        std::uniform_int_distribution<int> distrib(1, 3);
        int randomIndex = distrib(gen);

       
        return randomIndex;
    }
}

std::string AIMind::GuessColor(int numCards)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::string> allPossibleColors;

    if (currentInfo->GetPossibleColors().size() != 0) {
      

        for (const auto& colorsInfo : currentInfo->GetPossibleColors()) {
            allPossibleColors.push_back(colorsInfo);
        }

      
        std::uniform_int_distribution<int> distrib(0, (int)allPossibleColors.size() - 1);
        int randomIndex = distrib(gen);

       
        return allPossibleColors[randomIndex];
    }
    else {
       
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
        else
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
    for (const auto& suitInfo : currentInfo->GetPossibleSuits())
    {
        allPossibleSuits.push_back(suitInfo);
    }

    std::uniform_int_distribution<int> distrib(0, (int)allPossibleSuits.size() - 1);
    int randomIndex = distrib(gen);

    return allPossibleSuits[randomIndex];
}

void AIMind::LearnOutGameInfo(Value value)
{
    outGameValue.push_back(value);
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

void AIMind::CheckForPriority()
{
    for (auto& info : privateInfo)
    {
		if ((info.second.GetPossibleSuits().size() == 1) || 
            (info.second.GetPossibleNum().size() == 1 && *info.second.GetPossibleNum().begin() == 2 
            && info.second.GetPossibleColors().size() == 1 && *info.second.GetPossibleColors().begin() != "Mixed"))
        {
            info.second.SetPriority();
        }
    }
}

void AIMind::SetPrivateInfo(Hand* leftHand, Hand* rightHand, Hand* myHand)
{
    privateInfo = inGameInfo;
    std::set<Value> myValues;
    for (auto& card : myHand->GetCards())
    {
        myValues.insert(card.first);
    }
    for (auto& v : myValues)
    {
        int blackNum = 0;
        int redNum = 0;
        std::list <Suit> mySuits;
        auto range = myHand->GetCards().equal_range(v);
        for (auto& it = range.first; it != range.second; ++it)
        {
            mySuits.push_back(it->second.GetSuit());
            if (it->second.GetSuit() == Suit::Clubs || it->second.GetSuit() == Suit::Spades)
            {
                ++blackNum;
            }
            else
            {
                ++redNum;
            };
        }
        std::pair<Hand*, Value> key_l = std::make_pair(leftHand, v);
        std::pair<Hand*, Value> key_r = std::make_pair(rightHand, v);
        auto it_l = privateInfo.find(key_l);
        auto it_r = privateInfo.find(key_r);
        if (it_l != privateInfo.end()) {
            privateInfo[key_l].RemovePossibleSuits(mySuits); 
            if (myHand->GetCards().count(v) == 1)
            {
                if (it_r != privateInfo.end())
                {
                    privateInfo[key_l].RemovePossibleNum(3);
                    if (privateInfo[key_r].GetPossibleNum().size() == 1 && *privateInfo[key_r].GetPossibleNum().begin() == 2)
                    {
                        privateInfo[key_l].RemovePossibleNum(2);
                    }
                    if (privateInfo[key_r].GetPossibleColors().size() == 1 && *privateInfo[key_r].GetPossibleColors().begin() != "Mixed")
                    {
                        privateInfo[key_l].RemovePossibleColors(*privateInfo[key_r].GetPossibleColors().begin());
                    }
                }
            }
            else if (myHand->GetCards().count(v) == 2)
            {
                privateInfo[key_l].RemovePossibleNum(3);
                if (it_r != privateInfo.end()) { privateInfo[key_l].RemovePossibleNum(2); }
                if (blackNum == 0 && redNum == 2)
                {
                    privateInfo[key_l].ConfirmPossibleColors("Black");
                }
                else if (blackNum == 2 && redNum == 0)
                {
                    privateInfo[key_l].ConfirmPossibleColors("Red");
                }
                else if (blackNum == 1 && redNum == 1)
                {
                    if (privateInfo[key_l].GetPossibleNum().size() == 1 && *privateInfo[key_l].GetPossibleNum().begin() == 2)
                    {
                        privateInfo[key_l].ConfirmPossibleColors("Mixed");
                    }
                    else if (it_r != privateInfo.end() && privateInfo[key_r].GetPossibleColors().size() == 1)
                    {
                        privateInfo[key_l].RemovePossibleColors(*privateInfo[key_r].GetPossibleColors().begin());
                    }
                }

            }
            else if (myHand->GetCards().count(v) == 3)
            {
                privateInfo[key_l].ConfirmPossibleNum(1);
                if (blackNum == 2) { privateInfo[key_l].ConfirmPossibleColors("Red"); }
                else if (redNum == 2) { privateInfo[key_l].ConfirmPossibleColors("Black"); }
            }

            if (it_r != privateInfo.end()) {
                privateInfo[key_r].RemovePossibleSuits(mySuits);
                if (myHand->GetCards().count(v) == 1)
                {
                    if (it_l != privateInfo.end())
                    {
                        privateInfo[key_r].RemovePossibleNum(3);
                        if (privateInfo[key_l].GetPossibleNum().size() == 1 && *privateInfo[key_l].GetPossibleNum().begin() == 2)
                        {
                            privateInfo[key_r].RemovePossibleNum(2);
                        }
                        if (privateInfo[key_l].GetPossibleColors().size() == 1 && *privateInfo[key_l].GetPossibleColors().begin() != "Mixed")
                        {
                            privateInfo[key_r].RemovePossibleColors(*privateInfo[key_l].GetPossibleColors().begin());
                        }
                    }
                }
                else if (myHand->GetCards().count(v) == 2)
                {
                    privateInfo[key_r].RemovePossibleNum(3);
                    if (it_l != privateInfo.end()) { privateInfo[key_r].RemovePossibleNum(2); }
                    if (blackNum == 0 && redNum == 2)
                    {
                        privateInfo[key_r].ConfirmPossibleColors("Black");
                    }
                    else if (blackNum == 2 && redNum == 0)
                    {
                        privateInfo[key_r].ConfirmPossibleColors("Red");
                    }
                    else if (blackNum == 1 && redNum == 1)
                    {
                        if (privateInfo[key_r].GetPossibleNum().size() == 1 && *privateInfo[key_r].GetPossibleNum().begin() == 2)
                        {
                            privateInfo[key_r].ConfirmPossibleColors("Mixed");
                        }
                        else if (it_l != privateInfo.end() && privateInfo[key_l].GetPossibleColors().size() == 1)
                        {
                            privateInfo[key_r].RemovePossibleColors(*privateInfo[key_l].GetPossibleColors().begin());
                        }
                    }

                }
                else if (myHand->GetCards().count(v) == 3)
                {
                    privateInfo[key_r].ConfirmPossibleNum(1);
                    if (blackNum == 2) { privateInfo[key_r].ConfirmPossibleColors("Red"); }
                    else if (redNum == 2) { privateInfo[key_r].ConfirmPossibleColors("Black"); }
                }

            }

        }
    }
}

KnownInfo::KnownInfo(std::list<int> possibleNums) : possibleNum(possibleNums)
{
    priority = false;
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
    priority = false;
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

void KnownInfo::RemovePossibleNum(int num)
{
    possibleNum.remove(num);
    if (num == 1)
    {
        possibleSuits.remove({ Suit::Spades });
        possibleSuits.remove({ Suit::Clubs });
        possibleSuits.remove({ Suit::Diamonds });
        possibleSuits.remove({ Suit::Hearts });
    }
    else  if (num == 2)
    {
        RemovePossibleColors("Mixed");
    }
    else if (num == 3)
    {
        possibleSuits.remove({ Suit::Spades, Suit::Clubs, Suit::Diamonds });
        possibleSuits.remove({ Suit::Spades, Suit::Clubs, Suit::Hearts });
        possibleSuits.remove({ Suit::Spades, Suit::Diamonds, Suit::Hearts });
        possibleSuits.remove({ Suit::Clubs, Suit::Diamonds, Suit::Hearts });
    }

}

void KnownInfo::ConfirmPossibleNum(int num)
{
    possibleNum.clear();
    possibleNum.push_back(num);
    if (num == 1)
    {
		RemovePossibleColors("Mixed");
        possibleSuits.remove({ Suit::Spades, Suit::Clubs, Suit::Diamonds });
        possibleSuits.remove({ Suit::Spades, Suit::Clubs, Suit::Hearts });
        possibleSuits.remove({ Suit::Spades, Suit::Diamonds, Suit::Hearts });
        possibleSuits.remove({ Suit::Clubs, Suit::Diamonds, Suit::Hearts });
	} 
    else if (num == 2)
    {
        possibleSuits.remove({ Suit::Spades });
        possibleSuits.remove({ Suit::Clubs });
        possibleSuits.remove({ Suit::Diamonds });
        possibleSuits.remove({ Suit::Hearts });
        possibleSuits.remove({ Suit::Spades, Suit::Clubs, Suit::Diamonds });
        possibleSuits.remove({ Suit::Spades, Suit::Clubs, Suit::Hearts });
        possibleSuits.remove({ Suit::Spades, Suit::Diamonds, Suit::Hearts });
        possibleSuits.remove({ Suit::Clubs, Suit::Diamonds, Suit::Hearts });
       
    }
    else if (num == 3)
    {
        RemovePossibleColors("Mixed");
        RemovePossibleColors("Red");
        RemovePossibleColors("Black");
    }
}

void KnownInfo::RemovePossibleColors(std::string color)
{
    possibleColors.remove(color);
    if (color == "Mixed")
    {
        possibleSuits.remove({ Suit::Spades, Suit::Diamonds });
        possibleSuits.remove({ Suit::Spades, Suit::Hearts });
        possibleSuits.remove({ Suit::Clubs, Suit::Diamonds });
        possibleSuits.remove({ Suit::Clubs, Suit::Hearts });
    }
    else if (color == "Black")
    {
        possibleSuits.remove({ Suit::Spades });
        possibleSuits.remove({ Suit::Clubs });
    }
    else if (color == "Red")
    {
        possibleSuits.remove({ Suit::Diamonds });
        possibleSuits.remove({ Suit::Hearts });
    }
}

void KnownInfo::ConfirmPossibleColors(std::string color)
{
    possibleColors.clear();
    possibleColors.push_back(color);
    if (color == "Black")
    {
        possibleSuits.remove({ Suit::Diamonds });
        possibleSuits.remove({ Suit::Hearts });
    }
    else if (color == "Red")
    {
        possibleSuits.remove({ Suit::Spades });
        possibleSuits.remove({ Suit::Clubs }); 
    }
}

void KnownInfo::RemovePossibleSuits(std::list<Suit> suit)
{
    std::list<std::list<Suit>> suitsToRemove;
    for (auto& s : suit)
    {
        for (std::list<Suit> possibleS : possibleSuits)
        {
            if (std::ranges::find(possibleS, s) != possibleS.end())
            {
                suitsToRemove.push_back(possibleS);
            }
        }
    }       
  
    for (auto& rem_s : suitsToRemove)
    {
        possibleSuits.remove(rem_s);
    }
}

void KnownInfo::ConfirmPossibleSuits(std::list<Suit> suit)
{
    std::list<std::list<Suit>> suitsToRemove; 
    for (auto& s : suit) 
    {
        for (std::list<Suit> possibleS : possibleSuits) 
        {
            if (std::ranges::find(possibleS, s) == possibleS.end())
            {
                suitsToRemove.push_back(possibleS); 
            }
        }
    }

    for (auto& rem_s : suitsToRemove)
    {
        possibleSuits.remove(rem_s); 
    }
}
