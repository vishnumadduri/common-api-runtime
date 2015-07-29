/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core 3.1.2.v201507021046.
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#ifndef COMMONAPI_TESTS_TEST_INTERFACE_HPP_
#define COMMONAPI_TESTS_TEST_INTERFACE_HPP_




#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/Types.hpp>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace v1_0 {
namespace commonapi {
namespace tests {

class TestInterface {
public:
    virtual ~TestInterface() { }

    static inline const char* getInterface();
    static inline CommonAPI::Version getInterfaceVersion();
};

const char* TestInterface::getInterface() {
    return ("commonapi.tests.TestInterface");
}

CommonAPI::Version TestInterface::getInterfaceVersion() {
    return CommonAPI::Version(1, 0);
}


} // namespace tests
} // namespace commonapi
} // namespace v1_0

namespace CommonAPI {
}

#endif // COMMONAPI_TESTS_TEST_INTERFACE_HPP_