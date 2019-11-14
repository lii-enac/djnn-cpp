#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/control/action.h"
#include "core/tree/ref_property.h"

#include <string>

namespace djnn {

  class UpdateSrcOrDst : public Action
  {
    public:
      UpdateSrcOrDst (Process* p, const string &n, RefProperty* prop, const string &spec, Process** to_update);
      UpdateSrcOrDst () // needed for pointer-less zombie initialization
      //: _to_update(zombie)
      {} 
      
      virtual ~UpdateSrcOrDst () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      Process** _to_update;
      RefProperty* _prop;
      string _spec;

    //static Process* zombie;
  };

  class SrcToDstLink : public Process {
  public:
    SrcToDstLink (bool is_model = false) : Process (is_model) {}
    SrcToDstLink (Process* p, const string &n, bool is_model = false) : Process (n, is_model) {}
    virtual ~SrcToDstLink () {}
    virtual void about_to_update_graph () = 0;
    virtual void update_graph () = 0;
  protected:
    struct ref_info {
      ref_info() : _ref(nullptr) {}
      bool is_ref() const { return _ref != nullptr; }
      RefProperty * _ref;

      // FIXME: name should not be stored, useless loss of memory!! use string tmp_refname1 = string(), tmp_refname2 = string() in constructors to have tmp variable
      //string _spec;
    };
    struct ref_update {
      ref_update() {}
      ref_update(Process *p, const ref_info& ri, const string& spec, Process** to_update) :
        _update(p, "update_src_action", ri._ref, spec, to_update),
        _c(ri._ref, ACTIVATION, &_update, ACTIVATION, true)
        { //_update.impl_activate();
        }
      UpdateSrcOrDst _update;
      Coupling _c;
    };
  };
}