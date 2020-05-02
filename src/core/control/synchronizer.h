/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/control/native_expression_action.h"


namespace djnn {
  //using namespace std;

  class Synchronizer : public FatProcess
  {
    friend class BindingAction;
  private:
    class SynchronizerAction : public NativeExpressionAction
    {
    public:
      SynchronizerAction (FatProcess* parent, const std::string& name) : NativeExpressionAction (parent, name) {}
      virtual ~SynchronizerAction () {};
      void impl_activate () override {
        if (get_parent ()->somehow_activating ())
          ((Synchronizer*) get_parent ())->propagate ();
      }
      void impl_deactivate () override {}
    };
  public:
    Synchronizer (FatProcess* parent, const std::string& name, CoreProcess* dst, const std::string&  dspec);
    virtual ~Synchronizer ();
    void add_source (CoreProcess* src, const std::string& spec);
    void impl_activate () override;
    void impl_deactivate () override;
    void add_native_edge (CoreProcess *src, CoreProcess* dst);
    
  private:
    void propagate ();

    void set_parent (FatProcess* p) override;
    struct Init { Init(Synchronizer *, FatProcess *parent, const std::string& name, CoreProcess* dst, const std::string&  dspec); };
    friend struct Init;
    CoreProcess *_dst;
    Init _init;
    std::vector<Coupling*> _c_list;
    SynchronizerAction _action;
  };
}
