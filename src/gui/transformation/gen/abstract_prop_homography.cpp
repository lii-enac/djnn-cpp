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


#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display.h"
#include "display/abstract_display.h"
#include "display/window.h"
#include "gui/shapes/shapes.h"
#include "gui/style/style.h"
#include "core/ontology/coupling.h"

#include "abstract_prop_homography.h"

namespace djnn
{
  AbstractPropHomography::AbstractPropHomography (Process *p, const std::string& n, double m11, double m12, double m13, double m14, double m21, double m22, double m23, double m24, double m31, double m32, double m33, double m34, double m41, double m42, double m43, double m44) :
    AbstractTransformation (p, n),
    raw_props{.m11=m11, .m12=m12, .m13=m13, .m14=m14, .m21=m21, .m22=m22, .m23=m23, .m24=m24, .m31=m31, .m32=m32, .m33=m33, .m34=m34, .m41=m41, .m42=m42, .m43=m43, .m44=m44},
    _cm11 (nullptr), _cm12 (nullptr), _cm13 (nullptr), _cm14 (nullptr), _cm21 (nullptr), _cm22 (nullptr), _cm23 (nullptr), _cm24 (nullptr), _cm31 (nullptr), _cm32 (nullptr), _cm33 (nullptr), _cm34 (nullptr), _cm41 (nullptr), _cm42 (nullptr), _cm43 (nullptr), _cm44 (nullptr)
  {
    
    
  }

  AbstractPropHomography::AbstractPropHomography (double m11, double m12, double m13, double m14, double m21, double m22, double m23, double m24, double m31, double m32, double m33, double m34, double m41, double m42, double m43, double m44) :
    AbstractTransformation (), 
    raw_props{.m11=m11, .m12=m12, .m13=m13, .m14=m14, .m21=m21, .m22=m22, .m23=m23, .m24=m24, .m31=m31, .m32=m32, .m33=m33, .m34=m34, .m41=m41, .m42=m42, .m43=m43, .m44=m44},
    _cm11 (nullptr), _cm12 (nullptr), _cm13 (nullptr), _cm14 (nullptr), _cm21 (nullptr), _cm22 (nullptr), _cm23 (nullptr), _cm24 (nullptr), _cm31 (nullptr), _cm32 (nullptr), _cm33 (nullptr), _cm34 (nullptr), _cm41 (nullptr), _cm42 (nullptr), _cm43 (nullptr), _cm44 (nullptr)
  {
    
  }

  AbstractPropHomography::~AbstractPropHomography ()
  {
    delete _cm11;
		delete _cm12;
		delete _cm13;
		delete _cm14;
		delete _cm21;
		delete _cm22;
		delete _cm23;
		delete _cm24;
		delete _cm31;
		delete _cm32;
		delete _cm33;
		delete _cm34;
		delete _cm41;
		delete _cm42;
		delete _cm43;
		delete _cm44;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (symtable ().size () > 2) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("m11");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("m12");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("m13");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("m14");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("m21");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("m22");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("m23");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("m24");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("m31");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("m32");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("m33");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("m34");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("m41");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("m42");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("m43");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("m44");
			if (it != symtable ().end ())
				delete it->second;
    }
  }
 
  Process*
  AbstractPropHomography::find_component (const string& name)
  {
    Process* res = AbstractTransformation::find_component(name);
    if(res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    Coupling ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef string text;
    text* rawp_Text = nullptr;
    int notify_mask = notify_none;
    
    if(name=="m11") {
      coupling=&_cm11;
      rawp_Double=&raw_props.m11;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="m12") {
      coupling=&_cm12;
      rawp_Double=&raw_props.m12;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="m13") {
      coupling=&_cm13;
      rawp_Double=&raw_props.m13;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="m14") {
      coupling=&_cm14;
      rawp_Double=&raw_props.m14;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="m21") {
      coupling=&_cm21;
      rawp_Double=&raw_props.m21;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="m22") {
      coupling=&_cm22;
      rawp_Double=&raw_props.m22;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="m23") {
      coupling=&_cm23;
      rawp_Double=&raw_props.m23;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="m24") {
      coupling=&_cm24;
      rawp_Double=&raw_props.m24;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="m31") {
      coupling=&_cm31;
      rawp_Double=&raw_props.m31;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="m32") {
      coupling=&_cm32;
      rawp_Double=&raw_props.m32;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="m33") {
      coupling=&_cm33;
      rawp_Double=&raw_props.m33;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="m34") {
      coupling=&_cm34;
      rawp_Double=&raw_props.m34;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="m41") {
      coupling=&_cm41;
      rawp_Double=&raw_props.m41;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="m42") {
      coupling=&_cm42;
      rawp_Double=&raw_props.m42;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="m43") {
      coupling=&_cm43;
      rawp_Double=&raw_props.m43;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="m44") {
      coupling=&_cm44;
      rawp_Double=&raw_props.m44;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    return nullptr;
    
    if(prop_Double) {
      DoublePropertyProxy* prop = nullptr; // do not cache
      res = create_GObj_prop(&prop, coupling, rawp_Double, name, notify_mask);
    }
    else if(prop_Int) {
      IntPropertyProxy* prop = nullptr; // do not cache
      res = create_GObj_prop(&prop, coupling, rawp_Int, name, notify_mask);
    }
    else if(prop_Text) {
      TextPropertyProxy* prop = nullptr; // do not cache
      res = create_GObj_prop(&prop, coupling, rawp_Text, name, notify_mask);
    }

    return res;
  }

  void
  AbstractPropHomography::get_properties_values (double& m11, double& m12, double& m13, double& m14, double& m21, double& m22, double& m23, double& m24, double& m31, double& m32, double& m33, double& m34, double& m41, double& m42, double& m43, double& m44)
  {
    m11 = raw_props.m11;
		m12 = raw_props.m12;
		m13 = raw_props.m13;
		m14 = raw_props.m14;
		m21 = raw_props.m21;
		m22 = raw_props.m22;
		m23 = raw_props.m23;
		m24 = raw_props.m24;
		m31 = raw_props.m31;
		m32 = raw_props.m32;
		m33 = raw_props.m33;
		m34 = raw_props.m34;
		m41 = raw_props.m41;
		m42 = raw_props.m42;
		m43 = raw_props.m43;
		m44 = raw_props.m44;
  }

  void
  AbstractPropHomography::impl_activate ()
  {
    AbstractTransformation::impl_activate ();
    auto _frame = frame ();
    if(_cm11) _cm11->enable (_frame);
		if(_cm12) _cm12->enable (_frame);
		if(_cm13) _cm13->enable (_frame);
		if(_cm14) _cm14->enable (_frame);
		if(_cm21) _cm21->enable (_frame);
		if(_cm22) _cm22->enable (_frame);
		if(_cm23) _cm23->enable (_frame);
		if(_cm24) _cm24->enable (_frame);
		if(_cm31) _cm31->enable (_frame);
		if(_cm32) _cm32->enable (_frame);
		if(_cm33) _cm33->enable (_frame);
		if(_cm34) _cm34->enable (_frame);
		if(_cm41) _cm41->enable (_frame);
		if(_cm42) _cm42->enable (_frame);
		if(_cm43) _cm43->enable (_frame);
		if(_cm44) _cm44->enable (_frame);
  }

  void
  AbstractPropHomography::impl_deactivate ()
  {
    if(_cm11) _cm11->disable ();
		if(_cm12) _cm12->disable ();
		if(_cm13) _cm13->disable ();
		if(_cm14) _cm14->disable ();
		if(_cm21) _cm21->disable ();
		if(_cm22) _cm22->disable ();
		if(_cm23) _cm23->disable ();
		if(_cm24) _cm24->disable ();
		if(_cm31) _cm31->disable ();
		if(_cm32) _cm32->disable ();
		if(_cm33) _cm33->disable ();
		if(_cm34) _cm34->disable ();
		if(_cm41) _cm41->disable ();
		if(_cm42) _cm42->disable ();
		if(_cm43) _cm43->disable ();
		if(_cm44) _cm44->disable ();
    AbstractTransformation::impl_deactivate ();
  }

  

  
  
} /* namespace djnn */
