/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */


#include "display-priv.h"
#include "core/tree/set.h"
#include "core/utils/uri.h"
#include "update_drawing.h"

namespace djnn
{
  
  static bool __module_initialized = false;
  FatProcess *DrawingRefreshManager;

  int mouse_tracking = 0;
  int full_screen = 0;
  int hide_pointer = 0;

  FatProcess *GPUs = nullptr;
  FatProcess *Displays = nullptr;

  DisplayBackend::Impl* DisplayBackend::_instance;

  AbstractDisplay*
  DisplayBackend::instance ()
  {
    return _instance->backend;
  }

  void
  init_display () {
    UpdateDrawing::init ();
    if ( __module_initialized == false ) {
      //std::cerr << __FILE__ << " " << __LINE__ << std::endl;
      DrawingRefreshManager = UpdateDrawing::instance ();
      GPUs = new Set (nullptr, "GPUs");
      GPUs->set_activation_state (ACTIVATED);
      Displays = new Set (nullptr, "Displays");
      Displays->set_activation_state (ACTIVATED);
      URI::add_uri ("gpu", GPUs);
      URI::add_uri ("display", Displays);
      //std::cerr << __FILE__ << " " << __LINE__ << std::endl;
      p_init_display ();
      djnn::loadedModules.push_back("display");
      __module_initialized = true;
    }
  }

  void
  clear_display () {
    UpdateDrawing::clear ();
    p_clear_display ();
  }

}
