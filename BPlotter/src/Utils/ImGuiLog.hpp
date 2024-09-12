#include <deque>
#include <mutex>
#include <spdlog/sinks/base_sink.h>
#include <string>

/**
 * \brief A log entry that can be displayed in ImGui.
 */
struct LogEntry
{
    std::string message;
    spdlog::level::level_enum level;
};

/**
 * \brief A log that can be displayed in ImGui.
 */
class ImGuiLog
{
public:
    std::deque<LogEntry> entry;
    std::mutex mutex;

    /**
     * \brief The maximum number of log entries that can be stored in the log.
     */
    void clear();

    /**
     * \brief Adds a new log entry to the log.
     * \param msg The message to be logged.
     * \param lvl The level of the log message.
     */
    void log(const std::string& msg, spdlog::level::level_enum lvl);

    /**
     * \brief Converts a log level to an ImGui color.
     * \param level The log level to convert.
     * \return The ImGui color corresponding to the log level.
     */
    static ImVec4 toColor(const spdlog::level::level_enum& level);

private:
    size_t MaxSize = 1000;
};


/**
 * \brief A sink for spdlog that logs to an ImGuiLog.
 * @tparam Mutex The mutex type to use.
 */
template<typename Mutex>
class ImGuiLogSink : public spdlog::sinks::base_sink<Mutex>
{
protected:
    ImGuiLog* log;

    /**
     * \brief Logs the message to the ImGuiLog.
     * \param msg The message to log.
     */
    void sink_it_(const spdlog::details::log_msg& msg) override
    {
        spdlog::memory_buf_t formatted;
        this->formatter_->format(msg, formatted);
        log->log(fmt::to_string(formatted), msg.level);
    }

    /**
     * \brief Flushes the log.
     */
    void flush_() override
    {
    }

public:
    explicit ImGuiLogSink(ImGuiLog* log)
        : log(log)
    {
    }
};