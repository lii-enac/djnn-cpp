#pragma once

#include "gui/abstract_gobj.h"
namespace djnn
{

  class AbstractStyle : public AbstractGObj
  {
  public:
    AbstractStyle (ParentProcess* parent, const std::string& name);
    virtual ~AbstractStyle ();
  };
}