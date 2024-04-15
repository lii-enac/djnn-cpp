/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2014-2021)
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

#include <QtGui/QImage>

#include "qt_picking_view.h"

#include "core/utils/remotery.h"
#include "core/utils/utils-dev.h"
#include "display/background_rect.h"
#include "utils/debug.h"

#include "gui/qt/qt/qqt_window_moc.h"

using namespace djnnstl;

namespace djnn {

struct MyQLabel : QLabel {
    MyQLabel (QtPickingView* qpv) : _qtpickingview(qpv) {}
    
    virtual void mousePressEvent (QMouseEvent* event) override { _qtpickingview->qtwindow()->mousePressEvent(event); return QLabel::mousePressEvent(event); }
    virtual void mouseReleaseEvent (QMouseEvent* event) override { _qtpickingview->qtwindow()->mouseReleaseEvent(event); return QLabel::mouseReleaseEvent(event); }
    virtual void mouseMoveEvent (QMouseEvent* event) override{ _qtpickingview->qtwindow()->mouseMoveEvent(event); return QLabel::mouseMoveEvent(event); }
    //virtual void wheelEvent (QWheelEvent* event) override;
    QtPickingView* _qtpickingview;
};

QtPickingView::QtPickingView (Window* win, MyQQWidget* qtwindow)
    : ColorPickingView (win), _image (nullptr), _painter (nullptr), _pick_debug_win (nullptr), _qtwindow(qtwindow)
{
    if (_DEBUG_SEE_COLOR_PICKING_VIEW) {
        _pick_debug_win = new MyQLabel (this);
        if (mouse_tracking)
            _pick_debug_win->setMouseTracking (true);
    }
}

QtPickingView::~QtPickingView ()
{
    delete _painter;
    delete _image;

    if (_DEBUG_SEE_COLOR_PICKING_VIEW)
        delete _pick_debug_win;
}

int
QtPickingView::get_pixel (unsigned int x, unsigned int y)
{
    if (!_image || x < 0 || x >= (unsigned int)_image->width () || y < 0 || y >= (unsigned int)_image->height ())
        return -1;
    return _image->pixel (x, y);
}

void
QtPickingView::init ()
{
    rmt_BeginCPUSample (ColorPickingView_init, RMTSF_None);
    ColorPickingView::init ();
    rmt_EndCPUSample ();

    rmt_BeginCPUSample (ColorPickingView_setting, RMTSF_None);
    double w = _win->width ()->get_value ();
    double h = _win->height ()->get_value ();
    if (_painter != nullptr)
        delete _painter;
    if (_image != nullptr && (w != _image->width () || h != _image->height ())) {
        delete _image;
        _image = nullptr;
    }
    rmt_EndCPUSample ();

    rmt_BeginCPUSample (ColorPickingView_new, RMTSF_None);
    if (_image == nullptr)
        _image = new QImage (w, h, QImage::Format_RGB32);
    _painter = new QPainter (_image);
    /* no antialiasing in Color Picking otherwise it will modify a given color */
    _painter->setCompositionMode (QPainter::CompositionMode_Source);
    _painter->fillRect (0, 0, w, h, QColor (255, 255, 255, 255));                           // 0xfffffff fast method to fill color
    _color_map.insert (pair<unsigned int, PickUI*> (0xffffffff, _win->background_rect ())); // always add background , without cached , in any layer
    rmt_EndCPUSample ();
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
