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
#include "../../core/utils/error.h"

#include "../backend.h"
#include "../abstract_backend.h"

#include <array>
#include <cmath>

namespace djnn
{

  void
  ScreenToLocal::stl_action::impl_activate () {

    if (_stl->_shape->inverted_matrix () == nullptr)
      warning (this, "ScreenToLocal::stl_action::activate - inverted_matrix has not been initialized");
    else {
      Homography *hi = dynamic_cast<Homography*> (_stl->_shape->inverted_matrix ());

      double resultX = hi->raw_props.m11 * _stl->_inX->get_value () + 
                       hi->raw_props.m12 * _stl->_inY->get_value () + 
                       hi->raw_props.m14 - _stl->_shape->origin_x ()->get_value () ;
      double resultY = hi->raw_props.m21 * _stl->_inX->get_value () + 
                       hi->raw_props.m22 * _stl->_inY->get_value () +
                       hi->raw_props.m24 - _stl->_shape->origin_y ()->get_value ();

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

    //hack to create inverted_matrix
    _shape->find_component("inverted_matrix");

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

    Process::finalize_construction ();
  }

  ScreenToLocal::~ScreenToLocal () 
  {

    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action);
    Graph::instance().remove_edge(_inX, _action);
    Graph::instance().remove_edge(_inY, _action);
    Graph::instance().remove_edge(_action, _outX);
    Graph::instance().remove_edge(_action, _outY);
    delete _cinY;
    delete _cinX;
    delete _action;
    delete _outX;
    delete _outY;
    delete _inX;
    delete _inY;

  }

  void
  ScreenToLocal::impl_activate () 
  {
    _cinX->enable ();
    _cinY->enable ();
  }

  void
  ScreenToLocal::impl_deactivate () 
  {
    _cinX->disable ();
    _cinY->disable ();
  }

  void
  LocalToScreen::lts_action::impl_activate () {

    if (_lts->_shape->matrix () == nullptr)
      warning (this, "LocalToScreen::stl_action::activate - matrix has not been initialized");
    else {
      Homography *h = dynamic_cast<Homography*> (_lts->_shape->matrix ());

      double x = _lts->_inX->get_value () + _lts->_shape->origin_x ()->get_value ();
      double y = _lts->_inY->get_value () + _lts->_shape->origin_y ()->get_value ();
      double resultX = h->raw_props.m11 * x + 
                       h->raw_props.m12 * y + 
                       h->raw_props.m14 ;
      double resultY = h->raw_props.m21 * x + 
                       h->raw_props.m22 * y +
                       h->raw_props.m24;

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

    //hack to create matrix
    _shape->find_component("matrix");

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

    Process::finalize_construction ();
  }

  LocalToScreen::~LocalToScreen () 
  {

    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action);
    Graph::instance().remove_edge(_inX, _action);
    Graph::instance().remove_edge(_inY, _action);
    Graph::instance().remove_edge(_action, _outX);
    Graph::instance().remove_edge(_action, _outY);
    delete _cinY;
    delete _cinX;
    delete _action;
    delete _outX;
    delete _outY;
    delete _inX;
    delete _inY;

  }

  void
  LocalToScreen::impl_activate () 
  {
    _cinX->enable ();
    _cinY->enable ();
  }

  void
  LocalToScreen::impl_deactivate () 
  {
    _cinX->disable ();
    _cinY->disable ();
  }

}

