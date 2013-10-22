/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef COMMONAPI_TYPES_H_
#define COMMONAPI_TYPES_H_

#include <cstdint>


#ifdef WIN32
#define CCALL __cdecl
#pragma section(".CRT$XCU",read)
#define INITIALIZER(f) \
	static void __cdecl f(void); \
	__declspec(allocate(".CRT$XCU")) void(__cdecl*f##_)(void) = f; \
	static void __cdecl f(void)
#else
#define CCALL
#define INITIALIZER(f) \
	static void f(void) __attribute__((constructor)); \
	static void f(void)
#endif


#ifdef WIN32
#define usleep(micSec) \
	std::this_thread::sleep_for(std::chrono::microseconds(micSec))
#endif

namespace CommonAPI {

enum class AvailabilityStatus {
	UNKNOWN,
	AVAILABLE,
	NOT_AVAILABLE
};


enum class CallStatus {
	SUCCESS,
	OUT_OF_MEMORY,
	NOT_AVAILABLE,
	CONNECTION_FAILED,
	REMOTE_ERROR
};


struct Version {
	Version() = default;

	Version(const uint32_t& majorValue, const uint32_t& minorValue):
			Major(majorValue),
			Minor(minorValue) { }

	uint32_t Major;
	uint32_t Minor;
};


} // namespace CommonAPI

#endif // COMMONAPI_TYPES_H_
