/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019-2024)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include <cmath>

#include "interaction.h"

namespace djnn {
Matrix4x4::Matrix4x4 ()
{
    _matrix = new double*[4];
    for (int i = 0; i < 4; i++) {
        _matrix[i] = new double[4];
        for (int j = 0; j < 4; j++) {
            if (i == j)
                _matrix[i][j] = 1;
            else
                _matrix[i][j] = 0;
        }
    }
}

Matrix4x4::Matrix4x4 (double m11, double m12, double m13, double m14,
                      double m21, double m22, double m23, double m24,
                      double m31, double m32, double m33, double m34,
                      double m41, double m42, double m43, double m44)
{
    _matrix = new double*[4];
    for (int i = 0; i < 4; i++)
        _matrix[i] = new double[4];

    _matrix[0][0] = m11;
    _matrix[0][1] = m12;
    _matrix[0][2] = m13;
    _matrix[0][3] = m14;
    _matrix[1][0] = m21;
    _matrix[1][1] = m22;
    _matrix[1][2] = m23;
    _matrix[1][3] = m24;
    _matrix[2][0] = m31;
    _matrix[2][1] = m32;
    _matrix[2][2] = m33;
    _matrix[2][3] = m34;
    _matrix[3][0] = m41;
    _matrix[3][1] = m42;
    _matrix[3][2] = m43;
    _matrix[3][3] = m44;
}

Matrix4x4::Matrix4x4 (const Matrix4x4& m)
{
    _matrix = new double*[4];
    for (int i = 0; i < 4; i++)
        _matrix[i] = new double[4];

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            _matrix[i][j] = m._matrix[i][j];
}

Matrix4x4::~Matrix4x4 ()
{
    for (int i = 0; i < 4; i++)
        delete _matrix[i];
    delete _matrix;
}

void
Matrix4x4::left_multiply_by (const Matrix4x4& m)
{
    Matrix4x4 buff; // Identity
    // Carefull: Initialize a zero matrix not identity
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            buff[i][j] = 0;
        }
    }

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                buff[i][j] += m[i][k] * _matrix[k][j];
            }
        }
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            _matrix[i][j] = buff[i][j];
        }
}

void
Matrix4x4::right_multiply_by (const Matrix4x4& m)
{
    Matrix4x4 buff; // Identity
    // Carefull: Initialize a zero matrix not identity
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            buff[i][j] = 0;
        }
    }

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                buff[i][j] += _matrix[i][k] * m[k][j];
            }
        }
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            _matrix[i][j] = buff[i][j];
        }
}

void
Matrix4x4::set_sin_cos (double sina, double cosa)
{
    _matrix[0][0] = cosa;
    _matrix[0][1] = -sina;
    _matrix[1][0] = sina;
    _matrix[1][1] = cosa;
}

void
Matrix4x4::right_translate_by (double tx, double ty)
{
    Matrix4x4 trans;
    trans._matrix[0][0] = 1;
    trans._matrix[1][1] = 1;
    trans._matrix[2][2] = 1;
    trans._matrix[3][3] = 1;

    trans._matrix[0][3] = tx;
    trans._matrix[1][3] = ty;
    right_multiply_by (trans);
}

void
Matrix4x4::left_translate_by (double tx, double ty)
{
    Matrix4x4 trans;
    trans._matrix[0][0] = 1;
    trans._matrix[1][1] = 1;
    trans._matrix[2][2] = 1;
    trans._matrix[3][3] = 1;

    trans._matrix[0][3] = tx;
    trans._matrix[1][3] = ty;
    left_multiply_by (trans);
}

Vector::Vector (const Point& p1, const Point& p2)
    : _dx (p2._x - p1._x), _dy (p2._y - p1._y)
{
}

double
Vector::norm_sq () const
{
    return _dx * _dx + _dy * _dy;
}

double
Vector::norm () const
{
    return sqrt (norm_sq ());
}

double
Vector::scalar_product (const Vector& u, const Vector& v)
{
    return u._dx * v._dx + u._dy * v._dy;
}
double
Vector::cross_product (const Vector& u, const Vector& v)
{
    return u._dx * v._dy - u._dy * v._dx;
}
Vector
Vector::plus (const Vector& v1, const Vector& v2)
{
    double dx = (v1._dx + v2._dx);
    double dy = (v1._dy + v2._dy);
    return Vector (dx, dy);
}
Vector
Vector::minus (const Vector& v1, const Vector& v2)
{
    double dx = (v1._dx - v2._dx);
    double dy = (v1._dy - v2._dy);
    return Vector (dx, dy);
}
Vector
Vector::div (const Vector& v1, double d)
{
    double dx = (v1._dx / d);
    double dy = (v1._dy / d);
    return Vector (dx, dy);
}

double
Point::distance_sq (const Point& p1, const Point& p2)
{
    return Vector (p1, p2).norm_sq ();
}
double
Point::distance (const Point& p1, const Point& p2)
{
    return sqrt (distance_sq (p1, p2));
}
Vector
Point::minus (const Point& p1, const Point& p2)
{
    double dx = (p1._x - p2._x);
    double dy = (p1._y - p2._y);
    return Vector (dx, dy);
}
Point
Point::middle (const Point& p1, const Point& p2)
{
    double x = (p1._x + p2._x) / 2;
    double y = (p1._y + p2._y) / 2;
    return Point (x, y);
}

} /* namespace djnn */
