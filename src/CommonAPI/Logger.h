/* Copyright (C) 2015 BMW Group
 * Author: Lutz Bichler (lutz.bichler@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef COMMONAPI_LOGGER_H_
#define COMMONAPI_LOGGER_H_

#include <iostream>
#include <memory>

namespace CommonAPI {

class Logger {
public:
	template<typename... _LogEntries>
	static void log(_LogEntries... _entries) {
		std::cout << "[CAPI] ";
		Logger::get()->log_intern(_entries...);
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

};

} /* namespace CommonAPI */

#endif /* COMMONAPI_LOGGER_H_ */
