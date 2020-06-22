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
 *      Stephane Conversy <mathieu.stephane.conversy@enac.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/control/src_to_dst_link.h"

namespace djnn {

  class AbstractAssignment : public SrcToDstLink {
   friend class Assignment;
   friend class PausedAssignment;
   /* PRIVATE CLASS */
  private:
   
   class AssignmentAction : public Action
   {
   public:
    AssignmentAction (ParentProcess* parent, const std::string& name, FatProcess** src, AbstractProperty** dst, bool propagate) : Action (parent, name), _src (src), _dst (dst), _propagate (propagate) {}
    virtual ~AssignmentAction () {}
    void impl_activate () override { if (_src && _dst) AbstractAssignment::do_assignment(*_src, *_dst, _propagate); };
    void impl_deactivate () override {}
    void set_src (FatProcess** src) { _src = src; }
    void set_dst (AbstractProperty** dst) { _dst = dst; }
   private:
    FatProcess** _src;
    AbstractProperty** _dst;
    bool _propagate;
  };
    
  public:
    AbstractAssignment (ParentProcess* parent, const std::string& name, FatProcess* src, const std::string& ispec, FatProcess* dst, const std::string& dspec, bool isModel,
      const std::string src_ref_spec = std::string(), const std::string dst_ref_spec = std::string()); // hack to create temporary const std::string objects for init of refs
    virtual ~AbstractAssignment ();
    //void update_graph () override;
    static void do_assignment (CoreProcess* src, AbstractProperty* dst, bool propagate);
  
  protected:
    void set_parent (ParentProcess* parent) override;

    struct Init { Init(AbstractAssignment *, const std::string& name, FatProcess* src, const std::string& ispec, FatProcess* dst, const std::string& dspec,
      const std::string src_ref_spec, const std::string dst_ref_spec); };
    friend struct Init;
    void check_init(const std::string& ispec, const std::string& dspec);

    ref_info _ref_info_src, _ref_info_dst;
    Init _init; // will be "created" third
    FatProcess* _src;
    AbstractProperty* _dst;
    ref_update _ref_update_src, _ref_update_dst;
    
    bool _has_coupling;
  };

  class Assignment : public AbstractAssignment {
  private:
    void init_Assignment ();
  public:
    Assignment (ParentProcess* parent, const std::string& name, FatProcess* src, const std::string& ispec, FatProcess* dst, const std::string& dspec, bool isModel);
    Assignment (FatProcess* src, const std::string& ispec, FatProcess* dst, const std::string& dspec, bool isModel);
    void impl_activate () override;
    void post_activate () override { post_activate_auto_deactivate (); }
    void impl_deactivate () override {}
    void update_graph () override;
    void about_to_update_graph () override ;
    virtual ~Assignment ();

  private:
    AssignmentAction _action;

#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

  class PausedAssignment : public AbstractAssignment {
  private:
    void init_PausedAssignment ();
  public:
    PausedAssignment (ParentProcess* parent, const std::string& name, FatProcess* src, const std::string& ispec, FatProcess* dst, const std::string& dspec, bool isModel);
    PausedAssignment (FatProcess* src, const std::string& ispec, FatProcess* dst, const std::string& dspec, bool isModel);
    void impl_activate () override;
    void post_activate () override { post_activate_auto_deactivate (); }
    void impl_deactivate () override {}
    void update_graph () override {};
    void about_to_update_graph () override {};
    virtual ~PausedAssignment ();

  private:
    AssignmentAction _action;

#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

}
