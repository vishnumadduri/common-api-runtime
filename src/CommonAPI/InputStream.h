/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_INPUT_STREAM_H_
#define COMMONAPI_INPUT_STREAM_H_

#include "ByteBuffer.h"
#include "Deployable.h"
#include "Deployment.h"
#include "Variant.h"
#include "types.h"

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <type_traits>

namespace CommonAPI {

template<class _Binding>
class InputStream {
public:
    virtual ~InputStream() {}
    virtual bool hasError() const = 0;

    InputStream &readValue(bool &_value, const Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    InputStream &readValue(int8_t &_value, const Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    InputStream& readValue(int16_t &_value, const Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    InputStream& readValue(int32_t &_value, const Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    InputStream& readValue(int64_t &_value, const Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    InputStream& readValue(uint8_t &_value, const Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    InputStream& readValue(uint16_t &_value, const Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    InputStream& readValue(uint32_t &_value, const Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    InputStream& readValue(uint64_t &_value, const Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    InputStream& readValue(float &_value, const Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    InputStream& readValue(double &_value, const Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    InputStream& readValue(std::string &_value, const Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<typename _ElementType>
    InputStream &readValue(std::vector<_ElementType> &_value, const Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<typename _KeyType, typename _ValueType, typename _HasherType>
    InputStream &readValue(std::unordered_map<_KeyType, _ValueType, _HasherType> &_value,
    					   const Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<typename... _Types>
    InputStream &readValue(Variant<_Types...> &_value, const Deployment *_depl = nullptr) {
        if(_value.hasValue()) {
        	DeleteVisitor<_value.maxSize> visitor(_value.valueStorage_);
            ApplyVoidVisitor<DeleteVisitor<_value.maxSize>,
    			Variant<_Types...>, _Types... >::visit(visitor, _value);
        }

        _value.valueType_ = _value.typeIndex;
        InputStreamReadVisitor<_Types...> visitor((*this), _value, _depl);
        ApplyVoidVisitor<InputStreamReadVisitor<_Binding, _Types... >,
    		Variant<_Types...>, _Types...>::visit(visitor, _value);

        return (*this);
    }

    InputStream& readValue(Version &_value) {
    	return get()->readValue(_value);
    }

private:
    inline _Binding *get() {
    	return static_cast<_Binding *>(this);
    }
};

template<class _Binding>
InputStream<_Binding> &operator>>(InputStream<_Binding> &_input, bool &_value) {
	return _input.readValue(_value);
}

template<class _Binding>
InputStream<_Binding> &operator>>(InputStream<_Binding> &_input, int8_t &_value) {
	return _input.readValue(_value);
}

template<class _Binding>
InputStream<_Binding> &operator>>(InputStream<_Binding> &_input, int16_t &_value) {
	return _input.readValue(_value);
}

template<class _Binding>
InputStream<_Binding> &operator>>(InputStream<_Binding> &_input, int32_t &_value) {
	return _input.readValue(_value);
}

template<class _Binding>
InputStream<_Binding> &operator>>(InputStream<_Binding> &_input, int64_t &_value) {
	return _input.readValue(_value);
}

template<class _Binding>
InputStream<_Binding> &operator>>(InputStream<_Binding> &_input, uint8_t &_value) {
	return _input.readValue(_value);
}

template<class _Binding>
InputStream<_Binding> &operator>>(InputStream<_Binding> &_input, uint16_t &_value) {
	return _input.readValue(_value);
}

template<class _Binding>
InputStream<_Binding> &operator>>(InputStream<_Binding> &_input, uint32_t &_value) {
	return _input.readValue(_value);
}

template<class _Binding>
InputStream<_Binding> &operator>>(InputStream<_Binding> &_input, uint64_t &_value) {
	return _input.readValue(_value);
}

template<class _Binding>
InputStream<_Binding> &operator>>(InputStream<_Binding> &_input, float &_value) {
	return _input.readValue(_value);
}

template<class _Binding>
InputStream<_Binding> &operator>>(InputStream<_Binding> &_input, double &_value) {
	return _input.readValue(_value);
}

template<class _Binding>
InputStream<_Binding> &operator>>(InputStream<_Binding> &_input, std::string &_value) {
	return _input.readValue(_value);
}

template<class _Binding>
InputStream<_Binding> &operator>>(InputStream<_Binding> &_input, Version &_value) {
	return _input.readValue(_value);
}

template<class _Binding, typename... _Types>
InputStream<_Binding> & operator>>(InputStream<_Binding> &_input, Variant<_Types...> &_value) {
	return _input.readValue(_value);
}

template<class _Binding, typename _ElementType>
InputStream<_Binding> &operator>>(InputStream<_Binding> &_input, std::vector<_ElementType> &_value) {
	return _input.readValue(_value);
}

template<class _Binding, typename _KeyType, typename _ValueType, typename _HasherType>
InputStream<_Binding> &operator>>(InputStream<_Binding> &_input, std::unordered_map<_KeyType, _ValueType, _HasherType> &_value) {
	return _input.readValue(_value);
}

template<class _Binding, typename _Type, typename _TypeDeployment>
InputStream<_Binding> &operator>>(InputStream<_Binding> &_input, Deployable<_Type, _TypeDeployment> &_value) {
	return _input.readValue(_value.getValue(), _value.getDepl());
}

} // namespace CommonAPI

#endif // COMMONAPI_INPUT_STREAM_H_
