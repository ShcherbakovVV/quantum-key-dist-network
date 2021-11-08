#ifndef ID_HPP
#define ID_HPP

#include <limits>
#include <iostream>

template< typename Obj, std::unsigned_integral Rep >
struct Id
{
    using object_type = Obj;
    using value_type  = Rep;
    using this_type   = Id<object_type, value_type>;

private:

    value_type v;

public:

    Id ( value_type id = 0 ) : v { id } {};

    value_type value() const { return v; }

    static this_type max() { return std::numeric_limits<value_type>::max(); }

    this_type& operator++ ()
    {
        if ( v == max().value() )
            throw std::overflow_error { "Id::value overflow" };
        ++ v;
        return *this;
    }

    this_type& operator-- ()
    {
        if ( v == 0 )
            throw std::underflow_error { "Id::value underflow" };
        -- v;
        return *this;
    }

    template <std::unsigned_integral Rep2>
    bool operator== ( const Id<Obj, Rep2>& id ) const { return v == id.v; }

    template <std::unsigned_integral Rep2>
    bool operator< ( const Id<Obj, Rep2>& id2 ) const { return v < id2.v; }
};

template <typename Obj, std::unsigned_integral Rep>
std::string to_string( const Id<Obj, Rep>& id )
{
    return std::to_string( id.value() );
}

template <typename Obj, std::unsigned_integral Rep>
std::ostream& operator<< ( std::ostream& os, const Id<Obj, Rep>& id )
{
    return os << id.value();
}

#endif  // ID_HPP
