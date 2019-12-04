#pragma once
/*
 *  gl2d
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include <cmath>


// code compatible with glsl

namespace djnn {

  struct vec2 {
    double x,y;
    vec2(double x_, double y_) : x(x_), y(y_) {}
  };

  inline
  const vec2
  operator-(const vec2& v1, const vec2& v2)
  {
    vec2 res = v1;
    res.x -= v2.x;
    res.y -= v2.y;
    return res;
  }

  inline
  const vec2
  operator/(const vec2& v1, double d)
  {
    vec2 res = v1;
    res.x /= d;
    res.y /= d;
    return res;
  }

  inline
  const vec2 abs(vec2& v)
  {
    vec2 res = {::abs(v.x), ::abs(v.y)};
    return res;
  }

  inline
  double abs(double d)
  {
    return ::abs(d);
  }

  inline
  double max(double d1, double d2)
  {
    return d1 > d2 ? d1 : d2;
  }

  inline
  const vec2 max(const vec2& v, double m)
  {
    vec2 res = {max(v.x, m), max(v.y, m)};
    return res;
  }

  inline
  double length(const vec2& v)
  {
    return ::hypot(v.x, v.y);
  }

  inline
  const vec2 floor(const vec2& v)
  {
    vec2 res = {::floor(v.x), ::floor(v.y)};
    return res;
  }

  inline
  float SDF_circle(vec2 p, float radius)
  {
    return length(p) - radius;
  }

  inline
  float SDF_box(vec2 p, vec2 size)
  {
    vec2 d = abs(p) - size;
    return min(max(d.x,d.y),0.0) + length(max(d,0.0));
  }

  // Code by Nicolas Rougier & Inigo Quilez - MIT License
  inline
  float SDF_round_box(vec2 p, vec2 size, float radius)
  {
    //return SDF_box(p, size) - radius;
    return SDF_box(p, size) - radius;
    //return SDF_fake_box(p, size) - radius;
  }
}
