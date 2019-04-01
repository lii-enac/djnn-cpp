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

#include "../backend.h"

#include "../../display/display.h"
#include "../../display/abstract_display.h"

#include "qt_backend.h"
#include "qt_mainloop.h"

namespace djnn
{
  class Backend::Impl
  {
  public:
    QtBackend* qt_backend;
  };

  Backend::Impl* Backend::_instance;

  AbstractBackend*
  Backend::instance ()
  {
    return _instance->qt_backend;
  }

  void
  Backend::init ()
  {
    if (_instance != nullptr)
      return;
    _instance = new Impl ();
    _instance->qt_backend = QtBackend::instance ();
    QtMainloop::instance ();
  }

  class QtDisplayBackend : public AbstractDisplay {
  public:
    static QtDisplayBackend* instance ();
    WinImpl* create_window (Window *win, const std::string& title,  double x, double y, double w, double h);

    struct Impl;
    Impl *impl;

    static std::shared_ptr<QtDisplayBackend> _instance;
    static std::once_flag onceFlag;

  };

  std::shared_ptr<QtDisplayBackend> QtDisplayBackend::_instance;
  std::once_flag QtDisplayBackend::onceFlag;

  WinImpl*
  QtDisplayBackend::create_window (Window *win, const std::string& title,  double x, double y, double w, double h)
  {
    return new QtWindow (win, title, x, y, w, h);
  }

  QtDisplayBackend*
  QtDisplayBackend::instance ()
  {
    //std::cerr << __FILE__ << " " << __LINE__ << std::endl;
    std::call_once (QtDisplayBackend::onceFlag, [] () {
      _instance.reset(new QtDisplayBackend);
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
