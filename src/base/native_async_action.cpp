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

#include "native_async_action.h"

#include "exec_env/global_mutex.h"
#include "core/utils/djnn_dynamic_cast.h"
#include "core/execution/graph.h"

// #include "core/utils/error.h"

namespace djnn
{
  using namespace std;

  NativeAsyncAction::NativeAsyncAction (ParentProcess* parent, const std::string& name, NativeCode action, void* data,
                              bool isModel) :
      NativeAction (parent, name, action, data, isModel), 
      ExternalSource(name), 
      _end (this, "end")
  {
  }

  NativeAsyncAction::~NativeAsyncAction ()
  {
    // please_stop (); // FIXME
    // see https://github.com/google/sanitizers/issues/602
    join ();
  }

  void
  NativeAsyncAction::impl_activate ()
  {
    ExternalSource::start ();
  }

  void
  NativeAsyncAction::impl_deactivate ()
  {
    ExternalSource::please_stop ();
  }

  void
  NativeAsyncAction::run ()
  {
    NativeAction::impl_activate ();

    djnn::get_exclusive_access (DBG_GET);
    _end.notify_activation ();
    GRAPH_EXEC;
    djnn::release_exclusive_access (DBG_REL);
  }

}
