#pragma once

#include "base/operators.h"
#include "core/property/bool_property.h"
#include "core/property/double_property.h"
#include "core/property/int_property.h"
#include "core/property/property_trait.h"
#include "core/property/text_property.h"
#include "core/serializer/serializer.h"


namespace djnn {

template <typename T>
class TPrevious : public FatProcess {
  private:
    class PreviousAction : public Action {
      public:
        PreviousAction (CoreProcess* parent, const string& name, TPrevious& prev, const T& init_val, bool diff_only=false)
            : Action (parent, name),
              _prev (prev),
              _current_value (init_val),
              _diff_only (diff_only) { finalize_construction (parent, name); }
        virtual ~PreviousAction () {}
        void impl_activate () { // input has been activated with a new value
          if (!_diff_only || (_current_value != _prev._input.get_value ()) ) {
            _prev._output.set_value (_current_value, true); // set the output property with the previously stored "current" value
            _current_value = _prev._input.get_value ();     // store the new value into current value
          }
        }

      private:
        TPrevious<T>& _prev;          // ref to the Process Previous
        T             _current_value; // allow to store the new value (input) that will be copied into previous value (output)
        bool          _diff_only;     // indicating if the output property (previous value) is set if the new value (input property) is the same

    };

  public:
    TPrevious (CoreProcess* parent, const string& name, const T& init_val, bool diff_only=false);
    virtual ~TPrevious () { uninit_unary_couplings (this, _input, _output, _action, _coupling); }
    void impl_activate () override {
        _coupling.enable ();
        _action.activate ();
    }
    void impl_deactivate () override {
        _coupling.disable ();
        _action.deactivate ();
    };

  protected:
#ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const string& format) override;
#endif
    typename PropertyTrait<T>::property_type _input;  // store the new value
    typename PropertyTrait<T>::property_type _output; // store the previous value
    PreviousAction                           _action;
    Coupling                                 _coupling;
};

typedef TPrevious<double>          PreviousDouble;
typedef TPrevious<int>             PreviousInt;
typedef TPrevious<bool>            PreviousBool;
typedef TPrevious<djnnstl::string> PreviousText;
typedef PreviousDouble             Previous;

//   using PreviousDouble = TPrevious<double>;
//   using PreviousInt = TPrevious<int>;
//   using PreviousText = TPrevious<string>;
//   using PreviousBool = TPrevious<bool>;
//   using Previous = PreviousDouble;

// inline
template <typename T>
TPrevious<T>::TPrevious (CoreProcess* parent, const string& name, const T& init_val, bool diff_only)
    : FatProcess (name),
      _input (this, "input", init_val),    // store the new value
      _output (this, "output", init_val),  // store the previous value
      _action (this, "action", *this, init_val, diff_only),
      _coupling (&_input, ACTIVATION, &_action, ACTIVATION) {
    init_unary_couplings (_input, _output, _action, _coupling);
    finalize_construction (parent, name);
}

template<>
inline void
TPrevious<double>::PreviousAction::impl_activate () // input has been activated with a new value
{
  if (!_diff_only || (abs (_prev._input.get_value () - _current_value) > 1e-9) ) {
    _prev._output.set_value (_current_value, true); // set the output property with the previously stored "current" value
    _current_value = _prev._input.get_value ();     // store the new value into current value
  }
}


#ifndef DJNN_NO_SERIALIZE
// inline
template <typename T>
void TPrevious<T>::serialize (const string& type) {

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:previous");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    // AbstractSerializer::serializer->float_attribute ("input", _input.get_value ()); // FIXME
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);

#endif
}
} // namespace djnn
