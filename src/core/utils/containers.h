#pragma once

#if !DJNN_STL_STD && !DJNN_STL_EASTL && !DJNN_STL_DJNN
#define DJNN_STL_STD 1
#endif

#include "containers/string.h"
#include "containers/vector.h"

#if DJNN_STL_STD

#include <list>
#include <map>

namespace djnnstl {
using std::list;
using std::make_pair;
using std::map;
using std::pair;
using std::prev;
} // namespace djnnstl

#elif DJNN_STL_EASTL

#include <EASTL/list.h>
#include <EASTL/map.h>

namespace djnnstl = eastl;

#elif DJNN_STL_DJNN

// #include <map>
// #include <list>

#include <initializer_list>
// #include <iterator> // prev

#include <stddef.h> // size_t

namespace djnnstl {

template <typename T>
class list_impl_t;

template <typename T>
class list {
  public:
    using value_type     = T;
    using iterator       = T*;
    using const_iterator = const T*;

    list ();
    ~list ();

    size_t size () const;
    void   clear ();
    void   push_back (const T&);
    void   push_front (const T&);

    iterator erase (iterator pos);

    iterator       begin ();
    iterator       end ();
    const_iterator begin () const;
    const_iterator end () const;

  private:
    list_impl_t<T>* impl;
};

template <typename Key, typename T>
class map_impl_t;

template <typename T1, typename T2>
class pair {
  public:
    pair ();
    pair (const T1& t1, const T2& t2) : first (t1), second (t2) {}
    T1 first;
    T2 second;
};

template <typename T1, typename T2>
pair<T1, T2> make_pair (const T1&, const T2&);

template <typename Key, typename T>
class map {
  public:
    using key_type   = T;
    using value_type = pair<Key, T>;
    struct iterator {
        value_type     operator* ();
        value_type*    operator->();
        bool           operator!= (const iterator&);
        iterator&      operator++ ();
        const iterator operator++ (int);
        bool           operator== (const iterator&);
    };
    // using const_iterator = const iterator;
    struct const_iterator {
        const value_type&    operator* ();
        const value_type*    operator->();
        bool                 operator!= (const const_iterator&);
        const_iterator&      operator++ ();
        const const_iterator operator++ (int);
        bool                 operator== (const const_iterator&);
    };

    // using iterator = value_type*;
    // using const_iterator = const T*;

    map ();
    map (std::initializer_list<value_type> init);
    ~map ();

    bool   empty () const;
    size_t size () const;
    void   clear ();
    // void push_back (const T&);
    // void push_front (const T&);

    iterator       insert (const value_type&);
    iterator       insert (iterator, const value_type&);
    iterator       insert (iterator, iterator);
    const_iterator insert (const_iterator, const value_type&) const;

    iterator       erase (iterator pos);
    const_iterator erase (const_iterator pos);
    iterator       erase (const Key& key);

    iterator       begin ();
    iterator       end ();
    const_iterator begin () const;
    const_iterator end () const;
    const_iterator cbegin () const;
    const_iterator cend () const;

    iterator       rbegin ();
    iterator       rend ();
    const_iterator rbegin () const;
    const_iterator rend () const;

    iterator       find (const Key& key);
    const_iterator find (const Key& key) const;
    T&             operator[] (const Key& key);

  private:
    map_impl_t<Key, T>* impl;
};

} // namespace djnnstl

// namespace djnnstl = djnn_stl;

namespace djnnstl {
// using djnn::string;
// using djnnstl::to_string;
// using djnnstl::vector;
// using std::vector;
// using djnnstl::list;
// using std::list;
// using djnnstl::map;
// using std::map;
// using djnnstl::pair;
// using std::pair;
// using djnnstl::prev;
// using std::prev;
// using djnnstl::make_pair;
// using std::make_pair;
}

#endif
