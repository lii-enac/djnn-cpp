#pragma once

#include "gui/abstract_gobj.h"
namespace djnn
{

  class AbstractStyle : public AbstractGObj
  {
  public:
    AbstractStyle (ParentProcess* parent, const djnn::string& name);
    virtual ~AbstractStyle ();
  };
}