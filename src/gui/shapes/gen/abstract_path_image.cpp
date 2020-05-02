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


#include "abstract_path_image.h"

namespace djnn
{
  AbstractPathImage::AbstractPathImage (FatProcess *parent, const std::string& name, const std::string& path, double x, double y, double width, double height) :
    AbstractImage (parent, name, x, y, width, height),
    raw_props{.path=path},
    _cpath (nullptr)
  {
    set_origin (x, y);
    
  }

  AbstractPathImage::~AbstractPathImage ()
  {
    remove_edge (_cpath);
    delete _cpath;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (children_size () > 2) {
      symtable_t::iterator it;

      it = find_child_iterator ("path");
			if (it != children_end ())
				delete it->second;
    }
  }
 
  FatProcess*
  AbstractPathImage::find_child (const std::string& name)
  {
    FatProcess* res = AbstractImage::find_child(name);
    if(res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    CouplingWithData ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef std::string text;
    text* rawp_Text = nullptr;
    int notify_mask = notify_none;
    
    if(name=="path") {
      coupling=&_cpath;
      rawp_Text=&raw_props.path;
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

  void
  AbstractPathImage::get_properties_values (std::string& path, double& x, double& y, double& width, double& height)
  {
    path = raw_props.path;
    AbstractImage::get_properties_values(x, y, width, height);
  }

  void
  AbstractPathImage::impl_activate ()
  {
    AbstractImage::impl_activate ();
    auto _frame = frame ();
    enable(_cpath, _frame->damaged ());
  }

  void
  AbstractPathImage::impl_deactivate ()
  {
    disable(_cpath);
    AbstractImage::impl_deactivate ();
  }

  

  

} /* namespace djnn */
