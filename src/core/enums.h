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

  CoreProcess* getRef (CoreProcess *p);
  void setRef (CoreProcess *p, CoreProcess *v);
}
