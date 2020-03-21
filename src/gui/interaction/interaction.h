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

#include "gui/shapes/abstract_gshape.h"
#include "gui/transformation/transformations.h"
#include "core/control/synchronizer.h"

namespace djnn {

  typedef enum State {
    START, DRAGGING, RRR
  } State;

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

  class TouchAlive {
    public:
      TouchAlive (Point *last_pt, Point *new_pt, Coupling *cpl) :  _last_pt (last_pt), _new_pt (new_pt), _cpl (cpl) {}
      virtual ~TouchAlive () { delete _cpl; delete _last_pt; delete _new_pt;}
    Point *_last_pt, *_new_pt;
    Coupling *_cpl;
  };

  class ScaleRotateTranslate : public Process
  {
  private:
    class ScaleRotateTranslateAction : public Action
    {
    public:
      ScaleRotateTranslateAction (Process *parent, const std::string& name) : Action (parent, name) {}
      ~ScaleRotateTranslateAction () {}
      void impl_activate () override { ((ScaleRotateTranslate*)get_parent ())->update ();};
      void impl_deactivate () override {};
    };

    class TouchMoveAction : public Action
    {
    public:
      TouchMoveAction (Process *parent, const std::string& name) : Action (parent, name) {}
      ~TouchMoveAction () {}
      void impl_activate () override { ((ScaleRotateTranslate*)get_parent ())->touch_move ((Touch*)get_data ()); };
      void impl_deactivate () override {};
    };

    class AddTouchAction : public Action
    {
    public:
      AddTouchAction (Process *parent, const std::string& name) : Action (parent, name) {}
      ~AddTouchAction () {}
      void impl_activate () override { ((ScaleRotateTranslate*)get_parent ())->add_touch (); };
      void impl_deactivate () override {};
    };
    class RemoveTouchAction : public Action
    {
    public:
      RemoveTouchAction (Process *parent, const std::string& name) : Action (parent, name) {}
      ~RemoveTouchAction () {}
      void impl_activate () override { ((ScaleRotateTranslate*)get_parent ())->remove_touch ();};
      void impl_deactivate () override {};
    };
  public:
    ScaleRotateTranslate (Process *parent, const std::string& name, Process* shape, Process* matrix);
    virtual ~ScaleRotateTranslate ();
    void impl_activate () override;
    void impl_deactivate () override;
    void update ();
    void add_touch ();
    void remove_touch ();
    void touch_move (Touch *t);
  private:
    Process *_shape, *_touches, *_added, *_removed;
    AbstractHomography *_matrix;
    std::map <int, TouchAlive*> touches;
    Coupling *_c_on_add, *_c_on_del, *_c_move;
    ScaleRotateTranslateAction *_update_action;
    AddTouchAction *_add_touch_action;
    RemoveTouchAction *_remove_touch_action;
    TouchMoveAction *_t_move1;
  };
  bool estimateTSR (std::map< int, TouchAlive* > &pts, double *dx, double *dy, double *ds, double *dr);
}
