#pragma once
#include "States.hpp"

namespace BenchmarkPlotter
{

class StateStack;

/**
 * \brief  The state that is on the stack performs the various
 * functions of updating, drawing and handling user inputs.
 */
class State
{
public:
    explicit State(StateStack& stack);
    State(const State&) = delete;
    State(State&&) = delete;

    State& operator=(const State&) = delete;
    State&& operator=(State&&) = delete;

    virtual ~State() = default;

    /**
     * \brief Draws only this state to the passed target
     * \param target where it should be drawn to
     */
    virtual void draw(sf::RenderWindow& target) const;

    /**
     * \brief Updates the state logic at equal intervals independent of the frame rate.
     * \param deltaTime Time interval
     */
    virtual bool fixedUpdate(float deltaTime);

    /**
     * \brief Updates the application logic dependent, or independent of time, every rendered frame.
     * \param deltaTime the time that has passed since the application was last updated.
     */
    virtual bool update(float deltaTime);

    /**
     * \brief It takes input (event) from the user and interprets it
     * \param event user input
     */
    virtual bool handleEvent(const sf::Event& event);

    /**
     * \brief Updates the imgui panel logic dependent, or independent of time, every rendered frame.
     * \param deltaTime the time that has passed since the application was last updated.
     */
    virtual bool updateImGui(float deltaTime);

protected:
    /**
     * \brief The state will be pushed out in the next iteration of the stack.
     * \param stateID Identifier of the state to be pushed
     */
    void requestPush(State_ID stateID) const noexcept;


    /**
     * \brief The state on the top of the stack will be removed in
     * the next iteration of the stack.
     */
    void requestPop() const noexcept;


    /**
     * \brief All states on the stack will be removed in the
     * next iteration of the stack.
     */
    void requestClear() const noexcept;

    /**
     * \brief Returns the current state identifier
     * \return Current state identifier
     */
    [[nodiscard]] State_ID requestStateOnTop() const noexcept;

private:
    StateStack& mStack;//!< Pointer to the stack containing this state
};

}// namespace BenchmarkPlotter