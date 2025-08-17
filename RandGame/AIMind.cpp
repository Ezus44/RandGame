#include "AIMind.h"
#include <cstdlib>   
#include <random>   
#include <set> 
#include <ranges>

namespace
{
    // Универсальная функция выбора случайного элемента из контейнера
    template <typename Container, typename RNG>
    auto random_choice(const Container& container, RNG& gen)
    {
        std::uniform_int_distribution<size_t> distrib(0, container.size() - 1);
        auto it = container.begin();
        std::advance(it, distrib(gen));
        return *it;
    }
}


std::map <std::pair<Hand*, Value>, KnownInfo> AIMind::inGameInfo;
std::vector <Value> AIMind::outGameValue;

void AIMind::AddYesNoReplica(Hand* hand, bool answer)
{
    hand->AddReplica(answer ? "Yes" : "No");
}

void AIMind::UpdateInfoNum(Hand* hand, const Value& value, int num, bool confirm)
{
    if (confirm)
        inGameInfo[{hand, value}].ConfirmPossibleNum(num);
    else
        inGameInfo[{hand, value}].RemovePossibleNum(num);
}

void AIMind::UpdateInfoColor(Hand* hand, const Value& value, const CardColor& color, bool confirm)
{
    if (confirm)
        inGameInfo[{hand, value}].ConfirmPossibleColors(color);
    else
        inGameInfo[{hand, value}].RemovePossibleColors(color);
}

void AIMind::RemoveOrConfirmSuits(Hand* hand, const Value& value, const std::list<Suit>& suits, bool confirm)
{
    if (confirm)
        inGameInfo[{hand, value}].ConfirmPossibleSuits(suits);
    else
        inGameInfo[{hand, value}].GetPossibleSuits().remove(suits);
}

void AIMind::ProcessNotFourCase(Hand* myHand, Hand* otherHand, const Value& value, 
                                int guessingNum, const CardColor& color, const std::list<Suit>& suits)
{
    inGameInfo[{otherHand, value}].SetPriority();
    int haveNum = static_cast<int>(myHand->GetCards().count(value));
    inGameInfo[{otherHand, value}].ConfirmPossibleNum(4 - (haveNum + guessingNum));

    if (color != CardColor::Mixed) // Mixed можно игнорировать или обрабатывать отдельно
    {
        switch (color)
        {
        case CardColor::Red:
            UpdateInfoColor(myHand, value, CardColor::Red, true);
            inGameInfo[{myHand, value}].GetPossibleSuits().remove({ Suit::Spades, Suit::Clubs, Suit::Diamonds });
            inGameInfo[{myHand, value}].GetPossibleSuits().remove({ Suit::Spades, Suit::Clubs, Suit::Hearts });
            break;

        case CardColor::Black:
            UpdateInfoColor(myHand, value, CardColor::Black, true);
            inGameInfo[{myHand, value}].GetPossibleSuits().remove({ Suit::Spades, Suit::Diamonds, Suit::Hearts });
            inGameInfo[{myHand, value}].GetPossibleSuits().remove({ Suit::Clubs, Suit::Diamonds, Suit::Hearts });
            break;

        default:
            break;
        }
    }

    if (!suits.empty())
    {
        RemoveOrConfirmSuits(myHand, value, suits, true);
    }
}

bool AIMind::GuessTry(Hand* leftHand, Hand* rightHand, Hand* myHand, Value& valueForSwap)
{
    // 1. Устанавливаем приватную информацию о картах всех рук
    SetPrivateInfo(leftHand, rightHand, myHand);
    CheckForPriority(); // Проверяем приоритетные карты для угадывания

    // 2. Выбираем руку для угадывания
    Hand* chosenHand = ChooseHand(leftHand, rightHand);
    chosenHand->ChoosedForQuestions(); // Помечаем выбранную руку
    Hand* notChosenHand = (chosenHand == leftHand) ? rightHand : leftHand;

    // 3. Угадываем значение карты
    Value guessingValue = GuessValue(chosenHand);
    myHand->AddReplica(valueToString(guessingValue)); // Добавляем текстовое представление в диалог

    // 4. Проверка наличия карты с этим значением у выбранной руки
    if (!chosenHand->CheckCards(guessingValue))
    {
        AddYesNoReplica(chosenHand, false); // Добавляем "нет" в диалог
        SetCurrentInfo({ notChosenHand, guessingValue }, leftHand, rightHand, myHand);
        return false; // Угадывание неудачно
    }
    AddYesNoReplica(chosenHand, true); // Добавляем "да" в диалог
    SetCurrentInfo({ chosenHand, guessingValue }, leftHand, rightHand, myHand);

    // 5. Угадываем количество карт с этим значением
    int guessingNum = GuessNum();
    myHand->AddReplica(std::to_string(guessingNum)); // Добавляем количество в диалог

    // 6. Проверка количества
    if (!chosenHand->CheckCards(guessingValue, guessingNum))
    {
        AddYesNoReplica(chosenHand, false);
        UpdateInfoNum(chosenHand, guessingValue, guessingNum, false); // Обновляем внутреннюю информацию
        return false;
    }
    AddYesNoReplica(chosenHand, true);
    UpdateInfoNum(chosenHand, guessingValue, guessingNum, true);
    SetCurrentInfo({ chosenHand, guessingValue }, leftHand, rightHand, myHand);

    // 7. Если количество маленькое (1 или 2), угадываем цвет карты
    if (guessingNum == 1 || guessingNum == 2)
    {
        CardColor guessingColor = GuessColor(guessingNum);
        myHand->AddReplica(cardColorToString(guessingColor));

        // Проверка цвета
        if (!chosenHand->CheckCards(guessingValue, guessingNum, guessingColor))
        {
            AddYesNoReplica(chosenHand, false);
            UpdateInfoColor(chosenHand, guessingValue, guessingColor, false);
            return false;
        }
        AddYesNoReplica(chosenHand, true);

        // Если две карты и цвет не смешанный — можно менять карты
        if (guessingNum == 2 && guessingColor != CardColor::Mixed)
        {
            valueForSwap = guessingValue;
            inGameInfo.erase({ chosenHand, guessingValue });

            // Обработка случая, когда все карты с этим значением ещё не собраны
            if (myHand->GetCards().count(guessingValue) + guessingNum != 4)
            {
                ProcessNotFourCase(myHand, notChosenHand, guessingValue, guessingNum, guessingColor, {});
                UpdateInfoNum(myHand, guessingValue, 1, false);
            }
            return true;
        }

        // Обновляем информацию о цвете
        UpdateInfoColor(chosenHand, guessingValue, guessingColor, true);
        SetCurrentInfo({ chosenHand, guessingValue }, leftHand, rightHand, myHand);

        // 8. Угадываем масти карт
        std::list<Suit> guessingSuits = GuessSuits();
        myHand->AddReplica(suitsToString(guessingSuits));

        if (!chosenHand->CheckCards(guessingValue, guessingSuits))
        {
            AddYesNoReplica(chosenHand, false);
            RemoveOrConfirmSuits(chosenHand, guessingValue, guessingSuits, false);
            return false;
        }

        AddYesNoReplica(chosenHand, true);
        valueForSwap = guessingValue;
        inGameInfo.erase({ chosenHand, guessingValue });

        // Обработка частичного количества карт
        if (myHand->GetCards().count(guessingValue) + guessingNum != 4)
        {
            ProcessNotFourCase(myHand, notChosenHand, guessingValue, guessingNum, guessingColor, guessingSuits);
            if (guessingNum == 2)
                UpdateInfoNum(myHand, guessingValue, 1, false);
        }
        return true;
    }

    // 9. Если количество больше 2 — сразу угадываем масти
    std::list<Suit> guessingSuits = GuessSuits();
    myHand->AddReplica(suitsToString(guessingSuits));

    if (!chosenHand->CheckCards(guessingValue, guessingSuits))
    {
        AddYesNoReplica(chosenHand, false);
        RemoveOrConfirmSuits(chosenHand, guessingValue, guessingSuits, false);
        return false;
    }

    AddYesNoReplica(chosenHand, true);
    valueForSwap = guessingValue;
    inGameInfo.erase({ chosenHand, guessingValue });

    // 10. Обновляем информацию о частичных картах для других рук
    if (myHand->GetCards().count(guessingValue) + guessingNum != 4)
    {
        inGameInfo[{notChosenHand, guessingValue}].SetPriority();
        inGameInfo[{notChosenHand, guessingValue}].ConfirmPossibleNum(1);
        UpdateInfoNum(myHand, guessingValue, 3, true);
        RemoveOrConfirmSuits(myHand, guessingValue, guessingSuits, true);
    }

    return true;
}

Hand* AIMind::ChooseHand(Hand* leftHand, Hand* rightHand)
{
    // Если одна из рук пуста — выбираем другую
    if (leftHand->GetCards().empty()) return rightHand;
    if (rightHand->GetCards().empty()) return leftHand;

    std::set<Hand*> candidates;

    // Добавляем руки с приоритетом
    for (auto& [key, info] : privateInfo)
    {
        if (info.IsPriority())
        {
            if (key.first == leftHand) candidates.insert(leftHand);
            else if (key.first == rightHand) candidates.insert(rightHand);
        }
    }

    // Если нет приоритетных — обе в кандидаты
    if (candidates.empty())
    {
        candidates.insert(leftHand);
        candidates.insert(rightHand);
    }

    // Случайный выбор из кандидатов
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(0, static_cast<int>(candidates.size()) - 1);

    auto it = candidates.begin();
    std::advance(it, distrib(gen));

    return *it;
}

Value AIMind::GuessValue(Hand* owner)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<Value> possibleValues;

    // Сначала приоритетные
    for (const auto& [key, info] : privateInfo)
    {
        if (info.IsPriority() && key.first == owner &&
            std::ranges::find(outGameValue, key.second) == outGameValue.end())
        {
            possibleValues.push_back(key.second);
        }
    }

    // Если приоритетных нет — все доступные
    if (possibleValues.empty())
    {
        for (const auto& [key, info] : privateInfo)
        {
            if (key.first == owner &&
                std::ranges::find(outGameValue, key.second) == outGameValue.end())
            {
                possibleValues.push_back(key.second);
            }
        }
    }

    if (!possibleValues.empty())
    {
        return random_choice(possibleValues, gen);
    }

    // Иначе — любые значения, кроме вышедших из игры
    for (Value val = Value::A; val <= Value::K;
        val = static_cast<Value>(static_cast<int>(val) + 1))
    {
        if (std::ranges::find(outGameValue, val) == outGameValue.end())
        {
            possibleValues.push_back(val);
        }
    }

    return random_choice(possibleValues, gen);
}

void AIMind::SetCurrentInfo(std::pair<Hand*, Value> key, Hand* leftHand, Hand* rightHand, Hand* myHand)
{
    // Если нет записи — создаём
    if (!inGameInfo.contains(key))
    {
        inGameInfo[key] = KnownInfo();
    }

    SetPrivateInfo(leftHand, rightHand, myHand);

    auto it = privateInfo.find(key);
    currentInfo = (it != privateInfo.end()) ? &it->second : nullptr;
}

int AIMind::GuessNum()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    const auto& possibleNums = currentInfo->GetPossibleNum();
    if (!possibleNums.empty())
    {
        return random_choice(possibleNums, gen);
    }

    // Если данных нет — случайно от 1 до 3
    std::uniform_int_distribution<int> distrib(1, 3);
    return distrib(gen);
}

CardColor AIMind::GuessColor(int numCards)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    const auto& possibleColors = currentInfo->GetPossibleColors();
    if (!possibleColors.empty())
    {
        return random_choice(possibleColors, gen);
    }

    // Если нет информации — выбираем из вариантов
    std::vector<CardColor> fallbackColors = { CardColor::Red, CardColor::Black };
    if (numCards == 2) fallbackColors.push_back(CardColor::Mixed);

    return random_choice(fallbackColors, gen);
}

std::list<Suit> AIMind::GuessSuits()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    const auto& possibleSuits = currentInfo->GetPossibleSuits();
    if (possibleSuits.empty())
    {
        return {};
    }

    return random_choice(possibleSuits, gen);
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

std::string AIMind::cardColorToString(CardColor color)
{
    switch (color) {
    case CardColor::Red:   return "Red";
    case CardColor::Black: return "Black";
    case CardColor::Mixed: return "Mixed";
    default:               return "Unknown";
    }
}

void AIMind::CheckForPriority()
{
    for (auto& [key, info] : privateInfo)
    {
        const auto& possibleSuits = info.GetPossibleSuits();
        const auto& possibleNums = info.GetPossibleNum();
        const auto& possibleColors = info.GetPossibleColors();

        bool singleSuit = (possibleSuits.size() == 1);

        bool hasExactlyTwoCards = (possibleNums.size() == 1 && *possibleNums.begin() == 2);

        bool singleColorAndNotMixed = (possibleColors.size() == 1 && *possibleColors.begin() != CardColor::Mixed);

        if (singleSuit || (hasExactlyTwoCards && singleColorAndNotMixed))
        {
            info.SetPriority();
        }
    }
}

void AIMind::UpdatePrivateInfoForHand(Hand* hand, Hand* otherHand, Hand* myHand, Value v, 
                                      int blackNum, int redNum, const std::list<Suit>& mySuits)
{
    std::pair<Hand*, Value> key_hand = { hand, v };
    std::pair<Hand*, Value> key_other = { otherHand, v };

    auto it_hand = privateInfo.find(key_hand);
    auto it_other = privateInfo.find(key_other);

    if (it_hand == privateInfo.end()) return;

    privateInfo[key_hand].RemovePossibleSuits(mySuits);

    if (static_cast<int>(hand->GetCards().size()) < 3)
        privateInfo[key_hand].RemovePossibleNum(3);

    if (static_cast<int>(hand->GetCards().size()) < 2)
        privateInfo[key_hand].RemovePossibleNum(2);


    int myCount = static_cast<int>(myHand->GetCards().count(v));

    if (myCount == 1)
    {
        if (it_other != privateInfo.end())
        {
            privateInfo[key_hand].RemovePossibleNum(3);

            if (privateInfo[key_other].GetPossibleNum().size() == 1 &&
                *privateInfo[key_other].GetPossibleNum().begin() == 2)
            {
                privateInfo[key_hand].RemovePossibleNum(2);
            }

            if (privateInfo[key_other].GetPossibleColors().size() == 1 &&
                *privateInfo[key_other].GetPossibleColors().begin() != CardColor::Mixed)
            {
                privateInfo[key_hand].RemovePossibleColors(*privateInfo[key_other].GetPossibleColors().begin());
            }
        }
    }
    else if (myCount == 2)
    {
        privateInfo[key_hand].RemovePossibleNum(3);
        if (it_other != privateInfo.end())
        {
            privateInfo[key_hand].RemovePossibleNum(2);
        }

        if (blackNum == 0 && redNum == 2)
        {
            privateInfo[key_hand].ConfirmPossibleColors(CardColor::Black);
        }
        else if (blackNum == 2 && redNum == 0)
        {
            privateInfo[key_hand].ConfirmPossibleColors(CardColor::Red);
        }
        else if (blackNum == 1 && redNum == 1)
        {
            if (privateInfo[key_hand].GetPossibleNum().size() == 1 &&
                *privateInfo[key_hand].GetPossibleNum().begin() == 2)
            {
                privateInfo[key_hand].ConfirmPossibleColors(CardColor::Mixed);
            }
            else if (it_other != privateInfo.end() && privateInfo[key_other].GetPossibleColors().size() == 1)
            {
                privateInfo[key_hand].RemovePossibleColors(*privateInfo[key_other].GetPossibleColors().begin());
            }
        }
    }
    else if (myCount == 3)
    {
        privateInfo[key_hand].ConfirmPossibleNum(1);
        if (blackNum == 2)
        {
            privateInfo[key_hand].ConfirmPossibleColors(CardColor::Red);
        }
        else if (redNum == 2)
        {
            privateInfo[key_hand].ConfirmPossibleColors(CardColor::Black);
        }
    }
}

void AIMind::SetPrivateInfo(Hand* leftHand, Hand* rightHand, Hand* myHand)
{
    privateInfo = inGameInfo;
    std::set<Value> myValues;

    for (const auto& card : myHand->GetCards())
    {
        myValues.insert(card.first);
    }

    for (const auto& v : myValues)
    {
        int blackNum = 0;
        int redNum = 0;
        std::list<Suit> mySuits;

        auto range = myHand->GetCards().equal_range(v);
        for (auto it = range.first; it != range.second; ++it)
        {
            mySuits.push_back(it->second.GetSuit());

            if (it->second.GetSuit() == Suit::Clubs || it->second.GetSuit() == Suit::Spades)
                ++blackNum;
            else
                ++redNum;
        }

        UpdatePrivateInfoForHand(leftHand, rightHand, myHand, v, blackNum, redNum, mySuits);
        UpdatePrivateInfoForHand(rightHand, leftHand, myHand, v, blackNum, redNum, mySuits);
    }
}

KnownInfo::KnownInfo(const std::set<int>& possibleNums) : possibleNum(possibleNums), priority(false)
{
    if (possibleNum.count(1))
    {
        possibleColors.insert(CardColor::Black);
        possibleColors.insert(CardColor::Red);
        possibleSuits.push_back({ Suit::Spades });
        possibleSuits.push_back({ Suit::Clubs });
        possibleSuits.push_back({ Suit::Diamonds });
        possibleSuits.push_back({ Suit::Hearts });
    }
    else if (possibleNum.count(2))
    {
        possibleColors.insert(CardColor::Black);
        possibleColors.insert(CardColor::Red);
        possibleColors.insert(CardColor::Mixed);
        possibleSuits.push_back({ Suit::Spades, Suit::Diamonds });
        possibleSuits.push_back({ Suit::Spades, Suit::Hearts });
        possibleSuits.push_back({ Suit::Clubs, Suit::Diamonds });
        possibleSuits.push_back({ Suit::Clubs, Suit::Hearts });
    }
    else if (possibleNum.count(3))
    {
        possibleSuits.push_back({ Suit::Spades, Suit::Clubs, Suit::Diamonds });
        possibleSuits.push_back({ Suit::Spades, Suit::Clubs, Suit::Hearts });
        possibleSuits.push_back({ Suit::Spades, Suit::Diamonds, Suit::Hearts });
        possibleSuits.push_back({ Suit::Clubs, Suit::Diamonds, Suit::Hearts });
    }
}

KnownInfo::KnownInfo()
    : priority(false)
{
    possibleNum = { 1, 2, 3 };
    possibleColors = { CardColor::Black, CardColor::Red, CardColor::Mixed };

    possibleSuits = {
        {Suit::Spades}, {Suit::Clubs}, {Suit::Diamonds}, {Suit::Hearts},
        {Suit::Spades, Suit::Diamonds}, {Suit::Spades, Suit::Hearts},
        {Suit::Clubs, Suit::Diamonds}, {Suit::Clubs, Suit::Hearts},
        {Suit::Spades, Suit::Clubs, Suit::Diamonds}, {Suit::Spades, Suit::Clubs, Suit::Hearts},
        {Suit::Spades, Suit::Diamonds, Suit::Hearts}, {Suit::Clubs, Suit::Diamonds, Suit::Hearts}
    };
}

void KnownInfo::RemovePossibleNum(int num)
{
    possibleNum.erase(num);

    if (num == 1)
    {
        possibleSuits.remove({ Suit::Spades });
        possibleSuits.remove({ Suit::Clubs });
        possibleSuits.remove({ Suit::Diamonds });
        possibleSuits.remove({ Suit::Hearts });
    }
    else if (num == 2)
    {
        RemovePossibleColors(CardColor::Mixed);
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
    possibleNum.insert(num);

    if (num == 1)
    {
        RemovePossibleColors(CardColor::Mixed);
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
        RemovePossibleColors(CardColor::Mixed);
        RemovePossibleColors(CardColor::Red);
        RemovePossibleColors(CardColor::Black);
    }
}

void KnownInfo::RemovePossibleColors(const CardColor& color)
{
    possibleColors.erase(color);

    if (color == CardColor::Mixed)
    {
        possibleSuits.remove({ Suit::Spades, Suit::Diamonds });
        possibleSuits.remove({ Suit::Spades, Suit::Hearts });
        possibleSuits.remove({ Suit::Clubs, Suit::Diamonds });
        possibleSuits.remove({ Suit::Clubs, Suit::Hearts });
    }
    else if (color == CardColor::Black)
    {
        possibleSuits.remove({ Suit::Spades });
        possibleSuits.remove({ Suit::Clubs });
    }
    else if (color == CardColor::Red)
    {
        possibleSuits.remove({ Suit::Diamonds });
        possibleSuits.remove({ Suit::Hearts });
    }
}

void KnownInfo::ConfirmPossibleColors(const CardColor& color)
{
    possibleColors.clear();
    possibleColors.insert(color);

    if (color == CardColor::Black)
    {
        possibleSuits.remove({ Suit::Diamonds });
        possibleSuits.remove({ Suit::Hearts });
    }
    else if (color == CardColor::Red)
    {
        possibleSuits.remove({ Suit::Spades });
        possibleSuits.remove({ Suit::Clubs });
    }
}

void KnownInfo::RemovePossibleSuits(const std::list<Suit>& suit)
{
    std::list<std::list<Suit>> suitsToRemove;

    for (const auto& possibleS : possibleSuits)
    {
        for (const auto& s : suit)
        {
            if (std::find(possibleS.begin(), possibleS.end(), s) != possibleS.end())
            {
                suitsToRemove.push_back(possibleS);
                break;
            }
        }
    }

    for (const auto& rem_s : suitsToRemove)
    {
        possibleSuits.remove(rem_s);
    }
}

void KnownInfo::ConfirmPossibleSuits(const std::list<Suit>& suit)
{
    std::list<std::list<Suit>> suitsToRemove;

    for (const auto& possibleS : possibleSuits)
    {
        for (const auto& s : suit)
        {
            if (std::find(possibleS.begin(), possibleS.end(), s) == possibleS.end())
            {
                suitsToRemove.push_back(possibleS);
                break;
            }
        }
    }

    for (const auto& rem_s : suitsToRemove)
    {
        possibleSuits.remove(rem_s);
    }
}
