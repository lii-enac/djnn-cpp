#pragma once

#include "core/property/int_property.h"
#include "core/property/double_property.h"
#include "core/property/text_property.h"
#include "core/property/bool_property.h"

#include "core/property/property_trait.h"

#include "core/serializer/serializer.h"
#include "base/operators.h"

namespace djnn {


  template <typename T>
  class TPrevious : public FatProcess {
  private:
    class PreviousAction : public Action
    {
    public:
      PreviousAction (CoreProcess* parent, const string& name, TPrevious& np, const T& init_val)
      : Action(parent, name),
      _np(np),
       _prev (init_val) { finalize_construction (parent, name); }
      virtual ~PreviousAction () {}
      void impl_activate ()
      {
        _np._output.set_value (_prev, true);
        _prev = _np._input.get_value ();
      }
    private:
      TPrevious<T> &_np;
      T _prev;
    };
  public:
    TPrevious (CoreProcess* parent, const string& name, const T& i_val);
    virtual ~TPrevious () { uninit_unary_couplings(this, _input, _output, _action, _coupling); }
    void impl_activate () override { _coupling.enable (); _action.activate (); }
    void impl_deactivate () override { _coupling.disable (); _action.deactivate ();};
  protected:
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const string& format) override;
#endif
    typename PropertyTrait<T>::property_type _input;
    typename PropertyTrait<T>::property_type _output;
    PreviousAction _action;
    Coupling _coupling;
  };

  typedef TPrevious<double> PreviousDouble;
  typedef TPrevious<int> PreviousInt;
  typedef TPrevious<bool> PreviousBool;
  typedef TPrevious<djnnstl::string> PreviousText;
  typedef PreviousDouble Previous;

//   using PreviousDouble = TPrevious<double>;
//   using PreviousInt = TPrevious<int>;
//   using PreviousText = TPrevious<string>;
//   using PreviousBool = TPrevious<bool>;
//   using Previous = PreviousDouble;

  
  //inline
  template <typename T>
  TPrevious<T>::TPrevious (CoreProcess* parent, const string& name, const T& i_val)
  : FatProcess (name),
    _input(this, "input", i_val),
    _output(this, "output", i_val),
    _action(this, "action", *this, i_val),
    _coupling(&_input, ACTIVATION, &_action, ACTIVATION)
  {
    init_unary_couplings(_input, _output, _action, _coupling);
    finalize_construction (parent, name);
  }

#ifndef DJNN_NO_SERIALIZE
  //inline
  template <typename T>
  void
  TPrevious<T>::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:previous");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    //AbstractSerializer::serializer->float_attribute ("input", _input.get_value ()); // FIXME
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

#endif
}
}
