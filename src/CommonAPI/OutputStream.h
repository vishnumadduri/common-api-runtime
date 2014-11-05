/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
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
#include "Variant.h"
#include "types.h"

#include <iostream>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <type_traits>

namespace CommonAPI {

class Deployment;

template<typename _Binding>
class OutputStream {
public:
    OutputStream &writeValue(const bool &_value, const Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    OutputStream &writeValue(const int8_t &_value, const Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    OutputStream &writeValue(const int16_t &_value, const Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    OutputStream &writeValue(const int32_t &_value, const Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    OutputStream &writeValue(const int64_t &_value, const Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    OutputStream &writeValue(const uint8_t &_value, const Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    OutputStream &writeValue(const uint16_t &_value, const Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    OutputStream &writeValue(const uint32_t &_value, const Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    OutputStream &writeValue(const uint64_t &_value, const Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    OutputStream &writeValue(const float &_value, const Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    OutputStream &writeValue(const double &_value, const Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    OutputStream &writeValue(const std::string &_value, const Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    OutputStream &writeValue(const Version &_value, const Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    template<typename... _Types>
    OutputStream &writeValue(const Variant<_Types...> &_value, const Deployment *_depl = nullptr) {
    	OutputStreamWriteVisitor<_Binding> visitor(*this, _depl);
    	ApplyVoidVisitor<OutputStreamWriteVisitor<_Binding>,
			 Variant<_Types...>, _Types...>::visit(visitor, _value);
    	return (*this);
    }

    template<typename _ElementType>
    OutputStream &writeValue(const std::vector<_ElementType> &_value, const Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

    template<typename _KeyType, typename _ValueType, typename _HasherType>
    OutputStream &writeValue(const std::unordered_map<_KeyType, _ValueType, _HasherType> &_value, const Deployment *_depl = nullptr) {
    	return get()->writeValue(_value, _depl);
    }

private:
    inline _Binding *get() {
    	return static_cast<_Binding *>(this);
    }
};

template<class _Binding>
inline OutputStream<_Binding> &operator<<(OutputStream<_Binding> &_output, const bool &_value) {
    return _output.writeValue(_value);
}

template<class _Binding>
inline OutputStream<_Binding>& operator<<(OutputStream<_Binding> &_output, const int8_t &_value) {
    return _output.writeValue(_value);
}

template<class _Binding>
inline OutputStream<_Binding>& operator<<(OutputStream<_Binding> &_output, const int16_t &_value) {
    return _output.writeValue(_value);
}

template<class _Binding>
inline OutputStream<_Binding>& operator<<(OutputStream<_Binding> &_output, const int32_t &_value) {
    return _output.writeValue(_value);
}

template<class _Binding>
inline OutputStream<_Binding>& operator<<(OutputStream<_Binding> &_output, const int64_t &_value) {
    return _output.writeValue(_value);
}

template<class _Binding>
inline OutputStream<_Binding>& operator<<(OutputStream<_Binding> &_output, const uint8_t &_value) {
    return _output.writeValue(_value);
}

template<class _Binding>
inline OutputStream<_Binding>& operator<<(OutputStream<_Binding> &_output, const uint16_t &_value) {
    return _output.writeValue(_value);
}

template<class _Binding>
inline OutputStream<_Binding>& operator<<(OutputStream<_Binding> &_output, const uint32_t &_value) {
    return _output.writeValue(_value);
}

template<class _Binding>
inline OutputStream<_Binding>& operator<<(OutputStream<_Binding> &_output, const uint64_t &_value) {
    return _output.writeValue(_value);
}

template<class _Binding>
inline OutputStream<_Binding>& operator<<(OutputStream<_Binding> &_output, const float &_value) {
    return _output.writeValue(_value);
}

template<class _Binding>
inline OutputStream<_Binding>& operator<<(OutputStream<_Binding> &_output, const double &_value) {
    return _output.writeValue(_value);
}

template<class _Binding>
inline OutputStream<_Binding>& operator<<(OutputStream<_Binding> &_output, const std::string &_value) {
    return _output.writeValue(_value);
}

template<class _Binding>
inline OutputStream<_Binding>& operator<<(OutputStream<_Binding> &_output, const Version &_value) {
    return _output.writeValue(_value);
}

template<class _Binding, typename... _Types>
OutputStream<_Binding> &operator<<(OutputStream<_Binding> &_output, const Variant<_Types...> &_value) {
	return _output.writeValue(_value);
}

template<class _Binding, typename _ElementType>
OutputStream<_Binding> &operator<<(OutputStream<_Binding> &_output, const std::vector<_ElementType> &_value) {
	return _output.writeValue(_value);
}

template<class _Binding, typename _KeyType, typename _ValueType, typename _HasherType>
OutputStream<_Binding> &operator<<(OutputStream<_Binding> &_output,
                         const std::unordered_map<_KeyType, _ValueType, _HasherType> &_value) {
	return _output.writeValue(_value);
}

} // namespace CommonAPI

#endif // COMMONAPI_OUTPUT_STREAM_H_
