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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "../tree/process.h"
#include "coupling.h"

#include <string>

namespace djnn {
  using namespace std;

  class Binding : public Process
  {
  private:
    class BindingAction : public Process
    {
    public:
      BindingAction (Process* parent, const string &name, Process *src, Process* dst, bool activate) : Process (parent, name), _dst (dst), _src (src), _activate (activate){}
      virtual ~BindingAction () {};
      void activate () override {
	_dst->set_activation_source (_src);
	if (_activate) {
	  _dst->activation ();
	}
	else
	  _dst->deactivation ();
      }
      void deactivate () override {}
      void exec (int flag) override { activate (); }
    private:
      Process* _dst, *_src;
      bool _activate;
    };
  public:
    Binding (Process* parent, const string &name, Process* src, const string & ispec, Process* dst, const string & dspec);
    Binding (Process* parent, const string &name, Process* src, bool on_activation, const string & ispec, Process* dst, const string & dspec, bool activate);
    Binding (Process* src, const string & ispec, Process* dst, const string & dspec);
    virtual ~Binding ();
    void activate () override { _c_src->enable(); };
    void deactivate () override { _c_src->disable (); }
    void serialize (const string& format) override;
  private:
    void init_binding (Process* src, const string & ispec, bool on_activation, Process* dst, const string & dspec, bool to_activate);
    Process *_src, *_dst;
    Coupling *_c_src;
    BindingAction *_action;
  };
}
