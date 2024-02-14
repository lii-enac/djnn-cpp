#pragma once

#if !DJNN_STL_STD && !DJNN_STL_EASTL && !DJNN_STL_DJNN
#define DJNN_STL_STD 1
#endif

#if DJNN_STL_STD

#include <list>
#include <map>
#include <string>
#include <vector>

namespace djnnstl {
using std::list;
using std::make_pair;
using std::map;
using std::pair;
using std::prev;
using std::string;
using std::to_string;
using std::vector;
} // namespace djnnstl

#elif DJNN_STL_EASTL

#include <EASTL/list.h>
#include <EASTL/map.h>
#include <EASTL/string.h>
#include <EASTL/vector.h>

namespace djnnstl = eastl;

extern int    stoi (const eastl::string&);
extern int    stoi (const eastl::string&, size_t* idx, int base = 10);
extern double stof (const eastl::string&);
extern double stof (const eastl::string&, size_t* idx, int base = 10);

#elif DJNN_STL_DJNN

// #include <string>
// #include <vector>

// #include <map>
// #include <list>

#include <initializer_list>
// #include <iterator> // prev

#include <stddef.h> // size_t

namespace djnnstl {
template <typename T>
class string_impl_t;

// template <typename T>
class string
{
  public:
    string (const char*);
    using T              = char;
    using value_type     = T;
    using iterator       = T*;
    using const_iterator = const T*;
    static size_t npos;
    using size_type = size_t;

    string ();
    // string(std::initializer_list<T> init);
    string (value_type, size_type);
    string (const value_type*, size_type);
    string (const_iterator, const_iterator);
    ~string ();
    bool              empty () const;
    size_t            size () const;
    void              resize (size_t);
    size_t            length () const;
    void              clear ();
    const value_type* c_str () const;
    const value_type* data () const;
    T& operator[] (size_t);
    const T& operator[] (size_t) const;
    T&             at (size_t);
    const T&       at (size_t) const;
    string         substr (size_t) const;
    string         substr (size_t, size_t) const;
    size_t         find (const string&) const;
    size_t         find (value_type) const;
    size_t         find (value_type, size_type) const;
    size_t         find (const value_type*, size_type) const;
    size_t         find_first_of (value_type) const;
    size_t         find_first_not_of (value_type) const;
    size_t         find_last_not_of (value_type) const;
    size_t         rfind (const string&, size_type = 0) const;
    size_t         rfind (value_type) const;
    size_t         rfind (value_type, size_type) const;
    size_t         rfind_first_of (value_type) const;
    size_t         rfind_first_not_of (value_type) const;
    size_t         rfind_last_not_of (value_type) const;
    int            compare (const char*) const;
    int            compare (const string&) const;
    iterator       begin ();
    iterator       end ();
    const_iterator begin () const;
    const_iterator end () const;
    // iterator rbegin ();
    // iterator rend ();
    // const_iterator rbegin () const;
    // const_iterator rend () const;
    T&       back ();
    const T& back () const;
    void     append (const char*);
    void     append (const string&);
    iterator insert (size_type, const string&);
    void     erase (size_type);
    void     erase (size_type, size_type);
    void     erase (iterator, iterator);
    void     push_back (const string&);
    void     pop_back ();
    string&  operator+= (const char*);
    string&  operator+= (const string&);
    string& operator+= (value_type);
};

string operator+ (const string&, const string&);
bool   operator< (const string&, const string&);
string operator+ (const char*, const string&);
string operator+ (const string&, const char*);
string operator+ (char, const string&);
string operator+ (const string&, char);
bool   operator== (const string&, const string&);
bool   operator== (const char*, const string&);
bool   operator== (const string&, const char*);
bool   operator!= (const string&, const string&);
bool   operator!= (const char*, const string&);
bool   operator!= (const string&, const char*);

int stoi (const string&, size_t* pos = nullptr, int base = 10);
int stof (const string&);

class ostream;
ostream& operator<< (ostream&, const string&);

template <typename T>
class vector_impl_t;

template <typename T>
class vector
{
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

template <typename T>
class list_impl_t;

template <typename T>
class list
{
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
class pair
{
  public:
    pair ();
    pair (const T1& t1, const T2& t2) : first (t1), second (t2) {}
    T1 first;
    T2 second;
};

template <typename T1, typename T2>
pair<T1, T2> make_pair (const T1&, const T2&);

template <typename Key, typename T>
class map
{
  public:
    using key_type   = T;
    using value_type = pair<Key, T>;
    struct iterator
    {
        value_type     operator* ();
        value_type*    operator->();
        bool           operator!= (const iterator&);
        iterator&      operator++ ();
        const iterator operator++ (int);
        bool           operator== (const iterator&);
    };
    // using const_iterator = const iterator;
    struct const_iterator
    {
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
