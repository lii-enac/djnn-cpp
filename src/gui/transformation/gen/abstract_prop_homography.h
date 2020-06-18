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
 *  !! this file has been automatically generated - do NOT modify !!
 *
 */


#pragma once

namespace djnn
{
  class AbstractPropHomography : public AbstractTransformation
  {
  public:
    AbstractPropHomography (FatProcess *parent, const std::string& name, double m11=1, double m12=0, double m13=0, double m14=0, double m21=0, double m22=1, double m23=0, double m24=0, double m31=0, double m32=0, double m33=1, double m34=0, double m41=0, double m42=0, double m43=0, double m44=1);
    virtual ~AbstractPropHomography ();
    
    
    void get_properties_values (double& m11, double& m12, double& m13, double& m14, double& m21, double& m22, double& m23, double& m24, double& m31, double& m32, double& m33, double& m34, double& m41, double& m42, double& m43, double& m44);
    virtual FatChildProcess* find_child (const std::string&) override;
		AbstractDoubleProperty* m11 () { return (AbstractDoubleProperty*) find_child ("m11"); }
		AbstractDoubleProperty* m12 () { return (AbstractDoubleProperty*) find_child ("m12"); }
		AbstractDoubleProperty* m13 () { return (AbstractDoubleProperty*) find_child ("m13"); }
		AbstractDoubleProperty* m14 () { return (AbstractDoubleProperty*) find_child ("m14"); }
		AbstractDoubleProperty* m21 () { return (AbstractDoubleProperty*) find_child ("m21"); }
		AbstractDoubleProperty* m22 () { return (AbstractDoubleProperty*) find_child ("m22"); }
		AbstractDoubleProperty* m23 () { return (AbstractDoubleProperty*) find_child ("m23"); }
		AbstractDoubleProperty* m24 () { return (AbstractDoubleProperty*) find_child ("m24"); }
		AbstractDoubleProperty* m31 () { return (AbstractDoubleProperty*) find_child ("m31"); }
		AbstractDoubleProperty* m32 () { return (AbstractDoubleProperty*) find_child ("m32"); }
		AbstractDoubleProperty* m33 () { return (AbstractDoubleProperty*) find_child ("m33"); }
		AbstractDoubleProperty* m34 () { return (AbstractDoubleProperty*) find_child ("m34"); }
		AbstractDoubleProperty* m41 () { return (AbstractDoubleProperty*) find_child ("m41"); }
		AbstractDoubleProperty* m42 () { return (AbstractDoubleProperty*) find_child ("m42"); }
		AbstractDoubleProperty* m43 () { return (AbstractDoubleProperty*) find_child ("m43"); }
		AbstractDoubleProperty* m44 () { return (AbstractDoubleProperty*) find_child ("m44"); }

  protected:
    struct raw_props_t { double m11; double m12; double m13; double m14; double m21; double m22; double m23; double m24; double m31; double m32; double m33; double m34; double m41; double m42; double m43; double m44; };
    raw_props_t raw_props;
    CouplingWithData *_cm11, *_cm12, *_cm13, *_cm14, *_cm21, *_cm22, *_cm23, *_cm24, *_cm31, *_cm32, *_cm33, *_cm34, *_cm41, *_cm42, *_cm43, *_cm44;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
