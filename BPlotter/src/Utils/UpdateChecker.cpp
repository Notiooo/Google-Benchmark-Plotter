#include "UpdateChecker.hpp"
#include <semver/semver.hpp>

namespace BPlotter
{

namespace
{
constexpr const char* UpdateAvailable = "Update Available";
constexpr const char* NoUpdate = "No Updates";
}// namespace

UpdateChecker::UpdateChecker(const std::string& repoOwner, const std::string& repoName)
    : mRepoOwner(repoOwner)
    , mRepoName(repoName)
{
}

void UpdateChecker::openURLInBrowser(const std::string& url)
{
    std::string command = "xdg-open " + url;
#ifdef _WIN32
    command = "start " + url;
#elif __APPLE__
    command = "open " + url;
#endif
    if (std::system(command.c_str()) != 0)
    {
        spdlog::error("Failed to open URL in browser: {}", url);
    }
}

void UpdateChecker::updateCheckForUpdateButton()
{
    handleUpdateCheckButton();
    renderUpdateAvailablePopup();
    renderNoUpdatePopup();
}

void UpdateChecker::handleUpdateCheckButton()
{
    if (ImGui::Button("Check for update"))
    {
        if (checkForUpdates())
        {
            ImGui::OpenPopup(UpdateAvailable);
        }
        else
        {
            ImGui::OpenPopup(NoUpdate);
        }
    }
}

void UpdateChecker::renderUpdateAvailablePopup()
{
    if (ImGui::BeginPopupModal(UpdateAvailable, nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        ImGui::Text("A new version is available: %s", mLatestVersion.number.c_str());

        if (ImGui::Button("Open Download Page"))
        {
            openURLInBrowser(mLatestVersion.downloadUrl);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void UpdateChecker::renderNoUpdatePopup()
{
    if (ImGui::BeginPopupModal(NoUpdate, nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        ImGui::Text("You are already using the latest version (%s).", GIT_TAG);

        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

cpp::result<std::string, std::string> UpdateChecker::fetchJsonBodyFromGithub()
{
    httplib::Client client("https://api.github.com");
    auto res = client.Get(("/repos/" + mRepoOwner + "/" + mRepoName + "/releases/latest").c_str());

    if (!res || res->status != 200)
    {
        return cpp::fail(fmt::format("HTTP Error {}: Unable to fetch latest release info.",
                                     res ? res->status : -1));
    }

    return res->body;
}


cpp::result<UpdateChecker::Version, std::string> UpdateChecker::fetchLatestRelease()
{
    auto response = fetchJsonBodyFromGithub();
    if (!response)
    {
        return cpp::fail(response.error());
    }

    try
    {
        auto jsonData = nlohmann::json::parse(response.value());
        Version version{jsonData["tag_name"].get<std::string>(), ""};

        for (const auto& asset: jsonData["assets"])
        {
            if (asset["name"].get<std::string>().find("bplotter_release_windows") !=
                std::string::npos)
            {
                version.downloadUrl = asset["browser_download_url"].get<std::string>();
                return version;
            }
        }

        return cpp::fail("No suitable asset found in release data.");
    }
    catch (const std::exception& e)
    {
        return cpp::fail(fmt::format("JSON Parsing Error: {}", e.what()));
    }
}

cpp::result<bool, std::string> UpdateChecker::isNewerVersion(const std::string& currentVersion,
                                                             const std::string& remoteVersion)
{
    try
    {
        semver::version current = semver::version::parse(currentVersion, false);
        semver::version remote = semver::version::parse(remoteVersion, false);
        return remote > current;
    }
    catch (const std::exception& e)
    {
        return cpp::fail(e.what());
    }
}

bool UpdateChecker::checkForUpdates()
{
    auto releaseResult = fetchLatestRelease();
    if (!releaseResult)
    {
        spdlog::error("Error fetching release: {}", releaseResult.error());
        return false;
    }

    auto versionCheckResult = isNewerVersion(GIT_TAG, releaseResult.value().number);
    if (!versionCheckResult)
    {
        spdlog::error("Version comparison failed: {}", versionCheckResult.error());
        return false;
    }

    if (versionCheckResult.value())
    {
        mLatestVersion = releaseResult.value();
        return true;
    }

    return false;
}

}// namespace BPlotter