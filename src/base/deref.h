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
    class SetToDstAction : public Action
    {
    public:
      SetToDstAction (Process *parent, const std::string& name) : Action (parent, name) {}
      virtual ~SetToDstAction () {}
      void impl_activate () override {
        if (!((AbstractDeref*)get_parent ())->propagating()) {
          ((AbstractDeref*)get_parent ())->set_propagating (true);
          ((AbstractDeref*)get_parent ())->set_to_dst ();
        } else {
          ((AbstractDeref*)get_parent ())->set_propagating (false);
        }
      };
      void impl_deactivate () override {};
    };
    class SetToSrcAction : public Action
    {
    public:
      SetToSrcAction (Process *parent, const std::string& name) : Action (parent, name) {}
      virtual ~SetToSrcAction () {}
      void impl_activate () override {
        if (!((AbstractDeref*)get_parent ())->propagating()) {
          ((AbstractDeref*)get_parent ())->set_propagating (true);
          ((AbstractDeref*)get_parent ())->set_to_src ();
        } else {
          ((AbstractDeref*)get_parent ())->set_propagating (false);
        }
      };
      void impl_deactivate () override {};
    };
    public:
      AbstractDeref (Process *parent, const std::string& name, Process *ref_prop, const std::string& path);
      virtual ~AbstractDeref ();
      void impl_activate () override;
      void impl_deactivate () override;
      void update_src ();
      bool propagating () { return _propagating; }
      void set_propagating (bool v) { _propagating = v; }
      virtual void set_to_src () = 0;
      virtual void set_to_dst () = 0;
      virtual void change_src (Process* src) = 0;
    protected:
      void set_parent (Process* p) override;
      TextProperty _path;
      RefProperty* _ref;
      DerefAction _action;
      SetToDstAction _set_dst;
      SetToSrcAction _set_src;
      Coupling _cref, _cpath, _csrc_to_dst;
      bool _propagating;
  };

  class Deref : public AbstractDeref
  {
  private:
    public:
      Deref (Process *parent, const std::string& name, Process *ref_prop, const std::string& path);
      virtual ~Deref () {}
      void impl_activate () override;
      void impl_deactivate () override;
      void set_to_src () override;
      void set_to_dst () override;
      void change_src (Process* src) override;
#ifndef DJNN_NO_SERIALIZE
      void serialize (const std::string& type) override;
#endif
    private:
      Spike _activation;
      Process *_src;
      Coupling _cdst_to_src;
  };

  class DerefString : public AbstractDeref
  {
    public:
      DerefString (Process *parent, const std::string& name, Process *ref_prop, const std::string& path);
      virtual ~DerefString () {}
      void impl_activate () override;
      void impl_deactivate () override;
      void set_to_src () override;
      void set_to_dst () override;
      void change_src (Process* src) override;
#ifndef DJNN_NO_SERIALIZE
      void serialize (const std::string& type) override;
#endif
    private:
      TextProperty _value;
      AbstractProperty *_src;
      Coupling _cdst_to_src;
  };
}
