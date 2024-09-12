#pragma once


#include <SFML/Graphics/RenderWindow.hpp>

#include "Resources/Resources.hpp"
#include "States/StateStack.hpp"
#include "Utils/ImGuiLog.hpp"

namespace BPlotter
{

/**
 * \brief The main engine class that controls the entire flow of the application.
 *
 * The whole task of this class is the run() function,
 * which ensures that the engine runs. It runs the processes
 * of displaying the engine (image), capturing user input
 * and updating the engine logic.
 */
class Application
{
public:
    Application();
    /**
     * \brief Starts the engine and keeps it running until the user finishes it.
     *
     * Updates the engine logic, displays the engine image and captures the
     * user inputs to the various parts of the program.
     */
    void run();

private:
    /**
     * \brief The main loop that controls the operation of the engine in the loop.
     *
     * Updates the engine logic, displays the engine image and captures the
     * user inputs to the various parts of the program.
     */
    void performApplicationLoop();

    /**
     * \brief Intercepts user inputs and passes them to processes inside the application.
     */
    void processEvents();

    /**
     * \brief Updates the application logic at equal intervals independent of the frame rate.
     * \param deltaTime Time interval
     *
     * This function is very aggravating and highly inefficient. In a certain amount of time it
     * executes an equal number of times by which it is ideal, for example, for collision
     * recalculation. Thus, in case of lag, there are no situations where the user can run through
     * a wall. Intermediate intervals regardless of the frame rate are always recalculated!
     *
     * Updates the application logic by passing the time that has elapsed
     * since the previous interval. This allows objects to move independently
     * of the speed at which subsequent iterations of the program are executed.
     * (distance = speed * time)
     */
    void fixedUpdate(const sf::Time& deltaTime);

    /**
     * It performs fixed updates at equal intervals. In case of huge time gaps, it makes up for it
     * by executing one by one successive calls with the same fixed time argument. to avoid behavior
     * where, due to high lag, a character is moved off the wall avoiding collision checking.
     */
    void fixedUpdateAtEqualIntervals();

    /**
     * \brief Updates the display of the imgui menu for the application logger
     */
    void updateImGuiLogger();

    /**
     * \brief Creates ImGui objects (every frame)
     * \param deltaTime Time elapsed since the previous frame
     */
    void updateImGui(const sf::Time& deltaTime);

    /**
     * \brief Updates the application logic dependent, or independent of time, every rendered frame.
     * \param deltaTime the time that has passed since the application was last updated.
     *
     * It is not recommended to use this feature for physics, or general movement.
     * Updates the application logic by passing the time that has elapsed since the previous
     * update call. This allows objects to move independently of the speed at which
     * subsequent frames of the program are executed.
     * (distance = speed * time)
     */
    void update(const sf::Time& deltaTime);

    /**
     * \brief Displays the application on the image of the application window
     *
     * It collects all the application elements that should be displayed in the current
     * iteration and displays them on the screen in the order they appear in the code.
     * It clears the screen with a black image before displaying a new frame.
     */
    void render();

    /**
     * \brief Loads the resources needed by the processes inside the application.
     *
     * Not all available resources are listed here, as some are loaded on the fly
     * inside the app. This is because for example there is no need to keep the whole
     * app in memory when the user is really only inside the app's main menu.
     */
    void loadResources();

    /**
     * \brief Sets the ImGui style
     */
    static void setupImGuiStyle();

    /**
     * Initializes spdlog with two sinks:
     * - ImGuiLogSink: Displays logs in an ImGui window, formatting timestamps and log levels.
     * - stdout_color_sink_mt: Outputs logs to the console with color formatting.
     */
    void configureImGuiSinks();

    /**
     * \brief Configures the ImGui library
     */
    void configureImGui();

    /**
     * \brief Sets the flow states of the application inside of statestack
     */
    void setupFlowStates();

    /**
     * @brief The time it takes for one app frame to be generated.
     */
    static const sf::Time TIME_PER_FRAME;

    /**
     * @brief Time between one fixed update and another
     */
    static const sf::Time TIME_PER_FIXED_UPDATE_CALLS;

    /**
     * @brief Default app window width
     */
    static const int SCREEN_WIDTH;

    /**
     * @brief Default app window height
     */
    static const int SCREEN_HEIGHT;

    /**
     * @brief The window to which the app image should be drawn.
     */
    sf::RenderWindow mWindow;

    /**
     * TODO: THIS
     */
    bool isApplicationRunning = true;

    /**
     * @brief A clock used to determine the last time the fixedUpdate function was called
     */
    sf::Clock mFixedUpdateClock;

    /**
     * @brief Time since the last call to the fixedUpdate function
     */
    sf::Time mTimeSinceLastFixedUpdate = sf::Time::Zero;

    /**
     * @brief Any app assets from textures or fonts
     */
    ApplicationResources mApplicationResources;

    /**
     * \brief Stores and manages in-app states.
     *
     * This allows you to create some application flow between application states.
     * Among other things, it allows to go from the main menu of the app to the app
     * itself, as well as to pause the app.
     */
    StateStack mAppStack;

    /**
     * \brief The ImGui log object that stores the logs displayed in the application.
     */
    ImGuiLog mImguiLog;
};

}// namespace BPlotter
