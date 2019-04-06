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

#include "../../../display/display.h"
#include "../../../display/abstract_display.h"

#include "../../../display/qt/qt_display.h"
#include "../../../display/qt/qt_window.h"
#include "../../../display/qt/qt_mainloop.h"

#include "my_qqwindow.h"

#include <iostream>

namespace djnn {
  #if 1
  class QtQtDisplayBackend : public QtDisplayBackend {
  public:
    //static QtDisplayBackend* instance ();
    virtual WinImpl* create_window (Window *win, const std::string& title,  double x, double y, double w, double h) override;
    virtual MyQWidget* create_qwidget(Window *win, QtWindow*) override;

    struct Impl;
    Impl *impl;

    static std::shared_ptr<QtDisplayBackend> _instance;
    static std::once_flag onceFlag;

  };
#endif
  std::shared_ptr<QtDisplayBackend> QtDisplayBackend::_instance;
  std::once_flag QtDisplayBackend::onceFlag;

  WinImpl*
  QtQtDisplayBackend::create_window (Window *win, const std::string& title,  double x, double y, double w, double h)
  {
    //std::cerr << __FILE__ << " " << __LINE__ << std::endl;
    return new QtWindow (win, title, x, y, w, h);
  }

  MyQWidget*
  QtQtDisplayBackend::create_qwidget(Window *win, QtWindow *qtwin)
  {
    return new MyQQWidget (win, qtwin);
  }

  QtDisplayBackend*
  QtDisplayBackend::instance ()
  {
    //std::cerr << __FILE__ << " " << __LINE__ << std::endl;
    std::call_once (QtDisplayBackend::onceFlag, [] () {
      _instance.reset(new QtQtDisplayBackend);
    });

    return _instance.get ();
  }

  void
  DisplayBackend::init ()
  {
    //std::cerr << __FILE__ << " " << __LINE__ << std::endl;
    if (_instance != nullptr)
      return;
    _instance = new Impl ();
    _instance->backend = QtDisplayBackend::instance ();
    //std::cerr << __FILE__ << " " << __LINE__ << std::endl;
  }

  void p_init_p_display() {
    DisplayBackend::init ();
  }

}
