/**
 * Learning AI Class - Header File
 * libMorris
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 *
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef LEARNING_AI_H
#define LEARNING_AI_H

#include "GameStepElement.hpp"
#include "Game.hpp"

class LearningAI
{
private:
    /** Pointer to the game object */
    Game* game = nullptr;

    /** AI is spectator, human is controlling the player */
    bool spectator;

    /** Vector of game steps (history) */
    std::vector<GameStepElement>* history;

    /** Vector of game step elements (AI storage) */
    std::vector<GameStepElement>* storage;

    /** Current game field state */
    std::array<unsigned short, 3> currentState = { 0, 0, 0 };

    /** Current game step */
    GameStepElement currentStep;

    /**
     * Convert game field state to the storage one
     *
     * @param[in] gameField The game field
     *
     * @return The state of the game field
     */
    void Convert(std::array<unsigned char, NUM_OF_FIELD_PLACES>* gameField);

    /**
     * Random generate step change
     *
     * @return The changes in the field
     */
    void RandomGenerate();

    /**
     * Set step result
     *
     * @param[in] ei Game step element iterator
     * @param[in] winner Store steps as the game has won by the AI
     */
    void SetStepResult(std::vector<GameStepElement>::iterator ei, bool winner);

public:

    /**
     * Initialize object
     *
     * @param[in] game Pointer to the game object
     * @param[in] spectator AI is spectator
     *
     * @return Initialization was successful
     */
    bool Initialize(Game* game, bool spectator = false);

    /**
     * Is AI spectator?
     *
     * @return AI is spectator
     */
    bool IsSpectator();

    /**
     * Load from AI storage file
     *
     * @param[in] fileName Filename of the AI storage file to load from
     *
     * @return Loading was successful
     */
    bool Load(std::string fileName);

    /**
     * Save to AI storage file
     *
     * @param[in] fileName Filename of the AI storage file to save to
     *
     * @return Saving was successful
     */
    bool Save(std::string fileName);

    /**
     * Get the next step
     *
     * @param[in] retry Retry getting valid step
     *
     * @return The changes in the field
     */
    std::array<unsigned char, 2> GetNextStep(bool retry = false);

    /**
     * Register step in history
     *
     * @param[in] changes The changes in the field
     */
    void Register(std::array<unsigned char, 2> changes);

    /**
     * Store results in storage
     *
     * @param[in] winner Store steps as the game has won by the AI
     */
    void Store(bool winner);
};

#endif // LEARNING_AI_H
