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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stéphane Chatty <chatty@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "../backend.h"

#include "qt_picking_view.h"
#include <QtGui/QImage>

#define DEBUG_PICKING  0

namespace djnn
{

  QtPickingView::QtPickingView (Window *win) :
      ColorPickingView (win), _pick_debug_win (nullptr), _image (nullptr), _painter (nullptr)
  {
#if DEBUG_PICKING 
    _pick_debug_win = new QLabel ();
#endif
  }

  QtPickingView::~QtPickingView ()
  {
    delete _painter;
    delete _image;

#if DEBUG_PICKING 
    delete _pick_debug_win;
#endif   
  }

  int
  QtPickingView::get_pixel (int x, int y)
  {
    if (x < 0 || x >= _image->width () || y < 0 || y >= _image->height ())
      return -1;
    return _image->pixel (x, y);
  }

  void
  QtPickingView::init ()
  {
    ColorPickingView::init ();
    double w = _win->width ()->get_value ();
    double h = _win->height ()->get_value ();
    if (_painter != nullptr)
      delete _painter;
    if (_image != nullptr)
      delete _image;
    _image = new QImage (w, h, QImage::Format_RGB32);
    _image->fill (0xffffffff);
    _painter = new QPainter (_image);
  }

  void
  QtPickingView::display ()
  {
    if (_pick_debug_win == nullptr)
      return;
    double w = _win->width ()->get_value ();
    double h = _win->height ()->get_value ();
    double x = _win->pos_x ()->get_value ();
    double y = _win->pos_y ()->get_value ();
    _pick_debug_win->setGeometry (x + w + 20, y, w, h);
    _pick_debug_win->setPixmap (QPixmap::fromImage (*_image));
    _pick_debug_win->show ();
    _pick_debug_win->update ();
  }
} /* namespace djnn */
