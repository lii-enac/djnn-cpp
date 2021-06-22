/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#pragma once

#include "abstract_property.h"

#include <string>

namespace djnn {

  class AbstractRemoteProperty : public AbstractProperty {
  public:
    AbstractRemoteProperty (ParentProcess* parent, const string& name, int notify_mask=notify_none) : AbstractProperty (parent, name, notify_mask), _send (false) { finalize_construction (parent, name); };
    virtual int get_prop_type () const override { return Dist; }

    // AbstractProperty interface
    void set_value (int newValue, bool propagate) override;
    void set_value (double v, bool propagate) override;
    void set_value (bool v, bool propagate) override;
    void set_value (CoreProcess* v, bool propagate) override;
    void set_value (const string& v, bool propagate) override;
    void set_incoming_value (const string& v, bool propagate);
    void set_value (const char* v, bool propagate) override { set_value(string(v), propagate); };
    double get_double_value () override;
    double get_double_value () const override;
    string get_string_value () override { return get_ref_value (); }
    string get_string_value () const override { return get_ref_value (); }
    string& get_value () { return get_ref_value(); };
    bool send () { return _send; }
  protected:
    virtual string& get_ref_value() = 0;
    virtual const string& get_ref_value() const= 0;
    bool _send;
  public:
#ifndef DJNN_NO_DEBUG
    void dump (int level=0) override;
#endif
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
  };

  class RemoteProperty : public AbstractRemoteProperty {
  public:
    RemoteProperty (ParentProcess* parent, const string& name, const string& v) : AbstractRemoteProperty (parent, name), _value(v) { }
    FatProcess* clone () override;

    /* This method has to be specific because of the duality of a remote property.
     * Indeed it can be used to transmit values, as any other property, or to
     * transmit the (de)activation of another process. In this latter case, the coupling usually set a flag
     * that is read by the GRAPH_EXEC. But, in these case it has no effect because the properties are removed from
     * the execution graph. Consequently, we have to implement a specific behavior.
     */
    void set_activation_flag (activation_flag_e VALUE) override {
      switch (VALUE){
        case ACTIVATION:
          set_value ("ACTIVATION", true);
          break;
        case DEACTIVATION:
          set_value ("DEACTIVATION", true);
          break;
        default:;
      }
    }
    void sent () { _send = false; }
  protected:
    virtual string& get_ref_value() override { return _value; }
    virtual const string& get_ref_value() const override { return _value; }
  private:
    string _value;
  };
}
