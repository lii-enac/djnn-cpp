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
      UpdateSrcOrDst (ParentProcess* parent, const std::string& name, RefProperty* prop, const std::string& spec, FatProcess** to_update);
      UpdateSrcOrDst (ParentProcess* parent, const std::string& name);
      virtual ~UpdateSrcOrDst () {}
      void impl_activate () override;
    private:
      FatProcess** _to_update;
      RefProperty* _prop;
      std::string _spec;

  };

  class SrcToDstLink : public FatProcess {
  public:
    SrcToDstLink (ParentProcess* parent, const std::string& name, bool is_model = false) : FatProcess (name, is_model) {}
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
      ref_update () :
        _update (nullptr, "null_update_src_action") 
        {
        }
      ref_update (ParentProcess* parent, const ref_info& ri, const std::string& spec, FatProcess** to_update) :
        _update(parent, "update_src_action", ri._ref, spec, to_update),
        _c (ri._ref, ACTIVATION, &_update, ACTIVATION, true)
        { //_update.impl_activate();
        }
      UpdateSrcOrDst _update;
      Coupling _c;
    };
  };
}