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
//#include "gui-dev.h"
#include "core/tree/structure_observer.h"
#include "core/tree/double_property.h"
#include "core/tree/spike.h"

namespace djnn {
  class GUIStructureHolder : public FatProcess {
    public:
      GUIStructureHolder () : FatProcess ("GUIStructureHolder") {}
      void add_gui_child (FatChildProcess *c, int index) ;
      void add_gui_child_at (FatChildProcess *c, int neighboor_index, int spec, int new_index);
      void remove_gui_child (FatChildProcess *c);
      void move_child_to (FatChildProcess *c, int neighboor_index, int spec, int new_index);
      void swap_children (int i, int j);
      void set_child (FatChildProcess *child, int i);
      void draw () override;
      AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
      void impl_activate () override {}
      void impl_deactivate () override {}
    private:
      typedef std::vector<std::pair<FatChildProcess*, int>> children_t;
      children_t _children;
    };

    class GUIStructureObserver : public StructureObserver {
    public:
      GUIStructureObserver () { }
      virtual ~GUIStructureObserver () override;
      void add_container (FatProcess *cont) override;
      void remove_container (FatProcess *cont) override;
      void add_child_to_container (FatProcess *cont, FatChildProcess *c, int index) override;
      void add_child_at (FatProcess *cont, FatChildProcess *c, int neighboor_index, int spec, int new_index) override;
      void move_child_to (FatProcess *cont, FatChildProcess *c, int neighboor_index, int spec, int new_index) override;
      void remove_child_from_container (FatProcess *cont, FatChildProcess *c) override;
      void swap_children (FatProcess *cont, int i, int j) override;
      void set_child (FatProcess *cont, FatChildProcess *child, int i) override;
    private:
      typedef std::map<FatChildProcess*, GUIStructureHolder*> structures_t;
      structures_t _structure_map;
    };

    class GUIMouseButton : public FatProcess
    {
    public:
      GUIMouseButton (ParentProcess* parent, const std::string& name);
      ~GUIMouseButton () { delete _press; delete _release; }
      FatProcess* press () { return _press; }
      FatProcess* release () { return _release; }
      void impl_activate () {}
      void impl_deactivate () {}
    private:
      FatProcess *_press, *_release;
    };

    class GUIMouse : public FatProcess {
    public:
      GUIMouse (ParentProcess* parent, const std::string& name);
      ~GUIMouse ();
      void impl_activate () { _left->activate (); _right->activate (); _middle->activate ();}
      void impl_deactivate () {}
      GUIMouseButton* left () { return _left; }
      GUIMouseButton* right () { return _right; }
      GUIMouseButton* middle () { return _middle; }
      FatProcess* move () { return _move; }
      FatProcess* wheel () { return _wheel; }
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
  void p_clear_gui ();
  extern GUIStructureObserver* gui_structure_observer;
}
