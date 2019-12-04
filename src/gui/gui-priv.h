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
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once
#include "gui-dev.h"

namespace djnn {
  class GUIStructureHolder : public Process {
    public:
      GUIStructureHolder () : Process () {}
      void add_gui_child (Process *c, int index) ;
      void add_gui_child_at (Process *c, int neighboor_index, int spec, int new_index);
      void move_child_to (Process *c, int neighboor_index, int spec, int new_index);
      void remove_gui_child (Process *c);
      void swap_children (int i, int j);
      void draw () override;
      AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
      void impl_activate () override {}
      void impl_deactivate () override {}
    private:
      std::vector<std::pair<Process*, int>> _children;
    };

    class GUIStructureObserver : public StructureObserver {
    public:
      GUIStructureObserver () { }
      virtual ~GUIStructureObserver () {}
      void add_container (Process *cont) override;
      void remove_container (Process *cont) override;
      void add_child_to_container (Process *cont, Process *c, int index) override;
      void add_child_at (Process *cont, Process *c, int neighboor_index, int spec, int new_index) override;
      void move_child_to (Process *cont, Process *c, int neighboor_index, int spec, int new_index) override;
      void remove_child_from_container (Process *cont, Process *c) override;
      void swap_children (Process *cont, int i, int j) override;
    private:
      std::map<Process*, GUIStructureHolder*> _structure_map;
    };

    class GUIMouseButton : public Process
    {
    public:
      GUIMouseButton (Process *parent, const string &name);
      ~GUIMouseButton () { delete _press; delete _release; }
      Process* press () { return _press; }
      Process* release () { return _release; }
      void impl_activate () {}
      void impl_deactivate () {}
    private:
      Process *_press, *_release;
    };

    class GUIMouse : public Process {
    public:
      GUIMouse (Process *parent, const string &name);
      ~GUIMouse ();
      void impl_activate () { _left->activate (); _right->activate (); _middle->activate ();}
      void impl_deactivate () {}
      GUIMouseButton* left () { return _left; }
      GUIMouseButton* right () { return _right; }
      GUIMouseButton* middle () { return _middle; }
      Process* move () { return _move; }
      Process* wheel () { return _wheel; }
      DoubleProperty* x () { return _pos_x; }
      DoubleProperty* y () { return _pos_y; }
      DoubleProperty* dx () { return _dx; }
      DoubleProperty* dy () { return _dy; }
    private:
      GUIMouseButton *_left, *_right, *_middle;
      Spike *_move, *_wheel;
      DoubleProperty *_pos_x, *_pos_y, *_dx, *_dy;
    };

  void p_init_gui ();
  extern GUIStructureObserver* gui_structure_observer;
}
