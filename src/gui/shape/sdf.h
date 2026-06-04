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

struct vec3 {
    double x, y, z;
    vec3 (double x_, double y_, double z_)
        : x (x_), y (y_), z (z_) {}
    vec3 (double x_, double y_)
        : x (x_), y (y_) {}
    vec3 (double v)
        : x (v), y (v) {}
};

struct bvec3 {
    double x,y,z;
    bvec3 (double x_, double y_, double z_)
        : x (x_), y (y_), z (z_) {}
};

inline const vec2
operator+ (const vec2& v1, const vec2& v2) {
    vec2 res = v1;
    res.x += v2.x;
    res.y += v2.y;
    return res;
}

inline const vec2
operator- (const vec2& v1, const vec2& v2) {
    vec2 res = v1;
    res.x -= v2.x;
    res.y -= v2.y;
    return res;
}

inline const vec3
operator- (const vec3& v1, const vec3& v2) {
    vec3 res = v1;
    res.x -= v2.x;
    res.y -= v2.y;
    res.z -= v2.z;
    return res;
}

inline const vec2
operator/ (const vec2& v1, double d) {
    vec2 res = v1;
    res.x /= d;
    res.y /= d;
    return res;
}

inline const vec2 operator*(double d, const vec2& v)
{
    return vec2(d*v.x, d*v.y);
}
inline const vec2 operator*(const vec2& v, double d)
{
    return vec2(d*v.x, d*v.y);
}

inline const vec2 operator*(const vec2& v1, const vec2& v2)
{
    return vec2(v1.x*v2.x, v1.y*v2.y);
}

inline const vec3 operator*(const vec3& v, double d)
{
    return vec3(d*v.x, d*v.y, d*v.z);
}

inline const vec2 abs (vec2& v) {
    vec2 res = {::fabs (v.x), ::fabs (v.y)};
    return res;
}

inline const vec2 pow (vec2 v, vec2 p) {
    vec2 res = {std::pow(v.x, p.x), std::pow(v.y, p.y)};
    return res;
}

inline double pow (double v, double p) {
    return std::pow(v, p);
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

inline double clamp(double v, double min_, double max_) {
    return v < min_ ? min_ : v > max_ ? max_ : v;
}

inline vec3 clamp(vec3 v, double min_, double max_) {
    return vec3(clamp(v.x, min_, max_),clamp(v.y, min_, max_),clamp(v.z, min_, max_));
}

inline double all(bvec3 v) {
    return v.x && v.y && v.z;
}

inline bvec3 not_(bvec3 v) {
    return bvec3(!v.x, !v.y, !v.z);
}

inline double dot2(const vec2 v) {
    return v.x*v.x+v.y*v.y;
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

inline const vec2 sign (vec2 v) {
    vec2 res = {sign(v.x), sign(v.y)};
    return res;
}

inline const vec2 normalize (const vec2& v) {
    auto h = length (v);
    return vec2 (v.x / h, v.y / h);
}

inline double dot (const vec2& v1, const vec2& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}




inline double SDF_plane (vec2 p, vec2 p0, vec2 p1) {
    vec2 T = p1 - p0;
    vec2 O = normalize (vec2 (T.y, -T.x));
    return dot (O, p0 - p);
}

inline double SDF_circle (vec2 p, double radius) {
    return length (p) - radius;
}

inline double SDF_box (vec2 p, vec2 size) {
    vec2 d = abs (p) - size;
    return min (max (d.x, d.y), 0.0) + length (max (d, 0.0));
}

// Code by Nicolas Rougier & Inigo Quilez - MIT License
inline double SDF_round_box (vec2 p, vec2 size, double radius) {
    // return SDF_box(p, size) - radius;
    return SDF_box (p, size) - radius;
    // return SDF_fake_box(p, size) - radius;
}

// Code by Inigo Quilez
// See https://www.shadertoy.com/view/4sS3zz
inline double SDF_ellipse (vec2 p, vec2 ab) {
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
    double l  = ab.y * ab.y - ab.x * ab.x;
    double m  = ab.x * p.x / l;
    double n  = ab.y * p.y / l;
    double m2 = m * m;
    double n2 = n * n;
    double c  = (m2 + n2 - 1.0) / 3.0;
    double c3 = c * c * c;
    double q  = c3 + m2 * n2 * 2.0;
    double d  = c3 + m2 * n2;
    double g  = m + m * n2;
    double co;

    if (d < 0.0) {
        double p  = acos (q / c3) / 3.0;
        double s  = cos (p);
        double t  = sin (p) * sqrt (3.0);
        double rx = sqrt (-c * (s + t + 2.0) + m2);
        double ry = sqrt (-c * (s - t + 2.0) + m2);
        co       = (ry + sign (l) * rx + abs (g) / (rx * ry) - m) / 2.0;
    } else {
        double h  = 2.0 * m * n * sqrt (d);
        double s  = sign (q + h) * pow (abs (q + h), 1.0 / 3.0);
        double u  = sign (q - h) * pow (abs (q - h), 1.0 / 3.0);
        double rx = -s - u - c * 4.0 + 2.0 * m2;
        double ry = (s - u) * sqrt (3.0);
        double rm = sqrt (rx * rx + ry * ry);
        double p  = ry / sqrt (rm - rx);
        co       = (p + 2.0 * g / rm - m) / 2.0;
    }
    double si = sqrt (1.0 - co * co);
    vec2  r  = vec2 (ab.x * co, ab.y * si);
    return length (r - p) * sign (p.y - r.y);
}

// https://iquilezles.org/articles/distfunctions2d/
inline double SDF_polygon(vec2 v[], int N, vec2 p)
{
    double d = dot(p-v[0],p-v[0]);
    double s = 1.0;
    for( int i=0, j=N-1; i<N; j=i, i++ )
    {
        vec2 e = v[j] - v[i];
        vec2 w =    p - v[i];
        vec2 b = w - e * clamp( dot(w,e)/dot(e,e), 0.0, 1.0 );
        d = min( d, dot(b,b) );
        bvec3 c = bvec3(p.y>=v[i].y,p.y<v[j].y,e.x*w.y>e.y*w.x);
        if( all(c) || all(not_(c)) ) s*=-1.0;
    }
    return s*sqrt(d);
}

inline double SDF_quadratic_bezier(vec2 pos, vec2 A, vec2 B, vec2 C )
{
    vec2 a = B - A;
    vec2 b = A - 2.0*B + C;
    vec2 c = a * 2.0;
    vec2 d = A - pos;
    double kk = 1.0/dot(b,b);
    double kx = kk * dot(a,b);
    double ky = kk * (2.0*dot(a,a)+dot(d,b)) / 3.0;
    double kz = kk * dot(d,a);
    double res = 0.0;
    double p = ky - kx*kx;
    double p3 = p*p*p;
    double q = kx*(2.0*kx*kx-3.0*ky) + kz;
    double h = q*q + 4.0*p3;
    if( h >= 0.0)
    { 
        h = sqrt(h);
        vec2 x = (vec2(h,-h)-q)/2.0;
        vec2 uv = sign(x)*pow(abs(x), vec2(1.0/3.0));
        double t = clamp( uv.x+uv.y-kx, 0.0, 1.0 );
        res = dot2(d + (c + b*t)*t);
    }
    else
    {
        double z = sqrt(-p);
        double v = acos( q/(p*z*2.0) ) / 3.0;
        double m = cos(v);
        double n = sin(v)*1.732050808;
        vec3  t = clamp(vec3(m+m,-n-m,n-m)*z-kx,0.0,1.0);
        res = min( dot2(d+(c+b*t.x)*t.x),
                   dot2(d+(c+b*t.y)*t.y) );
        // the third root cannot be the closest
        // res = min(res,dot2(d+(c+b*t.z)*t.z));
    }
    return sqrt( res );
}

inline
double SDF_arc(vec2 p, vec2 sc, double ra, double rb )
{
    // sc is the sin/cos of the arc's aperture
    p.x = abs(p.x);
    return ((sc.y*p.x>sc.x*p.y) ? length(p-sc*ra) :
                                  abs(length(p)-ra)) - rb;
}

} // namespace djnn
