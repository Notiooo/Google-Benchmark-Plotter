#pragma once

#include <functional>
#include <future>
#include <string>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <result.hpp>


#ifndef GIT_TAG
    #define GIT_TAG "0.0.0"
#endif

namespace BPlotter
{

/**
 * @brief A utility class to check for updates from a GitHub repository.
 *
 * This class interacts with the GitHub API to fetch the latest release of
 * the specified repository and determines if a newer version is available.
 */
class UpdateChecker
{
private:
    /**
     * @brief Represents the version information of the latest release.
     */
    struct Version
    {
        std::string number;     //!< The semantic version string of the release.
        std::string downloadUrl;//!< The URL to download the release.
    };

public:
    /**
     * @brief Constructs an UpdateChecker for a specific GitHub repository.
     * @param repoOwner The owner of the repository.
     * @param repoName The name of the repository.
     */
    UpdateChecker(const std::string& repoOwner, const std::string& repoName);

    /**
     * @brief Renders a button in the ImGui interface to check for updates.
     * When clicked, it triggers an update check and displays the appropriate popup.
     */
    void updateCheckForUpdateButton();

private:
    /**
     * @brief Checks if a newer version of the application is available.
     * @return True if a newer version is available, false otherwise.
     */
    bool checkForUpdates();

    /**
     * @brief Handles the action of the "Check for update" button.
     * Triggers an update check and opens the relevant popup (update available or no updates).
     */
    void handleUpdateCheckButton();

    /**
     * @brief Opens the specified URL in the default web browser.
     * @param url The URL to open.
     */
    static void openURLInBrowser(const std::string& url);

    /**
     * @brief Renders the popup modal shown when an update is available.
     */
    void renderUpdateAvailablePopup();

    /**
     * @brief Renders the popup modal shown when no updates are available.
     */
    void renderNoUpdatePopup();

    /**
     * @brief Fetches the latest release JSON data from the GitHub API.
     * @return The JSON body as a string if successful, or an error message.
     */
    cpp::result<std::string, std::string> fetchJsonBodyFromGithub();

    /**
     * @brief Fetches the version information of the latest release from GitHub.
     * @return A Version struct containing the latest release details, or an error message.
     */
    cpp::result<Version, std::string> fetchLatestRelease();

    /**
     * @brief Compares two version of app to determine if one is newer.
     * @param currentVersion The current application version.
     * @param remoteVersion The version to compare against.
     * @return True if the remote version is newer, false otherwise.
     */
    static cpp::result<bool, std::string> isNewerVersion(const std::string& currentVersion,
                                                         const std::string& remoteVersion);

private:
    std::string mRepoOwner;
    std::string mRepoName;
    Version mLatestVersion;
};

}// namespace BPlotter