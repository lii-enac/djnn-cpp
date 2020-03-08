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

#include "display/display.h"
#include "display/abstract_display.h"

#include "display/qt/qt_display.h"
#include "display/qt/qt_window.h"
#include "syshook/qt/qt_mainloop.h"

#include "my_qqwindow.h"

#include <iostream>

namespace djnn {

  class QtQtDisplayBackend : public QtDisplayBackend {
  public:
    //static QtDisplayBackend* instance ();
    virtual WinImpl* create_window (Window *win, const std::string& title,  double x, double y, double w, double h) override;
    virtual MyQWidget* create_qwidget(Window *win, QtWindow*) override;

    struct Impl;
    Impl *impl;

    static std::shared_ptr<QtQtDisplayBackend> _instance;
    static std::once_flag onceFlag;

    static QtQtDisplayBackend* instance ();

  };

  std::shared_ptr<QtQtDisplayBackend> QtQtDisplayBackend::_instance;
  std::once_flag QtQtDisplayBackend::onceFlag;

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

  class QtDisplayBackendMainLoopListener : public QtMainloopListener {
  public:
    QtDisplayBackendMainLoopListener(QtDisplayBackend* b) : _backend(b) {}
    virtual void slot_for_about_to_block () {
      _backend->slot_for_about_to_block();
    }
  private:
    QtDisplayBackend* _backend;
  };

  extern QtDisplayBackend* __instance;
  
  QtQtDisplayBackend*
  QtQtDisplayBackend::instance ()
  {
    //std::cerr << __FILE__ << " " << __LINE__ << std::endl;
    std::call_once (QtQtDisplayBackend::onceFlag, [] () {
      _instance.reset(new QtQtDisplayBackend);
      QtMainloop::instance ().add_listener(new QtDisplayBackendMainLoopListener(_instance.get ()));
      __instance = _instance.get ();
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
    _instance->backend = QtQtDisplayBackend::instance ();
    //std::cerr << __FILE__ << " " << __LINE__ << std::endl;
  }

  void
  p_init_p_display() {
    DisplayBackend::init ();
  }

}
