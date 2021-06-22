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
#include "abstract_data_image.h"

namespace djnn
{
  AbstractDataImage::AbstractDataImage (ParentProcess* parent, const djnn::string& name, const djnn::string& data, double x, double y, double width, double height) :
    AbstractImage (parent, name, x, y, width, height),
    raw_props{.data=data},
    _cdata (nullptr)
  {
    set_origin (x, y);
    
  }

  AbstractDataImage::~AbstractDataImage ()
  {
    remove_edge (_cdata);
    delete _cdata;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (children_size () > 2) {
      symtable_t::iterator it;

      it = find_child_iterator ("data");
			if (it != children_end ())
				delete it->second;
    }
  }
 
  FatChildProcess*
  AbstractDataImage::find_child_impl (const djnn::string& name)
  {
    auto * res = AbstractImage::find_child_impl(name);
    if (res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    Coupling ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef djnn::string text;
    text* rawp_Text = nullptr;
    int notify_mask = notify_none;
    
    if(name=="data") {
      coupling=&_cdata;
      rawp_Text=&raw_props.data;
      notify_mask = notify_damaged_geometry;
      prop_Text=true;
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

  const vector<djnn::string>&
  AbstractDataImage::get_properties_name () const
  {
    static const vector<djnn::string> res = {
    "data",
    };
    return res;
  }

  void
  AbstractDataImage::get_properties_values (djnn::string& data, double& x, double& y, double& width, double& height)
  {
    data = raw_props.data;
    AbstractImage::get_properties_values(x, y, width, height);
  }

  void
  AbstractDataImage::impl_activate ()
  {
    AbstractImage::impl_activate ();
    auto _frame = get_frame ();
    enable(_cdata, _frame->damaged ());
  }

  void
  AbstractDataImage::impl_deactivate ()
  {
    disable(_cdata);
    AbstractImage::impl_deactivate ();
  }

  

  

} /* namespace djnn */
