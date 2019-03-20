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
      void draw () override;
      void activate () override {}
      void deactivate () override {}
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
    private:
      std::map<Process*, GUIStructureHolder*> _structure_map;
    };

    class GUIMouseButton : public Process
    {
    public:
      GUIMouseButton (Process *p, const string &n);
      ~GUIMouseButton () { delete _press; delete _release; }
      Process* press () { return _press; }
      Process* release () { return _release; }
      void activate () {}
      void deactivate () {}
    private:
      Process *_press, *_release;
    };

    class GUIMouse : public Process {
    public:
      GUIMouse (Process *p, const string &n);
      ~GUIMouse ();
      void activate () { _left->activation (); _right->activation (); _middle->activation ();}
      void deactivate () {}
      GUIMouseButton* left () { return _left; }
      GUIMouseButton* right () { return _right; }
      GUIMouseButton* middle () { return _middle; }
      DoubleProperty* x () { return _pos_x; }
      DoubleProperty* y () { return _pos_y; }
    private:
      GUIMouseButton *_left, *_right, *_middle;
      DoubleProperty *_pos_x, *_pos_y;
    };

  typedef enum mouse_button {
    BUTTON_LEFT,
    BUTTON_MIDDLE,
    BUTTON_RIGHT
  } mouse_button;

  void p_init_gui ();
  extern GUIStructureObserver* gui_structure_observer;
}
