#ifndef __ARTEMIS_API_LOGGING_HXX__
#define __ARTEMIS_API_LOGGING_HXX__

#include <memory>		// std::unique_ptr
#include <iostream>		// std::ostream
#include <fstream>		// std::ofstream
#include <string_view>	// std::string_view
#include <optional>		// std::optional
#include <chrono>		// std::chrono::system_clock::time_point
#include <functional>	// std::function

#include "Definitions.hxx"
#include "Console.hxx"	// console_color

namespace Artemis::API {
	/**
	 * @brief Represents the severity of a logging event.
	 */
	struct log_severity {
		enum {
			info,
			success,
			warning,
			error
		} _Value;

		/**
		 * @brief Constructs a log severity with a value.
		 * @param[in] _Value The instance value.
		 */
		constexpr log_severity(decltype(_Value) _Value) : _Value(_Value) {}

		/**
		 * @brief Gets the string representation of a log severity.
		 * @return A string representation of the contained value.
		 */
		ARTEMIS_API std::string_view str() const noexcept;

		/**
		 * @brief Gets the color associated with the log severity.
		 * @return The color associated with the instance value.
		 */
		ARTEMIS_API console_color color() const noexcept;
	};

	/**
	 * @brief Represents a logger that can log to standard out and a file.
	 */
	class logger {
		std::ostream* _ConsoleStream;
		std::unique_ptr<std::ofstream> _FileStream;

		std::function<std::optional<std::string_view>()> _FetchSenderCallback;
		std::string _Sender;

		bool _WithTime;
		bool _WithColor;

		ARTEMIS_API static std::unique_ptr<std::ofstream> make_filestream(std::string_view _FileName);

		logger(nullptr_t) noexcept;

	protected:
		/**
		 * @brief Outputs the message to the relevant devices.
		 * @param[in] _Message The output message.
		 */
		ARTEMIS_API void print(std::string_view _Message) const noexcept;

		/**
		 * @brief Sends a formatted output to the print function.
		 * The standard implemented format is `[_Time] [_Sender/_LogSeverity] _Message`.
		 * This function is used by the implementations of info, success, warning error and operator(), and can be overridden.
		 * @param[in] _Time An optional object representing the time of the log.
		 * @param[in] _Sender An optional object representing the sender name.
		 * @param[in] _LogSeverity A string representing the severity of the log.
		 * @param[in] _SeverityColor An optional object representing the color associated with the severity.
		 * @param[in] _Message The message to log.
		 * @throws invalid_state_exception
		 * @throws win32_exception
		 */
		ARTEMIS_API virtual void log(
			std::optional<std::chrono::system_clock::time_point>&& _Time,
			std::optional<std::string_view>&& _Sender,
			std::string_view _LogSeverity,
			std::optional<console_color>&& _SeverityColor,
			std::string_view _Message
		) const;

	public:
		logger();
		logger(const logger&)				= delete;
		logger(logger&&)					= default;

		/**
		 * @brief Logs a message with the info severity.
		 * @param[in] _Message The message to log.
		 * @throws invalid_state_exception
		 * @throws win32_exception
		 */
		ARTEMIS_API void info(std::string_view _Message) const;

		/**
		 * @brief Logs a message with the success severity.
		 * @param[in] _Message The message to log.
		 * @throws invalid_state_exception
		 * @throws win32_exception
		 */
		ARTEMIS_API void success(std::string_view _Message) const;

		/**
		 * @brief Logs a message with the warning severity.
		 * @param[in] _Message The message to log.
		 * @throws invalid_state_exception
		 * @throws win32_exception
		 */
		ARTEMIS_API void warning(std::string_view _Message) const;

		/**
		 * @brief Logs a message with the error severity.
		 * @param[in] _Message The message to log.
		 * @throws invalid_state_exception
		 * @throws win32_exception
		 */
		ARTEMIS_API void error(std::string_view _Message) const;

		/**
		 * @brief Sets a callback function that gets the sender string.
		 * The result of this function will override the default sender.
		 * @param[in] _Callback A callback function to fetch the log sender.
		 */
		ARTEMIS_API void set_sender_fetch_callback(std::function<std::optional<std::string_view>()> _Callback) noexcept;

		/**
		 * @brief Resets a previously registered sender callback.
		 */
		ARTEMIS_API void reset_sender_fetch_callback() noexcept;

		/**
		 * @brief Checks whether the current instance has a registered sender fetch callback.
		 * @return `true` if the current instance has a sender fetch callback function, otherwise `false`.
		 */
		ARTEMIS_API bool has_sender_fetch_callback() noexcept;

		/**
		 * @brief Sets the current instance sender.
		 * @param[in] _Sender The sender string.
		 */
		ARTEMIS_API void set_sender(std::string_view _Sender) noexcept;

		/**
		 * @brief Logs the message with the provided severity.
		 * @param[in] _Severity The log severity.
		 * @param[in] _Message The message to log.
		 * @throws invalid_state_exception
		 * @throws win32_exception
		 */
		ARTEMIS_API void operator()(log_severity _Severity, std::string_view _Message) const;

		logger& operator=(const logger&)	= delete;
		logger& operator=(logger&&)			= default;

		friend class logger_factory;
	};

	class logger_factory {
		bool _LogToConsole;
		bool _WithColor;

		bool _LogToFile;
		std::string_view _FileName;

		bool _WithTime;

	public:
		ARTEMIS_API logger_factory();

		/**
		 * @brief Enables the produced logger to log to the standard output stream.
		 * @return A reference to the current instance.
		 */
		ARTEMIS_API logger_factory& with_console_logging() noexcept;

		/**
		 * @brief Enables the produced logger to log to a file of the specified name.
		 * @param[in] _FileName The name of the file to create for logging.
		 * @return A reference to the current instance.
		 */
		ARTEMIS_API logger_factory& with_file_logging(std::string_view _FileName) noexcept;

		/**
		 * @brief Enables timestamps on logs with the produced logger.
		 * @return A reference to the current instance.
		 */
		ARTEMIS_API logger_factory& with_time() noexcept;

		/**
		 * @brief Enables the produced logger to use colors on the console logs.
		 * @return 
		 */
		ARTEMIS_API logger_factory& with_color() noexcept;

		/**
		 * @brief Constructs an instance of a logger based on this factory's settings.
		 * @return 
		 */
		ARTEMIS_API logger create() const;
	};

	/**
	 * @brief Enables a class to have a logger set for optional internal use.
	 */
	class loggable {
	protected:
		/**
		 * @brief A pointer to a logger instance. This pointer may be null.
		 */
		logger* Log;

	public:
		/**
		 * @brief Sets a logger instance to be used by this class.
		 * @param[in] _Logger A pointer to the instance to set.
		 */
		ARTEMIS_API void set_instance_logger(logger* _Logger) noexcept;
	};
}

#endif // !__ARTEMIS_API_LOGGING_HXX__