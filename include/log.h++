#pragma once
#include "log/internal_debug.h++"

#if DEBUG_LOG == false
#include "types.h++"
#include "util/level.h++"
#endif

#include <atomic>
#include <shared_mutex>

// MARK: (LOG) experimental instance class pre-processor
#ifndef EXPERIMENTAL_INSTANCE
// HINT: default value if not defined elsewhere
#define EXPERIMENTAL_INSTANCE false
#endif

#if EXPERIMENTAL_INSTANCE == true

#warning "EXPERIMENTAL_INSTANCE is enabled"

#include "log/instance.h++"
#endif

constexpr size_t LOG_MAX_SIZE = 10 * 1024 * 1024;

namespace nutsloop {

using namespace nlog::types;

class log {
public:
  // Prevents the instantiation of the class.
  // The class is designed to have only static members.
  log() = delete;
  // Deleting the destructor reinforces the intent
  // that this class should never be instantiated or destroyed.
  ~log() = delete;

  /**
   * Activates the logging system and prepares necessary resources for logging.
   *
   * This method ensures the logging system is in an active state, allowing log
   * entries to be recorded. If already active, it updates the active status.
   * Additionally, it sets up the log registry if it is not already initialized.
   * Internal debug information is logged when the DEBUG flag is enabled.
   * Thread-safe locking mechanisms are used during the activation process.
   *
   * @note This method is intended to be called before logging any entries to
   * guarantee the logging system is operational. The activation status is
   * tracked, and a debug stream logs updates if logging in debug mode is
   * enabled.
   */
  static void activate();
  /**
   * Deactivates the logging system and releases its active state.
   *
   * This method transitions the logging system to an inactive state, ensuring
   * that no further log entries can be recorded until reactivation. If the
   * system was previously active, the change is logged in the internal debug
   * stream, provided debugging is enabled. Thread-safe mechanisms are employed
   * to ensure safe updates to the active status.
   *
   * @note This method is primarily used when logging functionality is no longer
   * needed or before shutting down the application. Any logs generated after
   * deactivation will not be recorded until reactivation.
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
  static void set(log_settings_t &settings);
  /**
   * Initiates the logging process for the specified log identifier.
   *
   * This function sets the 'running' status of the identified log to active if
   * the log exists in the registry. It ensures that the log is initialized and
   * ready for receiving entries. Debug information regarding the state of the
   * log is reported if debugging is enabled.
   *
   * If the provided identifier is not found in the log registry, the function
   * logs an error message in debug mode and exits without altering any state.
   * Thread-safe mechanisms, such as shared locks, are utilized to ensure
   * proper access control during the procedure.
   *
   * @param ident A string representing the unique identifier of the log
   * instance to activate. This identifier is used to find the corresponding log
   * in the registry.
   *
   * @note If the DEBUG flag is enabled, extensive debug logs are generated to
   * trace the function execution, including whether the log was previously in a
   * 'running' state, and any errors associated with missing identifiers.
   */
  static void start(const std::string &ident);
  /**
   * Stops the logging process for a given log identifier.
   *
   * This method marks the logging process associated with the provided
   * identifier as no longer running. If the log identifier does not exist in
   * the registry, the method returns without making changes. Includes debug
   * logging to provide detailed diagnostics when `DEBUG` is enabled.
   * Thread-safe locking mechanisms are employed during operations to ensure
   * safety in multi-threaded contexts.
   *
   * @param ident The identifier of the log to be stopped. If the identifier is
   * not found, no action is performed.
   */
  static void stop(const std::string &ident);
  /**
   * Flushes and resets the specified log output stream based on its identifier.
   *
   * This method ensures that the log associated with the provided identifier is
   * properly flushed and reopened with clean content. The internal logging
   * structure is thread-safe with appropriate locks during the operation.
   *
   * If the log identifier does not exist in the registry, the method logs an
   * error in debug mode and terminates without further action.
   *
   * @param ident The unique identifier of the log to be flushed. This
   * identifier must exist in the log registry for the operation to complete.
   */
  static void flush(const std::string &ident);

#if EXPERIMENTAL_INSTANCE == true
  // MARK: (LOG) experimental instance methods and fields
  static std::unique_ptr<nlog::instance> get_instance(const std::string &ident);
#endif

  // HINT: not completely implemented yet.
  static bool full_running(const std::string &ident);

  // HINT: not implemented yet
  static void close(std::string ident) = delete;
  static void remove(std::string ident) = delete;
  static void unset(std::string ident) = delete;

  /**
   * Provides an output stream to log messages for a specified identifier with
   * metadata.
   *
   * This method retrieves a logging stream associated with the given
   * identifier. The metadata such as file name, line number, and log level are
   * also added as a log entry prefix. It performs multiple checks to ensure the
   * log is active, running, and properly configured. If any of these checks
   * fail, a null stream is returned. Otherwise, a valid stream for the
   * specified identifier is provided for logging.
   *
   * Thread-safe locking mechanisms are used to ensure consistent log behavior
   * during concurrent operations. Logs debugging details when debug mode is
   * enabled.
   *
   * @param ident The unique identifier of the log to use.
   * @param file The name of the source file where the log is recorded.
   * @param line The line number in the source file where the log is recorded.
   * @param c The log level (e.g., INFO, DEBUG, ERROR, NONE) for the log entry.
   *              Defaults to INFO if not specified.
   * @return A reference to the logging output stream if successful, or a null
   * stream if conditions do not permit logging.
   */
  static std::ostream &stream(const char *ident, const char *file, int line,
                              Level c = INFO);
  static std::ostream &stream(const char *ident);

  /**
   * Activates the stream redirection within the logging system.
   *
   * This method enables the mechanism to redirect logging streams for capturing
   * or handling log outputs differently. If the stream redirection is already
   * active, its previous state is reported and updated. Debug information is
   * logged when operating in DEBUG mode. The method ensures thread safety by
   * using appropriate synchronization primitives.
   *
   * @note This functionality is critical when an alternate handling of log
   * streams is required. Debug logs provide insights into the activation state
   * changes.
   */
  static void activate_stream_redirect();
  // HINT: not implemented yet
  static void deactivate_stream_redirect();

private:
#if DEBUG_LOG == true
  // MARK: (LOG) private static log debug info file methods and fields
  static std::unique_ptr<nlog::internal_debug> internal_debug_;
  static void set_internal_debug_();
#endif

  // MARK: (LOG) private static methods and fields
  static std::shared_mutex mtx_;

  /**
   * Checks whether the logging system is currently activated.
   *
   * This method determines the activation status of the logging system,
   * utilizing an atomic flag to ensure thread-safe access. If debug mode is
   * enabled, a message with the current activation state is logged.
   *
   * @return True if the logging system is active; otherwise, false.
   */
  bool static is_activated_();
  static std::atomic<bool> activated_;

  static void set_called_();
  /**
   * // HINT: maybe find a purpose for this? :D
   * Checks if the internal 'set' function of the logging system has been
   * called.
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
   * Checks and retrieves any error message associated with the logging file of
   * a given logging context.
   *
   * This method evaluates the provided logging context file to determine if any
   * errors have occurred during logging operations. If an error is detected,
   * the corresponding error message is returned. If no error is found or the
   * input is null, an empty optional is returned.
   *
   * @param log_ident A pointer to the logging context whose file is being
   * inspected for errors.
   * @return An optional string containing the error message if an error exists,
   * or an empty optional otherwise.
   */
  static std::optional<std::string> error_on_log_file_(const log_t *log_ident);

  static void set_log_registry_();
  static bool registry_has_item_(std::string ident);
  static std::unique_ptr<log_registry_t> log_registry_;

  static log_t* set_log_(log_settings_t* settings);
  static std::unique_ptr<log_t> log_;

  /**
   * Creates a directory at the specified path using default settings.
   *
   * This method attempts to create a directory at the given path. If the
   * directory already exists, no action is performed. If the creation fails due
   * to insufficient permissions or other issues, an exception may be thrown.
   *
   * @param path The filesystem path where the directory should be created.
   */
  static void mkdir_default_(const std::filesystem::path &path);

  static std::optional<log_t *> null_stream_(const std::string &ident);

  /**
   * Generates a new session header string for logging purposes.
   *
   * This method constructs a formatted header string that includes the session
   * identifier, the current timestamp, and the associated file name, providing
   * a contextual wrapper for log entries.
   *
   * @param ident The identifier for the logging session. Typically used to
   * represent a unique session or context for the logs.
   * @param filename The name of the file being logged or associated with the
   * logging session.
   * @return A formatted string representing the session header, including the
   * identifier, timestamp, and file name.
   */
  static std::string generate_new_session_header_(const std::string &ident,
                                                  const std::string &filename);

  // MARK: (LOGGER) private classes
  /**
   * Manages the redirection of `std::ostream` streams to a new stream buffer,
   * allowing custom handling of standard output (`std::cout`) and error
   * (`std::cerr`) streams.
   *
   * When an instance of this class is created, it redirects the specified
   * output and error streams to a provided stream buffer. The original stream
   * states are preserved and automatically restored upon the destruction of the
   * instance.
   *
   * @param stream_out Reference to the output stream that will be redirected
   * (e.g., `std::cout`).
   * @param stream_err Reference to the error stream that will be redirected
   * (e.g., `std::cerr`).
   * @param log_file_buffer The new stream buffer to which the streams will be
   * redirected.
   *
   * @note Automatically restores the original stream buffers for `std::cout`
   * and `std::cerr` when the instance is destroyed.
   *
   * @warning The provided stream buffer (`log_file_buffer`) must remain valid
   * for the lifetime of the `stream_redirect_` instance. Destroying or
   * modifying the buffer while redirection is in place can lead to undefined
   * behavior.
   *
   * The destructor ensures the integrity of the streams by restoring their
   * original stream buffers, preventing any unintended stream behaviors after
   * the instance is destructed.
   */
  class stream_redirect_ {
  public:
    /**
     * Constructs a stream redirector to reroute standard output and error
     * streams to designated log files.
     *
     * The constructor initializes internal buffers and opens log files for both
     * the `stdout` and `stderr` streams. If the log files fail to open, debug
     * messages are generated in debug mode. Upon successful initialization, it
     * redirects the provided streams to the corresponding log files, ensuring
     * that subsequent output and error messages are stored in these files.
     *
     * @param stream_out The output stream (e.g., `std::cout`) to be redirected.
     * @param stream_err The error stream (e.g., `std::cerr`) to be redirected.
     *
     * @return No return value, as this is a constructor.
     */
    stream_redirect_(std::ostream &stream_out, std::ostream &stream_err);

    /**
     * Destructor for the stream_redirect_ class, responsible for cleaning up
     * and restoring the redirection of standard output and error streams.
     *
     * This method closes any open files used for redirection and restores the
     * original stream buffers for both std::cout and std::cerr. Ensures proper
     * resource management and reverts the output streams to their original
     * state upon object destruction.
     *
     * It is implicitly invoked when an instance of stream_redirect_ goes out of
     * scope or is explicitly deleted.
     *
     * @note Proper cleanup of redirected streams is essential to prevent
     * unexpected behavior or resource leaks. This method is thread-safe for
     * sequential use, but the caller must ensure safe usage in a multithreaded
     * context.
     */
    ~stream_redirect_();

  private:
    std::streambuf *original_cout_;
    std::streambuf *original_cerr_;
    std::ofstream cout_file_;
    std::ofstream cerr_file_;
  };
  static std::unique_ptr<stream_redirect_> stream_redirect_pointer_;
  /**
   * Checks whether the stream redirection is currently active.
   *
   * This method verifies the status of stream redirection and determines
   * if it is enabled. If debugging is enabled, it logs the status
   * to the internal debug stream for diagnostic purposes.
   * The status is tracked using an atomic variable for thread safety.
   *
   * @return True if stream redirection is active, false otherwise.
   */
  static bool is_stream_redirect_active_();
  static std::atomic<bool> stream_redirect_active_;

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
    int overflow(const int input) override { return input; }
  };

  /**
   * A specialized logger class derived from `std::ostream` that performs no-op
   * operations for all output streams redirected to it.
   *
   * This class is intended for use in cases where logging functionality must
   * exist, but actual output is not required. It effectively discards any input
   * given to it.
   *
   * The `null_logger_` leverages an internal `null_buffer_` that implements a
   * stream buffer which discards all written data.
   *
   * @note Designed for scenarios where a placeholder logger is needed. It
   * prevents unnecessary overhead caused by output redirection while
   * maintaining interface compliance with standard logging practices.
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
     * with a `null_buffer_`, effectively ensuring that all output written to
     * this stream is redirected to a sink that does nothing.
     *
     * @return An instance of `null_logger_` configured to silently discard any
     * output.
     */
    null_logger_() : std::ostream(&null_buffer_) {}

  private:
    null_buffer_ null_buffer_;
  };
  /**
   * A static global member of `null_logger_` type that serves as a no-op
   * logger.
   *
   * This member is designed to silently discard any log messages sent to it. It
   * is useful in scenarios where logging needs to be disabled or when a
   * placeholder logger is required.
   *
   * By using `null_log_`, no output will be generated, and no resources will be
   * consumed for formatting or storing log entries. This makes it an efficient
   * logging sink for situations where logging is unnecessary.
   *
   * @note Intended for use when a logger is required for compatibility reasons,
   *       but actual logging is not desired.
   */
  static null_logger_ null_log_;
};
} // namespace nutsloop
