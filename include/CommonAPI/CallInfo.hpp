// Copyright (C) 2015 BMW Group
// Author: Lutz Bichler (lutz.bichler@bmw.de)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef COMMONAPI_CALLINFO_HPP_
#define COMMONAPI_CALLINFO_HPP_

#include <CommonAPI/Types.hpp>

namespace CommonAPI {

struct COMMONAPI_EXPORT CallInfo {
	CallInfo() = default;
	CallInfo(Timeout_t _timeout)
		: status_(CallStatus::UNKNOWN), timeout_(_timeout), sender_(0) {
	}
	CallInfo(Sender_t _sender)
		: status_(CallStatus::UNKNOWN), timeout_(DEFAULT_TIMEOUT_MS), sender(_sender) {
	}
	CallInfo(Timeout_t _timeout, Sender_t _sender)
		: status_(CallStatus::UNKNOWN), timeout_(_timeout), sender(_sender) {
	}

	CallStatus status_;
	Timeout_t timeout_;
	Sender_t sender_;
};

} // namespace CommonAPI

#endif // COMMONAPI_ADDRESS_HPP_
