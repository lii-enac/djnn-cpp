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

#include "core/ontology/coupling.h"
#include "core/control/action.h"
#include "core/ontology/process.h"
#include "core/control/spike.h"
#include "core/property/ref_property.h"
#include "core/property/text_property.h"
#include "core/property/double_property.h"
#include "core/property/property_trait.h"

#include "base/enums.h"

namespace djnn
{
  class AbstractDeref : public FatProcess
  {
  private:
    class DerefAction : public Action
    {
    public:
      DerefAction (CoreProcess* parent, const string& name) : Action (parent, name) {}
      virtual ~DerefAction () {}
      void impl_activate () override { ((AbstractDeref*)get_parent ())->update_src (); };
    };
    class GetAction : public Action
    {
    public:
      GetAction (CoreProcess *parent, const string& name) : Action (parent, name) {}
      virtual ~GetAction () {}
      void impl_activate () override {
        ((AbstractDeref*)get_parent ())->get ();
      };
    };
    class SetAction : public Action
    {
    public:
      SetAction (CoreProcess *parent, const string& name) : Action (parent, name) {}
      virtual ~SetAction () {}
      void impl_activate () override {
        ((AbstractDeref*)get_parent ())->set ();
      };
    };
    public:
      AbstractDeref (CoreProcess* parent, const string& name, CoreProcess *ref_prop, const string& path, djnn_dir_t dir = DJNN_IGNORE);
      virtual ~AbstractDeref ();
      void impl_activate () override;
      void impl_deactivate () override;
      void update_src ();
      bool propagating () { return _propagating; }
      void set_propagating (bool v) { _propagating = v; }
      RefProperty* get_ref () { return _ref; }
      const RefProperty* get_ref () const { return _ref; }
      const string& get_str_path () const { return _path.get_value (); }
      djnn_dir_t get_dir () const { return _dir; }
      virtual void set () = 0;
      virtual void get () = 0;
      virtual void change_src (CoreProcess* src) = 0;
    protected:
      void set_parent (CoreProcess* parent) override;
      TextProperty _path;
      RefProperty* _ref;
      DerefAction _action;
      SetAction _set;
      GetAction _get;
      Coupling _cref, _cpath, _cget, _cset;
      djnn_dir_t _dir;
      bool _propagating;
      bool _src_updated;
  };

  class Deref : public AbstractDeref
  {
  private:
    public:
      Deref (CoreProcess* parent, const string& name, CoreProcess *ref_prop, const string& path, djnn_dir_t dir = DJNN_IGNORE);
      virtual ~Deref ();
      void set () override;
      void get () override;
      void change_src (CoreProcess* src) override;
      
#ifndef DJNN_NO_SERIALIZE
      void serialize (const string& type) override;
#endif
    private:
      Spike _activation;
      CoreProcess *_src;
      Coupling _cdst_to_src;
  };

  class DerefProperty : public AbstractDeref
  {
  private:
    public:
      DerefProperty (CoreProcess* parent, const string& name, CoreProcess *ref_prop, const string& path, djnn_dir_t dir = DJNN_IGNORE);
      void change_src (CoreProcess* src) override;
      
    protected:
      AbstractSimpleProperty *_src;
  };

  template <typename T>
  class TDeref : public DerefProperty
  {
    public:
      TDeref (CoreProcess* parent, const string& name, CoreProcess *ref_prop, const string& path, djnn_dir_t dir = DJNN_IGNORE)
      : DerefProperty (parent, name, ref_prop, path, dir),
      _value (this, "value", T())
      {
        _cset.init (&_value, ACTIVATION, &_set, ACTIVATION);
        finalize_construction (parent, name);
      }

      virtual ~TDeref () { _cset.uninit (); }
      
      void get () override {
        if (_src) {
          _cset.disable ();
          _value.set_value (PropertyTrait<T>::get_value(_src), true);
          _cset.enable ();
        }
      }
      void set () override {
        if (_src) {
          _cget.disable ();
          _src->set_value (_value.get_value (), true);
          _cget.enable ();
        }
      }

      CoreProcess* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override
      {
        auto ref = origs_clones.find (get_ref ());
        auto s = ref != origs_clones.end () ? ref->second : get_ref ();
        auto res = new TDeref<T> (nullptr, get_name (), const_cast<CoreProcess*>(s), get_str_path (), get_dir ());
        origs_clones[this] = res;
        origs_clones[&_value] = res->get_value ();
        return res;
      }

      typename PropertyTrait<T>::property_type* get_value () { return &_value; }

#ifndef DJNN_NO_SERIALIZE
      void serialize (const string& type) override;
#endif
    private:
      typename PropertyTrait<T>::property_type _value;
  };

  typedef TDeref<double> DerefDouble;
  typedef TDeref<string> DerefText;
  typedef TDeref<string> DerefString;
  typedef TDeref<int> DerefInt;
  typedef TDeref<bool> DerefBool;

  

  /*
  class DerefString : public AbstractDeref
  {
    public:
      DerefString (CoreProcess* parent, const string& name, CoreProcess *ref_prop, const string& path, djnn_dir_t dir = DJNN_IGNORE);
      virtual ~DerefString ();
      void set () override;
      void get () override;
      void change_src (CoreProcess* src) override;
#ifndef DJNN_NO_SERIALIZE
      void serialize (const string& type) override;
#endif
    private:
      TextProperty _value;
      AbstractSimpleProperty *_src;
  };
  class DerefDouble : public AbstractDeref
    {
      public:
        DerefDouble (CoreProcess* parent, const string& name, CoreProcess *ref_prop, const string& path, djnn_dir_t dir = DJNN_IGNORE);
        virtual ~DerefDouble ();
        void set () override;
        void get () override;
        void change_src (CoreProcess* src) override;
  #ifndef DJNN_NO_SERIALIZE
        void serialize (const string& type) override;
  #endif
      private:
        DoubleProperty _value;
        AbstractSimpleProperty *_src;
    };
    */
}
