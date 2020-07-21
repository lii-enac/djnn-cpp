/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "core/utils/error.h"
#include "files/alt/alt_directory_observer.h"
#include "files/files.h"

namespace djnn
{
  DirectoryObserverData*
  p_init_directory_watcher (const std::string& path)
  {
    return nullptr;
  }

  djn_dir_event
  p_run_directory_watcher (DirectoryObserverData *data)
  {
    warning (nullptr, "Directory observer not yet implemented for this platform");
    return DJN_UNKNOWN;
  }
}
