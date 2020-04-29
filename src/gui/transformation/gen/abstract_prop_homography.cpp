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


#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display.h"
#include "display/abstract_display.h"
#include "display/window.h"
#include "gui/shapes/shapes.h"
#include "gui/style/style.h"
#include "core/ontology/process.h"
#include "core/ontology/coupling.h"

#include "audio/style/audio_style.h"


#include "abstract_prop_homography.h"

namespace djnn
{
  AbstractPropHomography::AbstractPropHomography (Process *parent, const std::string& name, double m11, double m12, double m13, double m14, double m21, double m22, double m23, double m24, double m31, double m32, double m33, double m34, double m41, double m42, double m43, double m44) :
    AbstractTransformation (parent, name),
    raw_props{.m11=m11, .m12=m12, .m13=m13, .m14=m14, .m21=m21, .m22=m22, .m23=m23, .m24=m24, .m31=m31, .m32=m32, .m33=m33, .m34=m34, .m41=m41, .m42=m42, .m43=m43, .m44=m44},
    _cm11 (nullptr), _cm12 (nullptr), _cm13 (nullptr), _cm14 (nullptr), _cm21 (nullptr), _cm22 (nullptr), _cm23 (nullptr), _cm24 (nullptr), _cm31 (nullptr), _cm32 (nullptr), _cm33 (nullptr), _cm34 (nullptr), _cm41 (nullptr), _cm42 (nullptr), _cm43 (nullptr), _cm44 (nullptr)
  {
    
    
  }

  AbstractPropHomography::~AbstractPropHomography ()
  {
    remove_edge (_cm11);
		remove_edge (_cm12);
		remove_edge (_cm13);
		remove_edge (_cm14);
		remove_edge (_cm21);
		remove_edge (_cm22);
		remove_edge (_cm23);
		remove_edge (_cm24);
		remove_edge (_cm31);
		remove_edge (_cm32);
		remove_edge (_cm33);
		remove_edge (_cm34);
		remove_edge (_cm41);
		remove_edge (_cm42);
		remove_edge (_cm43);
		remove_edge (_cm44);
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
    if (children_size () > 0) {
      symtable_t::iterator it;

      it = find_child_iterator ("m11");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("m12");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("m13");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("m14");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("m21");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("m22");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("m23");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("m24");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("m31");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("m32");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("m33");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("m34");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("m41");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("m42");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("m43");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("m44");
			if (it != children_end ())
				delete it->second;
    }
  }
 
  Process*
  AbstractPropHomography::find_child (const std::string& name)
  {
    Process* res = AbstractTransformation::find_child(name);
    if(res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    CouplingWithData ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef std::string text;
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
    enable(_cm11, _frame->damaged ());
		enable(_cm12, _frame->damaged ());
		enable(_cm13, _frame->damaged ());
		enable(_cm14, _frame->damaged ());
		enable(_cm21, _frame->damaged ());
		enable(_cm22, _frame->damaged ());
		enable(_cm23, _frame->damaged ());
		enable(_cm24, _frame->damaged ());
		enable(_cm31, _frame->damaged ());
		enable(_cm32, _frame->damaged ());
		enable(_cm33, _frame->damaged ());
		enable(_cm34, _frame->damaged ());
		enable(_cm41, _frame->damaged ());
		enable(_cm42, _frame->damaged ());
		enable(_cm43, _frame->damaged ());
		enable(_cm44, _frame->damaged ());
  }

  void
  AbstractPropHomography::impl_deactivate ()
  {
    disable(_cm11);
		disable(_cm12);
		disable(_cm13);
		disable(_cm14);
		disable(_cm21);
		disable(_cm22);
		disable(_cm23);
		disable(_cm24);
		disable(_cm31);
		disable(_cm32);
		disable(_cm33);
		disable(_cm34);
		disable(_cm41);
		disable(_cm42);
		disable(_cm43);
		disable(_cm44);
    AbstractTransformation::impl_deactivate ();
  }

  

  

} /* namespace djnn */
