#include "MainAppOpen.hpp"
#include "pch.hpp"

namespace BenchmarkPlotter
{

MainAppOpen::MainAppOpen(StateStack& stack)
    : State(stack)
{
}
void MainAppOpen::draw(sf::RenderWindow& target) const
{
}
bool MainAppOpen::fixedUpdate(const float deltaTime)
{
    return true;
}
bool MainAppOpen::update(const float deltaTime)
{
    return true;
}
bool MainAppOpen::handleEvent(const sf::Event& event)
{
    return true;
}
bool MainAppOpen::updateImGui(const float deltaTime)
{
    return true;
}

}// namespace BenchmarkPlotter