// Copyright (C) 2015 BMW Group
// Author: Lutz Bichler (lutz.bichler@bmw.de)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef COMMONAPI_LOGGER_HPP_
#define COMMONAPI_LOGGER_HPP_

#include <cstdint>
#include <iostream>
#include <memory>

#define COMMONAPI_LOGLEVEL_FATAL	0
#define COMMONAPI_LOGLEVEL_ERROR	1
#define COMMONAPI_LOGLEVEL_WARNING	2
#define COMMONAPI_LOGLEVEL_INFO		3
#define COMMONAPI_LOGLEVEL_DEBUG	4
#define COMMONAPI_LOGLEVEL_VERBOSE	5

#ifndef COMMONAPI_LOGLEVEL
#define COMMONAPI_LOGLEVEL COMMONAPI_LOGLEVEL_INFO
#endif

#define COMMONAPI_FATAL(params...) \
	do { Logger::log(Logger::Level::FATAL, params); } while (false);

#if COMMONAPI_LOGLEVEL >= COMMONAPI_LOGLEVEL_ERROR
	#define COMMONAPI_ERROR(params...) \
		do { Logger::log(Logger::Level::ERROR, params); } while (false);
#else
	#define COMMONAPI_ERROR(params...)
#endif

#if COMMONAPI_LOGLEVEL >= COMMONAPI_LOGLEVEL_WARNING
	#define COMMONAPI_WARNING(params...) \
		do { Logger::log(Logger::Level::WARNING, params); } while (false);
#else
	#define COMMONAPI_WARNING(params...)
#endif

#if COMMONAPI_LOGLEVEL >= COMMONAPI_LOGLEVEL_INFO
	#define COMMONAPI_INFO(params...) \
		do { Logger::log(Logger::Level::INFO, params); } while (false);
#else
	#define COMMONAPI_INFO(params...)
#endif

#if COMMONAPI_LOGLEVEL >= COMMONAPI_LOGLEVEL_DEBUG
	#define COMMONAPI_DEBUG(params...) \
		do { Logger::log(Logger::Level::DEBUG, params); } while (false);
#else
	#define COMMONAPI_DEBUG(params...)
#endif

#if COMMONAPI_LOGLEVEL >= COMMONAPI_LOGLEVEL_VERBOSE
	#define COMMONAPI_VERBOSE(params...) \
		do { Logger::log(Logger::Level::VERBOSE, params); } while (false);
#else
	#define COMMONAPI_VERBOSE(params...)
#endif

namespace CommonAPI {

class Logger {
public:
	enum class Level : uint8_t {
		FATAL = 0, ERROR = 1, WARNING = 2, INFO = 3, DEBUG = 4, VERBOSE = 5
	};

	Logger();

	template<typename... _LogEntries>
	static void log(Level _level, _LogEntries... _entries) {
		if (Logger::get()->useConsole_) {
			std::cout << "[CAPI " << Logger::get()->level_as_string(_level) << "] ";
			Logger::get()->log_intern(_entries...);
		}
	}

private:
	static inline std::shared_ptr<Logger> get() {
		static std::shared_ptr<Logger> theLogger = std::make_shared<Logger>();
		return theLogger;
	}

	void log_intern() {
		std::cout << std::endl;
	};

	template<typename _LogEntry, typename... _MoreLogEntries>
	void log_intern(_LogEntry _entry, _MoreLogEntries... _moreEntries) {
		std::cout << _entry;
		log_intern(_moreEntries...);
	}

	std::string level_as_string(Level _level) {
		switch (_level) {
		case Level::FATAL:
			return "FATAL";
		case Level::ERROR:
			return "ERROR";
		case Level::WARNING:
			return "WARNING";
		case Level::INFO:
			return "INFO";
		case Level::DEBUG:
			return "DEBUG";
		case Level::VERBOSE:
			return "VERBOSE";
		default:
			return "";
		}
	}

	bool useConsole_;
	bool useFile_;
	bool useDlt_;
	Level maximumLogLevel_;
};

} // namespace CommonAPI

#endif // COMMONAPI_LOGGER_HPP_
