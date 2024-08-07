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

#pragma once

#include <QtCore/QAbstractEventDispatcher>
#include <QtWidgets/QApplication>

#include "core/core-dev.h" // graph add/remove edge
#include "core/utils/algorithm.h"
#include "exec_env/external_source.h"

namespace djnn {

class QtMainloopListener {
  public:
    virtual ~QtMainloopListener () {}
    virtual void slot_for_about_to_block ();
};

class MainLoop;

class QtMainloop : public ExternalSource {

  public:
    static void        build_instance (MainLoop*);
    static QtMainloop& instance ();
    static void        clear ();
    virtual ~QtMainloop ();
    virtual void please_stop () override;
    void         run () override;
    void         wakeup ();
    void         set_please_exec (bool exec) { _please_exec = exec; }
    // void add_window (QtWindow* win) { _windows.push_back (win); }
    // void remove_window (QtWindow* win) { _windows.erase (remove (_windows.begin (), _windows.end (), win), _windows.end ()); }
    void add_listener (QtMainloopListener* mll) { _mlls.push_back (mll); }
    void remove_listener (QtMainloopListener* mll) { _mlls.erase (djnnstl::remove (_mlls.begin (), _mlls.end (), mll), _mlls.end ()); }

    QApplication* get_QApplication () { return _qapp; }

  private:
    static QtMainloop* _instance;
    QtMainloop (MainLoop*);
    bool                      _please_exec;
    QApplication*             _qapp;
    QAbstractEventDispatcher* _qevtdispatcher;
    void                      slot_for_awake ();
    void                      slot_for_about_to_block ();
    // vector<QtWindow*> _windows;
    djnnstl::vector<QtMainloopListener*> _mlls;
    bool                                 already_awaken;
    int                                  argc;
    char**                               argv;
};
} // namespace djnn
