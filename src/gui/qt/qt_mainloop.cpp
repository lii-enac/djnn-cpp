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
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 */

#include "../backend.h"

#if QT

#include "../../core/syshook/main_loop.h"
#include "qt_mainloop.h"

namespace djnn
{

  std::shared_ptr<QtMainloop> QtMainloop::_instance;
  std::once_flag QtMainloop::onceFlag;

  QtMainloop&
  QtMainloop::instance ()
  {
    std::call_once (QtMainloop::onceFlag, [] () {
      _instance.reset(new QtMainloop);
    });

    return *(_instance.get ());
  }

  QtMainloop::QtMainloop () :
      _please_exec (false), _qapp (nullptr), _qevtdispatcher (nullptr)
  {
    MainLoop::another_source_wants_to_be_mainloop = this;
    argc = 0;
    argv = 0;
    _qapp = new QApplication (argc, argv);

    _qevtdispatcher = QAbstractEventDispatcher::instance ();
    QObject::connect (_qevtdispatcher, &QAbstractEventDispatcher::aboutToBlock,
                      [=]() {this->slot_for_about_to_block();});
    QObject::connect (_qevtdispatcher, &QAbstractEventDispatcher::awake, [=]() {this->slot_for_awake();});
  }

  QtMainloop::~QtMainloop ()
  {
    _qapp->quit ();
  }
  void
  QtMainloop::please_stop ()
  {
    ExternalSource::please_stop ();
  }

  void
  QtMainloop::activate_from_mainloop ()
  {
    //set_please_stop (false);
    _qapp->exec ();
  }

  void
  QtMainloop::wakeup ()
  {
    _qevtdispatcher->wakeUp ();
  }

  void
  QtMainloop::slot_for_about_to_block ()
  {
    if (_please_exec) {
      Graph::instance ().exec ();
      _please_exec = false;
    }
    for (auto w : _windows) {
      w->check_for_update ();
    }
    djnn::release_exclusive_access (DBG_REL);
  }

  void
  QtMainloop::slot_for_awake ()
  {
    if (!get_please_stop ()) {
      djnn::get_exclusive_access (DBG_GET);
    } else {
      _qapp->quit ();
    }
  }
}
#endif

