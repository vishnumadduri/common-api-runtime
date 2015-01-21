/* Copyright (C) 2013, 2014 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * Author: Lutz Bichler (lutz.bichler@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_OUTPUT_STREAM_H_
#define COMMONAPI_OUTPUT_STREAM_H_

#include "ByteBuffer.h"
#include "Deployable.h"
#include "Deployment.h"
#include "Struct.h"
#include "Variant.h"
#include "Version.h"

#include <unordered_map>

namespace CommonAPI {

template<class _Derived>
class OutputStream {
public:
	template<class _Deployment>
    OutputStream &writeValue(const bool &_value, const _Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

	template<class _Deployment>
    OutputStream &writeValue(const int8_t &_value, const _Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

	template<class _Deployment>
    OutputStream &writeValue(const int16_t &_value, const _Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

	template<class _Deployment>
    OutputStream &writeValue(const int32_t &_value, const _Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

	template<class _Deployment>
    OutputStream &writeValue(const int64_t &_value, const _Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

	template<class _Deployment>
    OutputStream &writeValue(const uint8_t &_value, const _Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

	template<class _Deployment>
    OutputStream &writeValue(const uint16_t &_value, const _Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

	template<class _Deployment>
    OutputStream &writeValue(const uint32_t &_value, const _Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

	template<class _Deployment>
    OutputStream &writeValue(const uint64_t &_value, const _Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

	template<class _Deployment>
    OutputStream &writeValue(const float &_value, const _Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

	template<class _Deployment>
    OutputStream &writeValue(const double &_value, const _Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

	template<class _Deployment>
    OutputStream &writeValue(const std::string &_value, const _Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

	template<class _Deployment>
    OutputStream &writeValue(const Version &_value, const _Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    template<class _Deployment, typename... _Types>
    OutputStream &writeValue(const Struct<_Types...> &_value, const _Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    template<class _Deployment, class _PolymorphicStruct>
    OutputStream &writeValue(const std::shared_ptr<_PolymorphicStruct> &_value, const _Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    template<class _Deployment, typename... _Types>
    OutputStream &writeValue(const Variant<_Types...> &_value, const _Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    template<class _Deployment, typename _ElementType>
    OutputStream &writeValue(const std::vector<_ElementType> &_value, const _Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    template<class _Deployment, typename _KeyType, typename _ValueType, typename _HasherType>
    OutputStream &writeValue(const std::unordered_map<_KeyType, _ValueType, _HasherType> &_value,
    						 const _Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    bool hasError() const {
    	return get()->hasError();
    }

private:
    inline _Derived *get() {
    	return static_cast<_Derived *>(this);
    }

    inline const _Derived *get() const {
    	return static_cast<const _Derived *>(this);
    }
};

template<class _Derived>
inline OutputStream<_Derived> &operator<<(OutputStream<_Derived> &_output, const bool &_value) {
    return _output.template writeValue<EmptyDeployment>(_value);
}

template<class _Derived>
inline OutputStream<_Derived>& operator<<(OutputStream<_Derived> &_output, const int8_t &_value) {
    return _output.template writeValue<EmptyDeployment>(_value);
}

template<class _Derived>
inline OutputStream<_Derived>& operator<<(OutputStream<_Derived> &_output, const int16_t &_value) {
    return _output.template writeValue<EmptyDeployment>(_value);
}

template<class _Derived>
inline OutputStream<_Derived>& operator<<(OutputStream<_Derived> &_output, const int32_t &_value) {
    return _output.template writeValue<EmptyDeployment>(_value);
}

template<class _Derived>
inline OutputStream<_Derived>& operator<<(OutputStream<_Derived> &_output, const int64_t &_value) {
    return _output.template writeValue<EmptyDeployment>(_value);
}

template<class _Derived>
inline OutputStream<_Derived>& operator<<(OutputStream<_Derived> &_output, const uint8_t &_value) {
    return _output.template writeValue<EmptyDeployment>(_value);
}

template<class _Derived>
inline OutputStream<_Derived>& operator<<(OutputStream<_Derived> &_output, const uint16_t &_value) {
    return _output.template writeValue<EmptyDeployment>(_value);
}

template<class _Derived>
inline OutputStream<_Derived>& operator<<(OutputStream<_Derived> &_output, const uint32_t &_value) {
    return _output.template writeValue<EmptyDeployment>(_value);
}

template<class _Derived>
inline OutputStream<_Derived>& operator<<(OutputStream<_Derived> &_output, const uint64_t &_value) {
    return _output.template writeValue<EmptyDeployment>(_value);
}

template<class _Derived>
inline OutputStream<_Derived>& operator<<(OutputStream<_Derived> &_output, const float &_value) {
    return _output.template writeValue<EmptyDeployment>(_value);
}

template<class _Derived>
inline OutputStream<_Derived>& operator<<(OutputStream<_Derived> &_output, const double &_value) {
    return _output.template writeValue<EmptyDeployment>(_value);
}

template<class _Derived>
inline OutputStream<_Derived>& operator<<(OutputStream<_Derived> &_output, const std::string &_value) {
    return _output.template writeValue<EmptyDeployment>(_value);
}

template<class _Derived, typename _Type, typename _TypeDepl>
inline OutputStream<_Derived> &operator<<(OutputStream<_Derived> &_output, const Deployable<_Type, _TypeDepl> &_value) {
	return _output.template writeValue<_TypeDepl>(_value.getValue(), _value.getDepl());
}

template<class _Derived>
inline OutputStream<_Derived>& operator<<(OutputStream<_Derived> &_output, const Version &_value) {
    return _output.template writeValue<EmptyDeployment>(_value);
}

template<class _Derived, typename... _Types>
OutputStream<_Derived> &operator<<(OutputStream<_Derived> &_output, const Struct<_Types...> &_value) {
	return _output.template writeValue<EmptyDeployment>(_value);
}

template<class _Derived, class _PolymorphicStruct>
OutputStream<_Derived> &operator<<(OutputStream<_Derived> &_output,	const std::shared_ptr<_PolymorphicStruct> &_value) {
	return _output.template writeValue<EmptyDeployment>(_value);
}

template<class _Derived, typename... _Types>
OutputStream<_Derived> &operator<<(OutputStream<_Derived> &_output, const Variant<_Types...> &_value) {
	return _output.template writeValue<EmptyDeployment>(_value);
}

template<class _Derived, typename _ElementType>
OutputStream<_Derived> &operator<<(OutputStream<_Derived> &_output, const std::vector<_ElementType> &_value) {
	return _output.template writeValue<EmptyDeployment>(_value);
}

template<class _Derived, typename _KeyType, typename _ValueType, typename _HasherType>
OutputStream<_Derived> &operator<<(OutputStream<_Derived> &_output,
                         const std::unordered_map<_KeyType, _ValueType, _HasherType> &_value) {
	return _output.template writeValue<EmptyDeployment>(_value);
}

} // namespace CommonAPI

#endif // COMMONAPI_OUTPUT_STREAM_H_
