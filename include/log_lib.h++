#pragma once

#include <filesystem>
#include <shared_mutex>
#include <unordered_map>

namespace nutsloop {

const std::filesystem::path nutsloop_dir = std::filesystem::path(std::getenv("HOME")) / ".nutsloop";
const std::filesystem::path nutsloop_logs_directory = nutsloop_dir / "logs";

class log {
public:

  /**
   * Sets up the logger with a unique identifier, log file name, and an
   * optional flag determining whether logging is enabled or disabled.
   * If logging is enabled, it ensures that the specified log file exists,
   * initializes stream redirections for `std::cout` and `std::cerr` to the log file,
   * and prepares to write output to the log file.
   *
   * This function creates a hierarchical directory structure for storing logs,
   * retains original stream buffers, and appends new log entries to the given
   * file to preserve existing logs. Each session is marked with a specific header.
   *
   * If logging is set to disabled, the function exits immediately.
   *
   * @param ident A unique identifier for the logger instance.
   * @param filename The name of the log file where logs will be written.
   * @param enabled Flag to indicate whether logging is enabled. Defaults to true.
   *
   * @note This function is thread-safe and uses a locking mechanism
   *       to prevent multiple threads from interfering with resource setup.
   * @note Redirects `std::cout` and `std::cerr` streams to the specified log file.
   *       These streams will automatically flush after each output.
   * @throws An error message is logged if the specified log file cannot be opened.
   */
  static void setup( const std::string& ident, const std::string& filename, bool enabled = true );

  /**
   * Provides a logging stream associated with a specific identifier.
   *
   * This method outputs log messages to a logging stream associated with a provided identifier
   * and writes additional debugging information, such as file name, line number, and log level.
   *
   * - If the logging system is disabled, a null stream is returned.
   * - If the identifier does not correspond to an open log file, a null stream is returned.
   *
   * @param ident The identifier for the logging stream. Typically used to group logs by context or module.
   * @param file The name of the source file generating the log message.
   * @param line The line number in the source file generating the log message.
   * @param level The log level for the message, represented as a single character (e.g., 'I' for INFO, 'W' for WARN,
   * 'E' for ERROR). Defaults to 'I' if not specified.
   * @return A reference to a logging output stream associated with the identifier, or a null stream if logging is
   * disabled or the identifier is not valid.
   */
  static std::ostream& stream( const char* ident, const char* file, int line, char level = 'I' );

  /**
   * Releases resources used by the logger system, including closing all log files
   * and restoring the original standard output (std::cout) and error (std::cerr) streams.
   *
   * If logging is disabled, this function returns immediately without taking any action.
   *
   * This method should be called before program termination to ensure that all
   * allocated resources associated with the logger are properly cleaned up.
   */
  static void cleanup();

  /**
   * Sets the logging state for the logger system by enabling or disabling it.
   * When disabled, logging functionality is completely turned off.
   *
   * @param disable A boolean flag indicating whether to disable logging.
   *                Pass `true` to disable logging, and `false` to enable it.
   */
  static void set_logs( const bool disable ) { logs_disabled_ = disable; }
private:

  static std::shared_mutex log_mtx_;
  static bool logs_disabled_;

  /**
   * Generates a shortened version of the provided file system path.
   * This function modifies the input path to create a more concise representation,
   * which may be useful for log messages, user interfaces, or other scenarios
   * where a full path might be too verbose.
   *
   * The method preserves critical components of the path while abbreviating
   * less significant segments. The specific logic for shortening depends on
   * implemented rules and heuristics.
   *
   * @param dir The original path to be shortened, represented as a `std::filesystem::path`.
   *
   * @return A string containing the shortened version of the provided path.
   */
  static std::string shortened_path_(const std::filesystem::path &dir);

  /**
   * Creates a formatted session header marker string for a log file, indicating
   * the start of a new session and including the current timestamp.
   *
   * The header consists of a line with the provided filename and a time-stamp
   * formatted as "YYYY-MM-DD HH:MM:SS". The marker is designed to visually
   * separate log messages from different sessions within the same log file.
   *
   * @param filename The name of the log file for which the session header marker is being generated.
   *                 It is included in the marker for clarity and reference.
   * @return A formatted string representing the session header marker. This will include the provided
   *         filename and the current local time.
   */
  static std::string generate_new_session_header_( const std::string& filename );

  static std::unique_ptr<std::unordered_map<std::string, std::tuple<bool, std::ofstream>>> log_files_;

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
    stream_redirect_(std::ostream& stream_out, std::ostream& stream_err, std::streambuf* log_file_buffer) :
      original_cout_(stream_out.rdbuf()),
      original_cerr_(stream_err.rdbuf()) {

      stream_out.rdbuf(log_file_buffer);
      stream_err.rdbuf(log_file_buffer);
    }

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
   * This class is intended for use cases where logging functionality must exist,
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
   * A static global member of type `null_logger_` that serves as a no-op logger.
   *
   * This member is designed to silently discard any log messages sent to it. It is useful
   * in scenarios where logging needs to be disabled or when a placeholder logger is required.
   *
   * By using `null_log_`, no output will be generated, and no resources will be consumed
   * for formatting or storing log entries. This makes it an efficient logging sink
   * for situations where logging is unnecessary.
   *
   * @note Intended for use when a logger is required for compatibility reasons
   *       but actual logging is not desired.
   */
  static null_logger_ null_log_;
};
}
