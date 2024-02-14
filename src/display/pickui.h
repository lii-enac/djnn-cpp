/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2021)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"
#include "display/ui.h"

namespace djnn {

class PickUI
{
  public:
    PickUI (bool cache) : _ui (nullptr), _cache (cache) {}
    ~PickUI ()
    {
        if (_ui)
            delete _ui;
    }
    bool         cache () const { return _cache; }
    void         cache (bool cache) { _cache = cache; }
    void         color (unsigned int c) { _color = c; }
    unsigned int color () { return _color; }
    bool         has_ui () { return _ui != nullptr; }
    UI*          ui () { return _ui; }
    virtual void set_mouse_local_coords (double x, double y, bool is_move) = 0;

  protected:
    UI* _ui;

  private:
    unsigned int _color;
    bool         _cache;
};
} // namespace djnn