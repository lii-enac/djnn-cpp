/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "core/tree/list.h"
#include "core/utils/ext/remotery/Remotery.h"

#include "gui/backend.h"

#include "qt_context.h"
#include "qt_backend.h"
#include "display/qt/qt_window.h"
#include "display/qt/qt_window_moc.h"
#include "display/abstract_display.h"
#include "gui/shape/shapes.h"
#include "gui/layer.h"
#include "gui/widgets/multiline_edit.h"

#include <QtGui/QTextDocument>
#include <QtGui/QTextCursor>
#include <QFontMetrics>
#include <QPicture>
#include <QtWidgets/QWidget>
#include <QtCore/QtMath>
#include <QtCore/QFileInfo>
#include <cmath>

#include "exec_env/exec_env-dev.h"

#include "core/utils/iostream.h"
#include "core/utils/error.h"
#include "utils/debug.h"


#if _DEBUG_SEE_GUI_INFO_PREF
/* define in gui.cpp */
extern int __nb_Drawing_object;
extern int __nb_Drawing_object_picking;
#endif



namespace djnn
{
  int
  QtBackend::draw_simple_text (SimpleText *t, QFontMetrics* fm, int posX, int posY)
  {
    rmt_BeginCPUSample(draw_simple_text, RMTSF_Aggregate);
    QString s = QObject::tr (t->get_content ().c_str ());
    QtContext *cur_context = _context_manager->get_current ();
    //rmt_BeginCPUSample(text_load_drawing_context, RMTSF_Aggregate);
    load_drawing_context (t, posX, posY, 1, 1);
    //rmt_EndCPUSample ();
    QPointF p (posX, posY);
    QPen oldPen = cur_context->pen;
    QPen newPen (oldPen);
    newPen.setColor (cur_context->brush.color ());
    if (cur_context->brush.style () == Qt::SolidPattern)
      newPen.setStyle (Qt::SolidLine);
    else
      newPen.setStyle (Qt::NoPen);
    _painter->setPen (newPen);

    rmt_BeginCPUSample(text_bounding_rect, RMTSF_Aggregate);
    QRect rect = fm->boundingRect (s);
    rmt_EndCPUSample ();
    /* applying alignment attribute */
    switch (cur_context->textAnchor)
    {
      case DJN_MIDDLE_ANCHOR:
      posX = posX - (rect.width () / 2.0);
      p.setX (posX);
      break;
      case DJN_END_ANCHOR:
      posX = posX - rect.width ();
      p.setX (posX);
      break;
    }
    rect.moveTo (posX, posY - fm->ascent ());
    rmt_BeginCPUSample(text_painter_drawText, RMTSF_Aggregate);
    _painter->drawText (p, s);
    rmt_EndCPUSample ();
#if _DEBUG_SEE_GUI_INFO_PREF
    __nb_Drawing_object++;
#endif

    /* Don't forget to reset the old pen color */
    _painter->setPen (oldPen);
    if (is_in_picking_view (t)) {
      load_pick_context (t);
      _picking_view->painter ()->drawRect (rect);
#if _DEBUG_SEE_GUI_INFO_PREF
      __nb_Drawing_object_picking++;
#endif
    }
    rmt_EndCPUSample ();
    return rect.width ();
  }

  int
  QtBackend::compute_text_width (FontMetricsImpl fm, SimpleText *t)
  {
    QFontMetrics *qfm = (QFontMetrics*) fm;
    if (qfm == nullptr)
      return 0;
    QString s = QObject::tr (t->get_content ().c_str ());
    QRect rec = qfm->boundingRect (s);
    return rec.width();
  }

  void
  QtBackend::draw_simple_text_edit (MultilineEditor* ste)
  {
    rmt_BeginCPUSample(draw_simple_text_edit, RMTSF_Aggregate);

    QtContext *cur_context = _context_manager->get_current ();
    _painter->setFont (cur_context->font);
    QFontMetrics fm = _painter->fontMetrics ();

    /* hack to enable the use of a correct FontMetrics for future cursor positioning */
    QFontMetrics *last_fm = (QFontMetrics*) ste->get_font_metrics ();
    delete last_fm;
    ste->set_font_metrics ((FontMetricsImpl*)(new QFontMetrics (fm)));

    load_drawing_context (ste, 0, 0, 1, 1);
    _context_manager->get_current ()->matrix.translate (ste->x (), ste->y () + fm.ascent ());
    int y = 0;
    int height = fm.height() + fm.leading ();
    for (auto l : ste->lines ()->children ()) {
      SimpleText* line = (SimpleText*)l;
      if (ste->first_draw()) {
        line->set_y (y);
        y += height;
      }
      draw_simple_text(line, &fm, line->get_x (), line->get_y ());
    }
    load_pick_context (ste);
    QRect rect (0, -fm.ascent (), ste->width(), ste->height());
    _picking_view->painter ()->drawRect (rect);
    // Update font metrics data
    ste->set_ascent  (fm.ascent ());
    ste->set_descent (fm.descent ());
    ste->set_leading (fm.leading ());
    _context_manager->get_current ()->matrix.translate (-ste->x (), -ste->y () - fm.ascent ());
#if _DEBUG_SEE_GUI_INFO_PREF
    __nb_Drawing_object_picking++;
#endif
    rmt_EndCPUSample ();

  }

  static double curTextX = 0.;
  static double curTextY = 0.;
  void
  QtBackend::draw_text (Text *t)
  {
    rmt_BeginCPUSample(draw_text, RMTSF_Aggregate);

    double x, y, dx, dy;
    int dxU, dyU, width, height, encoding;
    string text;
    t->get_properties_values(x, y, dx, dy, dxU, dyU, width, height, encoding, text);

    QtContext *cur_context = _context_manager->get_current ();
    double dxfactor = cur_context->factor[dxU];
    double dyfactor = cur_context->factor[dyU];

    if (dxU == DJN_PERCENT) {
      int v = DisplayBackend::instance()->window()->width ()->get_value ();
      dx = v * dx / 100;
    }
    if (dyU == DJN_PERCENT) {
      int v = DisplayBackend::instance()->window()->height ()->get_value ();
      dy = v * dy / 100;
    }

    if (x == -1) {
      x = curTextX;
    }
    if (y == -1) {
      y = curTextY;
    }
    double posX = x + (dx * dxfactor);
    double posY = y + (dy * dyfactor);
    QPointF p (posX, posY);
    QString s;
    switch (encoding)
    {
      case DJN_LATIN1:
      case DJN_ASCII:
      s = QString::fromLatin1 (text.c_str ());
      break;
      case DJN_UTF8:
      default:
      s = QString::fromUtf8 (text.c_str ());
    }

    /* dummy value for width-height parameters because we do not manage
     gradient for text object
     */
    load_drawing_context (t, x, y, 1, 1);

    /* Qt draws text with the outline color
     but we want it to use the fill color */
    QPen oldPen = cur_context->pen;
    QPen newPen (oldPen);
    newPen.setColor (cur_context->brush.color ());
    if (cur_context->brush.style () == Qt::SolidPattern)
      newPen.setStyle (Qt::SolidLine);
    else
      newPen.setStyle (Qt::NoPen);
    _painter->setPen (newPen);
    _painter->setFont (cur_context->font);

    rmt_BeginCPUSample(qt_fontMetrics, RMTSF_Aggregate);
    QFontMetrics fm = _painter->fontMetrics ();
    rmt_EndCPUSample ();
    rmt_BeginCPUSample(boundingRect, RMTSF_Aggregate);
    QRect rect = fm.boundingRect (s);
    rmt_EndCPUSample ();

    /* hack to enable the use of a correct FontMetrics for future cursor positioning*/
    rmt_BeginCPUSample(qt_get_font_metrics, RMTSF_Aggregate);
    QFontMetrics *last_fm = (QFontMetrics*) t->get_font_metrics ();
    rmt_EndCPUSample ();
    delete last_fm;
    t->set_font_metrics ((FontMetricsImpl*)(new QFontMetrics (fm)));

    /* applying alignment attribute */
    switch (cur_context->textAnchor)
    {
      case DJN_MIDDLE_ANCHOR:
      posX = posX - (rect.width () / 2.0);
      p.setX (posX);
      break;
      case DJN_END_ANCHOR:
      posX = posX - rect.width ();
      p.setX (posX);
      break;
    }

    rect.moveTo (posX, posY - rect.height ());
    rmt_BeginCPUSample(qt_horizontalAdvance, RMTSF_Aggregate);
    #if (QT_VERSION < QT_VERSION_CHECK(5,12,0))
    curTextX = rect.x () + fm.width (s);  // for qt 5.9
    #else
    curTextX = rect.x () + fm.horizontalAdvance (s); // for qt 5.12
    #endif
    rmt_EndCPUSample ();
    curTextY = rect.y () + fm.height ();

    rmt_BeginCPUSample(qt_drawText, RMTSF_Aggregate);
    _painter->drawText (p, s);
    rmt_EndCPUSample ();
#if _DEBUG_SEE_GUI_INFO_PREF
    __nb_Drawing_object++;
#endif

    /* Don't forget to reset the old pen color */
    _painter->setPen (oldPen);

    if (is_in_picking_view (t)) {
      load_pick_context (t);
      _picking_view->painter ()->drawRect (rect);
#if _DEBUG_SEE_GUI_INFO_PREF
      __nb_Drawing_object_picking++;
#endif
    }
    rmt_EndCPUSample ();
  }

  double
  QtBackend::get_cursor_from_index (Text *t, int i)
  {
    string text = t->get_raw_text ().substr (0, i);
    QString s (text.c_str ());
#if (QT_VERSION < QT_VERSION_CHECK(5,12,0))
    int w = ((QFontMetrics*)t->get_font_metrics())->width (s);
#else
    int w = ((QFontMetrics*)t->get_font_metrics())->horizontalAdvance (s);
#endif
    return w;
  }

  static int
  next_index (size_t i, string &str)
  {
    if (i > str.size ())
      return i;
    int offset = 1;
    auto cp = str.data () + i;
    while (++cp <= (str.data () + str.size ()) && ((*cp & 0b10000000) && !(*cp & 0b01000000))) {
     offset++;
   }
   return i + offset;
 }

 pair<double,int>
 QtBackend::get_cursor_from_local_x (Text *t, double loc_x)
 {
  string text = t->get_raw_text ();
  if (text.length() == 0)
    return pair<double,int>(0,0);
  QString s (text.c_str ());
  QFontMetrics *fm = (QFontMetrics*)(t->get_font_metrics());

#if (QT_VERSION < QT_VERSION_CHECK(5,12,0))
  int end = fm->width (s);
#else
  int end = fm->horizontalAdvance (s);
#endif

  if (loc_x > end)
    return pair<double,int>(end, text.length());
  if (loc_x < 0) {
    return pair<double,int>(-1, 0);
  }

  for (size_t i = 0; i < text.size (); i = next_index (i, text)) {
    int i2 = next_index (i, text);
    string s1 = text.substr (0, i);
    string s2 = text.substr (0, i2);
#if (QT_VERSION < QT_VERSION_CHECK(5,12,0))
    int r1 = fm->width (QString (s1.c_str ()));
    int r2 = fm->width (QString (s2.c_str ()));
#else
    int r1 = fm->horizontalAdvance (QString (s1.c_str ()));
    int r2 = fm->horizontalAdvance (QString (s2.c_str ()));
#endif
    if (loc_x >= r1) {
      if (loc_x <= r2) {
        if (loc_x - r1 <= r2 - loc_x)
          return pair<double, int>(r1, i);
        return pair<double, int>(r2, i2);
      }
    }
  }
    // this should never happen
  return pair<double, int>(0, 0);
}

int
QtBackend::compute_index (FontMetricsImpl fm, SimpleText* t, int x)
{
  QFontMetrics *qfm = (QFontMetrics*) fm;
  string text = t->get_content ();
  if (text.length() == 0 || x <= 0)
    return 0;
  QString s (text.c_str ());
#if (QT_VERSION < QT_VERSION_CHECK(5,12,0))
  int end = qfm->width (s);
#else
  int end = qfm->horizontalAdvance (s);
#endif

  if (x >= end)
    return text.length ();
  int res = 0;
  for (size_t i = 0; i < text.size (); i = next_index (i, text)) {
    int i2 = next_index (i, text);
    string s1 = text.substr (0, i);
    string s2 = text.substr (0, i2);
 #if (QT_VERSION < QT_VERSION_CHECK(5,12,0))
    int r1 = qfm->width (QString (s1.c_str ()));
    int r2 = qfm->width (QString (s2.c_str ()));
 #else
    int r1 = qfm->horizontalAdvance (QString (s1.c_str ()));
    int r2 = qfm->horizontalAdvance (QString (s2.c_str ()));
 #endif
    if (x >= r1) {
      if (x <= r2) {
        if (x - r1 <= r2 - x) {
          res = i;
          break;
        }
        res =  i2;
        break;
      }
    }
  }
  return res;
}

int
QtBackend::compute_x (FontMetricsImpl fm, SimpleText* t, int index)
{
  QFontMetrics *qfm = (QFontMetrics*) fm;
  string text = t->get_content ();
  if (text.length() == 0 || index == 0)
    return 0;
  QString s (text.c_str ());
  if (qfm == nullptr)
    return 0;
  string sub;
  if (index >= text.length()) {
    sub = text;
  } else {
    int i = 0;
    while (i < index) {
      i = next_index (i, text);
    }
    sub = text.substr (0, i);
  }
#if (QT_VERSION < QT_VERSION_CHECK(5,12,0))
  return qfm->width (QString (sub.c_str ()));
#else
  return qfm->horizontalAdvance (QString (sub.c_str ()));
#endif
}
} /* namespace djnn */
