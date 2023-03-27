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

#include "core/tree/structure_observer.h"
#include "core/property/double_property.h"
#include "core/control/spike.h"


namespace djnn {

  typedef vector<pair<CoreProcess*, size_t>> children_t; // TODO better structure ??
  class GUIStructureHolder : public FatProcess {
    public:
      GUIStructureHolder (FatProcess* content) : FatProcess ("GUIStructureHolder_of_" + content->get_debug_name()), content_process(content) {}
      void add_gui_child (CoreProcess *c, size_t index) ;
      void add_gui_child_at (CoreProcess *c, size_t neighboor_index, child_position_e spec, size_t new_index);
      void remove_gui_child (CoreProcess *c);
      void move_child_to (CoreProcess *c, size_t neighboor_index, child_position_e spec, size_t new_index);
      void swap_children (size_t i, size_t j);
      void set_child (CoreProcess *child, size_t i);
      void draw () override;
      AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
      void impl_activate () override {}
      void impl_deactivate () override {}
      children_t& children () { return _children;}
    private:
      children_t _children;
      FatProcess* content_process;
    };

    typedef map<CoreProcess*, GUIStructureHolder*> structures_t;
    typedef map<CoreProcess*, GUIStructureHolder*>::iterator structures_t_it;
    class GUIStructureObserver : public StructureObserver {
    public:
      GUIStructureObserver () { }
      virtual ~GUIStructureObserver () override;
      void add_container (FatProcess *cont) override;
      void remove_container (FatProcess *cont) override;
      void add_child_to_container (FatProcess *cont, CoreProcess *c, int index) override;
      void add_child_at (FatProcess *cont, CoreProcess *c, int neighboor_index, child_position_e spec, int new_index) override;
      void move_child_to (FatProcess *cont, CoreProcess *c, int neighboor_index, child_position_e spec, int new_index) override;
      void remove_child_from_container (FatProcess *cont, CoreProcess *c) override;
      void swap_children (FatProcess *cont, int i, int j) override;
      void set_child (FatProcess *cont, CoreProcess *child, int i) override;
      GUIStructureHolder* find_holder (CoreProcess* cont);
      //void print_structure_map ();
      structures_t& structure_map () { return _structure_map;};
    private:
      structures_t _structure_map;
    };

    class GUIMouseButton : public FatProcess
    {
    public:
      GUIMouseButton (CoreProcess* parent, const string& name);
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
      GUIMouse (CoreProcess* parent, const string& name);
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
