/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Nicolas Saporito <nicolas.saporito@enac.fr>
 *
 */

#include "../transformation/transformations.h"
#include "../../core/tree/spike.h"
#include "../../core/error.h"

#include "../backend.h"
#include "../abstract_backend.h"

#include <array>
#include <cmath>

namespace djnn
{

  void
  ScreenToLocal::stl_action::activate () {

    if (_stl->_shape->inverted_matrix () == nullptr)
      warning (this, "ScreenToLocal::stl_action::activate - inverted_matrix has not been initialized");
    else {
      Homography *hi = dynamic_cast<Homography*> (_stl->_shape->inverted_matrix ());

      double resultX = hi->_m11->get_value () * _stl->_inX->get_value () + 
                       hi->_m12->get_value () * _stl->_inY->get_value () + 
                       hi->_m14->get_value () - _stl->_shape->origin_x ()->get_value () ;
      double resultY = hi->_m21->get_value () * _stl->_inX->get_value () + 
                       hi->_m22->get_value () * _stl->_inY->get_value () +
                       hi->_m24->get_value () - _stl->_shape->origin_y ()->get_value ();

      _stl->_outX->set_value (resultX, true);
      _stl->_outY->set_value (resultY, true);
    }
      
  }

  ScreenToLocal::ScreenToLocal (Process *p, const string &n, Process* shape) :
  Process (p, n)
  {
    _shape = dynamic_cast<AbstractGShape*> (shape);
    if (_shape == nullptr)
      warning (this, "screenToLocal - shape has to be a graphical shape Rectangle|Circle ..." );

    //FIXME : hack to create matrix
    _shape->find_component("press");

    _inX = new DoubleProperty (this, "inX", 0);
    _inY = new DoubleProperty (this, "inY", 0);
    _outX = new DoubleProperty (this, "outX", 0);
    _outY = new DoubleProperty (this, "outY", 0);

    _action = new stl_action (this, "action_screen_to_local", this);
    _cinX = new Coupling (_inX, ACTIVATION, _action, ACTIVATION);
    _cinX->disable ();
    _cinY = new Coupling (_inY, ACTIVATION, _action, ACTIVATION);
    _cinY->disable ();
    Graph::instance().add_edge(_inX, _action);
    Graph::instance().add_edge(_inY, _action);
    Graph::instance().add_edge(_action, _outX);
    Graph::instance().add_edge(_action, _outY);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _action);

    Process::finalize ();
  }

  ScreenToLocal::~ScreenToLocal () 
  {

    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action);
    Graph::instance().remove_edge(_inX, _action);
    Graph::instance().remove_edge(_inY, _action);
    Graph::instance().remove_edge(_action, _outX);
    Graph::instance().remove_edge(_action, _outY);
    if (_cinY) { delete _cinY ; _cinY = nullptr;}
    if (_cinX) { delete _cinX ; _cinX = nullptr;}
    if (_action) { delete _action ; _action = nullptr;}
    if (_outX) { delete _outX ; _outX = nullptr;}
    if (_outY) { delete _outY ; _outY = nullptr;}
    if (_inX) { delete _inX ; _inX = nullptr;}
    if (_inY) { delete _inY ; _inY = nullptr;}

  }

  void
  ScreenToLocal::activate () 
  {
    _cinX->enable ();
    _cinY->enable ();
  }

  void
  ScreenToLocal::deactivate () 
  {
    _cinX->disable ();
    _cinY->disable ();
  }

  void
  LocalToScreen::lts_action::activate () {

    if (_lts->_shape->matrix () == nullptr)
      warning (this, "LocalToScreen::stl_action::activate - matrix has not been initialized");
    else {
      Homography *h = dynamic_cast<Homography*> (_lts->_shape->matrix ());

      double x = _lts->_inX->get_value () + _lts->_shape->origin_x ()->get_value ();
      double y = _lts->_inY->get_value () + _lts->_shape->origin_y ()->get_value ();
      double resultX = h->_m11->get_value () * x + 
                       h->_m12->get_value () * y + 
                       h->_m14->get_value () ;
      double resultY = h->_m21->get_value () * x + 
                       h->_m22->get_value () * y +
                       h->_m24->get_value () ;

      _lts->_outX->set_value (resultX, true);
      _lts->_outY->set_value (resultY, true);
    }
      
  }

  LocalToScreen::LocalToScreen (Process *p, const string &n, Process* shape) :
  Process (p, n)
  {
    _shape = dynamic_cast<AbstractGShape*> (shape);
    if (_shape == nullptr)
      warning (this, "LocalToScreen - shape has to be a graphical shape Rectangle|Circle ..." );

    //FIXME : hack to create matrix
    _shape->find_component("press");

    _inX = new DoubleProperty (this, "inX", 0);
    _inY = new DoubleProperty (this, "inY", 0);
    _outX = new DoubleProperty (this, "outX", 0);
    _outY = new DoubleProperty (this, "outY", 0);

    _action = new lts_action (this, "action_local_to_screen", this);
    _cinX = new Coupling (_inX, ACTIVATION, _action, ACTIVATION);
    _cinX->disable ();
    _cinY = new Coupling (_inY, ACTIVATION, _action, ACTIVATION);
    _cinY->disable ();
    Graph::instance().add_edge(_inX, _action);
    Graph::instance().add_edge(_inY, _action);
    Graph::instance().add_edge(_action, _outX);
    Graph::instance().add_edge(_action, _outY);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _action);

    Process::finalize ();
  }

  LocalToScreen::~LocalToScreen () 
  {

    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action);
    Graph::instance().remove_edge(_inX, _action);
    Graph::instance().remove_edge(_inY, _action);
    Graph::instance().remove_edge(_action, _outX);
    Graph::instance().remove_edge(_action, _outY);
    if (_cinY) { delete _cinY ; _cinY = nullptr;}
    if (_cinX) { delete _cinX ; _cinX = nullptr;}
    if (_action) { delete _action ; _action = nullptr;}
    if (_outX) { delete _outX ; _outX = nullptr;}
    if (_outY) { delete _outY ; _outY = nullptr;}
    if (_inX) { delete _inX ; _inX = nullptr;}
    if (_inY) { delete _inY ; _inY = nullptr;}

  }

  void
  LocalToScreen::activate () 
  {
    _cinX->enable ();
    _cinY->enable ();
  }

  void
  LocalToScreen::deactivate () 
  {
    _cinX->disable ();
    _cinY->disable ();
  }

}

