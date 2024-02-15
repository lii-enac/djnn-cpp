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

#include "core/core-dev.h" // graph add/remove edge
#include "core/utils/djnn_dynamic_cast.h"
#include "exec_env/global_mutex.h"

// #include "core/utils/error.h"

namespace djnn {

NativeAsyncAction::NativeAsyncAction (CoreProcess* parent, const CoreProcess::string& name, NativeCode action, void* data,
                                      bool isModel)
    : NativeAction (parent, name, action, data, isModel),
      ExternalSource (name),
      _end (this, "end"),
      _restart (this, "restart"),
      _action (this, "action"),
      _c_restart (&_restart, ACTIVATION, &_action, ACTIVATION)
{
}

NativeAsyncAction::~NativeAsyncAction ()
{
    please_stop (); // FIXME
                    // see https://github.com/google/sanitizers/issues/602
                    // join ();
}

void
NativeAsyncAction::impl_activate ()
{
    _c_restart.enable ();
    ExternalSource::start ();
}

void
NativeAsyncAction::impl_deactivate ()
{
    _c_restart.disable ();
    ExternalSource::please_stop ();
}

void
NativeAsyncAction::restart ()
{
    ExternalSource::please_stop ();
    ExternalSource::start ();
}

void
NativeAsyncAction::run ()
{
    // this is executed in its own thread

    NativeAction::impl_activate (); // execute the action

    djnn::get_exclusive_access (DBG_GET);
    _end.schedule_activation ();
    GRAPH_EXEC;
    djnn::release_exclusive_access (DBG_REL);
}

} // namespace djnn
