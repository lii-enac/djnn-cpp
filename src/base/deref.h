/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "core/control/action.h"
#include "core/tree/component.h"
#include "core/tree/spike.h"
#include "core/tree/ref_property.h"
#include "core/tree/text_property.h"
#include "core/ontology/coupling.h"

namespace djnn
{

  class Deref : public Process
  {
  private:
    class DerefAction : public Action
    {
    public:
      DerefAction (Process *parent, const std::string& name) : Action (parent, name) {}
      virtual ~DerefAction () {}
      void impl_activate () override { ((Deref*)get_parent ())->update_src (); };
      void impl_deactivate () override {};
    };
    public:
      Deref (Process *parent, const std::string& name, Process *ref_prop, const std::string& path);
      virtual ~Deref ();
      void impl_activate () override;
      void impl_deactivate () override;
      void update_src ();
#ifndef DJNN_NO_SERIALIZE
      void serialize (const std::string& type) override;
#endif
    private:
      void set_parent (Process* p) override;
      TextProperty _path;
      RefProperty* _ref;
      Spike _activation;
      DerefAction _action;
      Coupling _cref, _cpath, _cactivation;
  };
}
