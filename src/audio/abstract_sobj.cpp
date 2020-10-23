#include "audio/abstract_sobj.h"

namespace djnn {
  void
  AbstractSObj::impl_activate ()
  {
    //Backend::instance ()->activate_gobj (this);
  }

  void
  AbstractSObj::impl_deactivate ()
  {
  }
}