#pragma once

#include "abstract_style.h"
#include "gen/abstract_prop_gradient.h"
#include "style_types.h"

namespace djnn {

class List;

class AbstractGradient : public AbstractPropGradient
{
  public:
    AbstractGradient (CoreProcess* parent, const string& name, int spread, int coords);
    AbstractGradient (CoreProcess* parent, const string& name);
    virtual ~AbstractGradient ();
    virtual void update ();
    List*        transforms () { return _transforms; }
    List*        stops () { return _stops; }
    bool         is_linear () { return _linear; }
    // does this gradient owned thoses children or it merged with another gradient so it should not delete them
    bool bstops, btransforms, bspread, bcoords;

  protected:
    List *            _stops, *_transforms;
    AbstractGradient* _g;
    int               _linear;
};
} // namespace djnn