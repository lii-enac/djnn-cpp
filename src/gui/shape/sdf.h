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
    double x, y;
    vec2 (double x_, double y_)
        : x (x_), y (y_) {}
    vec2 (double v)
        : x (v), y (v) {}
};

inline const vec2
operator- (const vec2& v1, const vec2& v2) {
    vec2 res = v1;
    res.x -= v2.x;
    res.y -= v2.y;
    return res;
}

inline const vec2
operator/ (const vec2& v1, double d) {
    vec2 res = v1;
    res.x /= d;
    res.y /= d;
    return res;
}

inline const vec2 abs (vec2& v) {
    vec2 res = {::fabs (v.x), ::fabs (v.y)};
    return res;
}

inline double abs (double d) {
    return std::abs (d);
}

inline double min (double d1, double d2) {
    return d1 < d2 ? d1 : d2;
}

inline const vec2 min (const vec2& v, double m) {
    vec2 res = {min (v.x, m), min (v.y, m)};
    return res;
}

inline double max (double d1, double d2) {
    return d1 > d2 ? d1 : d2;
}

inline const vec2 max (const vec2& v, double m) {
    vec2 res = {max (v.x, m), max (v.y, m)};
    return res;
}

inline double length (const vec2& v) {
    return ::hypot (v.x, v.y);
}

inline const vec2 floor (const vec2& v) {
    vec2 res = {::floor (v.x), ::floor (v.y)};
    return res;
}

// inline double sign (double v) {
//     if (v > 0)
//         return 1.0;
//     else if (v < 0)
//         return -1.0;
//     else
//         return 0.0;
// }

// https://stackoverflow.com/a/1903975/2036022
inline double sign (double v) {
    return (v > 0) - (v < 0);
}

inline const vec2 normalize (const vec2& v) {
    auto h = length (v);
    return vec2 (v.x / h, v.y / h);
}

inline int dot (const vec2& v1, const vec2& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

inline float SDF_plane (vec2 p, vec2 p0, vec2 p1) {
    vec2 T = p1 - p0;
    vec2 O = normalize (vec2 (T.y, -T.x));
    return dot (O, p0 - p);
}

inline float SDF_circle (vec2 p, float radius) {
    return length (p) - radius;
}

inline float SDF_box (vec2 p, vec2 size) {
    vec2 d = abs (p) - size;
    return min (max (d.x, d.y), 0.0) + length (max (d, 0.0));
}

// Code by Nicolas Rougier & Inigo Quilez - MIT License
inline float SDF_round_box (vec2 p, vec2 size, float radius) {
    // return SDF_box(p, size) - radius;
    return SDF_box (p, size) - radius;
    // return SDF_fake_box(p, size) - radius;
}

// Code by Inigo Quilez
// See https://www.shadertoy.com/view/4sS3zz
inline float SDF_ellipse (vec2 p, vec2 ab) {
    // The function does not like circles
    // if (ab.x == ab.y) ab.x = ab.x*0.9999;
    if (ab.x == ab.y)
        return SDF_circle (p, ab.x); // [added by SCO]

    p = abs (p);
    if (p.x > p.y) {
        // p=p.yx; ab=ab.yx;
        std::swap (p.x, p.y);
        std::swap (ab.x, ab.y);
    }
    float l  = ab.y * ab.y - ab.x * ab.x;
    float m  = ab.x * p.x / l;
    float n  = ab.y * p.y / l;
    float m2 = m * m;
    float n2 = n * n;
    float c  = (m2 + n2 - 1.0) / 3.0;
    float c3 = c * c * c;
    float q  = c3 + m2 * n2 * 2.0;
    float d  = c3 + m2 * n2;
    float g  = m + m * n2;
    float co;

    if (d < 0.0) {
        float p  = acos (q / c3) / 3.0;
        float s  = cos (p);
        float t  = sin (p) * sqrt (3.0);
        float rx = sqrt (-c * (s + t + 2.0) + m2);
        float ry = sqrt (-c * (s - t + 2.0) + m2);
        co       = (ry + sign (l) * rx + abs (g) / (rx * ry) - m) / 2.0;
    } else {
        float h  = 2.0 * m * n * sqrt (d);
        float s  = sign (q + h) * pow (abs (q + h), 1.0 / 3.0);
        float u  = sign (q - h) * pow (abs (q - h), 1.0 / 3.0);
        float rx = -s - u - c * 4.0 + 2.0 * m2;
        float ry = (s - u) * sqrt (3.0);
        float rm = sqrt (rx * rx + ry * ry);
        float p  = ry / sqrt (rm - rx);
        co       = (p + 2.0 * g / rm - m) / 2.0;
    }
    float si = sqrt (1.0 - co * co);
    vec2  r  = vec2 (ab.x * co, ab.y * si);
    return length (r - p) * sign (p.y - r.y);
}
} // namespace djnn
