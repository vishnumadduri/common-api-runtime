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

#ifdef WIN32
#define COMMONAPI_FATAL(...) \
	do { Logger::log(Logger::Level::LL_FATAL, __VA_ARGS__); } while (false);

#if COMMONAPI_LOGLEVEL >= COMMONAPI_LOGLEVEL_ERROR
#define COMMONAPI_ERROR(...) \
		do { Logger::log(Logger::Level::LL_ERROR, __VA_ARGS__); } while (false);
#else
#define COMMONAPI_ERROR(...)
#endif

#if COMMONAPI_LOGLEVEL >= COMMONAPI_LOGLEVEL_WARNING
#define COMMONAPI_WARNING(...) \
		do { Logger::log(Logger::Level::LL_WARNING, __VA_ARGS__); } while (false);
#else
#define COMMONAPI_WARNING(...)
#endif

#if COMMONAPI_LOGLEVEL >= COMMONAPI_LOGLEVEL_INFO
#define COMMONAPI_INFO(...) \
		do { Logger::log(Logger::Level::LL_INFO, __VA_ARGS__); } while (false);
#else
#define COMMONAPI_INFO(...)
#endif

#if COMMONAPI_LOGLEVEL >= COMMONAPI_LOGLEVEL_DEBUG
#define COMMONAPI_DEBUG(...) \
		do { Logger::log(Logger::Level::LL_DEBUG, __VA_ARGS__); } while (false);
#else
#define COMMONAPI_DEBUG(...)
#endif

#if COMMONAPI_LOGLEVEL >= COMMONAPI_LOGLEVEL_VERBOSE
#define COMMONAPI_VERBOSE(...) \
		do { Logger::log(Logger::Level::LL_VERBOSE, __VA_ARGS__); } while (false);
#else
#define COMMONAPI_VERBOSE(...)
#endif
#else
#define COMMONAPI_FATAL(params...) \
	do { Logger::log(Logger::Level::LL_FATAL, params); } while (false);

#if COMMONAPI_LOGLEVEL >= COMMONAPI_LOGLEVEL_ERROR
	#define COMMONAPI_ERROR(params...) \
		do { Logger::log(Logger::Level::LL_ERROR, params); } while (false);
#else
	#define COMMONAPI_ERROR(params...)
#endif

#if COMMONAPI_LOGLEVEL >= COMMONAPI_LOGLEVEL_WARNING
	#define COMMONAPI_WARNING(params...) \
		do { Logger::log(Logger::Level::LL_WARNING, params); } while (false);
#else
	#define COMMONAPI_WARNING(params...)
#endif

#if COMMONAPI_LOGLEVEL >= COMMONAPI_LOGLEVEL_INFO
	#define COMMONAPI_INFO(params...) \
		do { Logger::log(Logger::Level::LL_INFO, params); } while (false);
#else
	#define COMMONAPI_INFO(params...)
#endif

#if COMMONAPI_LOGLEVEL >= COMMONAPI_LOGLEVEL_DEBUG
	#define COMMONAPI_DEBUG(params...) \
		do { Logger::log(Logger::Level::LL_DEBUG, params); } while (false);
#else
	#define COMMONAPI_DEBUG(params...)
#endif

#if COMMONAPI_LOGLEVEL >= COMMONAPI_LOGLEVEL_VERBOSE
	#define COMMONAPI_VERBOSE(params...) \
		do { Logger::log(Logger::Level::LL_VERBOSE, params); } while (false);
#else
	#define COMMONAPI_VERBOSE(params...)
#endif
#endif
namespace CommonAPI {

class Logger {
public:
	enum class Level : uint8_t {
		LL_FATAL = 0, LL_ERROR = 1, LL_WARNING = 2, LL_INFO = 3, LL_DEBUG = 4, LL_VERBOSE = 5
	};

	Logger() :
		useConsole_(true),
		useFile_(false),
		useDlt_(false),
		maximumLogLevel_(static_cast<Level>(COMMONAPI_LOGLEVEL)) {
	}

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
	}

	template<typename _LogEntry, typename... _MoreLogEntries>
	void log_intern(_LogEntry _entry, _MoreLogEntries... _moreEntries) {
		std::cout << _entry;
		log_intern(_moreEntries...);
	}

	std::string level_as_string(Level _level) {
		switch (_level) {
		case Level::LL_FATAL:
			return "FATAL";
		case Level::LL_ERROR:
			return "ERROR";
		case Level::LL_WARNING:
			return "WARNING";
		case Level::LL_INFO:
			return "INFO";
		case Level::LL_DEBUG:
			return "DEBUG";
		case Level::LL_VERBOSE:
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
