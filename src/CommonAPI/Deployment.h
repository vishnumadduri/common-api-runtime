/* Copyright (C) 2014 BMW Group
 * Author: Lutz Bichler (lutz.bichler@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_DEPLOYMENT_H_
#define COMMONAPI_DEPLOYMENT_H_

#include <tuple>

namespace CommonAPI {

template<typename... _Types>
struct Deployment {
	Deployment(_Types... _t) : members_(_t...) {};
	std::tuple<_Types...> members_;
};

typedef Deployment<> EmptyDeployment;

} // namespace CommonAPI

#endif // COMMONAPI_DEPLOYABLE_H_
