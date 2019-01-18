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

#include "../backend.h"
#include "../abstract_backend.h"
#include "../../core/control/coupling.h"
#include "shapes.h"
#include "../window.h"

namespace djnn
{
  void
  Text::TextSizeAction::activate ()
  {
    Backend::instance ()->update_text_geometry (_text, _ff, _fsz, _fs, _fw);
  }

  Text::Text (Process *p, const std::string& n, double x, double y, const std::string &text) :
      AbstractGShape (p, n),

      raw_props{.x=x, .y=y, .dx=0, .dy=0, .fsize=12, .dxU=0, .dyU=0, .width=0, .height=0,
        .encoding=djnUtf8, .fstyle=0, .fweight=400, .text=text, .ffamily="sans"},

      _cx (nullptr), _cy (nullptr), _cdx (nullptr), _cdy (nullptr), _cfsize (nullptr), _cdxU (nullptr), _cdyU (nullptr),
      _cwidth (nullptr), _cheight (nullptr), _cencoding (nullptr), _cfstyle (nullptr), _cfweight (nullptr), _ctext (nullptr), _cffamily (nullptr)
      
  {
    djnTextEncoding code = djnUtf8;
    set_origin (x, y);
    
    /* populate : text, width, height */
    this->text();
    this->width();
    this->height();

    _update_size = new TextSizeAction (this, "size_action", this);
    Graph::instance ().add_edge (this->text(), _update_size);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _update_size);
    Process::finalize ();
  }

  Text::Text (Process *p, const std::string& n, double x, double y, double dx, double dy, int dxU, int dyU,
              const std::string &encoding, const std::string &text) :
      AbstractGShape (p, n),

      raw_props{.x=x, .y=y, .dx=dx, .dy=dy, .fsize=12, .dxU=dxU, .dyU=dyU, .width=0, .height=0,
        .encoding=0, .fstyle=0, .fweight=400, .text=text, .ffamily="sans"},

      _cx (nullptr), _cy (nullptr), _cdx (nullptr), _cdy (nullptr), _cfsize (nullptr), _cdxU (nullptr), _cdyU (nullptr),
      _cwidth (nullptr), _cheight (nullptr), _cencoding (nullptr), _cfstyle (nullptr), _cfweight (nullptr), _ctext (nullptr), _cffamily (nullptr)
      
  {
    djnTextEncoding code = djnUtf8;
    set_origin (x, y);

    /* populate : text, width, height */
    this->text();
    this->width();
    this->height();

    _update_size = new TextSizeAction (this, "size_action", this);
    Graph::instance ().add_edge (this->text(), _update_size);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _update_size);
    Process::finalize ();
  }

  Text::Text (double x, double y, const std::string &text) :
      AbstractGShape (),

      raw_props{.x=x, .y=y, .dx=0, .dy=0, .fsize=12, .dxU=0, .dyU=0, .width=0, .height=0,
        .encoding=0, .fstyle=0, .fweight=400, .text=text, .ffamily="sans"},

      _cx (nullptr), _cy (nullptr), _cdx (nullptr), _cdy (nullptr), _cfsize (nullptr), _cdxU (nullptr), _cdyU (nullptr),
      _cwidth (nullptr), _cheight (nullptr), _cencoding (nullptr), _cfstyle (nullptr), _cfweight (nullptr), _ctext (nullptr), _cffamily (nullptr)
      
  {
    djnTextEncoding code = djnUtf8;
    set_origin (x, y);

    /* populate : text, width, height */
    this->text();
    this->width();
    this->height();

    _update_size = new TextSizeAction (this, "size_action", this);
    Graph::instance ().add_edge (this->text(), _update_size);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _update_size);
  }

  Text::~Text ()
  {
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _update_size);
    Graph::instance ().remove_edge (this->text(), _update_size);
    
    if (_cx) {delete _cx; _cx = nullptr;}
    if (_cy) {delete _cy; _cy = nullptr;}
    if (_ctext) {delete _ctext; _ctext = nullptr;}
    if (_cfsize) {delete _cfsize; _cfsize = nullptr;}

    if (_cffamily) {
      Graph::instance ().remove_edge ( this->ffamily (), _update_size);
      delete _cffamily;
      //_cffamily = nullptr;
    }
    if (_cfsize) {
      Graph::instance ().remove_edge ( this->fsize (), _update_size);
      delete _cfsize;
      //_cfsize = nullptr;
    }
    if (_cfstyle) {
      Graph::instance ().remove_edge ( this->fstyle (), _update_size);
      delete _cfstyle;
      //_cfstyle = nullptr;
    }
    if (_cfweight) {
      Graph::instance ().remove_edge ( this->fweight (), _update_size);
      delete _cfweight;
      //_cfweight = nullptr;
    }
  }

  Process*
  Text::find_component (const string& name)
  {
    Process* res = AbstractGShape::find_component(name);
    if(res) return res;

    bool propd = false;
    bool propi = false;
    bool propt = false;
    Coupling ** coupling;
    double* rawpd;
    int* rawpi;
    string* rawpt;
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
      notify_mask = notify_damaged_geometry; // or transform ?
    } else
    if(name=="dy") {
      propd = true;
      coupling=&_cdy;
      rawpd=&raw_props.dy;
      notify_mask = notify_damaged_geometry; // or transform ?
    } else
    if(name=="fsize") {
      propd = true;
      coupling=&_cfsize;
      rawpd=&raw_props.fsize;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="dxU") {
      propi = true;
      coupling=&_cdxU;
      rawpi=&raw_props.dxU;
      notify_mask = notify_damaged_geometry; // or transform ?
    } else
    if(name=="dyU") {
      propi = true;
      coupling=&_cdyU;
      rawpi=&raw_props.dyU;
      notify_mask = notify_damaged_geometry; // or transform ?
    } else
    if(name=="width") {
      propi = true;
      coupling=&_cwidth;
      rawpi=&raw_props.width;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="height") {
      propi = true;
      coupling=&_cheight;
      rawpi=&raw_props.height;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="encoding") {
      propi = true;
      coupling=&_cencoding;
      rawpi=&raw_props.encoding;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="fstyle") {
      propi = true;
      coupling=&_cfstyle;
      rawpi=&raw_props.fstyle;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="fweight") {
      propi = true;
      coupling=&_cfweight;
      rawpi=&raw_props.fweight;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="text") {
      propt = true;
      coupling=&_ctext;
      rawpt=&raw_props.text;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="ffamily") {
      propt = true;
      coupling=&_cffamily;
      rawpt=&raw_props.ffamily;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    if (propd) {
      DoublePropertyProxy* prop = nullptr;
      res = create_GObj_prop(&prop, coupling, rawpd, name, notify_mask);
    }
    else if (propt) {
      TextPropertyProxy* prop = nullptr;
      res = create_GObj_prop(&prop, coupling, rawpt, name, notify_mask);
    }
    else if (propi) {
      IntPropertyProxy* prop = nullptr;
      res = create_GObj_prop(&prop, coupling, rawpi, name, notify_mask);
    }
    
    return res;
  }

  void
  Text::get_properties_values (double &x, double &y, double &dx, double &dy, double &fsize, int &dxU, int &dyU, int &width, int &height, int &encoding, int &fstyle, int &fweight, string &text, string &ffamily)
  {
    x = raw_props.x;
    y = raw_props.y;
    dx = raw_props.dx;
    dy = raw_props.dy;
    fsize = raw_props.fsize;
    dxU = raw_props.dxU;
    dyU = raw_props.dyU;
    width = raw_props.width;
    height = raw_props.height;
    encoding = raw_props.encoding;
    fstyle = raw_props.fstyle;
    fweight = raw_props.fweight;
    text = raw_props.text;
    ffamily = raw_props.ffamily;
  }
    

  void
  Text::activate ()
  {
    AbstractGObj::activate ();
    Container *c = dynamic_cast<Container*> (_parent);
    if (c) {
      Process* ff = c->get_from_context ("FontFamily");
      if (ff && ((FontFamily*) ff)->family ()->get_value () != raw_props.ffamily) {
        //this->ffamily () = ((FontFamily*) ff)->family ();
        _update_size->_ff = (FontFamily*) ff;
        if (_cffamily != nullptr) {
          Graph::instance ().remove_edge ( this->ffamily (), _update_size);
          delete _cffamily;
          _cffamily = nullptr;
        }
        if (this->ffamily () != nullptr) {
          _cffamily = new Coupling (this->ffamily (), ACTIVATION, _update_size, ACTIVATION);
          Graph::instance ().add_edge (this->ffamily (), _update_size);
        }
      }
      Process* fsz = c->get_from_context ("FontSize");
      if (fsz && ((FontSize*) fsz)->size ()->get_value () != raw_props.fsize) {
        //_fsize = ((FontSize*) fsz)->size ();
        _update_size->_fsz = (FontSize*) fsz;
        if (_cfsize != nullptr) {
          Graph::instance ().remove_edge (this->fsize (), _update_size);
          delete _cfsize;
          _cfsize = nullptr;
        }
        if (this->fsize () != nullptr) {
          _cfsize = new Coupling (this->fsize (), ACTIVATION, _update_size, ACTIVATION);
          Graph::instance ().add_edge (this->fsize (), _update_size);
        }
      }
      Process* fs = c->get_from_context ("FontStyle");
      if (fs && ((FontStyle*) fs)->style ()->get_value () != raw_props.fstyle) {
        //_fstyle = ((FontStyle*) fs)->style ();
        _update_size->_fs = (FontStyle*) fs;
        if (_cfstyle != nullptr) {
          Graph::instance ().remove_edge (this->fstyle (), _update_size);
          delete _cfstyle;
          _cfstyle = nullptr;
        }
        if (this->fstyle () != nullptr) {
          _cfstyle = new Coupling (this->fstyle (), ACTIVATION, _update_size, ACTIVATION);
          Graph::instance ().add_edge (this->fstyle (), _update_size);
        }
      }
      Process* fw = c->get_from_context ("FontWeight");
      if (fw && ((FontWeight*) fw)->weight ()->get_value () != raw_props.fweight) {
        //_fweight = ((FontWeight*) fw)->weight ();
        _update_size->_fw = (FontWeight*) fw;
        if (_cfweight != nullptr) {
          Graph::instance ().remove_edge (this->fweight (), _update_size);
          delete _cfweight;
          _cfweight = nullptr;
        }
        if (this->fweight () != nullptr) {
          _cfweight = new Coupling (this->fweight (), ACTIVATION, _update_size, ACTIVATION);
          Graph::instance ().add_edge (this->fweight (), _update_size);
        }
      }
    }
    if(_cx) _cx->enable (_frame);
    if(_cy) _cy->enable (_frame);
    if(_ctext) _ctext->enable (_frame);
    if (_cffamily) _cffamily->enable ();
    if (_cfsize) _cfsize->enable ();
    if (_cfstyle) _cfstyle->enable ();
    if (_cfweight) _cfweight->enable ();
    if(_cfsize) _cfsize->enable ();
    if(_update_size) _update_size->activation ();
  }

  void
  Text::deactivate ()
  {
    AbstractGObj::deactivate ();
    if(_cx) _cx->disable ();
    if(_cy) _cy->disable ();
    if(_ctext) _ctext->disable ();
    if (_cffamily) _cffamily->disable ();
    if (_cfsize) _cfsize->disable ();
    if (_cfstyle) _cfstyle->disable ();
    if (_cfweight) _cfweight->disable ();
    if(_cfsize) _cfsize->disable ();
  }

  void
  Text::draw ()
  {
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
        Backend::instance ()->draw_text (this);
    }
  }

  Process*
  Text::clone ()
  {
    return new Text (raw_props.x, raw_props.y, raw_props.text);
  } 
} /* namespace djnn */
