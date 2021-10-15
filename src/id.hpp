#ifndef ID_HPP
#define ID_HPP

template< typename Obj, std::unsigned_integral Rep >
struct Id
{
    using obj_type = Obj;
    using value_type = Rep;

    value_type value; 

    Id () : value { 0 } {};
    Id ( value_type id ) : value { id } {};

    Id& operator++ ()
    {
        ++value;
        return *this;
    }

    Id& operator-- ()
    {
        --value;
        return *this;
    }
    
    bool operator< (const Id& id2) const
    {
        return value < id2.value;
    }

    // добавить про конвертируемость
    template< std::unsigned_integral Rep2 >
    bool operator== ( const Id<Obj, Rep2>& id ) const
    {
        return value == id.value;
    }
};

template< typename Obj, std::unsigned_integral Rep >
std::ostream& operator<< ( std::ostream& os, const Id<Obj, Rep>& id )
{
    return os << id.value;
}

#endif  // ID_HPP
