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

#include "core/ontology/process.h"
#include "core/control/action.h"

namespace djnn {

class NativeExpressionAction : public Action {
  public:
    NativeExpressionAction (ParentProcess* parent, const std::string& name, bool model = false);
    virtual ~NativeExpressionAction ();

    void add_native_edge (CoreProcess * src, CoreProcess * dst);
    void remove_native_edge (CoreProcess * src, CoreProcess * dst);
    void remove_all_native_edges ();

    // make finalize_construction public and callable at the right place in code generated by smalac
    virtual void finalize_construction (ParentProcess* parent, const std::string& name, CoreProcess* state=nullptr) {
      Action::finalize_construction (parent, name, state);
    }

  private:
    CoreProcess *_src;
    std::vector<CoreProcess*> _dsts;
    bool _my_parent_is_an_assignmentsequence;
  };
}
