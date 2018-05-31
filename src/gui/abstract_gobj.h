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

#pragma once

#include "../core/double_property.h"
#include "../core/process.h"
#include "../core/component.h"
#include "../core/execution/component_observer.h"
#include "window.h"
#include <iostream>

namespace djnn
{

  extern bool gui_initialized;

  class AbstractGObj : public Process
  {
  public:
    AbstractGObj () : Process(), _frame (nullptr) {
      if (!gui_initialized) std::cout << "WARNING: gui not initialized" << std::endl;
      _cpnt_type = GOBJ;
    }
    AbstractGObj (Process *p, const std::string& n) : Process (p, n), _frame (nullptr) {
      if (!gui_initialized) std::cout << "WARNING: gui not initialized" << std::endl;
      _cpnt_type = GOBJ;
    }
    virtual ~AbstractGObj () {};
    Window*& frame () { return _frame; }
    void activate () override;
    void deactivate () override;
  protected:
    Window *_frame;
  };


  class UpdateDrawing : public Process
  {
  public:
    static UpdateDrawing* instance ();
    ~UpdateDrawing () {};
    void activate () override;
    void deactivate () override {}
    void set_activation_flag (int flag) override;
    static void init ();
    static void clear ();
  private:
    UpdateDrawing (const UpdateDrawing&) = delete;
    UpdateDrawing & operator=(const UpdateDrawing&) = delete;
    static std::shared_ptr<UpdateDrawing> _instance;
    static std::once_flag onceFlag;
    std::vector<Window*> _win_list;
    UpdateDrawing () : Process () {};
  };
}
