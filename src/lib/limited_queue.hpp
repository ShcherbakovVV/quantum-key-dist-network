#ifndef LIMITED_QUEUE_HPP
#define LIMITED_QUEUE_HPP

#include <queue>
#include <deque>
#include <algorithm>

template <typename T, typename Container = std::deque<T>>
class limited_queue
{
    public:

        using Queue = std::queue<T, Container>;

        using container_type  = Queue::container_type;
        using value_type      = Queue::value_type;
        using size_type	      = Queue::size_type;
        using reference	      = Queue::reference;
        using const_reference =	Queue::const_reference;

    private:

        Queue queue;
        size_type cap;

    public:

        limited_queue ( size_type cap = 1 );
        limited_queue ( size_type, std::initializer_list<value_type> );

        reference front();
        const_reference front() const;

        reference back();
        const_reference back() const;

        size_type try_push( const_reference );
        //size_type try_push( value_type&& );

        reference pop_front();
        const_reference pop_front() const;

        bool empty() const;
        size_type size() const;

        size_type capacity() const;
        void capacity( size_type );
};

template <typename T, typename Container>
limited_queue<T, Container>::limited_queue( size_type c )
:
    queue {},
    cap { c }
{}

template <typename T, typename Container>
limited_queue<T, Container>::limited_queue
( size_type c, std::initializer_list<value_type> l )
:
    queue { Container { c } },
    cap { cap }
{
    // невошедшие элементы отбрасываются
    for ( size_type i = 0; i != capacity; ++ i )
        queue.push( l[i] );
}

template <typename T, typename Container>
typename limited_queue<T, Container>::reference
limited_queue<T, Container>::front()
{
    return queue.front();
}

template <typename T, typename Container>
typename limited_queue<T, Container>::const_reference
limited_queue<T, Container>::front() const
{
    return queue.front();
}

template <typename T, typename Container>
typename limited_queue<T, Container>::reference
limited_queue<T, Container>::back()
{
    return queue.back();
}

template <typename T, typename Container>
typename limited_queue<T, Container>::const_reference
limited_queue<T, Container>::back() const
{
    return queue.back();
}

template <typename T, typename Container>
typename limited_queue<T, Container>::size_type
limited_queue<T, Container>::try_push( const_reference x )
{
    if ( cap > queue.size() )
    {
        queue.push( x );
        return queue.size();
    }
    return 0;
}

// template <typename T, typename Container>
// typename limited_queue<T, Container>::size_type
// limited_queue<T, Container>::try_push( value_type&& x )
// {
//     return try_push( std::move(x) );
// }

template <typename T, typename Container>
typename limited_queue<T, Container>::reference
limited_queue<T, Container>::pop_front()
{
    reference front = queue.front();
    queue.pop();
    return front;
}

template <typename T, typename Container>
bool limited_queue<T, Container>::empty() const
{
    return queue.empty();
}

template <typename T, typename Container>
typename limited_queue<T, Container>::size_type
limited_queue<T, Container>::size() const
{
    return queue.size();
}

template <typename T, typename Container>
typename limited_queue<T, Container>::size_type
limited_queue<T, Container>::capacity() const
{
    return cap;
}

template <typename T, typename Container>
void limited_queue<T, Container>::capacity( size_type new_cap )
{
    cap = new_cap;
}

#endif  // LIMITED_QUEUE_HPP
