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
      BindingAction (Process* parent, const string &name, Process* dst) : Process (parent, name), _dst (dst) {}
      virtual ~BindingAction () {};
      void activate () override { _dst->activation (); }
      void deactivate () override {}
    private:
      Process* _dst;
    };
  public:
    Binding (Process* parent, const string &name, Process* src, const string & ispec, Process* dst, const string & dspec);
    Binding (Process* src, const string & ispec, Process* dst, const string & dspec);
    virtual ~Binding ();
    void activate () { _c_src->enable(); };
    void deactivate () { _c_src->disable (); }
    void exec ();
  private:
    int init_binding (Process* src, const string & ispec, Process* dst, const string & dspec);
    Process *_src, *_dst;
    std::unique_ptr<Coupling> _c_src;
    std::unique_ptr<BindingAction> _action;
  };
}
