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

#include "gui/shape/abstract_gshape.h"
#include "core/control/synchronizer.h"

namespace djnn {

  class AbstractHomography;

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

  /*class TouchAlive {
    public:
      TouchAlive (Point *last_pt, Point *new_pt, CouplingWithData *cpl) :  _last_pt (last_pt), _new_pt (new_pt), _cpl (cpl) {}
      virtual ~TouchAlive () { delete _cpl; delete _last_pt; delete _new_pt;}
    Point *_last_pt, *_new_pt;
    CouplingWithData *_cpl;
  };*/

  class ScaleRotateTranslate;

  class TouchAlive {
    public:
      TouchAlive (ScaleRotateTranslate*, Touch*);//, Action*);
      virtual ~TouchAlive ();
      Point _last_pt, _new_pt;
      Touch* _t;
    private:
      class MoveTouchAction : public Action
      {
      public:
        MoveTouchAction (CoreProcess* parent, const string& name, TouchAlive* ta) : Action (parent, name), _ta(ta) {}
        ~MoveTouchAction () {}
        void impl_activate () override;
        TouchAlive * _ta;
      };
      friend class MoveTouchAction;
      //ScaleRotateTranslate* _srt;
      MoveTouchAction _move_touch_action;
      //CouplingWithData2 _cpl;
      Coupling _cpl;
      Coupling _c_update;
  };

  class ScaleRotateTranslate : public FatProcess
  {
  private:
    class ScaleRotateTranslateAction : public Action
    {
    public:
      ScaleRotateTranslateAction (CoreProcess* parent, const string& name) : Action (parent, name) {}
      ~ScaleRotateTranslateAction () {}
      void impl_activate () override { ((ScaleRotateTranslate*)get_parent ())->update ();};
    };
    class AddTouchAction : public Action
    {
    public:
      AddTouchAction (CoreProcess* parent, const string& name) : Action (parent, name) {}
      ~AddTouchAction () {}
      void impl_activate () override { ((ScaleRotateTranslate*)get_parent ())->add_touch (); };
    };

    class RemoveTouchAction : public Action
    {
    public:
      RemoveTouchAction (CoreProcess* parent, const string& name) : Action (parent, name) {}
      ~RemoveTouchAction () {}
      void impl_activate () override { ((ScaleRotateTranslate*)get_parent ())->remove_touch ();};
    };

    /*class MoveTouchAction : public Action
    {
    public:
      MoveTouchAction (CoreProcess* parent, const string& name) : Action (parent, name) {}
      ~MoveTouchAction () {}
      void impl_activate () override;
    };*/
  public:
    ScaleRotateTranslate (CoreProcess* parent, const string& name, CoreProcess* shape, CoreProcess* matrix);
    virtual ~ScaleRotateTranslate ();
    void impl_activate () override;
    void impl_deactivate () override;
    void update ();
    void add_touch ();
    void remove_touch ();
    void move_touch (Touch *t);
    ScaleRotateTranslateAction * get_update_action() { return _update_action; }
  private:
    CoreProcess *_shape;
    FatProcess *_touches, *_added, *_removed;
    AbstractHomography *_matrix;
    map <int, TouchAlive*> touches;
    Coupling *_c_on_add, *_c_on_del; //, *_c_move;
    ScaleRotateTranslateAction *_update_action;
    AddTouchAction *_add_touch_action;
    RemoveTouchAction *_remove_touch_action;
    //MoveTouchAction *_move_touch_action;
  };
  bool estimateTSR (djnnstl::map< int, TouchAlive* > &pts, double *dx, double *dy, double *ds, double *dr);
}
