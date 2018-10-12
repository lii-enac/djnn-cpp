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

#pragma once
#include "../backend.h"

#include "qt_window.h"
#include "../../core/syshook/external_source.h"
#include <QtCore/QAbstractEventDispatcher>
#include <QtWidgets/QApplication>
#include <vector>


namespace djnn {
  using namespace std;
  class QtMainloop : public ExternalSource {
  public:
    static QtMainloop& instance ();
    static void clear ();
    virtual ~QtMainloop ();
    virtual void please_stop () override ;
    //virtual void activate_from_mainloop () override;
    void run() override;// {}
    void wakeup ();
    void set_please_exec (bool exec) { _please_exec = exec; }
    void add_window (QtWindow* win) { _windows.push_back (win); }
    void remove_window (QtWindow* win) { _windows.erase (remove (_windows.begin (), _windows.end (), win), _windows.end ()); }
  protected:
    //void run() override {}
  private:
    static shared_ptr<QtMainloop> _instance;
    static once_flag onceFlag;
    QtMainloop ();
    bool _please_exec;
    QApplication* _qapp;
    QAbstractEventDispatcher*_qevtdispatcher;
    void slot_for_awake ();
    void slot_for_about_to_block ();
    vector<QtWindow*> _windows;
    bool already_awake;
    int argc;
    char **argv;
  };
}
