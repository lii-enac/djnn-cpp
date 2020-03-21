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

#include "shapes.h"

#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display.h"
#include "display/abstract_display.h"
#include "display/window.h"
#include "core/ontology/coupling.h"
#include "display/update_drawing.h"

#include "core/execution/graph.h"

#include <iostream>
#include "utils/debug.h"


namespace djnn
{
  void
  Text::TextSizeAction::impl_activate ()
  {
    // DEBUG
    // cout << endl << "TEXT SIZE ACTION ---- text: " << _text->text ()->get_value ()
    //                                 << " - family: " << ( _ff ? _ff->family ()->get_value () : "NO_family")
    //                                 << " - size: " <<  (_fsz ? _fsz->size ()->get_value () : 12 ) 
    //                                 << " - style: " << ( _fs ? _fs->style ()->get_value () : 0 )
    //                                 << " - weight:" << ( _fw ? _fw->weight ()->get_value () : 400 ) << endl;
                                   
    Backend::instance ()->update_text_geometry (_text, _ff, _fsz, _fs, _fw);
  }

  /*Text::Text (Process *parent, const std::string& name, double x, double y, const std::string& text) :
      AbstractGShape (parent, name),
      raw_props{.x=x, .y=y, .dx=0, .dy=0, .dxU=0, .dyU=0, .encoding=DJN_UTF8, .text=text},
      _cx (nullptr), _cy (nullptr), _cdx (nullptr), _cdy (nullptr), _cfsize (nullptr), _cdxU (nullptr), _cdyU (nullptr),
      _cencoding (nullptr), _cfstyle (nullptr), _cfweight (nullptr), _cffamily (nullptr),
      _fm (nullptr),
      _update_size (this, "size_action", this),
      _width (this, "width", 0),
      _height (this, "height", 0),
      _text (this, "text", raw_props.text, notify_damaged_geometry),
      _cupdate_size (&_text, ACTIVATION, &_update_size, ACTIVATION),
      _ctext (&_text, ACTIVATION, UpdateDrawing::instance ()->get_damaged (), ACTIVATION )
  {
    set_origin (x, y);

    Graph::instance ().add_edge (&_text, &_update_size);
    Graph::instance ().add_edge (&_text, UpdateDrawing::instance ()->get_damaged ());

    Process::finalize_construction (parent, name);
  }*/

  Text::Text (Process *parent, const std::string& name, double x, double y, const std::string& text) :
  Text(parent, name, x,y,0,0,0,0,"utf8", text) {}

  Text::Text (Process *parent, const std::string& name, double x, double y, double dx, double dy, int dxU, int dyU,
              const std::string& encoding, const std::string& text) :
      AbstractGShape (parent, name),
      /* FIXME: encoding - should be string or convert in Int  from Enum? */
      raw_props{.x=x, .y=y, .dx=dx, .dy=dy, .dxU=dxU, .dyU=dyU, .encoding=DJN_UTF8, .text=text},
      _cx (nullptr), _cy (nullptr), _cdx (nullptr), _cdy (nullptr), _cfsize (nullptr), _cdxU (nullptr), _cdyU (nullptr),
      _cencoding (nullptr), _cfstyle (nullptr), _cfweight (nullptr), _cffamily (nullptr),
      _fm (nullptr),
      _update_size (this, "size_action", this),
      _width (this, "width", 0),
      _height (this, "height", 0),
      _text (this, "text", raw_props.text, notify_damaged_geometry),
      _cupdate_size (&_text, ACTIVATION, &_update_size, ACTIVATION),
      _ctext (&_text, ACTIVATION, UpdateDrawing::instance ()->get_damaged (), ACTIVATION )
  {
    set_origin (x, y);

    Graph::instance ().add_edge (&_text, &_update_size);
    Graph::instance ().add_edge (&_text, UpdateDrawing::instance ()->get_damaged ());

    Process::finalize_construction (parent, name);
  }

  Text::~Text ()
  {
    //remove_state_dependency (get_parent (), UpdateDrawing::instance ()->get_damaged ());
    Graph::instance ().remove_edge (&_text, UpdateDrawing::instance ()->get_damaged ());
    remove_state_dependency (get_parent (), &_update_size);
    Graph::instance ().remove_edge (&_text, &_update_size);
    
    if (_cffamily) {
      Graph::instance ().remove_edge ( _update_size._ff->family (), &_update_size);
      delete _cffamily;
    }
    if (_cfsize) {
      Graph::instance ().remove_edge ( _update_size._fsz->size (), &_update_size);
      delete _cfsize;
    }
    if (_cfstyle) {
      Graph::instance ().remove_edge ( _update_size._fs->style (), &_update_size);
      delete _cfstyle;
    }
    if (_cfweight) {
      Graph::instance ().remove_edge ( _update_size._fw->weight (), &_update_size);
      delete _cfweight;
    }

    delete _cx;
    delete _cy;
    delete _cdx;
    delete _cdy;
    delete _cdxU;
    delete _cdyU;
    delete _cencoding;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("x");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("y");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("dx");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("dy");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("dxU");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("dyU");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("encoding");
      if (it != symtable ().end ())
        delete it->second;
    }

    Backend::instance ()->delete_text_impl (this);
  }

  void
  Text::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
       remove_state_dependency (get_parent (), &_update_size);
    }

    add_state_dependency (p, &_update_size);
    
    Process::set_parent (p); 
  }

  Process*
  Text::find_child (const std::string& name)
  {
    Process* res = AbstractGShape::find_child(name);
    if(res) return res;

    bool propd = false;
    bool propi = false;
    Coupling ** coupling;
    double* rawpd;
    int* rawpi;
    int notify_mask = notify_none;

    if(name=="x") {
      propd = true;
      coupling=&_cx;
      rawpd=&raw_props.x;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="y") {
      propd = true;
      coupling=&_cy;
      rawpd=&raw_props.y;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="dx") {
      propd = true;
      coupling=&_cdx;
      rawpd=&raw_props.dx;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="dy") {
      propd = true;
      coupling=&_cdy;
      rawpd=&raw_props.dy;
      notify_mask = notify_damaged_transform;
    } else 
    if(name=="dxU") {
      propi = true;
      coupling=&_cdxU;
      rawpi=&raw_props.dxU;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="dyU") {
      propi = true;
      coupling=&_cdyU;
      rawpi=&raw_props.dyU;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="encoding") {
      propi = true;
      coupling=&_cencoding;
      rawpi=&raw_props.encoding;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    if (propd) {
      DoublePropertyProxy* prop = nullptr;
      res = create_GObj_prop(&prop, coupling, rawpd, name, notify_mask);
    }
    else if (propi) {
      IntPropertyProxy* prop = nullptr;
      res = create_GObj_prop(&prop, coupling, rawpi, name, notify_mask);
    }
    
    return res;
  }

  void
  Text::get_properties_values (double &x, double &y, double &dx, double &dy, int &dxU, int &dyU, int &width, int &height, int &encoding, std::string &text)
  {
    x = raw_props.x;
    y = raw_props.y;
    dx = raw_props.dx;
    dy = raw_props.dy;
    dxU = raw_props.dxU;
    dyU = raw_props.dyU;
    width = _width.get_value ();
    height = _height.get_value ();
    encoding = raw_props.encoding;
    text = _text.get_value ();
  }
    

  void
  Text::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    Container *c = dynamic_cast<Container*> (get_parent ());
    if (c) {
      
      /* fontfamily changed ? */
      FontFamily* ff =  (FontFamily*) c->get_from_context ("FontFamily");
      if (ff) {

        /* remove old coupling */
        if (_cffamily != nullptr) {
          Graph::instance ().remove_edge (_update_size._ff->family (), &_update_size);
          delete _cffamily;
          _cffamily = nullptr;
        }

        /* update value */
        _update_size._ff = ff;
        _cffamily = new Coupling (ff->family (), ACTIVATION, &_update_size, ACTIVATION);
        Graph::instance ().add_edge (ff->family (), &_update_size);
      }

      /* fontsize changed ? */
      FontSize* fsz = (FontSize*) c->get_from_context ("FontSize");
      if (fsz) {
        
        /* remove old coupling */
        if (_cfsize != nullptr) {
          Graph::instance ().remove_edge (_update_size._fsz->size (), &_update_size);
          delete _cfsize;
          _cfsize = nullptr;
        }

        /* update value */
        _update_size._fsz = fsz;
        _cfsize = new Coupling (fsz->size (), ACTIVATION, &_update_size, ACTIVATION);
        Graph::instance ().add_edge (fsz->size (), &_update_size);
      }

      /* fontstyle changed ? */
      FontStyle* fs = (FontStyle*) c->get_from_context ("FontStyle");
      if (fs) {
        
        /* remove old coupling */
        if (_cfstyle != nullptr) {
          Graph::instance ().remove_edge (_update_size._fs->style (), &_update_size);
          delete _cfstyle;
          _cfstyle = nullptr;
        }

        /* update value */
        _update_size._fs = fs;
        _cfstyle = new Coupling (fs->style (), ACTIVATION, &_update_size, ACTIVATION);
        Graph::instance ().add_edge (fs->style (), &_update_size);
      }

      /* fontweight changed ? */
      FontWeight* fw = (FontWeight*) c->get_from_context ("FontWeight");
      if (fw) {
        
        /* remove old coupling */
        if (_cfweight != nullptr) {
          Graph::instance ().remove_edge (_update_size._fw->weight (), &_update_size);
          delete _cfweight;
          _cfweight = nullptr;
        }

        /* update value */
        _update_size._fw = fw;
        _cfweight = new Coupling (fw->weight (), ACTIVATION, &_update_size, ACTIVATION);
        Graph::instance ().add_edge (fw->weight (), &_update_size);
      }
    }

    auto _frame = frame ();
    
    if (_cx) _cx->enable (_frame);
    if (_cy) _cy->enable (_frame);
    //_ctext.enable (_frame);
    if (_cdx) _cdx->enable (_frame);
    if (_cdy) _cdy->enable (_frame);
    if (_cdxU) _cdxU->enable (_frame);
    if (_cdyU) _cdyU->enable (_frame);
    _cupdate_size.enable (_frame);
    if (_cencoding) _cencoding->enable (_frame);
    
    if (_cffamily) _cffamily->enable (_frame);
    if (_cfsize) _cfsize->enable (_frame);
    if (_cfstyle) _cfstyle->enable (_frame);
    if (_cfweight) _cfweight->enable (_frame);

    _ctext.enable(_frame);

    _update_size.activate ();
  }

  void
  Text::impl_deactivate ()
  {
    _ctext.disable();

    if (_cx) _cx->disable ();
    if (_cy) _cy->disable ();
    //_ctext.disable ();
    if (_cdx) _cdx->disable ();
    if (_cdy) _cdy->disable ();
    if (_cdxU) _cdxU->disable ();
    if (_cdyU) _cdyU->disable ();
    _cupdate_size.disable ();
    if (_cencoding) _cencoding->disable ();

    if (_cffamily) _cffamily->disable ();
    if (_cfsize) _cfsize->disable ();
    if (_cfstyle) _cfstyle->disable ();
    if (_cfweight) _cfweight->disable ();
    AbstractGShape::impl_deactivate ();
  }

  void
  Text::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      AbstractGShape::pre_draw ();
      Backend::instance ()->draw_text (this);
      AbstractGShape::post_draw ();
    }
  }

  void
  Text::get_bounding_box (double& x, double& y, double& w, double& h) const
  {
    UNIMPL;
  }

  double
  Text::sdf (double x, double y) const
  {
    UNIMPL;
    return 0;
  }

  double
  Text::get_cursor_from_index (int index)
  {
    return Backend::instance ()->get_cursor_from_index (this, index);
  }

  std::pair<double, int>
  Text::get_cursor_from_local_x (double pos)
  {
    return Backend::instance ()->get_cursor_from_local_x (this, pos);
  }

  Process*
  Text::clone ()
  {
    return new Text (nullptr, this->get_name (), raw_props.x, raw_props.y, _text.get_value ());
  } 
} /* namespace djnn */
