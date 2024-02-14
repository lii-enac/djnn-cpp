/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "core/core-dev.h"
#include "display/window.h"
#include "gui/picking/color_picking.h"
#include "gui/shape/abstract_gshape.h"

namespace djnn {

class CairoPickingView : public ColorPickingView
{
  public:
    CairoPickingView (Window* win);
    virtual ~CairoPickingView ();

    // Picking
    virtual void init () override;
    // ColorPickingView
    virtual int get_pixel (unsigned int x, unsigned int y) override;

    void set_data (unsigned char* data, int w, int h, int stride)
    {
        _cur_data = data;
        _w        = w;
        _h        = h;
        _stride   = stride;
    }

  private:
    unsigned char* _cur_data;
    int            _w, _h, _stride;
};
} /* namespace djnn */
