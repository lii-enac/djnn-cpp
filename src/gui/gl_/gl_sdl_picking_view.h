/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2014-2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stephane Conversy <stephane.conversy@enac.fr>
 */

#pragma once

#include "../picking/color_picking.h"
#include "../../core/core-dev.h"
#include "../window.h"
#include "../abstract_gshape.h"


namespace djnn
{
  class GLSDLWindow;

  class GLSDLPickingView : public ColorPickingView
  {
  public:
    GLSDLPickingView (Window *win, GLSDLWindow *sdlwin);
    virtual ~GLSDLPickingView ();
    
    // Picking
    virtual void init () override;
    // ColorPickingView
    virtual int get_pixel(unsigned int x, unsigned int y) override;

  private:
    GLSDLWindow * _sdlwin;
  };
} /* namespace djnn */
