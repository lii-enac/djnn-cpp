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

#include "gui/style/abstract_style.h"

#include "texture.h"

namespace djnn
{
  Texture::Texture (CoreProcess* parent, const string& name, const string& path) :
    AbstractStyle (parent, name),
    raw_props{.path=path},
    _cpath (nullptr)
  {
    
    finalize_construction (parent, name);
  }

  Texture::~Texture ()
  {
    remove_edge (_cpath);
    delete _cpath;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (children_size () > 0) {
      symtable_t::iterator it;

      it = find_child_iterator ("path");
			if (it != children_end ())
				delete it->second;
    }
  }
 
  CoreProcess*
  Texture::find_child_impl (const string& name)
  {
    auto * res = AbstractStyle::find_child_impl(name);
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
    
    if(name=="path") {
      coupling=&_cpath;
      rawp_Text=&raw_props.path;
      notify_mask = notify_damaged_style;
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
    else if(prop_Textp) {
      TextPropertyProxy* prop = nullptr; // do not cache
      res = create_GObj_prop(&prop, coupling, rawp_Textp, name, notify_mask);
    }

    return res;
  }

  const vector<string>&
  Texture::get_properties_name () const
  {
    static const vector<string> res = {
    "path",
    };
    return res;
  }

  void
  Texture::get_properties_values (string& path)
  {
    path = raw_props.path;
    
  }

  void
  Texture::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = get_frame ();
    enable(_cpath, _frame->damaged ());
  }

  void
  Texture::impl_deactivate ()
  {
    disable(_cpath);
    AbstractStyle::impl_deactivate ();
  }

  
  void
  Texture::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_texture (this);
    }
  }


  
  Texture*
  Texture::impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones)
  {
    auto res = new Texture (nullptr, get_name (), raw_props.path);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
  }


} /* namespace djnn */
