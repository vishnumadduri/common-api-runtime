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
/*
 * Binding-specific deployment structures should inherit
 * this struct and add the specific deployment parameters.
 * The "values_"-tuple contains the deployments for nested
 * elements in structures or arrays.
 *
 * Thus, a deployment struct for a structure containing
 * an UInt16 and a String value:
 *
 * struct SampleUInt16Deployment : CommonAPI::Deployment<> {
 * 		bool isBigEndian;
 * };
 *
 * struct SampleStringDeployment : CommonAPI::Deployment<> {
 * 		Encoding encoding;
 * };
 *
 * struct SampleStructDeployment
 * 			: CommonAPI::Deployment<
 * 				SampleUInt16Deployment,
 * 				SampleStringDeployment
 * 			  > {
 * 		Layout layout_;
 * };
 */
template<typename... _Types>
struct Deployment {
	std::tuple<_Types...> values_;
};

/*
 * Convenience definition of an empty deployment.
 */
typedef Deployment<> EmptyDeployment;

} // namespace CommonAPI

#endif // COMMONAPI_DEPLOYABLE_H_
