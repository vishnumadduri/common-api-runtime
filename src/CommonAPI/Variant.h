/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_SERIALIZABLE_VARIANT_H_
#define COMMONAPI_SERIALIZABLE_VARIANT_H_

#include <memory>
#include <cstdint>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <cassert>

namespace CommonAPI {

template<class _Derived>
class InputStream;

template<class _Derived>
class OutputStream;

template<typename ... _Types>
struct MaxSize;

template<>
struct MaxSize<> {
    static const unsigned int value = 0;
};

template<typename _Type, typename ... _Types>
struct MaxSize<_Type, _Types...> {
    static const unsigned int current_type_size = sizeof(_Type);
    static const unsigned int next_type_size = MaxSize<_Types...>::value;
    static const unsigned int value =
                    current_type_size > next_type_size ?
                                    current_type_size : next_type_size;
};

template<typename _SearchType, typename ... _RestTypes>
struct VariantTypeSelector;

template<typename _SearchType, typename ... _RestTypes>
struct VariantTypeSelector<_SearchType, _SearchType, _RestTypes...> {
    typedef _SearchType type;
};

/**
 * \brief A templated generic variant class which provides type safe access and operators
 *
 * A templated generic variant class which provides type safe access and operators
 */
template<typename ... _Types>
class Variant {
private:
    typedef std::tuple_size<std::tuple<_Types...>> TypesTupleSize;

public:

    static const unsigned int maxSize = MaxSize<_Types...>::value;

    /**
     * \brief Construct an empty variant
     *
     * Construct an empty variant
     */
    Variant();


    /**
     * \brief Copy constructor. Must have identical templates.
     *
     * Copy constructor. Must have identical templates.
     *
     * @param fromVariant Variant to copy
     */
    Variant(const Variant& fromVariant);

    /**
     * \brief Copy constructor. Must have identical templates.
     *
     * Copy constructor. Must have identical templates.
     *
     * @param fromVariant Variant to copy
     */
    Variant(Variant&& fromVariant);

    ~Variant();

    /**
      * \brief Assignment of another variant. Must have identical templates.
      *
      * Assignment of another variant. Must have identical templates.
      *
      * @param rhs Variant to assign
      */
    Variant& operator=(const Variant& rhs);
    /**
     * \brief Assignment of another variant. Must have identical templates.
     *
     * Assignment of another variant. Must have identical templates.
     *
     * @param rhs Variant to assign
     */
    Variant& operator=(Variant&& rhs);

    /**
     * \brief Assignment of a contained type. Must be one of the valid templated types.
     *
     * Assignment of a contained type. Must be one of the valid templated types.
     *
     * @param value Value to assign
     */
    template<typename _Type>
    typename std::enable_if<!std::is_same<_Type, Variant<_Types...>>::value, Variant<_Types...>&>::type
    operator=(const _Type& value);

    /**
     * \brief Equality of another variant. Must have identical template list and content.
     *
     * Equality of another variant. Must have identical template list and content.
     *
     * @param rhs Variant to compare
     */
    bool operator==(const Variant<_Types...>& rhs) const;

    /**
      * \brief Not-Equality of another variant. Must have identical template list and content.
      *
      * Not-Equality of another variant. Must have identical template list and content.
      *
      * @param rhs Variant to compare
      */
    bool operator!=(const Variant<_Types...>& rhs) const;

    /**
      * \brief Testif the contained type is the same as the template on this method.
      *
      * Testif the contained type is the same as the template on this method.
      *
      * @return Is same type
      */
    template <typename _Type>
    const bool isType() const;

    /**
     * \brief Construct variant with content type set to value.
     *
     * Construct variant with content type set to value.
     *
     * @param value Value to place
     */
    template <typename _Type>
    Variant(const _Type& value,
                    typename std::enable_if<!std::is_const<_Type>::value>::type* = 0,
                    typename std::enable_if<!std::is_reference<_Type>::value>::type* = 0,
                    typename std::enable_if<!std::is_same<_Type, Variant>::value>::type* = 0);

    /**
     * \brief Construct variant with content type set to value.
     *
     * Construct variant with content type set to value.
     *
     * @param value Value to place
     */
    template <typename _Type>
    Variant(_Type && value,
                    typename std::enable_if<!std::is_const<_Type>::value>::type* = 0,
                    typename std::enable_if<!std::is_reference<_Type>::value>::type* = 0,
                    typename std::enable_if<!std::is_same<_Type, Variant>::value>::type* = 0);

    /**
     * \brief Get value of variant, template to content type. Throws exception if type is not contained.
     *
     * Get value of variant, template to content type. Throws exception if type is not contained.
     */
    template <typename _Type>
    const _Type& get() const;

    /**
     * \brief Get index in template list of type actually contained, starting at 1 at the end of the template list
     *
     * Get index in template list of type actually contained, starting at 1 at the end of the template list
     *
     * @return Index of contained type
     */
    uint8_t getValueType() const {
        return valueType_;
    }

private:

    template<typename _U>
    void set( const _U& value, const bool clear);

    template<typename _U>
    void set( _U&& value, const bool clear);

    template<typename _FriendType>
    friend struct TypeWriter;
    template<typename ... _FriendTypes>
    friend struct AssignmentVisitor;
    template<typename _FriendType>
    friend struct TypeEqualsVisitor;
    template<typename ... _FriendTypes>
    friend struct PartialEqualsVisitor;
    template<class _Derived, class _Deployment, typename ... _FriendTypes>
    friend struct InputStreamReadVisitor;
    template<class Variant, typename ... _FTypes>
    friend struct ApplyVoidIndexVisitor;

public:
    inline bool hasValue() const {
        return valueType_ < TypesTupleSize::value;
    }
    typename std::aligned_storage<maxSize>::type valueStorage_;

    uint8_t valueType_;
};

template<class Variant, typename ... _Types>
struct ApplyVoidIndexVisitor;

template<class Variant>
struct ApplyVoidIndexVisitor<Variant> {
    static const uint8_t index = 0;

    static
    void visit(Variant&, uint8_t&) {
        //won't be called
        assert(false);
    }
};

template<class Variant, typename _Type, typename ... _Types>
struct ApplyVoidIndexVisitor<Variant, _Type, _Types...> {
    static const uint8_t index = ApplyVoidIndexVisitor<Variant,
                    _Types...>::index + 1;

    static void visit(Variant& var, uint8_t& ind) {
        if (ind == index) {
            new (&var.valueStorage_) _Type();
            var.valueType_ = index;
        } else {
            ApplyVoidIndexVisitor<Variant, _Types...>::visit(var, ind);
        }
    }
};

template<class Visitor, class Variant, typename ... _Types>
struct ApplyVoidVisitor;

template<class Visitor, class Variant>
struct ApplyVoidVisitor<Visitor, Variant> {
    static const uint8_t index = 0;

    static
    void visit(Visitor&, Variant&) {
        //won't be called
        assert(false);
    }

    static
    void visit(Visitor&, const Variant&) {
        //won't be called
        assert(false);
    }
};

template<class Visitor, class Variant, typename _Type, typename ... _Types>
struct ApplyVoidVisitor<Visitor, Variant, _Type, _Types...> {
    static const uint8_t index = ApplyVoidVisitor<Visitor, Variant,
                    _Types...>::index + 1;

    static void visit(Visitor& visitor, Variant& var) {
        if (var.getValueType() == index) {
            visitor(var.template get<_Type>());
        } else {
            ApplyVoidVisitor<Visitor, Variant, _Types...>::visit(visitor, var);
        }
    }

    static void visit(Visitor& visitor, const Variant& var) {
        if (var.getValueType() == index) {
            visitor(var.template get<_Type>());
        } else {
            ApplyVoidVisitor<Visitor, Variant, _Types...>::visit(visitor, var);
        }
    }
};

template<class Visitor, class Variant, typename ... _Types>
struct ApplyBoolVisitor
;

template<class Visitor, class Variant>
struct ApplyBoolVisitor<Visitor, Variant> {
    static const uint8_t index = 0;

    static bool visit(Visitor&, Variant&) {
        //won't be called
        assert(false);
        return false;
    }
};

template<class Visitor, class Variant, typename _Type, typename ... _Types>
struct ApplyBoolVisitor<Visitor, Variant, _Type, _Types...> {
    static const uint8_t index = ApplyBoolVisitor<Visitor, Variant,
                    _Types...>::index + 1;

    static bool visit(Visitor& visitor, Variant& var) {
        if (var.getValueType() == index) {
            return visitor(var.template get<_Type>());
        } else {
            return ApplyBoolVisitor<Visitor, Variant, _Types...>::visit(visitor,
                            var);
        }
    }
};

template<uint32_t size>
struct DeleteVisitor {
public:
    DeleteVisitor(typename std::aligned_storage<size>::type& storage) :
                    storage_(storage) {
    }

    template<typename _Type>
    void operator()(const _Type&) const {
        (reinterpret_cast<const _Type *>(&storage_))->~_Type();
    }

private:
    typename std::aligned_storage<size>::type& storage_;
};

template<class _Derived, class _Deployment>
struct OutputStreamWriteVisitor {
public:
    OutputStreamWriteVisitor(OutputStream<_Derived> &_output, const _Deployment *_depl)
		: output_(_output), depl_(_depl) {
    }

    template<typename _Type>
    void operator()(const _Type& value) const {
        Deployable<_Type, _Deployment> itsValue(value, depl_);
        output_ << itsValue;
    }

private:
    OutputStream<_Derived> &output_;
    const _Deployment *depl_;
};


template<class _Derived, class _Deployment, typename ... _Types>
struct InputStreamReadVisitor {
public:
    InputStreamReadVisitor(InputStream<_Derived> &_input, Variant<_Types...> &lhs, const _Deployment *_depl)
		: input_(_input), lhs_(lhs), depl_(_depl) {
    }

    template<typename _Type>
    void operator()(const _Type&) {
        Deployable<_Type, _Deployment> itsValue(depl_);
        input_ >> itsValue;
        lhs_.Variant<_Types...>::template set<_Type>(std::move(itsValue.getValue()), false);
    }

private:
    InputStream<_Derived> &input_;
    Variant<_Types...> &lhs_;
    const _Deployment *depl_;
};

template<class _Derived>
struct TypeOutputStreamWriteVisitor {
public:
    TypeOutputStreamWriteVisitor(_Derived &_output)
		: output_(_output) {
    }

    template<typename _Type>
    void operator()(const _Type &_value) const {
        output_ << _value;
    }

private:
    _Derived &output_;
};

template<typename _Type>
struct TypeEqualsVisitor
{
public:
    TypeEqualsVisitor(const _Type& rhs): rhs_(rhs) {
    }

    bool operator()(const _Type& lhs) const {
        return lhs == rhs_;
    }

    template<typename _U>
    bool operator()(const _U&) const {
        return false;
    }

private:
    const _Type& rhs_;
};

template<typename ... _Types>
struct PartialEqualsVisitor
{
public:
    PartialEqualsVisitor(const Variant<_Types...>& lhs) :
                    lhs_(lhs) {
    }

    template<typename _Type>
    bool
    operator()(const _Type& rhs) const
                      {
        TypeEqualsVisitor<_Type> visitor(rhs);
        return ApplyBoolVisitor<TypeEqualsVisitor<_Type>, const Variant<_Types...>, _Types...>::visit(visitor, lhs_);
    }

private:
    const Variant<_Types...>& lhs_;
};

template<typename ... _Types>
struct AssignmentVisitor {
public:
    AssignmentVisitor(Variant<_Types...>& lhs, const bool clear = true) :
                    lhs_(lhs), clear_(clear) {
    }

    template<typename _Type>
    void operator()(const _Type& value) const {
        lhs_.Variant<_Types...>::template set<_Type>(value, clear_);
    }

    template<typename _Type>
    void operator()(_Type& value) const {
        lhs_.Variant<_Types...>::template set<_Type>(value, clear_);
    }

private:
    Variant<_Types...>& lhs_;
    const bool clear_;
};

template<typename ... _Types>
struct TypeSelector;

template<typename _U>
struct TypeSelector<_U> {
};

//_U == _Type
template<typename _Type, typename ... _Types>
struct TypeSelector<_Type, _Type, _Types...> {
    typedef _Type type;
};

//_U& == _Type
template<typename _Type, typename ... _Types>
struct TypeSelector<_Type, _Type&, _Types...> {
    typedef _Type& type;
};

//_U == _Type&
template<typename _Type, typename ... _Types>
struct TypeSelector<_Type&, _Type, _Types...> {
    typedef _Type type;
};

//const _U& == _Type
template<typename _Type, typename ... _Types>
struct TypeSelector<_Type, const _Type&, _Types...> {
    typedef const _Type& type;
};

//_U == const _Type&
template<typename _Type, typename ... _Types>
struct TypeSelector<const _Type&, _Type, _Types...> {
    typedef _Type type;
};

//_U == X*
//_Type == const X*
template<typename _Type, typename ... _Types>
struct TypeSelector<_Type*, const _Type*, _Types...> {
    typedef const _Type* type;
};

//_U == X&
//_Type == const X&
template<typename _Type, typename ... _Types>
struct TypeSelector<_Type&, const _Type&, _Types...> {
    typedef const _Type& type;
};

//_U != _Type, let's try to find _U among _Types
template<typename _U, typename _Type, typename ... _Types>
struct TypeSelector<_U, _Type, _Types...> {
    typedef typename TypeSelector<_U, _Types...>::type type;
};

template<typename ... _Types>
struct TypeIndex;

template<>
struct TypeIndex<> {
    static const uint8_t index = 0;

    template<typename _U>
    static uint8_t get() {
        return 0;
    }
};

template<typename _Type, typename ... _Types>
struct TypeIndex<_Type, _Types...> {
    static const uint8_t index = TypeIndex<_Types...>::index + 1;

    template<typename _U>
    static uint8_t get(
                       typename std::enable_if<std::is_same<_Type, _U>::value>::type* = 0) {
        return index;
    }

    template<typename _U>
    static uint8_t get(typename std::enable_if<!std::is_same<_Type, _U>::value>::type* = 0) {
        return TypeIndex<_Types...>::template get<_U>();
    }
};

template<typename ... _Types>
Variant<_Types...>::Variant() :
                valueType_(TypesTupleSize::value) {
    ApplyVoidIndexVisitor<Variant<_Types...>, _Types...>::visit(*this, valueType_);
}

template<typename ... _Types>
Variant<_Types...>::Variant(const Variant& fromVariant) {
    AssignmentVisitor<_Types...> visitor(*this, false);
    ApplyVoidVisitor<AssignmentVisitor<_Types...> , Variant<_Types...>, _Types...>::visit(visitor, fromVariant);
}

template<typename ... _Types>
Variant<_Types...>::Variant(Variant&& fromVariant)
{
    AssignmentVisitor<_Types...> visitor(*this, false);
    ApplyVoidVisitor<AssignmentVisitor<_Types...> , Variant<_Types...>, _Types...>::visit(visitor, fromVariant);
}

template<typename ... _Types>
Variant<_Types...>::~Variant() {
    if (hasValue()) {
        DeleteVisitor<maxSize> visitor(valueStorage_);
        ApplyVoidVisitor<DeleteVisitor<maxSize>, Variant<_Types...>, _Types...>::visit(visitor, *this);
    }
}

template<typename ... _Types>
Variant<_Types...>& Variant<_Types...>::operator=(const Variant<_Types...>& rhs) {
    AssignmentVisitor<_Types...> visitor(*this, hasValue());
    ApplyVoidVisitor<AssignmentVisitor<_Types...>, Variant<_Types...>, _Types...>::visit(
                    visitor, rhs);
    return *this;
}

template<typename ... _Types>
Variant<_Types...>& Variant<_Types...>::operator=(Variant<_Types...>&& rhs) {
    AssignmentVisitor<_Types...> visitor(*this, hasValue());
    ApplyVoidVisitor<AssignmentVisitor<_Types...>, Variant<_Types...>, _Types...>::visit(visitor, rhs);
    return *this;
}

template<typename ... _Types>
template<typename _Type>
typename std::enable_if<!std::is_same<_Type, Variant<_Types...>>::value, Variant<_Types...>&>::type
Variant<_Types...>::operator=(const _Type& value) {
    set<typename TypeSelector<_Type, _Types...>::type>(value, hasValue());
    return *this;
}

template<typename ... _Types>
template<typename _Type>
const bool Variant<_Types...>::isType() const {
    typedef typename TypeSelector<_Type, _Types...>::type selected_type_t;
    uint8_t cType = TypeIndex<_Types...>::template get<selected_type_t>();
    if (cType == valueType_) {
        return true;
    } else {
        return false;
    }
}

template<typename ... _Types>
template<typename _Type>
Variant<_Types...>::Variant(const _Type& value,
                            typename std::enable_if<!std::is_const<_Type>::value>::type*,
                            typename std::enable_if<!std::is_reference<_Type>::value>::type*,
                            typename std::enable_if<!std::is_same<_Type, Variant<_Types...>>::value>::type*) {
    set<typename TypeSelector<_Type, _Types...>::type>(value, false);
}

template<typename ... _Types>
template<typename _Type>
Variant<_Types...>::Variant(_Type && value,
typename std::enable_if<!std::is_const<_Type>::value>::type*,
typename std::enable_if<!std::is_reference<_Type>::value>::type*,
typename std::enable_if<!std::is_same<_Type, Variant<_Types...>>::value>::type*) {
    set<typename TypeSelector<_Type, _Types...>::type>(std::move(value), false);
}


template<typename ... _Types>
template<typename _Type>
const _Type & Variant<_Types...>::get() const {
    typedef typename TypeSelector<_Type, _Types...>::type selected_type_t;
    uint8_t cType = TypeIndex<_Types...>::template get<selected_type_t>();
    if (cType == valueType_) {
        return *(reinterpret_cast<const _Type *>(&valueStorage_));
    } else {
#ifdef __EXCEPTIONS
        std::bad_cast toThrow;
        throw toThrow;
#else
        printf("SerializableVariant.hpp:%i %s: Incorrect access to variant; attempting to get type not currently contained", __LINE__, __FUNCTION__);
        abort();
#endif
    }
}


template<typename ... _Types>
template<typename _U>
void Variant<_Types...>::set(const _U& value, const bool clear) {
    typedef typename TypeSelector<_U, _Types...>::type selected_type_t;

    if (clear) {
		DeleteVisitor<maxSize> visitor(valueStorage_);
        ApplyVoidVisitor<DeleteVisitor<maxSize>, Variant<_Types...>, _Types...>::visit(visitor, *this);
    }
    new (&valueStorage_) selected_type_t(std::move(value));
    valueType_ = TypeIndex<_Types...>::template get<selected_type_t>();
}

template<typename ... _Types>
template<typename _U>
void Variant<_Types...>::set(_U&& value, const bool clear) {
    typedef typename TypeSelector<_U, _Types...>::type selected_type_t;

    selected_type_t&& any_container_value = std::move(value);
    if(clear)
    {
		DeleteVisitor<maxSize> visitor(valueStorage_);
        ApplyVoidVisitor<DeleteVisitor<maxSize>, Variant<_Types...>, _Types...>::visit(visitor, *this);
    } else {
        new (&valueStorage_) selected_type_t(std::move(any_container_value));
    }

    valueType_ = TypeIndex<_Types...>::template get<selected_type_t>();
}

template<typename ... _Types>
bool Variant<_Types...>::operator==(const Variant<_Types...>& rhs) const
                {
    PartialEqualsVisitor<_Types...> visitor(*this);
    return ApplyBoolVisitor<PartialEqualsVisitor<_Types...>, const Variant<_Types...>, _Types...>::visit(
                    visitor,
                    rhs);
}

template<typename ... _Types>
bool Variant<_Types...>::operator!=(const Variant<_Types...>& rhs) const
                {
    return !(*this == rhs);
}

} // namespace CommonAPI

#endif // COMMONAPI_SERIALIZABLE_VARIANT_H_
