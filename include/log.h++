#pragma once

#include <atomic>
#include <filesystem>
#include <fstream>
#include <optional>
#include <shared_mutex>
#include <unordered_map>

constexpr size_t LOG_MAX_SIZE = 10 * 1024 * 1024;

namespace nutsloop {

const std::filesystem::path nutsloop_dir = std::filesystem::path(std::getenv("HOME")) / ".nutsloop";
const std::filesystem::path nutsloop_logs_directory = nutsloop_dir / "logs";

struct log_settings_t {
  std::string ident;
  std::string filename;
  std::optional<std::filesystem::path> directory;
  std::optional<std::string> session_header;
  bool active;
};

struct log_t {
  log_settings_t settings;
  std::ofstream stream;
  bool running;
};

using log_registry_t = std::unordered_map<std::string, log_t>;

class log {
public:
  /**
   * Activates the logging system and prepares necessary resources for logging.
   *
   * This method ensures the logging system is in an active state, allowing log entries
   * to be recorded. If already active, it updates the active status.
   * Additionally, it sets up the log registry if it is not already initialized.
   * Internal debug information is logged when the DEBUG flag is enabled.
   * Thread-safe locking mechanisms are used during the activation process.
   *
   * @note This method is intended to be called before logging any entries to guarantee
   * the logging system is operational. The activation status is tracked, and a debug
   * stream logs updates if logging in debug mode is enabled.
   */
  static void activate();
  /**
   * Deactivates the logging system and releases its active state.
   *
   * This method transitions the logging system to an inactive state, ensuring that
   * no further log entries can be recorded until reactivation. If the system was
   * previously active, the change is logged in the internal debug stream, provided
   * debugging is enabled.
   * Thread-safe mechanisms are employed to ensure safe updates to the active status.
   *
   * @note This method is primarily used when logging functionality is no longer needed
   * or before shutting down the application. Any logs generated after deactivation
   * will not be recorded until reactivation.
   */
  static void deactivate();
  /**
   * Configures the logging system with the specified settings.
   *
   * This method applies the provided log settings to control the behavior and
   * configuration of the logging system. Existing settings will be updated
   * to match the provided configuration. The input settings determine aspects
   * like log level, output format, destinations, and filters.
   *
   * Proper thread synchronization is implemented to ensure a consistent state
   * when settings are being updated. Invoking this method with incomplete or
   * invalid settings may lead to undefined behavior.
   *
   * @param settings The configuration object containing logging parameters
   * to be applied to the logging system.
   */
  static void set( const log_settings_t& settings );

  // not implemented yet
  static bool full_running( std::string ident );
  static void start( std::string ident ) = delete;
  static void stop( std::string ident ) = delete;
  static void close( std::string ident ) = delete;
  static void flush( std::string ident ) = delete;
  static void remove( std::string ident ) = delete;

  /**
   * Prepares and returns a stream for logging with specified details.
   *
   * This method retrieves and validates a logging context based on the given identifier,
   * associates the provided file and line information, and prefixes the log entry with
   * the specified logging level. If logging is disabled, not running, or fails to initialize
   * correctly for the identifier, a null stream is returned.
   *
   * @param ident The unique identifier for the logging context to be used.
   * @param file The name or path of the source file where the log is generated.
   * @param line The line number in the source file where the log is generated.
   * @param level The severity level of the log message. Defaults to 'I' (Informational).
   * @return A reference to the logging output stream for writing log entries.
   */
  static std::ostream& stream( const char* ident, const char* file, int line, char level = 'I' );

private:

  // MARK: (LOG) private static log debug info file methods and fields
  static std::atomic<bool> tmp_debug_file_already_set_;
  static std::atomic<std::string*> tmp_debug_file_path_;
  static std::unique_ptr<std::ofstream> debug_tmp_file_stream_;
  /**
   * Ensures that the internal debug logging file is active and ready for use.
   *
   * This method validates and initializes the temporary debug file stream for
   * storing debug log entries. If the debugging file is not yet set or the stream
   * is null, it creates the debug file, opens the output stream, and writes a session
   * header along with relevant status information. If the stream is already active
   * and open, this method logs the current state and ensures the file is ready for use.
   *
   * The method manages and logs the state of the debug file activation process,
   * ensuring that file creation and stream handling occur as specified.
   */
  static void debug_file_is_active_();
  /**
   * Scans the temporary directory for internal debug log files, and provides
   * an option to reassign or clean up these files based on user input.
   *
   * This method searches for debug log files in the temporary directory, identifies
   * files with specific naming conventions, and allows the user to either delete the
   * files or reassign one of them as the active debug log file for internal logging.
   * If multiple debug files are found, the user is prompted to handle each file individually.
   *
   * @return A boolean indicating whether a debug log file was successfully reassigned.
   * Returns true if a file was reassigned, and false otherwise.
   */
  [[nodiscard]] static bool debug_tmp_file_reassign_or_cleanup_();
  /**
   * Creates a temporary debug file and manages its lifecycle.
   *
   * This method generates a unique, secure temporary debug file for internal logging purposes.
   * If a temporary file already exists or needs reassignment, the method invokes necessary
   * cleanup routines and exits early. The temporary file is created using a secure naming convention
   * with the current datetime to ensure uniqueness. Once created, the file's path is stored atomically
   * for thread-safe access, and a flag is set to indicate its readiness for use.
   *
   * If file creation or closing operations fail, the method logs an error and terminates the application.
   */
  static void debug_tmp_file_create_();
  /**
   * Provides an internal debug stream for logging, containing detailed contextual information.
   *
   * This method generates a debug stream entry with a specified severity level, source file, and line number.
   * It is used internally within the logging system for diagnostic or debugging purposes.
   *
   * @param file The name or path of the source file where the debug entry is generated.
   * @param line_number The line number in the source file where the debug entry is generated.
   * @param level The severity level for the debug message.
   * @return A reference to the debug output stream for writing debug log entries.
   */
  static std::ofstream& debug_stream_( const char* file, int line_number = 0, char level = 'I' );

  // MARK: (LOG) private static methods and fields
  static std::shared_mutex mtx_;

  /**
   * Checks whether the logging system is currently activated.
   *
   * This method determines the activation status of the logging system, utilizing
   * an atomic flag to ensure thread-safe access. If debug mode is enabled, a
   * message with the current activation state is logged.
   *
   * @return True if the logging system is active; otherwise, false.
   */
  bool static is_activated_();
  static std::atomic<bool> activated_;
  /**
   * // HINT: maybe find a purpose for this? :D
   * Checks if the internal 'set' function of the logging system has been called.
   *
   * This method provides a way to determine whether the logging system has been
   * initialized or configured by verifying the state of the internal flag.
   * For debugging purposes, additional information about the method call
   * and the current state of the flag is logged if debugging is enabled.
   *
   * @return True if the 'set' function has been called, otherwise false.
   */
  static bool is_set_called_();
  static std::atomic<bool> set_has_been_called_;

  /**
   * Checks and retrieves any error message associated with the logging file of a given logging context.
   *
   * This method evaluates the provided logging context file to determine if any errors
   * have occurred during logging operations. If an error is detected, the corresponding
   * error message is returned. If no error is found or the input is null, an empty optional is returned.
   *
   * @param log_ident A pointer to the logging context whose file is being inspected for errors.
   * @return An optional string containing the error message if an error exists, or an empty optional otherwise.
   */
  static std::optional<std::string> error_on_log_file_( const log_t* log_ident );
  static std::unique_ptr<log_t> log_;
  static std::unique_ptr<log_registry_t> log_registry_;

  /**
   * Creates a directory at the specified path using default settings.
   *
   * This method attempts to create a directory at the given path. If the directory
   * already exists, no action is performed. If the creation fails due to insufficient
   * permissions or other issues, an exception may be thrown.
   *
   * @param path The filesystem path where the directory should be created.
   */
  static void mkdir_default_( const std::filesystem::path& path );

  /**
   * Converts a log level character to its corresponding descriptive string.
   *
   * This method translates a single-character log level identifier (e.g., 'I', 'E', 'W')
   * into a human-readable string representation of the log level (e.g., "INFO", "ERROR", "WARN").
   *
   * - If the provided log level character is not recognized, the method may return a default
   *   or error string, depending on implementation.
   *
   * @param level The single-character log level identifier to be converted.
   * @return A constant character pointer to the descriptive string representing the log level.
   */
  static const char* level_( char level );

  /**
   * Generates a shortened version of the provided file system path.
   * This function modifies the input path to create a more concise representation,
   * which may be useful for log messages, user interfaces, or other scenarios
   * where a full path might be too verbose.
   *
   * The method preserves critical parts of the path while abbreviating
   * less significant segments. The specific logic for shortening depends on
   * implemented rules and heuristics.
   *
   * @param dir The original path to be shortened, represented as a `std::filesystem::path`.
   *
   * @return A string containing the shortened version of the provided path.
   */
  static std::string shortened_path_(const std::filesystem::path &dir);

  /**
   * Generates a new session header string for logging purposes.
   *
   * This method constructs a formatted header string that includes the session identifier,
   * the current timestamp, and the associated file name, providing a contextual wrapper
   * for log entries.
   *
   * @param ident The identifier for the logging session. Typically used to represent a unique
   * session or context for the logs.
   * @param filename The name of the file being logged or associated with the logging session.
   * @return A formatted string representing the session header, including the identifier,
   * timestamp, and file name.
   */
  static std::string generate_new_session_header_( const std::string& ident, const std::string& filename );

  /**
   * Retrieves the current date and time as a formatted string.
   *
   * This method fetches the system's current date and time, formatting it in the
   * "%d-%m-%Y_%H:%M:%S" format (day-month-year_hour:minute:second). If an error
   * occurs during formatting, a fallback value "unknown-time" is returned.
   *
   * Debugging information may be logged to a temporary debugging stream if debugging
   * is enabled and appropriate conditions are met.
   *
   * @return A string representing the current date and time, or "unknown-time" if
   * an error occurs during formatting.
   */
  static std::string datetime_();

  // MARK: (LOGGER) private classes
  /**
   * Manages the redirection of `std::ostream` streams to a new stream buffer, allowing
   * custom handling of standard output (`std::cout`) and error (`std::cerr`) streams.
   *
   * When an instance of this class is created, it redirects the specified output
   * and error streams to a provided stream buffer. The original stream states are
   * preserved and automatically restored upon the destruction of the instance.
   *
   * @param stream_out Reference to the output stream that will be redirected (e.g., `std::cout`).
   * @param stream_err Reference to the error stream that will be redirected (e.g., `std::cerr`).
   * @param log_file_buffer The new stream buffer to which the streams will be redirected.
   *
   * @note Automatically restores the original stream buffers for `std::cout` and
   *       `std::cerr` when the instance is destroyed.
   *
   * @warning The provided stream buffer (`log_file_buffer`) must remain valid for the lifetime
   *          of the `stream_redirect_` instance. Destroying or modifying the buffer
   *          while redirection is in place can lead to undefined behavior.
   *
   * The destructor ensures the integrity of the streams by restoring their original
   * stream buffers, preventing any unintended stream behaviors after the instance is destructed.
   */
  class stream_redirect_ {
  public:

    /**
     * Redirects the output streams `stream_out` and `stream_err` to a new stream buffer.
     * Saves the original stream buffers of `std::cout` and `std::cerr` for potential restoration.
     *
     * This constructor replaces the rdbuf of both provided output streams with the specified
     * `log_file_buffer`, redirecting their output to the given stream buffer. It also preserves
     * the original stream buffers for later use, such as restoring the streams to their
     * previous state.
     *
     * @param stream_out The output stream to redirect, typically `std::cout`.
     * @param stream_err The error stream to redirect, typically `std::cerr`.
     * @param log_file_buffer The new stream buffer to be assigned to both `stream_out` and `stream_err`.
     *
     * @note The new stream buffer will handle all output from both streams after the redirection.
     *       Ensure that the `log_file_buffer` remains valid for the lifetime of the redirection.
     */
    stream_redirect_(std::ostream& stream_out, std::ostream& stream_err, std::streambuf* log_file_buffer);

    /**
     * Destructor for the `stream_redirect_` class.
     *
     * Restores the original stream buffers for `std::cout` and `std::cerr`,
     * reverting any prior redirection performed by the `stream_redirect_` instance.
     * This ensures that the standard output and error streams resume their
     * original behavior after the lifetime of the `stream_redirect_` object ends.
     *
     * @note This function is called automatically when the `stream_redirect_` object
     *       goes out of scope.
     * @note It is essential that the saved original stream buffers are valid
     *       at the time of restoration.
     */
    ~stream_redirect_();

  private:
    std::streambuf* original_cout_;
    std::streambuf* original_cerr_;
  };
  static std::unique_ptr<stream_redirect_> stream_redirect_pointer_;

  /**
   * A specialized custom stream buffer that discards all inserted data.
   *
   * The `null_buffer_` class is a final class derived from `std::streambuf`,
   * used specifically for creating a `null sink` that consumes and ignores
   * any input without performing actual I/O operations. It is commonly used
   * in scenarios where logging or output needs to be suppressed.
   */
  class null_buffer_ final : public std::streambuf {
  public:

    /**
     * Handles the overflow of an input character in the stream buffer.
     * This function is invoked by the stream buffer when an overflow
     * operation occurs. It processes the input and determines the handling
     * of the overflow situation.
     *
     * @param input The integer representation of the character to be handled
     *              during overflow.
     * @return The input value, unchanged.
     */
    int overflow( const int input) override {
      return input;
    }
  };

  /**
   * A specialized logger class derived from `std::ostream` that performs no-op operations
   * for all output streams redirected to it.
   *
   * This class is intended for use in cases where logging functionality must exist,
   * but actual output is not required. It effectively discards any input given to it.
   *
   * The `null_logger_` leverages an internal `null_buffer_` that implements a stream buffer
   * which discards all written data.
   *
   * @note Designed for scenarios where a placeholder logger is needed. It prevents unnecessary
   *       overhead caused by output redirection while maintaining interface compliance with
   *       standard logging practices.
   *
   * @note This class is final and cannot be inherited further.
   */
  class null_logger_ final : public std::ostream {
  public:

    /**
     * Constructs an instance of `null_logger_`, which acts as a specialized
     * output stream that discards all written data.
     *
     * This constructor initializes the `null_logger_` object by associating it
     * with a `null_buffer_`, effectively ensuring that all output written to this
     * stream is redirected to a sink that does nothing.
     *
     * @return An instance of `null_logger_` configured to silently discard any output.
     */
    null_logger_() : std::ostream(&null_buffer_) {}
  private:

    null_buffer_ null_buffer_;
  };
  /**
   * A static global member of `null_logger_` type that serves as a no-op logger.
   *
   * This member is designed to silently discard any log messages sent to it. It is useful
   * in scenarios where logging needs to be disabled or when a placeholder logger is required.
   *
   * By using `null_log_`, no output will be generated, and no resources will be consumed
   * for formatting or storing log entries. This makes it an efficient logging sink
   * for situations where logging is unnecessary.
   *
   * @note Intended for use when a logger is required for compatibility reasons,
   *       but actual logging is not desired.
   */
  static null_logger_ null_log_;

};
}
