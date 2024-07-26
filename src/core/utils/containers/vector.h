#pragma once

#if !DJNN_STL_STD && !DJNN_STL_EASTL && !DJNN_STL_DJNN
#define DJNN_STL_STD 1
#endif

#if DJNN_STL_STD

#include <vector>

namespace djnnstl {
template <typename T>
using vector = std::vector<T>;
}

#elif DJNN_STL_EASTL

#include <EASTL/vector.h>

namespace djnnstl = eastl;

#elif DJNN_STL_DJNN

#include <initializer_list>
// #include <iterator> // prev

#include <stddef.h> // size_t

namespace djnnstl {
template <typename T>
class vector_impl_t;

template <typename T>
class vector {
  public:
    using value_type     = T;
    using iterator       = T*;
    using const_iterator = const T*;

    vector ();
    vector (std::initializer_list<T> init);
    ~vector ();
    bool   empty () const;
    size_t size () const;
    void   clear ();
    void   push_back (const T&);
    void   pop_back ();

    iterator       insert (const_iterator pos, const T& value);
    iterator       erase (iterator pos);
    const_iterator erase (const_iterator pos);
    iterator       erase (iterator first, iterator last);
    iterator       erase (const_iterator first, const_iterator last);
    iterator       begin ();
    iterator       end ();
    const_iterator begin () const;
    const_iterator end () const;
    iterator       rbegin ();
    iterator       rend ();
    const_iterator rbegin () const;
    const_iterator rend () const;

    T& operator[] (size_t);
    const T& operator[] (size_t) const;
    T&       at (size_t);
    const T& at (size_t) const;
    T&       back ();
    const T& back () const;

  private:
    vector_impl_t<T>* impl;
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
