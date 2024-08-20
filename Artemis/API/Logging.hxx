#ifndef ARTEMIS_API_LOGGING_HXX
#define ARTEMIS_API_LOGGING_HXX

#include <memory>		// std::unique_ptr
#include <iostream>		// std::ostream
#include <fstream>		// std::ofstream
#include <string_view>	// std::string_view
#include <optional>		// std::optional
#include <chrono>		// std::chrono::system_clock::time_point

#include "Definitions.hxx"
#include "Console.hxx"	// console_color

namespace Artemis::API {
	struct log_severity {
		enum {
			info,
			success,
			warning,
			error
		} _Value;

		inline log_severity(decltype(_Value) _Value) : _Value(_Value) {}

		ARTEMIS_API std::string_view str() const noexcept;
		ARTEMIS_API console_color color() const noexcept;
	};

	class logger {
		std::ostream* _ConsoleStream;
		std::unique_ptr<std::ofstream> _FileStream;

		bool _WithTime;
		bool _WithColor;

		ARTEMIS_API static std::unique_ptr<std::ofstream> make_filestream(const std::string_view& _FileName);

		logger(nullptr_t) noexcept;

	protected:
		ARTEMIS_API void print(const std::string_view& _Message) const noexcept;

		ARTEMIS_API virtual void log(
			std::optional<std::chrono::system_clock::time_point> _Time,
			const std::string_view& _LogSeverity,
			std::optional<console_color> _SeverityColor,
			const std::string_view& _Message
		) const;

	public:
		logger();
		logger(const logger&)				= delete;
		logger(logger&&)					= default;

		ARTEMIS_API void info(const std::string_view& _Message) const;
		ARTEMIS_API void success(const std::string_view& _Message) const;
		ARTEMIS_API void warning(const std::string_view& _Message) const;
		ARTEMIS_API void error(const std::string_view& _Message) const;

		ARTEMIS_API void operator()(log_severity _Severity, const std::string_view& _Message) const;

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

		ARTEMIS_API logger_factory& with_console_logging();
		ARTEMIS_API logger_factory& with_file_logging(const std::string_view& _FileName);
		ARTEMIS_API logger_factory& with_time();
		ARTEMIS_API logger_factory& with_color();

		ARTEMIS_API logger create() const;
	};

	class loggable {
	protected:
		std::shared_ptr<logger> Log;

	public:
		ARTEMIS_API void set_instance_logger(std::shared_ptr<logger> _Logger) noexcept;
	};
}

#endif // !ARTEMIS_API_LOGGING_HXX