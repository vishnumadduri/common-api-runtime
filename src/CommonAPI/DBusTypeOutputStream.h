/* Copyright (C) 2014 BMW Group
 * Author: Lutz Bichler (lutz.bichler@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef COMMONAPI_DBUS_DBUSTYPEOUTPUTSTREAM_H_
#define COMMONAPI_DBUS_DBUSTYPEOUTPUTSTREAM_H_

#include <CommonAPI/TypeOutputStream.h>

namespace CommonAPI {
namespace DBus {

class DBusTypeOutputStream: public TypeOutputStream<DBusTypeOutputStream> {
public:
    DBusTypeOutputStream() : signature_("") {}
    virtual ~DBusTypeOutputStream() {}

    TypeOutputStream &writeType(const bool &) {
        signature_.append("b");
        return (*this);
    }

    TypeOutputStream &writeType(const int8_t &) {
        signature_.append("y");
        return (*this);
    }

    TypeOutputStream &writeType(const int16_t &) {
        signature_.append("n");
        return (*this);
    }

    TypeOutputStream &writeType(const int32_t &) {
        signature_.append("i");
        return (*this);
    }

    TypeOutputStream &writeType(const  int64_t &) {
        signature_.append("x");
        return (*this);
    }

    TypeOutputStream &writeType(const uint8_t &) {
        signature_.append("y");
        return (*this);
    }

    TypeOutputStream &writeType(const uint16_t &) {
        signature_.append("q");
        return (*this);
    }

    TypeOutputStream &writeType(const uint32_t &) {
        signature_.append("u");
        return (*this);
    }

    TypeOutputStream &writeType(const uint64_t &) {
        signature_.append("t");
        return (*this);
    }

    TypeOutputStream &writeType(const float &) {
        signature_.append("d");
        return (*this);
    }

    TypeOutputStream &writeType(const double &) {
        signature_.append("d");
        return (*this);
    }

    TypeOutputStream &writeType(const std::string &) {
        signature_.append("s");
        return (*this);
    }

    TypeOutputStream &writeType() {
        signature_.append("ay");
        return (*this);
    }

    TypeOutputStream &writeVersionType() {
        signature_.append("(uu)");
        return (*this);
    }

    template<typename... _Types>
    TypeOutputStream &writeType(const Struct<_Types...> &_value) {
    	signature_.append("(");
        const auto itsSize(std::tuple_size<std::tuple<_Types...>>::value);
        StructTypeWriter<itsSize-1, DBusTypeOutputStream, Struct<_Types...>>{}
        	(*this, _value);
        signature_.append(")");
        return (*this);
    }

    template<class _PolymorphicStruct>
    TypeOutputStream &writeType(const std::shared_ptr<_PolymorphicStruct> &_value) {
    	signature_.append("(");
    	value->writeType(*this);
    	uint32_t dummy;
    	writeType(dummy);
        signature_.append(")");
        return (*this);
    }

    template<typename... _Types>
    TypeOutputStream &writeType(const Variant<_Types...> &_value) {
        signature_.append("(yv)");
     	TypeOutputStreamWriteVisitor<DBusTypeOutputStream> typeVisitor(*this);
        ApplyVoidVisitor<TypeOutputStreamWriteVisitor<DBusTypeOutputStream>,
        		Variant<_Types...>, _Types...>::visit(typeVisitor, _value);
        return (*this);
    }

	template<typename _ElementType>
	TypeOutputStream &writeType(const std::vector<_ElementType> &_value) {
		signature_.append("a");
		return (*this);
	}

	template<typename _KeyType, typename _ValueType, typename _HasherType>
	TypeOutputStream &writeType(const std::unordered_map<_KeyType, _ValueType, _HasherType> &_value) {
		signature_.append("a{");
		signature_.append("}");
		return (*this);
	}

    inline std::string getSignature() {
        return std::move(signature_);
    }

private:
    std::string signature_;
};

} /* namespace DBus */
} /* namespace CommonAPI */

#endif /* COMMONAPI_DBUS_DBUSTYPEOUTPUTSTREAM_H_ */
