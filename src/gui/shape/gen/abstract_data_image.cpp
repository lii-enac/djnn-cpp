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

#include "gui/shape/gen/abstract_image.h"
using AbstractTextpProperty = void; // hack to make 'AbstractTextpProperty* data ()' compile

#include "abstract_data_image.h"

namespace djnn
{
  AbstractDataImage::AbstractDataImage (CoreProcess* parent, const string& name, string*& data, int format, double x, double y, double width, double height) :
    AbstractImage (parent, name, x, y, width, height),
    raw_props{.data=data, .format=format},
    _cdata (nullptr), _cformat (nullptr)
  {
    set_origin (x, y);
    
  }

  AbstractDataImage::~AbstractDataImage ()
  {
    remove_edge (_cdata);
		remove_edge (_cformat);
    delete _cdata;
		delete _cformat;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (children_size () > 2) {
      symtable_t::iterator it;

      it = find_child_iterator ("data");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("format");
			if (it != children_end ())
				delete it->second;
    }
  }
 
  CoreProcess*
  AbstractDataImage::find_child_impl (const string& name)
  {
    auto * res = AbstractImage::find_child_impl(name);
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
    
    if(name=="data") {
      coupling=&_cdata;
      rawp_Textp=&raw_props.data;
      notify_mask = notify_damaged_geometry;
      prop_Textp=true;
    } else
    if(name=="format") {
      coupling=&_cformat;
      rawp_Int=&raw_props.format;
      notify_mask = notify_damaged_style;
      prop_Int=true;
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

  const djnnstl::vector<djnnstl::string>&
  AbstractDataImage::get_properties_name () const
  {
    static const vector<string> res = {
    "data",
			"format",
    };
    return res;
  }

  void
  AbstractDataImage::get_properties_values (string*& data, int& format, double& x, double& y, double& width, double& height)
  {
    data = raw_props.data;
		format = raw_props.format;
    AbstractImage::get_properties_values(x, y, width, height);
  }

  void
  AbstractDataImage::impl_activate ()
  {
    AbstractImage::impl_activate ();
    auto _frame = get_frame ();
    enable(_cdata, _frame->damaged ());
		enable(_cformat, _frame->damaged ());
  }

  void
  AbstractDataImage::impl_deactivate ()
  {
    disable(_cdata);
		disable(_cformat);
    AbstractImage::impl_deactivate ();
  }

  

  

} /* namespace djnn */
