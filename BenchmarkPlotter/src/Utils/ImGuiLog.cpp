#include "ImGuiLog.hpp"
#include "pch.hpp"

void ImGuiLog::clear()
{
    std::lock_guard lock(mutex);
    entry.clear();
    if (entry.size() > MaxSize)
    {
        entry.pop_front();
    }
}
void ImGuiLog::log(const std::string& msg, const spdlog::level::level_enum lvl)
{
    std::lock_guard lock(mutex);
    entry.push_back({msg, lvl});
}

ImVec4 ImGuiLog::toColor(const spdlog::level::level_enum& level)
{
    switch (level)
    {
        case spdlog::level::trace: return {0.8f, 0.8f, 0.8f, 1.0f};
        case spdlog::level::info: return {0.5f, 1.0f, 0.5f, 1.0f};
        case spdlog::level::warn: return {1.0f, 1.0f, 0.5f, 1.0f};
        case spdlog::level::err: return {1.0f, 0.5f, 0.5f, 1.0f};
        case spdlog::level::critical: return {1.0f, 0.5f, 1.0f, 1.0f};
        default: return {0.8f, 0.8f, 0.8f, 1.0f};
    }
}