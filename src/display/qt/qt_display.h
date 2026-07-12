/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019-2024)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
#pragma once

#include "core/utils/containers.h"
#include "display/abstract_display.h"

namespace djnn {

using djnnstl::string;
using djnnstl::vector;

class MyQWidget;
class QtWindow;

class QtDisplayBackend : public AbstractDisplay {
  public:
    static QtDisplayBackend* instance ();

    virtual ~QtDisplayBackend () {}

    virtual WinImpl* create_window (Window* win, const string& title, double x, double y, double w, double h) override {
        // std::cerr << __FILE__ << " " << __LINE__ << std::endl;
        return nullptr;
    }
    virtual MyQWidget* create_qwidget (Window* win, QtWindow*) { return nullptr; }

    struct Impl;
    Impl* impl;

    virtual void slot_for_about_to_block ();

    void add_window (QtWindow* win);
    void remove_window (QtWindow* win);

  private:
    vector<QtWindow*> _windows;
};
} // namespace djnn
