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
 */

#pragma once

#include "../abstract_gshape.h"
#include "../transformation/transformations.h"
#include "../../core/control/synchronizer.h"

namespace djnn {

  typedef enum State {
    START, DRAGGING, RRR
  } State;

  void estimateTSR (const vector< pair<double, double> > &old_pts, const vector< pair<double, double> > &new_pts, double *dx, double *dy, double *ds, double *dr);

  class Point;
  class Vector
  {
    friend Point;
  public:
    Vector (double dx = 0, double dy = 0) : _dx (dx), _dy (dy) {}
    Vector (const Point &p1, const Point &p2);
    virtual ~Vector () {}
    double dx () { return _dx; }
    double dy () { return _dy; }
    void set_dx (double dx) { _dx = dx; }
    void set_dy (double dy) { _dy = dy; }
    double norm_sq () const;
    double norm () const;
    static double scalar_product (const Vector &u, const Vector &v);
    static double cross_product (const Vector &u, const Vector &v);
    static Vector plus (const Vector &v1, const Vector &v2);
    static Vector minus (const Vector &v1, const Vector &v2);
    static Vector div (const Vector &v1, double d);
  private:
    double _dx, _dy;
  };

  class Point {
    friend Vector;
  public:
    Point (double x=0, double y=0) : _x (x), _y (y) {}
    Point (const Point &p) : _x (p._x), _y (p._y) {}
    virtual ~Point() {}
    void set_values (double x, double y) { _x = x; _y = y; }
    void set_values (const Point &p) { _x = p._x; _y = p._y; }
    void set_values (Point *p) { _x = p->_x; _y = p->_y; }
    double x() const { return _x; }
    double y() const { return _y; }
    bool equals (const Point &p) const { return (_x == p._x) && (_y == p._y); }
    static double distance_sq (const Point &p1, const Point &p2);
    static double distance (const Point &p1, const Point &p2);
    static Vector minus (const Point &p1, const Point &p2);
    static Point middle (const Point &p1, const Point &p2);
  private:
    double _x;
    double _y;
  };

  class Matrix4x4 {
  public:
    Matrix4x4 ();
    Matrix4x4 (const Matrix4x4 &m);
    Matrix4x4 (double m11, double m12, double m13, double m14,
            double m21, double m22, double m23, double m24,
            double m31, double m32, double m33, double m34,
            double m41, double m42, double m43, double m44);
    virtual ~Matrix4x4 ();
    void right_multiply_by (const Matrix4x4 &m);
    void set_sin_cos (double sina, double cosa);
    void right_translate_by (double tx, double ty);
    double* &operator [](const  int i ) const { return _matrix[i]; }
  private:
    double **_matrix;
  };
  class ScaleRotateTranslate : public Process
  {
  private:
    class ScaleRotateTranslateAction : public Action
    {
    public:
      ScaleRotateTranslateAction (Process *p, const string &n) : Action (p, n) { Process::finalize_construction (); }
      ~ScaleRotateTranslateAction () {}
      void impl_activate () override { ((ScaleRotateTranslate*)_parent)->update ();};
      void impl_deactivate () override {};
    };

    class TouchMoveAction : public Action
    {
    public:
      TouchMoveAction (Process *p, const string &n) : Action (p, n) { Process::finalize_construction (); }
      ~TouchMoveAction () {}
      void impl_activate () override { ((ScaleRotateTranslate*)_parent)->touch_move ((Touch*)_data); };
      void impl_deactivate () override {};
    };

    class AddTouchAction : public Action
    {
    public:
      AddTouchAction (Process *p, const string &n) : Action (p, n) { Process::finalize_construction (); }
      ~AddTouchAction () {}
      void impl_activate () override { ((ScaleRotateTranslate*)_parent)->add_touch (); };
      void impl_deactivate () override {};
    };
    class RemoveTouchAction : public Action
    {
    public:
      RemoveTouchAction (Process *p, const string &n) : Action (p, n) { Process::finalize_construction (); }
      ~RemoveTouchAction () {}
      void impl_activate () override { ((ScaleRotateTranslate*)_parent)->remove_touch ();};
      void impl_deactivate () override {};
    };
  public:
    ScaleRotateTranslate (Process *p, const string &n, Process* shape, Process* matrix);
    virtual ~ScaleRotateTranslate ();
    void impl_activate () override;
    void impl_deactivate () override;
    void update ();
    void add_touch ();
    void remove_touch ();
    void touch_move (Touch *t);
  private:
    void change_state (State s) { _state = s; }
    Process *_shape, *_touches, *_added, *_removed;
    AbstractHomography *_matrix;
    Coupling *_c_on_add, *_c_on_del, *_c_move_c1, *_c_move_c2, *_c_move_t1, *_c_move_t2;
    ScaleRotateTranslateAction *_update_action;
    AddTouchAction *_add_touch_action;
    RemoveTouchAction *_remove_touch_action;
    TouchMoveAction *_t_move1, *_t_move2;
    State _state;
    Point *_last_point1, *_last_point2, *_new_point1, *_new_point2, *_last_point_hyst_1, *_last_point_hyst_2;
    int _cursor1_id, _cursor2_id;
  };
}
