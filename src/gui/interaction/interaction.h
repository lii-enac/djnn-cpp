/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 */

#pragma once

#include "core/control/synchronizer.h"
#include "gui/shape/abstract_gshape.h"

namespace djnn {

typedef enum State
{
    START,
    WAITHYST,
    WAITHYST2,
    DRAGGING,
    RRR,
    RESIZING,
} State;

class AbstractHomography;

class Point;
class Vector
{
    friend Point;

  public:
    Vector (double dx = 0, double dy = 0)
        : _dx (dx), _dy (dy) {}
    Vector (const Point& p1, const Point& p2);
    virtual ~Vector () {}
    double
    dx ()
    {
        return _dx;
    }
    double
    dy ()
    {
        return _dy;
    }
    void
    set_dx (double dx)
    {
        _dx = dx;
    }
    void
    set_dy (double dy)
    {
        _dy = dy;
    }
    double        norm_sq () const;
    double        norm () const;
    static double scalar_product (const Vector& u, const Vector& v);
    static double cross_product (const Vector& u, const Vector& v);
    static Vector plus (const Vector& v1, const Vector& v2);
    static Vector minus (const Vector& v1, const Vector& v2);
    static Vector div (const Vector& v1, double d);

  private:
    double _dx, _dy;
};

class Point
{
    friend Vector;

  public:
    Point (double x = 0, double y = 0)
        : _x (x), _y (y) {}
    Point (const Point& p)
        : _x (p._x), _y (p._y) {}
    virtual ~Point () {}
    void
    set_values (double x, double y)
    {
        _x = x;
        _y = y;
    }
    void
    set_values (const Point& p)
    {
        _x = p._x;
        _y = p._y;
    }
    void
    set_values (Point* p)
    {
        _x = p->_x;
        _y = p->_y;
    }
    Point&
    operator= (const Point& p)
    {
        if (this != &p) {
            _x = p._x;
            _y = p._y;
        }
        return *this;
    }
    double
    x () const
    {
        return _x;
    }
    double
    y () const
    {
        return _y;
    }
    bool
    equals (const Point& p) const
    {
        return (_x == p._x) && (_y == p._y);
    }
    static double distance_sq (const Point& p1, const Point& p2);
    static double distance (const Point& p1, const Point& p2);
    static Vector minus (const Point& p1, const Point& p2);
    static Point  middle (const Point& p1, const Point& p2);

  private:
    double _x;
    double _y;
};

class Matrix4x4
{
  public:
    Matrix4x4 ();
    Matrix4x4 (const Matrix4x4& m);
    Matrix4x4 (double m11, double m12, double m13, double m14,
               double m21, double m22, double m23, double m24,
               double m31, double m32, double m33, double m34,
               double m41, double m42, double m43, double m44);
    virtual ~Matrix4x4 ();
    void right_multiply_by (const Matrix4x4& m);
    void left_multiply_by (const Matrix4x4& m);
    void set_sin_cos (double sina, double cosa);
    void right_translate_by (double tx, double ty);
    void left_translate_by (double tx, double ty);
    double*&
    operator[] (const int i) const
    {
        return _matrix[i];
    }

  private:
    double** _matrix;
};

class AbstractRRR;

class RRR_touch
{
  public:
    RRR_touch (AbstractRRR*, Touch*); //, Action*);
    virtual ~RRR_touch ();
    Point  last_pt, new_pt;
    Touch* t;

  private:
    class MoveTouchAction : public Action
    {
      public:
        MoveTouchAction (CoreProcess* parent, const string& name, RRR_touch* ta)
            : Action (parent, name), ta (ta) {}
        ~MoveTouchAction () {}
        void       impl_activate () override;
        RRR_touch* ta;
    };
    friend class MoveTouchAction;
    MoveTouchAction _move_touch_action;
    Coupling        _cpl;
};

class AbstractRRR : public FatProcess
{
  private:
    class AddTouchAction : public Action
    {
      public:
        AddTouchAction (CoreProcess* parent, const string& name)
            : Action (parent, name) {}
        ~AddTouchAction () {}

        void
        impl_activate () override
        {
            ((AbstractRRR*)get_parent ())->on_press_touch_event ();
        };
    };

    class RemoveTouchAction : public Action
    {
      public:
        RemoveTouchAction (CoreProcess* parent, const string& name)
            : Action (parent, name) {}
        ~RemoveTouchAction () {}
        void
        impl_activate () override
        {
            ((AbstractRRR*)get_parent ())->on_release_touch_event ();
        };
    };

    class MoveTouchAction : public Action
    {
      public:
        MoveTouchAction (CoreProcess* parent, const string& name)
            : Action (parent, name) {}
        ~MoveTouchAction () {}
        void impl_activate () override;
    };

  public:
    AbstractRRR (CoreProcess* parent, const string& name, CoreProcess* shape, CoreProcess* matrix);
    ~AbstractRRR ();
    void         impl_activate () override;
    void         impl_deactivate () override;
    virtual void on_press_touch_event () = 0;
    virtual void on_release_touch_event () = 0;
    virtual void on_move_touch_event (Touch* t) = 0;

  protected:
    State                _current_FSM_state;
    int                  _distHyst;
    CoreProcess*         _shape;
    FatProcess *         _touches_shape, *_added, *_removed;
    AbstractHomography*  _matrix;
    map<int, RRR_touch*> _touches_map_RRR;
    Coupling *           _c_on_add, *_c_on_del;
    AddTouchAction*      _press_touch_action;
    RemoveTouchAction*   _release_touch_action;
    MoveTouchAction*     _move_touch_action;
};

class RRR_2T : public AbstractRRR
{

  public:
    RRR_2T (CoreProcess* parent, const string& name, CoreProcess* shape, CoreProcess* matrix)
        : AbstractRRR (parent, name, shape, matrix) {};
    ~RRR_2T () {};
    void on_press_touch_event () override;
    void on_release_touch_event () override;
    void on_move_touch_event (Touch* t) override;

  private:
    Point _lastPointHyst1, _lastPointHyst2;
    Point _lastPoint1, _lastPoint2;
    int   _touchID1, _touchID2;
};

class RR_T : public AbstractRRR
{

  public:
    RR_T (CoreProcess* parent, const string& name, CoreProcess* shape, CoreProcess* matrix)
        : AbstractRRR (parent, name, shape, matrix) {};
    ~RR_T () {};
    void on_press_touch_event () override;
    void on_release_touch_event () override;
    void on_move_touch_event (Touch* t) override;

  private:
    Point _lastPointHyst1, _lastPointHyst2;
    Point _lastPoint1, _lastPoint2;
    int   _touchID1, _touchID2;
};

} // namespace djnn