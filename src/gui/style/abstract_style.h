#pragma once

#include "gui/abstract_gobj.h"
namespace djnn {

class AbstractStyle : public AbstractGObj {
  public:
    AbstractStyle (CoreProcess* parent, const string& name);
    virtual ~AbstractStyle ();
};
} // namespace djnn