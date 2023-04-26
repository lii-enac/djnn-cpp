#pragma once

#if !DJNN_STL_STD && !DJNN_STL_EASTL && !DJNN_STL_DJNN
    #define DJNN_STL_STD 1
#endif

#if DJNN_STL_STD

#include <string>

namespace djnnstl {
  using std::string;
  using std::to_string;
}

#elif DJNN_STL_EASTL

#include <EASTL/string.h>

namespace djnnstl = eastl;

extern int stoi(const eastl::string&);
extern int stoi(const eastl::string&, size_t* idx, int base = 10);
extern double stof(const eastl::string&);
extern double stof(const eastl::string&, size_t* idx, int base = 10);

#elif DJNN_STL_DJNN


#include <initializer_list>
//#include <iterator> // prev

#include <stddef.h> // size_t

namespace djnnstl {
  template <typename T>
  class string_impl_t;

  //template <typename T>
  class string {
    public:
    string(const char*);
    using T = char;
    using value_type = T;
    using iterator = T*;
    using const_iterator = const T*;
    static size_t npos;
    using size_type = size_t;
    
    string();
    //string(std::initializer_list<T> init);
    string(value_type, size_type);
    string(const value_type*, size_type);
    string(const_iterator, const_iterator);
    ~string();
    bool empty() const;
    size_t size() const;
    void resize(size_t);
    size_t length() const;
    void clear ();
    const value_type* c_str() const;
    const value_type* data() const;
    T& operator[] (size_t);
    const T& operator[] (size_t) const;
    T& at (size_t);
    const T& at (size_t) const;
    string substr(size_t) const;
    string substr(size_t, size_t) const;
    size_t find(const string&) const;
    size_t find(value_type) const;
    size_t find(value_type, size_type) const;
    size_t find(const value_type*, size_type) const;
    size_t find_first_of(value_type) const;
    size_t find_first_not_of(value_type) const;
    size_t find_last_not_of(value_type) const;
    size_t rfind(const string&, size_type=0) const;
    size_t rfind(value_type) const;
    size_t rfind(value_type, size_type) const;
    size_t rfind_first_of(value_type) const;
    size_t rfind_first_not_of(value_type) const;
    size_t rfind_last_not_of(value_type) const;
    int compare(const char*) const;
    int compare(const string&) const;
    iterator begin ();
    iterator end ();
    const_iterator begin () const;
    const_iterator end () const;
    // iterator rbegin ();
    // iterator rend ();
    // const_iterator rbegin () const;
    // const_iterator rend () const;
    T& back ();
    const T& back () const;
    void append(const char*);
    void append(const string&);
    iterator insert(size_type, const string&);
    void erase(size_type);
    void erase(size_type, size_type);
    void erase(iterator, iterator);
    void push_back (const string&);
    void pop_back ();
    string& operator += (const char*);
    string& operator += (const string&);
    string& operator += (value_type);
};

string operator+(const string&, const string&);
bool operator<(const string&, const string&);
string operator+(const char*, const string&);
string operator+(const string&, const char*);
string operator+(char, const string&);
string operator+(const string&, char);
bool operator==(const string&, const string&);
bool operator==(const char*, const string&);
bool operator==(const string&, const char*);
bool operator!=(const string&, const string&);
bool operator!=(const char*, const string&);
bool operator!=(const string&, const char*);

int stoi(const string&, size_t* pos = nullptr, int base = 10 );
int stof(const string&);

class ostream;
ostream& operator<<(ostream&, const string&);

  //using std::make_pair;
}


#endif


