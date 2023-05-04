#pragma once

namespace djnn {
  enum property_type_e {
    Boolean,
    Integer,
    Double,
    String,
    Reference,
    Remote,
    UserDefined
  };

  class CoreProcess;

  CoreProcess* getRef (CoreProcess* p);
  void setRef (CoreProcess* p, CoreProcess* v);
  void activate (CoreProcess* p);
  void deactivate (CoreProcess* p);
  void* get_native_user_data (CoreProcess* native_action);
}
