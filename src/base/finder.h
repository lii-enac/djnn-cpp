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

#include "core/control/action.h"
#include "core/tree/component.h"
#include "core/control/spike.h"
#include "core/property/text_property.h"
#include "core/property/ref_property.h"
#include "core/ontology/coupling.h"

namespace djnn
{

  class Finder : public FatProcess
  {
  private:
    class FinderAction : public Action
    {
    public:
      FinderAction (ParentProcess* parent, const string& name) : Action (parent, name) {}
      virtual ~FinderAction () {}
      void impl_activate () override;
    };
    public:
      Finder (ParentProcess* parent, const string& name, FatProcess *container, const string& path);
      virtual ~Finder ();
      void impl_activate () override;
      void impl_deactivate () override;
#ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const string& format) override;
#endif
    private:
      void set_parent (ParentProcess* parent) override;
      Container *_container;
      TextProperty _path, _key;
      RefProperty _result;
      Spike _found, _not_found;
      FinderAction _action;
      Coupling _cfind;
  };
}
