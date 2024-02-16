/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "core/property/abstract_property.h"

inline double
operator+ (const djnn::AbstractSimpleProperty& l, const djnn::AbstractSimpleProperty& r) {
    return l.get_double_value () + r.get_double_value ();
}

inline double
operator+ (const djnn::AbstractSimpleProperty& l, double r) {
    return l.get_double_value () + r;
}

inline double
operator+ (double l, const djnn::AbstractSimpleProperty& r) {
    return r + l;
}

inline const djnnstl::string
operator+ (const djnnstl::string& l, const djnn::AbstractSimpleProperty& r) {
    return l + r.get_string_value ();
}

inline const djnnstl::string
operator+ (const djnn::AbstractSimpleProperty& l, const djnnstl::string& r) {
    return l.get_string_value () + r;
}

inline double
operator- (const djnn::AbstractSimpleProperty& l) {
    return -l.get_double_value ();
}

inline double
operator- (const djnn::AbstractSimpleProperty& l, const djnn::AbstractSimpleProperty& r) {
    return l.get_double_value () - r.get_double_value ();
}

inline double
operator- (const djnn::AbstractSimpleProperty& l, double r) {
    return l.get_double_value () - r;
}

inline double
operator- (double l, const djnn::AbstractSimpleProperty& r) {
    return l - r.get_double_value ();
}

inline double
operator* (const djnn::AbstractSimpleProperty& l, const djnn::AbstractSimpleProperty& r) {
    return l.get_double_value () * r.get_double_value ();
}

inline double
operator* (const djnn::AbstractSimpleProperty& l, double r) {
    return l.get_double_value () * r;
}

inline double
operator* (double l, const djnn::AbstractSimpleProperty& r) {
    return r * l;
}

inline double
operator/ (const djnn::AbstractSimpleProperty& l, const djnn::AbstractSimpleProperty& r) {
    return l.get_double_value () / r.get_double_value ();
}

inline double
operator/ (const djnn::AbstractSimpleProperty& l, double r) {
    return l.get_double_value () / r;
}

inline double
operator/ (double l, const djnn::AbstractSimpleProperty& r) {
    return l / r.get_double_value ();
}

inline bool
operator&& (const djnn::AbstractSimpleProperty& l, const djnn::AbstractSimpleProperty& r) {
    return (int)l.get_double_value () && (int)r.get_double_value ();
}

inline bool
operator&& (const djnn::AbstractSimpleProperty& l, bool r) {
    return (int)l.get_double_value () && r;
}

inline bool
operator&& (bool l, const djnn::AbstractSimpleProperty& r) {
    return r && l;
}

inline bool
operator|| (const djnn::AbstractSimpleProperty& l, const djnn::AbstractSimpleProperty& r) {
    return (int)l.get_double_value () || (int)r.get_double_value ();
}

inline bool
operator|| (const djnn::AbstractSimpleProperty& l, bool r) {
    return (int)l.get_double_value () || r;
}

inline bool
operator|| (bool l, const djnn::AbstractSimpleProperty& r) {
    return r || l;
}

inline bool
operator!(const djnn::AbstractSimpleProperty& p) {
    return !(int)p.get_double_value ();
}

inline bool
operator== (const djnn::AbstractSimpleProperty& l, const djnn::AbstractSimpleProperty& r) {
    return l.get_double_value () == r.get_double_value ();
}

inline bool
operator== (const djnn::AbstractSimpleProperty& l, double r) {
    return l.get_double_value () == r;
}

inline bool
operator== (double l, const djnn::AbstractSimpleProperty& r) {
    return r == l;
}

inline bool
operator!= (const djnn::AbstractSimpleProperty& l, const djnn::AbstractSimpleProperty& r) {
    return l.get_double_value () != r.get_double_value ();
}

inline bool
operator!= (const djnn::AbstractSimpleProperty& l, double r) {
    return l.get_double_value () != r;
}

inline bool
operator!= (double l, const djnn::AbstractSimpleProperty& r) {
    return r != l;
}

inline bool
operator> (const djnn::AbstractSimpleProperty& l, const djnn::AbstractSimpleProperty& r) {
    return l.get_double_value () > r.get_double_value ();
}

inline bool
operator> (const djnn::AbstractSimpleProperty& l, double r) {
    return l.get_double_value () > r;
}

inline bool
operator> (double l, const djnn::AbstractSimpleProperty& r) {
    return l > r.get_double_value ();
}

inline bool
operator< (const djnn::AbstractSimpleProperty& l, const djnn::AbstractSimpleProperty& r) {
    return r > l;
}

inline bool
operator< (const djnn::AbstractSimpleProperty& l, double r) {
    return r > l;
}

inline bool
operator< (double l, const djnn::AbstractSimpleProperty& r) {
    return r > l;
}

inline bool
operator>= (const djnn::AbstractSimpleProperty& l, const djnn::AbstractSimpleProperty& r) {
    return l.get_double_value () >= r.get_double_value ();
}

inline bool
operator>= (const djnn::AbstractSimpleProperty& l, double r) {
    return l.get_double_value () >= r;
}

inline bool
operator>= (double l, const djnn::AbstractSimpleProperty& r) {
    return l >= r.get_double_value ();
}

inline bool
operator<= (const djnn::AbstractSimpleProperty& l, const djnn::AbstractSimpleProperty& r) {
    return r >= l;
}

inline bool
operator<= (const djnn::AbstractSimpleProperty& l, double r) {
    return r >= l;
}

inline bool
operator<= (double l, const djnn::AbstractSimpleProperty& r) {
    return r >= l;
}

inline int
operator++ (djnn::AbstractSimpleProperty& p) {
    return (int)p.get_double_value () + 1;
}

inline int
operator-- (djnn::AbstractSimpleProperty& p) {
    return (int)p.get_double_value () - 1;
}
