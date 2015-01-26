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
 * The binding-specific deployment parameters should be
 * defined like this:
 *
 * struct BindingUInt16Deployment : CommonAPI::Deployment<> {
 * 		// Binding-specific bool deployment parameters
 * };
 *
 * struct BindingStringDeployment : CommonAPI::Deployment<> {
 * 		// Binding-specific String deployment parameters
 * };
 *
 * template<typename... _Types>
 * struct BindingStructDeployment
 * 			: CommonAPI::Deployment<_Types...> {
 * 		BindingStructDeployment(<SPECIFIC PARAMETERS>, _Types... t)
 * 			: CommonAPI::Deployment<_Types...>(t),
 * 			  <SPECIFIC INITIALIZERS> {};
 *
 * 		// Binding-specific struct deployment parameters
 * };
 *
 * The generated code needs to use these definitions to
 * provide the deployment informations for the actual data.
 * E.g., for struct consisting of a boolean and a string
 * value, it needs to generate:
 *
 * CommonAPI::BindingStructDeployment<
 *     CommonAPI::BindingBoolDeployment,
 *     CommonAPI::BindingStringDeployment
 *  > itsDeployment(<PARAMS);
 */
template<typename... _Types>
struct Deployment {
	Deployment(_Types... _values) : values_(_values...) {}

	std::tuple<_Types...> values_;
};

template<typename _TypeDepl, typename... _Types>
struct VariantDeployment : Deployment<_Types...> {
	VariantDeployment(_TypeDepl *_typeDepl, _Types... _values)
		: Deployment<_Types...>(_values...),
		  typeDepl_(_typeDepl) {
	}

	_TypeDepl *typeDepl_;
};

template<typename _ElementDepl>
struct ArrayDeployment {
	ArrayDeployment(const _ElementDepl *_elementDepl)
		: elementDepl_(_elementDepl) {}

	_ElementDepl *elementDepl_;
};

template<typename _KeyDepl, typename _ValueDepl>
struct MapDeployment {
	MapDeployment(const _KeyDepl *_key, const _ValueDepl *_value)
		: key_(_key), value_(_value) {}

	_KeyDepl *key_;
	_ValueDepl *value_;
};

/*
 * Convenience definition of an empty deployment.
 */
typedef Deployment<> EmptyDeployment;

} // namespace CommonAPI

#endif // COMMONAPI_DEPLOYABLE_H_