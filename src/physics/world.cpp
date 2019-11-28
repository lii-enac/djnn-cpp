/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "abstract_physics_backend.h"
#include "core/execution/graph.h"
#include "core/tree/component.h"
#include "core/tree/spike.h"
#include "core/ontology/coupling.h"
#include "core/tree/double_property.h"
#include "core/utils/error.h"

#include <iostream>

namespace djnn
{
  void
  World::StepAction::impl_activate ()
  {
    double dt;

    World* w = (World*) get_parent ();
    w->get_dt (dt);

    w->get_impl ()->step (dt);
    for (auto p : w->get_phy_objects ()) {
      p->update ();
    }
  }

  World::World (Process *parent, const string& name, double x, double y, double z) :
      Process (name), raw_props
        { .x = x, .y = y, .z = z, .dt = 0.016 }, _dt (nullptr)

  {
    _step = new Spike ();
    add_symbol ("step", _step);
    _step_action = new StepAction (this, "step_action");
    _cstep = new Coupling (_step, ACTIVATION, _step_action, ACTIVATION, true);
    Graph::instance ().add_edge (_step, _step_action);

    _world_impl = PhysicsBackend::instance ()->create_world (this, x, y, z);
    if (get_parent () && get_parent ()->state_dependency () != nullptr)
      Graph::instance ().add_edge (get_parent ()->state_dependency (), _step_action);

    Process::finalize_construction (parent, name);
  }

  World::~World ()
  {
    if (get_parent () && get_parent ()->state_dependency () != nullptr)
      Graph::instance ().remove_edge (get_parent ()->state_dependency (), _step_action);
    Graph::instance ().remove_edge (_step, _step_action);
    delete _cstep;
    delete _step_action;
    delete _step;
    delete _dt;
  }

  Process*
  World::find_component (const string &n)
  {
    Process* res = Process::find_component (n);
    if (res)
      return res;
    if (n == "dt") {
      _dt = new DoublePropertyProxy (this, "dt", raw_props.dt);
      res = _dt;
    }
    return res;
  }

  void
  World::impl_activate ()
  {
    _cstep->enable ();
  }

  void
  World::impl_deactivate ()
  {
    _cstep->disable ();
  }

  void
  World::get_dt (double &dt)
  {
    dt = raw_props.dt;
  }

  void
  World::get_gravity (double &x, double &y)
  {
    x = raw_props.x;
    y = raw_props.y;
  }

  /* static Process*
   create_double_prop (Process *p, DoublePropertyProxy **prop, Coupling **cprop, Process *dest, double *rawp,
   const string& name)
   {
   *prop = new DoublePropertyProxy (p, name, *rawp, notify_none);
   *cprop = new Coupling (*prop, ACTIVATION, dest, ACTIVATION);
   if (p->somehow_activating ())
   (*cprop)->enable ();
   else
   (*cprop)->disable ();
   return *prop;
   }
   */

  PhyObj::PhyObj (Process *parent, const string& name, double x, double y, double z, double mass) :
      Process (name), _x (nullptr), _y (nullptr), _z (nullptr), _dx (nullptr), _dy (nullptr), _dz (nullptr), _roll (
          nullptr), _pitch (nullptr), _yall (nullptr), _mass (nullptr), _density (nullptr), _friction (nullptr), _cx (
          nullptr), _cy (nullptr), _cz (nullptr), _cdx (nullptr), _cdy (nullptr), _cdz (nullptr), _update_from_engine (
          false), raw_props
        { .x = x, .y = y, .z = z, .dx = 0, .dy = 0, .dz = 0, .roll = 0, .pitch = 0, .yall = 0, .mass = mass, .density =
            1.0f, .friction = 0.3f }, _world (nullptr), _impl (nullptr)
  {
    _position_action = new D3PhyObjUpdatePosition (this, "position_action");
    _velocity_action = new D3PhyObjUpdateVelocity (this, "velocity_action");

    _collision = new Spike (this, "collision");
  }

  PhyObj::~PhyObj ()
  {
    if (_cx) {
      Graph::instance ().remove_edge (_x, _position_action);
      delete _cx;
    }
    if (_cy) {
      Graph::instance ().remove_edge (_y, _position_action);
      delete _cy;
    }
    if (_cz) {
      Graph::instance ().remove_edge (_z, _position_action);
      delete _cz;
    }
    if (_cdx) {
      Graph::instance ().remove_edge (_dx, _velocity_action);
      delete _cdx;
    }
    if (_cdy) {
      Graph::instance ().remove_edge (_dy, _velocity_action);
      delete _cdy;
    }
    if (_cdz) {
      Graph::instance ().remove_edge (_dz, _velocity_action);
      delete _cdz;
    }
    delete _x;
    delete _y;
    delete _z;
    delete _position_action;
    delete _velocity_action;
    delete _dx;
    delete _dy;
    delete _dz;
    delete _density;
    delete _friction;
    delete _mass;
  }

  void
  PhyObj::impl_activate ()
  {
    if (_world == nullptr || _world->somehow_deactivating ()) {
      /*  this algorithm is a little bit tricky. We want to find the closest running world
       *  on the left side of the current object (cur_child). For this, we take its parent (curget_parent ()) and go through its
       *  children in order to find a world. If no world is found when the list iteration process arrived to (cur_child),
       *  then we set (cur_child) to its parent (curget_parent ()), and (curget_parent ()) is set to (curget_parent ()->parent).
       *  May be there is a place for simplification */
      if (_world)
        _world->remove_phy_object (this);
      bool found = false;
      Process *cur_parent = get_parent ();
      Process *cur_child = this;
      while (!found && cur_parent != nullptr) {
        if (cur_parent->get_cpnt_type () == COMPONENT_T) {
          Container *cont = dynamic_cast<Container*> (cur_parent);
          for (auto c : cont->children ()) {
            if (c == cur_child)
              break;
            else if (c->get_cpnt_type () == WORLD_T && c->somehow_activating ()) {
              _world = dynamic_cast<World*> (c);
              found = true;
            }
          }
        }
        do {
          cur_child = cur_parent;
          cur_parent = cur_parent->get_parent ();
        } while (cur_parent != nullptr && cur_parent->get_cpnt_type () != COMPONENT_T);
      }
      if (!found) {
        std::cout << "Warning no running world found\n";
        return;
      } else {
        _world->add_phy_object (this);
      }
    }
    PhysicsBackend::instance ()->enable_physical_object (this);
  }

  Process*
  PhyObj::find_component (const string &n)
  {
    Process *res = Process::find_component (n);
    if (res)
      return res;
    if (n == "x") {
      _x = new DoublePropertyProxy (this, "x", raw_props.x);
      _cx = new Coupling (_x, ACTIVATION, _position_action, ACTIVATION);
      Graph::instance ().add_edge (_x, _position_action);
      res = _x;
    }
    if (n == "y") {
      _y = new DoublePropertyProxy (this, "y", raw_props.y);
      _cy = new Coupling (_y, ACTIVATION, _position_action, ACTIVATION);
      Graph::instance ().add_edge (_y, _position_action);
      res = _y;
    }
    if (n == "z") {
      _z = new DoublePropertyProxy (this, "z", raw_props.z);
      _cz = new Coupling (_z, ACTIVATION, _position_action, ACTIVATION);
      Graph::instance ().add_edge (_z, _position_action);
      res = _z;
    }
    if (n == "dx") {
      _dx = new DoublePropertyProxy (this, "dx", raw_props.dx);
      _cdx = new Coupling (_dx, ACTIVATION, _velocity_action, ACTIVATION);
      Graph::instance ().add_edge (_dx, _velocity_action);
      res = _dx;
    }
    if (n == "dy") {
      _dy = new DoublePropertyProxy (this, "dy", raw_props.dy);
      _cdy = new Coupling (_dy, ACTIVATION, _velocity_action, ACTIVATION);
      Graph::instance ().add_edge (_dy, _velocity_action);
      res = _dy;
    }
    if (n == "dz") {
      _dz = new DoublePropertyProxy (this, "dz", raw_props.dz);
      _cdz = new Coupling (_dz, ACTIVATION, _velocity_action, ACTIVATION);
      Graph::instance ().add_edge (_dz, _velocity_action);
      res = _dz;
    }
    if (n == "roll") {
      _roll = new DoublePropertyProxy (this, "roll", raw_props.roll);
      res = _roll;
    }
    if (n == "pitch") {
      _pitch = new DoublePropertyProxy (this, "pitch", raw_props.pitch);
      res = _pitch;
    }
    if (n == "yall") {
      _yall = new DoublePropertyProxy (this, "yall", raw_props.yall);
      res = _yall;
    }
    if (n == "density") {
      _density = new DoublePropertyProxy (this, "density", raw_props.density);
      res = _density;
    }
    if (n == "friction") {
      _friction = new DoublePropertyProxy (this, "friction", raw_props.friction);
      res = _friction;
    }
    if (n == "mass") {
      res = new DoublePropertyProxy (this, "mass", raw_props.mass);
    }
    return res;
  }

  void
  PhyObj::impl_deactivate ()
  {
    PhysicsBackend::instance ()->disable_physical_object (this);
  }


  void
  PhyObj::D3PhyObjUpdatePosition::impl_activate ()
  {
    PhyObj* o = (PhyObj*) get_parent ();
    if (o->update_from_engine ())
      return;
    PhysicsBackend::instance ()->update_position (o);
  }

  void
  PhyObj::D3PhyObjUpdateVelocity::impl_activate ()
  {
    PhyObj* o = (PhyObj*) get_parent ();
    if (o->update_from_engine ())
      return;
    PhysicsBackend::instance ()->update_velocity (o);
  }

  void
  PhyObj::update ()
  {
    if (!is_activated ())
      return;
    PhysicsBackend::instance ()->get_position (this, raw_props.x, raw_props.y, raw_props.z);
    PhysicsBackend::instance ()->get_velocity (this, raw_props.dx, raw_props.dy, raw_props.dz);
    PhysicsBackend::instance ()->get_angle (this, raw_props.roll, raw_props.pitch, raw_props.yall);
    _update_from_engine = true;
    if (_x)
      _x->notify_activation ();
    if (_y)
      _y->notify_activation ();
    if (_z)
      _z->notify_activation ();
    if (_dx)
      _dx->notify_activation ();
    if (_dy)
      _dy->notify_activation ();
    if (_dz)
      _dz->notify_activation ();
    if (_roll)
      _roll->notify_activation ();
    if (_pitch)
      _pitch->notify_activation ();
    if (_yall)
      _yall->notify_activation ();
    _update_from_engine = false;
  }

  Plane::Plane (Process *parent, const string &name, double a, double b, double c, double d) :
      PhyObj (parent, name, 0, 0, 0, 0), _plane_props
        { .a = a, .b = b, .c = c, .d = d }, _a (nullptr), _b (nullptr), _c (nullptr), _d (nullptr)
  {
    Process::finalize_construction (parent, name);
  }

  Plane::~Plane ()
  {
    delete _a;
    delete _b;
    delete _c;
    delete _d;
  }

  Process*
  Plane::find_component (const string &n)
  {
    Process *res = PhyObj::find_component (n);
    if (res)
      return res;
    if (n == "a") {
      _a = new DoublePropertyProxy (this, "a", _plane_props.a);
      res = _a;
    }
    if (n == "b") {
      _b = new DoublePropertyProxy (this, "b", _plane_props.b);
      res = _b;
    }
    if (n == "c") {
      _c = new DoublePropertyProxy (this, "c", _plane_props.c);
      res = _c;
    }
    if (n == "d") {
      _d = new DoublePropertyProxy (this, "d", _plane_props.d);
      res = _d;
    }
    return res;
  }

  void
  Plane::impl_activate ()
  {
    PhyObj::impl_activate ();
    PhysicsBackend::instance ()->create_plane (this, _world, _plane_props.a, _plane_props.b, _plane_props.c,
                                               _plane_props.d);
  }

  void
  Plane::impl_deactivate ()
  {
    PhyObj::impl_deactivate ();
    PhysicsBackend::instance ()->destroy_plane (this, _world);
  }

  Box::Box (Process *parent, const string &name, double x, double y, double z, double w, double h, double d, double mass) :
      PhyObj (parent, name, x, y, z, mass), w (w), h (h), d (d), _w (nullptr), _h (nullptr), _d (nullptr)
  {
    Process::finalize_construction (parent, name);
  }

  Box::~Box ()
  {
    delete _w;
    delete _h;
    delete _d;
  }

  void
  Box::impl_activate ()
  {
    PhyObj::impl_activate ();
    PhysicsBackend::instance ()->create_box (this, _world, raw_props.x, raw_props.y, raw_props.z, w, h, d,  raw_props.mass);
  }

  void
  Box::impl_deactivate ()
  {
    PhyObj::impl_deactivate ();
    PhysicsBackend::instance ()->destroy_body (this);
  }

  Process*
  Box::find_component (const string &n)
  {
    Process* res = PhyObj::find_component (n);
    if (res)
      return res;
    if (n == "width") {
      _w = new DoublePropertyProxy (this, "width", w);
      res = _w;
    }
    if (n == "height") {
      _h = new DoublePropertyProxy (this, "height", h);
      res = _h;
    }
    if (n == "depth") {
      _d = new DoublePropertyProxy (this, "depth", d);
    }
    return res;
  }

  Sphere::Sphere (Process *parent, const string &name, double x, double y, double z, double radius, double mass) :
      PhyObj (parent, name, x, y, z, mass), radius (radius), _radius (nullptr)
  {
    Process::finalize_construction (parent, name);
  }

  Sphere::~Sphere ()
  {
    delete _radius;
  }
  void
  Sphere::impl_activate ()
  {
    PhyObj::impl_activate ();
    PhysicsBackend::instance ()->create_sphere (this, _world, raw_props.x, raw_props.y, raw_props.z, radius,
                                                raw_props.mass);
  }

  void
  Sphere::impl_deactivate ()
  {
    PhyObj::impl_deactivate ();
    PhysicsBackend::instance ()->destroy_body (this);
  }

  Process*
  Sphere::find_component (const string &n)
  {
    Process* res = PhyObj::find_component (n);
    if (res)
      return res;
    if (n == "r") {
      _radius = new DoublePropertyProxy (this, "r", radius);
      res = _radius;
    }
    return res;
  }
}
