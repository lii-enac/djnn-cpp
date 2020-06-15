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


#include "abstract_prop_gradient_stop.h"

namespace djnn
{
  AbstractPropGradientStop::AbstractPropGradientStop (FatProcess *parent, const std::string& name, double r, double g, double b, double a, double offset) :
    AbstractStyle (parent, name),
    raw_props{.r=r, .g=g, .b=b, .a=a, .offset=offset},
    _cr (nullptr), _cg (nullptr), _cb (nullptr), _ca (nullptr), _coffset (nullptr)
  {
    
    
  }

  AbstractPropGradientStop::~AbstractPropGradientStop ()
  {
    remove_edge (_cr);
		remove_edge (_cg);
		remove_edge (_cb);
		remove_edge (_ca);
		remove_edge (_coffset);
    delete _cr;
		delete _cg;
		delete _cb;
		delete _ca;
		delete _coffset;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (children_size () > 0) {
      symtable_t::iterator it;

      it = find_child_iterator ("r");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("g");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("b");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("a");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("offset");
			if (it != children_end ())
				delete it->second;
    }
  }
 
  ChildProcess*
  AbstractPropGradientStop::find_child (const std::string& name)
  {
    auto * res = AbstractStyle::find_child(name);
    if(res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    CouplingWithData ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef std::string text;
    text* rawp_Text = nullptr;
    int notify_mask = notify_none;
    
    if(name=="r") {
      coupling=&_cr;
      rawp_Double=&raw_props.r;
      notify_mask = notify_damaged_style;
      prop_Double=true;
    } else
    if(name=="g") {
      coupling=&_cg;
      rawp_Double=&raw_props.g;
      notify_mask = notify_damaged_style;
      prop_Double=true;
    } else
    if(name=="b") {
      coupling=&_cb;
      rawp_Double=&raw_props.b;
      notify_mask = notify_damaged_style;
      prop_Double=true;
    } else
    if(name=="a") {
      coupling=&_ca;
      rawp_Double=&raw_props.a;
      notify_mask = notify_damaged_style;
      prop_Double=true;
    } else
    if(name=="offset") {
      coupling=&_coffset;
      rawp_Double=&raw_props.offset;
      notify_mask = notify_damaged_style;
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
  AbstractPropGradientStop::get_properties_values (double& r, double& g, double& b, double& a, double& offset)
  {
    r = raw_props.r;
		g = raw_props.g;
		b = raw_props.b;
		a = raw_props.a;
		offset = raw_props.offset;
    
  }

  void
  AbstractPropGradientStop::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = get_frame ();
    enable(_cr, _frame->damaged ());
		enable(_cg, _frame->damaged ());
		enable(_cb, _frame->damaged ());
		enable(_ca, _frame->damaged ());
		enable(_coffset, _frame->damaged ());
  }

  void
  AbstractPropGradientStop::impl_deactivate ()
  {
    disable(_cr);
		disable(_cg);
		disable(_cb);
		disable(_ca);
		disable(_coffset);
    AbstractStyle::impl_deactivate ();
  }

  

  

} /* namespace djnn */
