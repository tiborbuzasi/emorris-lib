/**
 * Learning AI Class
 * libMorris
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 *
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "LearningAI.hpp"

#include <cstdlib>
#include <ctime>
#include <fstream>

// TODO: REWORK LOGGING
// #include "../eMorrisGUI/_Source/engine/UtilityFunctions.hpp"
// #include <bitset>
// #include <fstream>
// #include <sstream>

// template<typename T>
// std::string binaryToString(const T& x)
// {
//     std::stringstream ss;
//     ss << std::bitset<sizeof(T) * 8>(x);
//     return ss.str();
// }

bool LearningAI::Initialize(Game* game, bool spectator)
{
    this->game = game;
    if (game == nullptr)
    {
//         Log("AI", "Game not found.", true, true);
        return false;
    }
    // Set spectating mode
    this->spectator = spectator;

    // Initialize random generator
    srand(time(nullptr));

    history = new std::vector<GameStepElement>();
    storage = new std::vector<GameStepElement>();

    return true;
}

bool LearningAI::IsSpectator()
{
    return spectator;
}

bool LearningAI::Load(std::string fileName)
{
    std::ifstream file;
    file.open(fileName, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        file.close();
//         Log("AI", "Opening storage file \"" + fileName + "\" for loading failed.", true, true);
        return false;
    }

    file.seekg (0, std::ios::beg);
    while (file.good())
    {
        GameStepElement step;
        file.read(reinterpret_cast<char*>(&step.state0), sizeof(step.state0));
        file.read(reinterpret_cast<char*>(&step.state1), sizeof(step.state1));
        file.read(reinterpret_cast<char*>(&step.state2), sizeof(step.state2));
        file.read(reinterpret_cast<char*>(&step.changes0), sizeof(step.changes0));
        file.read(reinterpret_cast<char*>(&step.changes1), sizeof(step.changes1));
        file.read(reinterpret_cast<char*>(&step.wins), sizeof(step.wins));
        file.read(reinterpret_cast<char*>(&step.losses), sizeof(step.losses));
        storage->push_back(step);
    }

    // Calculate balance
    for (std::vector<GameStepElement>::iterator i = storage->begin(); i != storage->end(); ++i)
    {
        (*i).balance = (*i).wins - (*i).losses;
    }

    // TODO: REMOVE LOGGING
//     for (std::vector<GameStepElement>::iterator i = storage->begin(); i != storage->end(); ++i)
//     {
//         Log("L0", binaryToString((*i).state0));
//         Log("L1", binaryToString((*i).state1));
//         Log("L2", binaryToString((*i).state2));
//     }

    if (file.fail())
    {
//         Log("AI", "Loading storage file \"" + fileName + "\" failed.", true, true);
        return false;
    }

    file.close();
    return true;
}

bool LearningAI::Save(std::string fileName)
{
    std::ofstream file;
    file.open(fileName, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!file.is_open())
    {
        file.close();
//         Log("AI", "Opening storage file \"" + fileName + "\" for saving failed.", true, true);
        return false;
    }

    std::vector<GameStepElement>::iterator si = storage->begin();
    while (si != storage->end() && file.good())
    {
        GameStepElement step = (*si);
        file.write(reinterpret_cast<char*>(&step.state0), sizeof(step.state0));
        file.write(reinterpret_cast<char*>(&step.state1), sizeof(step.state1));
        file.write(reinterpret_cast<char*>(&step.state2), sizeof(step.state2));
        file.write(reinterpret_cast<char*>(&step.changes0), sizeof(step.changes0));
        file.write(reinterpret_cast<char*>(&step.changes1), sizeof(step.changes1));
        file.write(reinterpret_cast<char*>(&step.wins), sizeof(step.wins));
        file.write(reinterpret_cast<char*>(&step.losses), sizeof(step.losses));

        ++si;
    }

    // TODO: REMOVE LOGGING
//     for (std::vector<GameStepElement>::iterator i = storage->begin(); i != storage->end(); ++i)
//     {
//         Log("S0", binaryToString((*i).state0));
//         Log("S1", binaryToString((*i).state1));
//         Log("S2", binaryToString((*i).state2));
//     }

    if (file.fail())
    {
//         Log("AI", "Saving storage file \"" + fileName + "\" failed.", true, true);
        return false;
    }

    file.close();
    return true;
}

std::array<unsigned char, 2> LearningAI::GetNextStep(bool retry)
{
    if (!retry)
    {
        Convert(game->GetField());

        if (spectator)
        {
            return { 255, 255 };
        }
        GameStepElement* nextStepElement = nullptr;
        std::vector<GameStepElement>::iterator si;
        for (si = storage->begin(); si != storage->end(); ++si)
        {
            if ((*si).state0 == currentStep.state0 && (*si).state1 == currentStep.state1 && (*si).state2 == currentStep.state2
                    && (nextStepElement == nullptr || nextStepElement->balance > (*si).balance))
            {
                nextStepElement = &(*si);
                // TODO: REMOVE LOGGING
//                 Log("AI", "Using stored step!");
            }
        }

        if (si == storage->end())
        {
            RandomGenerate();
        }
        else
        {
            currentStep.changes0 = nextStepElement->changes0;
            currentStep.changes1 = nextStepElement->changes1;
        }
    }
    else
    {
        RandomGenerate();
    }

    return { currentStep.changes0, currentStep.changes1 };
}

void LearningAI::Register(std::array<unsigned char, 2> changes)
{
    history->push_back({ currentStep.state0, currentStep.state1, currentStep.state2, changes[0], changes[1] });

    // TODO: REMOVE LOGGING
//     Log("HIST", std::to_string(history->size()));
//     for (std::vector<GameStepElement>::iterator i = history->begin(); i != history->end(); ++i)
//     {
//         Log("H0", binaryToString((*i).state0));
//         Log("H1", binaryToString((*i).state1));
//         Log("H2", binaryToString((*i).state2));
//         Log("H10", std::to_string((*i).changes0));
//         Log("H11", std::to_string((*i).changes1));
//     }
}

void LearningAI::Store(bool winner)
{
    // Loop through the history
    for (std::vector<GameStepElement>::iterator hi = history->begin(); hi != history->end(); ++hi)
    {
        // Find game step in storage
        std::vector<GameStepElement>::iterator si;
        for (si = storage->begin(); si != storage->end(); ++si)
        {
            if ((*si).state0 == (*hi).state0 && (*si).state1 == (*hi).state1 && (*si).state2 == (*hi).state2
                    && (*si).changes0 == (*hi).changes0 && (*si).changes1 == (*hi).changes1)
            {
                break;
            }
        }

        // Insert element if not found in storage
        if (si == storage->end())
        {
            SetStepResult(hi, winner);
            storage->push_back((*hi));
            continue;
        }

        // Set result for step in storage
        SetStepResult(si, winner);
    }

    history->clear();
}

void LearningAI::Convert(std::array<unsigned char, NUM_OF_FIELD_PLACES>* gameField)
{
    for (unsigned char part = 0; part < 3; ++part)
    {
        unsigned short value = 0;

        unsigned char nextSquareStartingPlace = (part + 1) * NUM_OF_SQUARE_PLACES;
        for (unsigned char place = part * NUM_OF_SQUARE_PLACES; place < nextSquareStartingPlace; ++place)
        {
            value |= gameField->at(place);
            if (place < nextSquareStartingPlace - 1)
            {
                value <<= 2;
            }
            // TODO: REMOVE LOGGING
//             Log("P", binaryToString(gameField->at(place)));
//             Log("C", binaryToString(value));
        }

        currentState[part] = value;
        // TODO: REMOVE LOGGING
//         Log("CONV", binaryToString(value));
    }

    currentStep.state0 = currentState[0];
    currentStep.state1 = currentState[1];
    currentStep.state2 = currentState[2];
}

void LearningAI::RandomGenerate()
{
    currentStep.changes0 = rand() % NUM_OF_FIELD_PLACES;
    currentStep.changes1 = rand() % NUM_OF_FIELD_PLACES;
    // TODO: REMOVE LOGGING
//     Log("AI", "Try " + std::to_string(currentStep.changes0) + " " + std::to_string(currentStep.changes1));
}

void LearningAI::SetStepResult(std::vector<GameStepElement>::iterator ei, bool winner)
{
    if (winner)
    {
        (*ei).wins++;
    }
    else
    {
        (*ei).losses++;
    }
}
