#include "State.hpp"
#include "pch.hpp"

#include "StateStack.hpp"

namespace BenchmarkPlotter
{

State::State(StateStack& stack)
    : mStack(stack)
{
}

void State::draw(sf::RenderWindow& target) const
{
}

bool State::handleEvent(const sf::Event& event)
{
    /*
     * Set this state to transparent -- in other words
     * allow States below in stack to be handle events.
     */
    return true;
}

bool State::updateImGui(const float deltaTime)
{
    /*
     * Set this state to transparent -- in other words
     * allow States below in stack to be updated.
     */
    return true;
}


bool State::fixedUpdate(const float deltaTime)
{
    /*
     * Set this state to transparent -- in other words
     * allow States below in stack to be updated.
     */
    return true;
}

bool State::update(const float deltaTime)
{
    /*
     * Set this state to transparent -- in other words
     * allow States below in stack to be updated.
     */
    return true;
}

void State::requestPush(const State_ID stateID) const noexcept
{
    mStack.push(stateID);
}

void State::requestPop() const noexcept
{
    mStack.pop();
}

void State::requestClear() const noexcept
{
    mStack.clear();
}

State_ID State::requestStateOnTop() const noexcept
{
    return mStack.top();
}

}// namespace BenchmarkPlotter