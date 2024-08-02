#include "pch.h"
#include "Logging.hxx"
#include "Error.hxx"
#include "Console.hxx"

namespace Artemis::API {
	std::string_view log_severity::str() const noexcept {
		switch (_Value) {
		case info:
			return "INFO";
		case success:
			return "SUCCESS";
		case warning:
			return "WARNING";
		case error:
			return "ERROR";
		default:
			return "UNKNOWN";
		}
	}

	console_color log_severity::color() const noexcept {
		switch (_Value) {
		case info:
			return console_color::blue;
		case success:
			return console_color::green;
		case warning:
			return console_color::yellow;
		case error:
			return console_color::red;
		default:
			return console_color::gray;
		}
	}

	std::unique_ptr<std::ofstream> logger::make_filestream(const std::string_view& _FileName) {
		std::unique_ptr<std::ofstream> file = std::make_unique<std::ofstream>();
		file->rdbuf()->pubsetbuf(nullptr, 0); // Disable buffering to not lose log data in case of game crash.
		file->open(_FileName.data(), std::ios::out | std::ios::trunc);
	}

	void logger::print(const std::string_view& _Message) const noexcept {
		if (_ConsoleStream)
			(*_ConsoleStream) << _Message;
		if (_FileStream)
			(*_FileStream) << _Message;
	}

	void logger::log(
		std::optional<std::chrono::system_clock::time_point> _Time,
		const std::string_view& _LogSeverity,
		std::optional<console_color> _SeverityColor,
		const std::string_view& _Message
	) const {
		__stack_record();

		if (_Time.has_value())
			print(std::format("[{:%T}] ", _Time.value()));

		if (_SeverityColor.has_value())
			__stack_rethrow(set_console_foreground_color(_SeverityColor.value()));

		print(std::format("[{}] ", _LogSeverity));

		if (_SeverityColor.has_value())
			__stack_rethrow(set_console_foreground_color(console_color::gray));

		print(std::format("{}\n", _Message));

		__stack_escape();
	}

	logger::logger(nullptr_t) noexcept : _ConsoleStream(nullptr), _FileStream(nullptr), _WithTime(false), _WithColor(false) {}

	logger::logger() {
		_ConsoleStream = &std::cout;

		// Prepare path:	%LOCALAPPDATA%\artemis.log
		//					Stored in AppData\Local to avoid performance
		//					penalties from disabling buffering on hard drives.

		char path[MAX_PATH];
		GetEnvironmentVariableA("LOCALAPPDATA", path, MAX_PATH);
		strcat_s(path, "\\artemis.log");

		_FileStream = make_filestream(path);
	}

	// These four functions do not follow the rule of stack record and rethrow,
	// but this is for performance reasons.

	void logger::info(const std::string_view& _Message) const { this->operator()(log_severity::info, _Message); }

	void logger::success(const std::string_view& _Message) const { this->operator()(log_severity::success, _Message); }

	void logger::warning(const std::string_view& _Message) const { this->operator()(log_severity::warning, _Message); }

	void logger::error(const std::string_view& _Message) const { this->operator()(log_severity::error, _Message); }

	void logger::operator()(log_severity _Severity, const std::string_view& _Message) const {
		__stack_record();

		std::optional<std::chrono::system_clock::time_point> time = std::nullopt;
		if (_WithTime)
			time = std::chrono::system_clock::now();

		std::optional<console_color> color = std::nullopt;
		if (_WithColor)
			color = _Severity.color();

		__stack_rethrow(this->log(
			time,
			_Severity.str(),
			color,
			_Message
		));

		__stack_escape();
	}

	logger_factory::logger_factory() :
		_LogToConsole(false),
		_WithColor(false),
		_LogToFile(false),
		_FileName(""),
		_WithTime(false) {}

	logger_factory& logger_factory::with_console_logging() { this->_LogToConsole = true; }
	logger_factory& logger_factory::with_file_logging(const std::string_view& _FileName) { this->_LogToFile = true; this->_FileName = _FileName; }
	logger_factory& logger_factory::with_time() { this->_WithTime = true; }
	logger_factory& logger_factory::with_color() { this->_WithColor = true; }

	logger logger_factory::create() const {
		logger logger(nullptr);
		if (this->_LogToConsole)
			logger._ConsoleStream = &std::cout;
		if (this->_LogToFile)
			logger._FileStream = logger::make_filestream(this->_FileName);
	}
}