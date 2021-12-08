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
#include "core/tree/spike.h"
#include "core/tree/ref_property.h"
#include "core/tree/text_property.h"
#include "core/tree/double_property.h"
#include "core/tree/property_trait.h"

#include "base/connector.h"

namespace djnn
{
  typedef enum djnn_dir_t {
    DJNN_GET_ON_CHANGE,
    DJNN_SET_ON_CHANGE,
    DJNN_IGNORE
  } djnn_dir_t;

  class AbstractDeref : public FatProcess
  {
  private:
    class DerefAction : public Action
    {
    public:
      DerefAction (ParentProcess* parent, const string& name) : Action (parent, name) {}
      virtual ~DerefAction () {}
      void impl_activate () override { ((AbstractDeref*)get_parent ())->update_src (); };
    };
    class GetAction : public Action
    {
    public:
      GetAction (ParentProcess *parent, const string& name) : Action (parent, name) {}
      virtual ~GetAction () {}
      void impl_activate () override {
        ((AbstractDeref*)get_parent ())->get ();
      };
    };
    class SetAction : public Action
    {
    public:
      SetAction (ParentProcess *parent, const string& name) : Action (parent, name) {}
      virtual ~SetAction () {}
      void impl_activate () override {
        ((AbstractDeref*)get_parent ())->set ();
      };
    };
    public:
      AbstractDeref (ParentProcess* parent, const string& name, CoreProcess *ref_prop, const string& path, djnn_dir_t dir = DJNN_IGNORE);
      virtual ~AbstractDeref ();
      void impl_activate () override;
      void impl_deactivate () override;
      void update_src ();
      bool propagating () { return _propagating; }
      void set_propagating (bool v) { _propagating = v; }
      virtual void set () = 0;
      virtual void get () = 0;
      virtual void change_src (CoreProcess* src) = 0;
    protected:
      void set_parent (ParentProcess* parent) override;
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
      Deref (ParentProcess* parent, const string& name, CoreProcess *ref_prop, const string& path, djnn_dir_t dir = DJNN_IGNORE);
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
      DerefProperty (ParentProcess* parent, const string& name, CoreProcess *ref_prop, const string& path, djnn_dir_t dir = DJNN_IGNORE);
      void change_src (CoreProcess* src) override;
      
    protected:
      AbstractProperty *_src;
  };

  template <typename T>
  class TDeref : public DerefProperty
  {
    public:
      TDeref (ParentProcess* parent, const string& name, CoreProcess *ref_prop, const string& path, djnn_dir_t dir = DJNN_IGNORE)
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
      DerefString (ParentProcess* parent, const string& name, CoreProcess *ref_prop, const string& path, djnn_dir_t dir = DJNN_IGNORE);
      virtual ~DerefString ();
      void set () override;
      void get () override;
      void change_src (CoreProcess* src) override;
#ifndef DJNN_NO_SERIALIZE
      void serialize (const string& type) override;
#endif
    private:
      TextProperty _value;
      AbstractProperty *_src;
  };
  class DerefDouble : public AbstractDeref
    {
      public:
        DerefDouble (ParentProcess* parent, const string& name, CoreProcess *ref_prop, const string& path, djnn_dir_t dir = DJNN_IGNORE);
        virtual ~DerefDouble ();
        void set () override;
        void get () override;
        void change_src (CoreProcess* src) override;
  #ifndef DJNN_NO_SERIALIZE
        void serialize (const string& type) override;
  #endif
      private:
        DoubleProperty _value;
        AbstractProperty *_src;
    };
    */
}
