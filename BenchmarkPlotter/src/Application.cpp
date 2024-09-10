#include "Application.hpp"
#include "pch.hpp"

#include "States/CustomStates/ExitApplicationState.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace BenchmarkPlotter
{

/**
 * @brief I kindly ask that the following variable not be changed, as the
 * physics calculations have been adjusted under the following framerate.
 * Thank you.
 *
 * @warning If you do not comply, the user will move differently and jump lower or higher.
 */
constexpr int MINIMAL_FIXED_UPDATES_PER_FRAME = 60;

const sf::Time Application::TIME_PER_FIXED_UPDATE_CALLS =
    sf::seconds(1.f / MINIMAL_FIXED_UPDATES_PER_FRAME);
const int Application::SCREEN_WIDTH = 1280;
const int Application::SCREEN_HEIGHT = 720;


void Application::configureImGuiSinks()
{
    auto imguiSink = std::make_shared<ImGuiLogSink<std::mutex>>(&mImguiLog);
    imguiSink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleSink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
    const auto logger = std::make_shared<spdlog::logger>(
        "multi_sink", spdlog::sinks_init_list{imguiSink, consoleSink});
    spdlog::register_logger(logger);
    spdlog::set_default_logger(logger);
}

void Application::configureImGui()
{
    configureImGuiSinks();
    if (not ImGui::SFML::Init(mWindow))
    {
        spdlog::critical("Imgui-SFML not initialized properly");
    }
    setupImGuiStyle();
    if (auto& io = ImGui::GetIO(); !(io.ConfigFlags & ImGuiConfigFlags_DockingEnable))
    {
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }
}
void Application::setupFlowStates()
{
    mAppStack.saveState<ExitApplicationState>(State_ID::ExitApplicationState);
    mAppStack.saveState<ExitApplicationState>(State_ID::MainAppOpen);
}

Application::Application()
    : mWindow(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "BenchmarkPlotter")
{
    loadResources();
    configureImGui();
    setupFlowStates();
    mAppStack.push(State_ID::MainAppOpen);
}

void Application::run()
{
    spdlog::info("Apps starts, the resolution is {}x{}", mWindow.getSize().x, mWindow.getSize().y);

    performApplicationLoop();

    mWindow.close();
    ImGui::SFML::Shutdown();
}

void Application::performApplicationLoop()
{
    sf::Clock clock;
    auto frameTimeElapsed = sf::Time::Zero;
    mFixedUpdateClock.restart();
    while (isApplicationRunning)
    {
        frameTimeElapsed = clock.restart();
        update(frameTimeElapsed);
        fixedUpdateAtEqualIntervals();
        processEvents();

        render();
    }
    mAppStack.forceInstantClear();
}

void Application::fixedUpdateAtEqualIntervals()
{
    mTimeSinceLastFixedUpdate += mFixedUpdateClock.restart();
    if (mTimeSinceLastFixedUpdate > TIME_PER_FIXED_UPDATE_CALLS)
    {
        do
        {
            mTimeSinceLastFixedUpdate -= TIME_PER_FIXED_UPDATE_CALLS;
            fixedUpdate(TIME_PER_FIXED_UPDATE_CALLS);
        }
        while (mTimeSinceLastFixedUpdate > TIME_PER_FIXED_UPDATE_CALLS);
    }
}

void Application::updateImGuiLogger()
{
    if (ImGui::Button("Console"))
    {
        ImGui::SetNextWindowSize(ImVec2(mWindow.getSize().x, mWindow.getSize().y / 2.f));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::OpenPopup("ConsolePopup");
    }

    ImGuiWindowFlags popup_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;

    if (ImGui::BeginPopup("ConsolePopup", popup_flags))
    {
        if (ImGui::Button("Clear"))
        {
            mImguiLog.clear();
        }
        ImGui::SameLine();
        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::BeginChild("scrolling"))
        {
            std::lock_guard lock(mImguiLog.mutex);
            const auto scrollToBottom = ImGui::GetScrollY() >= ImGui::GetScrollMaxY();
            for (const auto& [message, level]: mImguiLog.entry)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGuiLog::toColor(level));
                ImGui::TextUnformatted(message.c_str());
                ImGui::PopStyleColor();
            }
            if (scrollToBottom)
            {
                ImGui::SetScrollHereY(1.0f);
            }
            ImGui::EndChild();
        }
        ImGui::EndPopup();
    }
}

void Application::updateImGui(const sf::Time& deltaTime)
{
    ImGui::SFML::Update(mWindow, deltaTime);
    ImGui::SetNextWindowSize(mWindow.getSize());
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                                    ImGuiWindowFlags_NoNavFocus;
    if (ImGui::Begin("Full-Screen Window", nullptr, window_flags))
    {
        if (ImGui::BeginMainMenuBar())
        {
            mAppStack.updateImGui(deltaTime.asSeconds());
            if (ImGui::BeginMenu("Help"))
            {
                updateImGuiLogger();
                ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();
    }
    ImGui::End();
}

void Application::processEvents()
{
    while (const auto optionalEvent = mWindow.pollEvent())
    {
        if (optionalEvent.has_value())
        {
            auto event = optionalEvent.value();
            if (event.is<sf::Event::Closed>())
            {
                isApplicationRunning = false;
            }

            ImGui::SFML::ProcessEvent(mWindow, event);

            if (not mWindow.hasFocus())
            {
                return;
            }
            mAppStack.handleEvent(event);
        }
    }
}

void Application::fixedUpdate(const sf::Time& deltaTime)
{
    const auto deltaTimeInSeconds = deltaTime.asSeconds();
    mAppStack.fixedUpdate(deltaTimeInSeconds);
}

void Application::update(const sf::Time& deltaTime)
{
    const auto deltaTimeInSeconds = deltaTime.asSeconds();

    updateImGui(deltaTime);

    mAppStack.update(deltaTimeInSeconds);

    if (mAppStack.top() == State_ID::ExitApplicationState)
    {
        isApplicationRunning = false;
    }
}

void Application::render()
{
    mWindow.clear();
    mAppStack.draw(mWindow);
    ImGui::SFML::Render();
    mWindow.display();
}


void Application::loadResources()
{
    // Empty
}

void Application::setupImGuiStyle()
{
    // Thank you Trippasch
    // https://github.com/ocornut/imgui/issues/707
    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.1f, 0.13f, 0.8f};
    colors[ImGuiCol_MenuBarBg] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Border
    colors[ImGuiCol_Border] = ImVec4{0.44f, 0.37f, 0.61f, 0.29f};
    colors[ImGuiCol_BorderShadow] = ImVec4{0.0f, 0.0f, 0.0f, 0.24f};

    // Text
    colors[ImGuiCol_Text] = ImVec4{1.0f, 1.0f, 1.0f, 1.0f};
    colors[ImGuiCol_TextDisabled] = ImVec4{0.5f, 0.5f, 0.5f, 1.0f};

    // Headers
    colors[ImGuiCol_Header] = ImVec4{0.13f, 0.13f, 0.17, 1.0f};
    colors[ImGuiCol_HeaderHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
    colors[ImGuiCol_HeaderActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{0.13f, 0.13f, 0.17, 1.0f};
    colors[ImGuiCol_ButtonHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
    colors[ImGuiCol_ButtonActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_CheckMark] = ImVec4{0.74f, 0.58f, 0.98f, 1.0f};

    // Popups
    colors[ImGuiCol_PopupBg] = ImVec4{0.1f, 0.1f, 0.13f, 0.92f};

    // Slider
    colors[ImGuiCol_SliderGrab] = ImVec4{0.44f, 0.37f, 0.61f, 0.54f};
    colors[ImGuiCol_SliderGrabActive] = ImVec4{0.74f, 0.58f, 0.98f, 0.54f};

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{0.13f, 0.13, 0.17, 1.0f};
    colors[ImGuiCol_FrameBgHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
    colors[ImGuiCol_FrameBgActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_TabHovered] = ImVec4{0.24, 0.24f, 0.32f, 1.0f};
    colors[ImGuiCol_TabActive] = ImVec4{0.2f, 0.22f, 0.27f, 1.0f};
    colors[ImGuiCol_TabUnfocused] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_TitleBgActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Scrollbar
    colors[ImGuiCol_ScrollbarBg] = ImVec4{0.1f, 0.1f, 0.13f, 1.0f};
    colors[ImGuiCol_ScrollbarGrab] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{0.24f, 0.24f, 0.32f, 1.0f};

    // Seperator
    colors[ImGuiCol_Separator] = ImVec4{0.44f, 0.37f, 0.61f, 1.0f};
    colors[ImGuiCol_SeparatorHovered] = ImVec4{0.74f, 0.58f, 0.98f, 1.0f};
    colors[ImGuiCol_SeparatorActive] = ImVec4{0.84f, 0.58f, 1.0f, 1.0f};

    // Resize Grip
    colors[ImGuiCol_ResizeGrip] = ImVec4{0.44f, 0.37f, 0.61f, 0.29f};
    colors[ImGuiCol_ResizeGripHovered] = ImVec4{0.74f, 0.58f, 0.98f, 0.29f};
    colors[ImGuiCol_ResizeGripActive] = ImVec4{0.84f, 0.58f, 1.0f, 0.29f};

    auto& style = ImGui::GetStyle();
    style.TabRounding = 4;
    style.ScrollbarRounding = 9;
    style.WindowRounding = 7;
    style.GrabRounding = 3;
    style.FrameRounding = 3;
    style.PopupRounding = 4;
    style.ChildRounding = 4;
}

}// namespace BenchmarkPlotter