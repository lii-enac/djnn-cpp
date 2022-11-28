/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2021)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "background_rect.h"
#include "core/core-dev.h" // graph add/remove edge

namespace djnn {

  BackgroundRect::BackgroundRect (ParentProcess* parent, const string& name) :
    Component (parent, name), PickUI (false)
  {
  }

  BackgroundRect::~BackgroundRect ()
  {
    //Note: it's seems no need to reorder destructors due to multiple heritage
    //PickUI::delete_UI ();
    //Container::clean_up_content ();
  }

  FatChildProcess*
  BackgroundRect::find_child_impl (const string& path)
  {
    // looking for ui interface
    if (_ui) {
      FatChildProcess* process = FatProcess::find_child_impl (path);
      if (process != nullptr)
        return process;
    }

    // looking for something else
    size_t found = path.find_first_of ('/');
    string key = path;
    if (found != string::npos) {
      key = path.substr (0, found);
    }
      /*  "press", "release", "move", "enter", "leave", "touches" */
      vector<string>::iterator it = __ui_interface.begin ();
      found = false;
      while (!found && it != __ui_interface.end ()) {
        if (key.compare (*it) == 0) {
          found = true;
          _ui = new UI (this, get_frame ());
        }
        it++;
      }
    return FatProcess::find_child_impl (path);
  }
}
