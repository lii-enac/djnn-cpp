#pragma once

#include "../abstract_display.h"

namespace djnn {

class MyQWidget;
class QtWindow;

class QtDisplayBackend : public AbstractDisplay {
  public:
    static QtDisplayBackend* instance ();

    virtual ~QtDisplayBackend() {}

    virtual WinImpl* create_window (Window *win, const std::string& title,  double x, double y, double w, double h) override {
    	std::cerr << __FILE__ << " " << __LINE__ << std::endl;
    	return nullptr;
    }
    virtual MyQWidget* create_qwidget(Window *win, QtWindow*) { return nullptr; }

    struct Impl;
    Impl *impl;

    static std::shared_ptr<QtDisplayBackend> _instance;
    static std::once_flag onceFlag;

  };
}
