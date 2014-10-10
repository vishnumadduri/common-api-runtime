/* Copyright (C) 2014 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_SERIALIZABLE_H_
#define COMMONAPI_SERIALIZABLE_H_

namespace CommonAPI {

class InputStream;
class OutputStream;

struct SerializableBase {
};

template<typename Type, typename TypeDepl>
struct Serializable : SerializableBase {

	Serializable() = delete;

	Serializable(const Type& v) : value_(const_cast<Type&>(v)), depl_(nullptr) {
	};

    Serializable(const Type& v, const TypeDepl* d) : value_(const_cast<Type&>(v)), depl_(const_cast<TypeDepl*>(d)) {
        // TODO Check if necessary
        this->depl_ = const_cast<TypeDepl*>(d);
    };

    template<typename TypeOther, typename TypeDeplOther>
    Serializable<Type, TypeDepl>& operator=( const Serializable<TypeOther, TypeDeplOther>& other ) {
        value_ = other.value_;
        depl_ = other.depl_;
        return *this;
    }

	virtual ~Serializable() { }

	virtual InputStream& deserialize(InputStream& inputStream) = 0;
	virtual OutputStream& serialize(OutputStream& outputStream) const = 0;

	// TODO Check if get function is optimal implemented (return Type and not Type&)
    Type get() {
        return value_;
    }

 protected:
	Type& value_;
	TypeDepl* depl_;
};

template<typename Type, typename TypeDepl>
struct SerializableArray : SerializableBase {

    SerializableArray() = delete;

    SerializableArray(const std::vector<Type>& v) : value_(const_cast<std::vector<Type>&>(v)), depl_(nullptr) {
    };

    SerializableArray(const std::vector<Type>& v, const TypeDepl* d) : value_(const_cast<std::vector<Type>&>(v)), depl_(const_cast<TypeDepl*>(d)) {
            this->depl_ = const_cast<TypeDepl*>(d);
    };

    template<typename TypeOther, typename TypeDeplOther>
    SerializableArray<Type, TypeDepl>& operator=( const Serializable<TypeOther, TypeDeplOther>& other ) {
        value_ = other.value_;
        depl_ = other.depl_;
        return *this;
    }

    virtual ~SerializableArray() { }

    virtual InputStream& deserialize(InputStream& inputStream) {
        return inputStream;
    }

    virtual OutputStream& serialize(OutputStream& outputStream) const {



        return outputStream;
    }

    virtual InputStream& deserializeElement(InputStream& inputStream) = 0;
    virtual OutputStream& serializeElement(OutputStream& outputStream) const = 0;

    Type get() {
        return value_;
    }

 protected:
    std::vector<Type>& value_;
    TypeDepl* depl_;
};

} // namespace CommonAPI

#endif // COMMONAPI_SERIALIZABLE_H_
