// Copyright (c) 2017-2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_VARIANT_H
#define BITCOIN_VARIANT_H

#include <utility>

#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>

//! Substitute for C++17 std::variant
template <class... Types>
class Variant
{
private:
    boost::variant<Types...> m_var;

public:
    Variant()
    {
        m_var = {};
    }
    Variant(const Variant& other)
    {
        m_var = other.m_var;
    }
    Variant(Variant& other)
    {
        m_var = other.m_var;
    }
    Variant(Variant&& other)
    {
        m_var = std::move(other.m_var);
    }
    template <class T>
    Variant(T& value)
    {
        m_var = value;
    }
    template <class T>
    Variant(const T& value)
    {
        m_var = value;
    }
    template <class T>
    Variant(T&& value)
    {
        m_var = std::move(value);
    }

    int which() const
    {
        return m_var.which();
    }

    template <class T>
    T* get_if()
    {
        return boost::get<T>(&m_var);
    }
    template <class T>
    const T* get_if() const
    {
        return boost::get<T>(&m_var);
    }
    template <class T>
    T& get()
    {
        return boost::get<T>(m_var);
    }
    template <class T>
    const T& get() const
    {
        return boost::get<T>(m_var);
    }

    Variant& operator=(const Variant& other)
    {
        m_var = other.m_var;
        return *this;
    }
    Variant& operator=(Variant&& other)
    {
        m_var = std::move(other.m_var);
        return *this;
    }

    // Allow support for boost::Variant
    template <class Visitor>
    typename Visitor::result_type apply_visitor(Visitor& visitor) const
    {
        return boost::apply_visitor(visitor, m_var);
    }
    template <class Visitor>
    typename Visitor::result_type apply_visitor(const Visitor& visitor) const
    {
        return boost::apply_visitor(visitor, m_var);
    }

    bool operator==(const Variant& other) const
    {
        return m_var == other.m_var;
    }
    bool operator!=(const Variant& other) const
    {
        return m_var != other.m_var;
    }
    bool operator<(const Variant& other) const
    {
        return m_var < other.m_var;
    }
    bool operator>(const Variant& other) const
    {
        return m_var > other.m_var;
    }
    bool operator<=(const Variant& other) const
    {
        return m_var <= other.m_var;
    }
    bool operator>=(const Variant& other) const
    {
        return m_var >= other.m_var;
    }
    // This are here to prevent implicit conversion to Variant and throw a compile time error.
    template <typename T>
    void operator==(const T&) const
    {
        static_assert(false && sizeof(T), "Compared a Variant directly with type T. this prevented an implicit conversion");
    }
    template <typename T>
    void operator!=(const T&) const
    {
        static_assert(false && sizeof(T), "Compared a Variant directly with type T. this prevented an implicit conversion");
    }
    template <typename T>
    void operator<(const T&) const
    {
        static_assert(false && sizeof(T), "Compared a Variant directly with type T. this prevented an implicit conversion");
    }
    template <typename T>
    void operator>(const T&) const
    {
        static_assert(false && sizeof(T), "Compared a Variant directly with type T. this prevented an implicit conversion");
    }
    template <typename T>
    void operator<=(const T&) const
    {
        static_assert(false && sizeof(T), "Compared a Variant directly with type T. this prevented an implicit conversion");
    }
    template <typename T>
    void operator>=(const T&) const
    {
        static_assert(false && sizeof(T), "Compared a Variant directly with type T. this prevented an implicit conversion");
    }
};

#endif // BITCOIN_VARIANT_H
