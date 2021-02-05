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

#include "core/control/action.h"
#include "abstract_style.h"


namespace djnn
{
  class AbstractColor : public AbstractStyle
  {
  private:
    class ToValueAction : public Action {
    public:
      ToValueAction (ParentProcess* parent, const std::string& name) : Action (parent, name) { finalize_construction (parent, name); };
      ~ToValueAction () {}
      void impl_activate () override {
        ((AbstractColor*) get_parent())->update_hex_from_rvb ();
      }
    };
    class ToRGBAction : public Action {
    public:
      ToRGBAction (ParentProcess* parent, const std::string& name) : Action (parent, name) { finalize_construction (parent, name); };
      ~ToRGBAction () {}
      void impl_activate () override {
        ((AbstractColor*) get_parent())->update_rvb_from_hex ();
      }
    };
  public:
    AbstractColor (ParentProcess* parent, const std::string& name, int r, int g, int b);
    AbstractColor (ParentProcess* parent, const std::string& name, int v);
    virtual ~AbstractColor ();
    
    void get_properties_values (double& r, double& g, double& b);
    virtual FatChildProcess* find_child_impl (const std::string&) override;
		AbstractIntProperty* r () { return (AbstractIntProperty*) find_child_impl ("r"); }
		AbstractIntProperty* g () { return (AbstractIntProperty*) find_child_impl ("g"); }
		AbstractIntProperty* b () { return (AbstractIntProperty*) find_child_impl ("b"); }
		AbstractIntProperty* value () { return (AbstractIntProperty*) find_child_impl ("value"); }

  protected:
    struct raw_props_t { int r; int g; int b; int value; };
    raw_props_t raw_props;
    CouplingWithData *_cr, *_cg, *_cb, *_cv;
    Coupling *_c_rv, *_c_gv, *_c_bv, *_c_vrgb;
    ToValueAction _toValue;
    ToRGBAction _toRGB;
    bool _is_updating;
    void impl_activate () override;
    void impl_deactivate () override;
    void update_rvb_from_hex ();
    void update_hex_from_rvb ();
  };

  class FillColor : public AbstractColor
  {
  public:
    FillColor (ParentProcess* parent, const std::string& name, int r, int g, int b) :
        AbstractColor (parent, name, r, g, b) { finalize_construction (parent, name); }
    FillColor (ParentProcess* parent, const std::string& name, int v) :
        AbstractColor (parent, name, v) { finalize_construction (parent, name); }
    virtual ~FillColor () {}
    void draw () override;
    FillColor* clone () override;
  };

  class OutlineColor : public AbstractColor
  {
  public:
    OutlineColor (ParentProcess* parent, const std::string& name, int r, int g, int b) :
        AbstractColor (parent, name, r, g, b) { finalize_construction (parent, name); }
    OutlineColor (ParentProcess* parent, const std::string& name, int v) :
         AbstractColor (parent, name, v) { finalize_construction (parent, name); }
    virtual ~OutlineColor () {}
    void draw () override;
    OutlineColor* clone () override;
  };

  class RGBToLCHConverter : public FatProcess
  {
  private:
    class ConverterAction : public Action {
      public:
        ConverterAction (RGBToLCHConverter *p, const std::string& n);
        virtual ~ConverterAction () {}
        void impl_activate () override;
      private:
        RGBToLCHConverter *_p;
    };
  public:
    RGBToLCHConverter (ParentProcess* parent, const std::string& name);
    virtual ~RGBToLCHConverter ();
    void impl_activate () override;
    void impl_deactivate () override;
  private:
    void set_parent (ParentProcess* parent) override;
    IntProperty *_r, *_g, *_b;
    DoubleProperty *_l, *_c, *_h;
    ConverterAction *_action;
    Coupling *_cr, *_cg, *_cb;
  };

  class LCHToRGBConverter : public FatProcess
  {
  private:
    class ConverterAction : public Action {
      public:
        ConverterAction (LCHToRGBConverter *p, const std::string& n);
        virtual ~ConverterAction () {}
        void impl_activate () override;
      private:
        LCHToRGBConverter *_p;
    };
  public:
    LCHToRGBConverter (ParentProcess* parent, const std::string& name);
    virtual ~LCHToRGBConverter ();
    void impl_activate () override;
    void impl_deactivate () override;
  private:
    void set_parent (ParentProcess* parent) override;
    IntProperty *_r, *_g, *_b;
    DoubleProperty *_l, *_c, *_h;
    ConverterAction *_action;
    Coupling *_cl, *_cc, *_ch;
  };
}
