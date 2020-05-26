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

#include "native_action.h"

#include "core/utils/djnn_dynamic_cast.h"

namespace djnn
{
  using namespace std;

  NativeAction::NativeAction (FatProcess* parent, const std::string& name, NativeCode *action, void* data,
                              bool isModel) :
      Action (parent, name), _data (data), _action (action), _activation_source (nullptr)
  {
    set_is_model (isModel);
    finalize_construction (parent, name);
  }

  NativeAction::~NativeAction ()
  {
  }

  void
  NativeAction::impl_activate ()
  {
    (_action) (this);
  }

  void*
  NativeAction::data ()
  {
    return _data;
  }

  void*
  get_native_user_data (NativeAction* native_action)
  {
    NativeAction *na = djnn_dynamic_cast<NativeAction*> (native_action);
    if (na == nullptr)
      return nullptr;
    return na->data ();
    //return native_action->data ();
  }
}
