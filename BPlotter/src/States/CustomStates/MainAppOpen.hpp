#pragma once

#include "States/State.hpp"

namespace BPlotter
{

class StateStack;

/**
 * @brief The main app state
 */
class MainAppOpen : public State
{
public:
    explicit MainAppOpen(StateStack& stack);

    /**
     * \brief Draws only this state to the passed target
     * \param target where it should be drawn to
     */
    void draw(sf::RenderWindow& target) const override;

    /**
     * \brief Updates the state logic at equal intervals independent of the frame rate.
     * \param deltaTime Time interval
     */
    bool fixedUpdate(float deltaTime) override;

    /**
     * \brief Updates the application logic dependent, or independent of time, every rendered frame.
     * \param deltaTime the time that has passed since the application was last updated.
     */
    bool update(float deltaTime) override;

    /**
     * \brief It takes input (event) from the user and interprets it
     * \param event user input
     */
    bool handleEvent(const sf::Event& event) override;

    /**
     * \brief Updates the imgui panel logic dependent, or independent of time, every rendered frame.
     * \param deltaTime the time that has passed since the application was last updated.
     */
    bool updateImGui(float deltaTime) override;
};

}// namespace BPlotter