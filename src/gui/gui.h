/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "../core/core.h"
#include "backend.h"
#include "window.h"
#include "style/style.h"
#include "shapes/shapes.h"
#include "transformation/transformations.h"

#include <algorithm>

namespace djnn {

  class GUIStructureHolder : public Process {
  public:
    GUIStructureHolder () : Process () {}
    void add_gui_child (Process *c, int index) ;
    void add_gui_child_at (Process *c, int index);
    void move_child_to (Process *c, int index);
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
    void add_child_at (Process *cont, Process *c, int index) override;
    void move_child_to (Process *cont, Process *c, int index) override;
    void remove_child_from_container (Process *cont, Process *c) override;
  private:
    std::map<Process*, GUIStructureHolder*> _structure_map;
  };

	extern std::vector<string> loadedModules; 
	extern int mouse_tracking;
	extern int full_screen;
	extern Process* DrawingRefreshManager;
	extern GUIStructureObserver* gui_structure_observer;

	void init_gui ();
	void init_svg_parser ();
	void clear_gui ();
	
}
