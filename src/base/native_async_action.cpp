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

namespace djnn
{
  using namespace std;

  NativeAsyncAction::NativeAsyncAction (FatProcess* parent, const std::string& name, NativeCode *action, void* data,
                              bool isModel) :
      Action (parent, name), ExternalSource(name), _data (data), _action (action), _activation_source (nullptr), _end (this, "end")
  {
    set_is_model (isModel);
    finalize_construction (parent, name);
  }

  NativeAsyncAction::~NativeAsyncAction ()
  {
  }

  void
  NativeAsyncAction::impl_activate ()
  {
    //_end.activate ();
    ExternalSource::start ();
  }

  void
  NativeAsyncAction::impl_deactivate ()
  {
    //_end.deactivate ();
    ExternalSource::please_stop ();
  }

  void
  NativeAsyncAction::run ()
  {
    (_action) (this);
    djnn::get_exclusive_access (DBG_GET);
    _end.notify_activation ();
    GRAPH_EXEC;
    djnn::release_exclusive_access (DBG_REL);
  }

  void*
  NativeAsyncAction::data ()
  {
    return _data;
  }

  void*
  get_native_async_user_data (CoreProcess* native_action)
  {
    NativeAsyncAction *na = djnn_dynamic_cast<NativeAsyncAction*> (native_action);
    if (na == nullptr)
      return nullptr;
    return na->data ();
    //return native_action->data ();
  }
}
