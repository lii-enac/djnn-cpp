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


#include "gui/shape/abstract_gshape.h"
#include "display/window.h"
#include "gui/style/abstract_style.h"

#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display-dev.h" // DisplayBackend::instance
#include "display/abstract_display.h"

#include "audio/style/audio_style.h"
#include "physics/abstract_pobj.h"
#include "physics/physics.h"


#include "abstract_prop_box.h"

namespace djnn
{
  AbstractPropBox::AbstractPropBox (CoreProcess* parent, const string& name, double w, double h, double d) :
    PhyObj (parent, name),
    raw_props{.w=w, .h=h, .d=d},
    _cw (nullptr), _ch (nullptr), _cd (nullptr)
  {
    
    
  }

  AbstractPropBox::~AbstractPropBox ()
  {
    remove_edge (_cw);
		remove_edge (_ch);
		remove_edge (_cd);
    delete _cw;
		delete _ch;
		delete _cd;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (children_size () > 0) {
      symtable_t::iterator it;

      it = find_child_iterator ("w");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("h");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("d");
			if (it != children_end ())
				delete it->second;
    }
  }
 
  CoreProcess*
  AbstractPropBox::find_child_impl (const string& name)
  {
    auto * res = PhyObj::find_child_impl(name);
    if (res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false, prop_Textp=false;
    Coupling ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef string text;
    text* rawp_Text = nullptr;
    typedef string* textp;
    textp* rawp_Textp = nullptr;
    int notify_mask = notify_none;
    
    if(name=="w") {
      coupling=&_cw;
      rawp_Double=&raw_props.w;
      notify_mask = notify_damaged_geometry;
      prop_Double=true;
    } else
    if(name=="h") {
      coupling=&_ch;
      rawp_Double=&raw_props.h;
      notify_mask = notify_damaged_geometry;
      prop_Double=true;
    } else
    if(name=="d") {
      coupling=&_cd;
      rawp_Double=&raw_props.d;
      notify_mask = notify_damaged_geometry;
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
    else if(prop_Textp) {
      TextPropertyProxy* prop = nullptr; // do not cache
      res = create_GObj_prop(&prop, coupling, rawp_Textp, name, notify_mask);
    }

    return res;
  }

  const vector<string>&
  AbstractPropBox::get_properties_name () const
  {
    static const vector<string> res = {
    "w",
			"h",
			"d",
    };
    return res;
  }

  void
  AbstractPropBox::get_properties_values (double& w, double& h, double& d)
  {
    w = raw_props.w;
		h = raw_props.h;
		d = raw_props.d;
    
  }

  void
  AbstractPropBox::impl_activate ()
  {
    PhyObj::impl_activate ();
    auto _frame = get_frame ();
    enable(_cw, _frame->damaged ());
		enable(_ch, _frame->damaged ());
		enable(_cd, _frame->damaged ());
  }

  void
  AbstractPropBox::impl_deactivate ()
  {
    disable(_cw);
		disable(_ch);
		disable(_cd);
    PhyObj::impl_deactivate ();
  }

  

  

} /* namespace djnn */
