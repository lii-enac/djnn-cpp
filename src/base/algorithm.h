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
#include "core/tree/spike.h"
#include "core/tree/component.h"
#include "core/tree/text_property.h"
#include "core/tree/bool_property.h"
#include "core/control/action.h"

namespace djnn
{

  class Sorter : public Process
  {
  private:
    class SortAction : public Action
    {
    public:
      SortAction (Process* parent, const std::string &name) : Action(parent, name) { Process::finalize_construction (parent); }
      virtual ~SortAction () {}
      void impl_activate () { ((Sorter*) get_parent())->sort (); }
      void impl_deactivate () {}
    };
  public:
    Sorter (Process *p, const std::string &name, Process *container, const std::string& spec);
    virtual ~Sorter () { }
    void impl_activate () override;
    void impl_deactivate () override;
    void sort ();
  private:
    int partition (int i, int j);
    void quick_sort (int i_first, int i_last);
    bool compare (AbstractProperty* left, AbstractProperty* right);
    AbstractProperty* get_and_check (int i);
  protected:
    void serialize (const std::string& type) override;
    BoolProperty _ascending;
    TextProperty _spec;
    Container *_container;
    SortAction _action;
    Spike _sort;
    Coupling _c_sort_action, _c_spec_action;
  };
}
