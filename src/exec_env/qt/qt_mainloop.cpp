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
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include <mutex> // std::call_once

#include "qt_mainloop.h"

#include "exec_env/global_mutex.h"
#include "exec_env/main_loop.h"

// #include "core/utils/iostream.h"
// #include "utils/debug.h"

namespace djnn {
using namespace djnnstl;

QtMainloop* QtMainloop::_instance;
// static std::once_flag onceFlag;

void
QtMainloop::build_instance (MainLoop* ml)
{
    _instance = new QtMainloop (ml);
}

QtMainloop&
QtMainloop::instance ()
{
    return *(_instance);
}

QtMainloop::QtMainloop (MainLoop* ml)
    : ExternalSource ("Qt"),
      _please_exec (false), _qapp (nullptr), _qevtdispatcher (nullptr), already_awaken (false)
{
    (*ml).set_another_source_wants_to_be_mainloop (this);
    argc  = 0;
    argv  = 0;
    _qapp = new QApplication (argc, argv);

    _qevtdispatcher = QAbstractEventDispatcher::instance ();
    QObject::connect (_qevtdispatcher, &QAbstractEventDispatcher::aboutToBlock,
                      [=] () { this->slot_for_about_to_block (); });
    QObject::connect (_qevtdispatcher, &QAbstractEventDispatcher::awake, [=] () { this->slot_for_awake (); });
}

QtMainloop::~QtMainloop ()
{
    // std::cerr << __PRETTY_FUNCTION__ << " " << this << std::endl;
    _qapp->quit ();
}

void
QtMainloop::please_stop ()
{
    _qapp->quit ();
    /* SL+MM : l'appel à please_stop demande l'interruption du thread via interrupt
     * ce qui est susceptible suivant les versions de l'OS de plus ou moins bien
     * se passer. A creuser, en attendant on demande à qt de quitter et ça résoud
     * le bug de core dump sous ubuntu (appels multiples de exit)
     * https://stackoverflow.com/questions/6375121/when-using-boostthreadinterrupt-do-you-need-to-catch-the-thread-interru
     * https://quantnet.com/threads/c-multithreading-in-boost.10028/ (18.8)
     *ExternalSource::please_stop ();
     */
}

void
QtMainloop::run ()
{
    // std::cerr << __PRETTY_FUNCTION__ << " " << this << std::endl;
    /* slot_about_to_block will / *might* be called ASA qapp->exec */
    // usleep(10000000); // QObject::_start_timer: Timers can only be used with threads started with QThread FIXME
    _qapp->exec ();
}

void
QtMainloop::wakeup ()
{ // DBG;
    _qevtdispatcher->wakeUp ();
}

void
QtMainloopListener::slot_for_about_to_block ()
{
}

void
QtMainloop::slot_for_about_to_block ()
{
    // DBG;
    if (_please_exec) {
        // DBG;
        djnn::get_exclusive_access (DBG_GET);
        GRAPH_EXEC;
        djnn::release_exclusive_access (DBG_REL);
        _please_exec = false;
    }

    for (auto mll : _mlls)
        mll->slot_for_about_to_block ();

    already_awaken = false;
}

void
QtMainloop::slot_for_awake ()
{
    if (already_awaken) {
        return;
    }

    if (!get_please_stop ()) {
        // now qt can call event method on windows
        already_awaken = true;
    } else
        _qapp->quit ();
}
} // namespace djnn
