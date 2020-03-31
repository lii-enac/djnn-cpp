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

#include "base/connector.h"
#include "core/control/action.h"
#include "core/ontology/process.h"
#include "core/tree/spike.h"
#include "core/tree/ref_property.h"
#include "core/tree/text_property.h"
#include "core/ontology/coupling.h"

namespace djnn
{
  class AbstractDeref : public Process
  {
  private:
    class DerefAction : public Action
    {
    public:
      DerefAction (Process *parent, const std::string& name) : Action (parent, name) {}
      virtual ~DerefAction () {}
      void impl_activate () override { ((AbstractDeref*)get_parent ())->update_src (); };
      void impl_deactivate () override {};
    };
    public:
      AbstractDeref (Process *parent, const std::string& name, Process *ref_prop, const std::string& path);
      virtual ~AbstractDeref ();
      void impl_activate () override;
      void impl_deactivate () override;
      virtual void update_src () = 0;
    protected:
      void set_parent (Process* p) override;
      TextProperty _path;
      RefProperty* _ref;
      DerefAction _action;
      Coupling _cref, _cpath;
  };

  class Deref : public AbstractDeref
  {
  private:
    public:
      Deref (Process *parent, const std::string& name, Process *ref_prop, const std::string& path);
      virtual ~Deref () {}
      void impl_activate () override;
      void impl_deactivate () override;
      void update_src () override;
#ifndef DJNN_NO_SERIALIZE
      void serialize (const std::string& type) override;
#endif
    private:
      Spike _activation;
      Coupling _cactivation;
  };

  class DerefString : public AbstractDeref
  {
    public:
      DerefString (Process *parent, const std::string& name, Process *ref_prop, const std::string& path);
      virtual ~DerefString ();
      void impl_activate () override;
      void impl_deactivate () override;
      void update_src () override;
#ifndef DJNN_NO_SERIALIZE
      void serialize (const std::string& type) override;
#endif
    private:
      TextProperty _value;
      Connector *_cvalue;
  };
}
