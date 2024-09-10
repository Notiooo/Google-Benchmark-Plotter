#pragma once

namespace BenchmarkPlotter
{

/**
 * \brief Identifiers of individual states that can be used.
 */
enum class State_ID
{
    // === Application flow states === //
    None,
    MainAppOpen,
    ExitApplicationState,
};

/**
 * \brief Converts state identifier to text
 * \param stateId State identifier
 * \return Textual representation of the state
 */
inline std::string toString(State_ID stateId)
{
    switch (stateId)
    {

        case State_ID::None: return "None";
        case State_ID::MainAppOpen: return "MainAppOpen";
        case State_ID::ExitApplicationState: return "ExitApplicationState";
        default:
            spdlog::warn("I was unable to perform the ID to text conversion.");
            return "Unknown State_ID";
    }
}

}// namespace BenchmarkPlotter
