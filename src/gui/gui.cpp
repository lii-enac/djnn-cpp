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

#include "gui-dev.h"
#include "gui-priv.h"
#include "abstract_gobj.h"
#include "backend.h"
#include "exec_env/main_loop.h"
#include "display/display-dev.h"
#include "display/window.h"

#include "core/utils/error.h"

#include <algorithm>

#include "utils/debug.h"

#if _DEBUG_SEE_GUI_INFO_PREF

int __nb_Drawing_object = 0;
int __nb_Drawing_object_picking = 0;
#endif

namespace djnn
{
  extern void p_init_p_display ();
  static bool __module_initialized = false;
  extern vector<string> loadedModules;

  extern FatProcess *GenericMouse;
  extern GUIStructureObserver * gui_structure_observer;

  int z_order_enabled = 0;

  void init_svg_parser ();
  void clear_svg_parser ();

  void
  init_gui ()
  {
    bool found_display = false;
    for(auto it = djnn::loadedModules.rbegin(); it != djnn::loadedModules.rend(); ++it) {
      if (*it == "display"){
        found_display = true;
        continue;
      }
    }
    if (!found_display)
        error (nullptr, " module display has not been initialized before module GUI and it should");

    if (__module_initialized == false) {

      __module_initialized = true;

      djnn::loadedModules.push_back ("gui");

      Backend::init ();
      
      GenericMouse = new GUIMouse (nullptr, "GenericMouse");
      GenericMouse->activate ();
      MainLoop::instance ().add_background_process (DrawingRefreshManager);
      init_svg_parser ();
      gui_structure_observer = new GUIStructureObserver ();
      structure_observer_list.push_back (gui_structure_observer);

      SVG_Utils::init_named_colors ();
      XMLEllipseAttrs_Hash::init ();

      p_init_p_display ();
      p_init_gui ();
    }
  }

  void
  clear_gui ()
  {   
    clear_svg_parser ();

    p_clear_gui ();

    /* remove container from structure_observer_list */
    structure_observer_list.erase (
      djnnstl::remove (structure_observer_list.begin (), structure_observer_list.end (), gui_structure_observer),
      structure_observer_list.end ()
    );
    /* and delete it */
    delete gui_structure_observer;
    delete GenericMouse;

    SVG_Utils::clear_named_colors ();
    XMLEllipseAttrs_Hash::clear ();

    __module_initialized = false;
  }

} /* namespace Smala */
