#pragma once

#include "States/State.hpp"

namespace BPlotter
{

class StateStack;

/**
 * @brief The state in which the user wants to close the application
 */
class ExitApplicationState : public State
{
public:
    explicit ExitApplicationState(StateStack& stack);
};

}// namespace BPlotter