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

#include "gui.h"
#include "backend.h"
#include "../core/syshook/main_loop.h"
namespace djnn
{

  static bool __module_initialized = false;
  Process *DrawingRefreshManager;

  void
  init_gui () {

    if ( __module_initialized == false ) {

      __module_initialized = true;
      
      djnn::loadedModules.push_back("gui");

      Backend::init ();
      UpdateDrawing::init ();
      DrawingRefreshManager = UpdateDrawing::instance ();
      MainLoop::instance ().add_background_process (DrawingRefreshManager);
      init_svg_parser ();
    }
  }

  void
  clear_gui () {

    __module_initialized = false;
    UpdateDrawing::clear ();

  }


} /* namespace Smala */
