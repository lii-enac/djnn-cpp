/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "../backend.h"

#include "../picking/color_picking.h"
#include "../../core/core-dev.h"
#include "../window.h"
#include "../abstract_gshape.h"
#include <map>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtWidgets/QLabel>

namespace djnn
{

  class QtPickingView : public ColorPickingView
  {
  public:
    QtPickingView (Window *win);
    virtual
    ~QtPickingView ();
    virtual void init ();
    virtual int get_pixel(int x, int y);
    QPainter* painter () { return _painter; }
    void display ();
  private:
    QLabel *_pick_debug_win;
    QImage *_image;
    QPainter *_painter;
  };
} /* namespace djnn */
