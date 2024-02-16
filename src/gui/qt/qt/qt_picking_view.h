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

#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtWidgets/QLabel>

#include "core/core-dev.h"
#include "display/window.h"
#include "gui/picking/color_picking.h"
#include "gui/shape/abstract_gshape.h"

namespace djnn {

class QtPickingView : public ColorPickingView {
  public:
    QtPickingView (Window* win);
    virtual ~QtPickingView ();

    // Picking
    virtual void init () override;
    // ColorPickingView
    virtual int get_pixel (unsigned int x, unsigned int y) override;

    // QtPickingView
    QPainter* painter () { return _painter; }
    void      set_painter (QPainter* p) { _painter = p; }
    void      display ();

  private:
    QLabel*   _pick_debug_win;
    QImage*   _image;
    QPainter* _painter;
};
} /* namespace djnn */
