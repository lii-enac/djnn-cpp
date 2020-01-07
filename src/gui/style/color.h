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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */


#pragma once

namespace djnn
{
  class AbstractColor : public AbstractStyle
  {
  private:
    class ToValueAction : public Action {
    public:
      ToValueAction (Process *parent, const string& name) : Action (parent, name) { Process::finalize_construction (parent, name); };
      ~ToValueAction () {}
      void impl_activate () override {
        ((AbstractColor*) get_parent())->update_hex_from_rvb ();
      }
      void impl_deactivate () override {}
    };
    class ToRGBAction : public Action {
    public:
      ToRGBAction (Process *parent, const string& name) : Action (parent, name) { Process::finalize_construction (parent, name); };
      ~ToRGBAction () {}
      void impl_activate () override {
        ((AbstractColor*) get_parent())->update_rvb_from_hex ();
      }
      void impl_deactivate () override {}
    };
  public:
    AbstractColor (Process *parent, const std::string& name, int r, int g, int b);
    AbstractColor (Process *parent, const std::string& name, int v);
    virtual ~AbstractColor ();
    
    
    void get_properties_values (double& r, double& g, double& b);
    virtual Process* find_component (const string&) override;
		AbstractIntProperty* r () { return (AbstractIntProperty*) find_component ("r"); }
		AbstractIntProperty* g () { return (AbstractIntProperty*) find_component ("g"); }
		AbstractIntProperty* b () { return (AbstractIntProperty*) find_component ("b"); }
		AbstractIntProperty* value () { return (AbstractIntProperty*) find_component ("value"); }

  protected:
    struct raw_props_t { int r; int g; int b; int value; };
    raw_props_t raw_props;
    Coupling *_cr, *_cg, *_cb, *_cv, *_c_rv, *_c_gv, *_c_bv, *_c_vrgb;
    ToValueAction _toValue;
    ToRGBAction _toRGB;
    bool _is_updating;
    void impl_activate () override;
    void impl_deactivate () override;
    void update_rvb_from_hex ();
    void update_hex_from_rvb ();
  };
}
