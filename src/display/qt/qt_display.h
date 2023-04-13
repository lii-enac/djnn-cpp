#pragma once


#include "display/abstract_display.h"

namespace djnn {

  class MyQWidget;
  class QtWindow;

  class QtDisplayBackend : public AbstractDisplay {
    public:
      static QtDisplayBackend* instance ();

      virtual ~QtDisplayBackend() {}

      virtual WinImpl* create_window (Window *win, const string& title,  double x, double y, double w, double h) override {
        //std::cerr << __FILE__ << " " << __LINE__ << std::endl;
        return nullptr;
      }
      virtual MyQWidget* create_qwidget(Window *win, QtWindow*) { return nullptr; }

      struct Impl;
      Impl *impl;

      virtual void slot_for_about_to_block ();

      void add_window (QtWindow* win) { _windows.push_back (win); }
      void remove_window (QtWindow* win) { _windows.erase (std::remove (_windows.begin (), _windows.end (), win), _windows.end ()); }

  private:
      vector<QtWindow*> _windows;

    };
}
