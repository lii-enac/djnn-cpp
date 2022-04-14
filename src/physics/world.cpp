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
#include "core/core-dev.h" // graph add/remove edge
#include "core/tree/component.h"
#include "core/control/spike.h"
#include "core/ontology/coupling.h"
#include "core/property/double_property.h"
#include "core/utils/error.h"

#include "core/utils/iostream.h"

namespace djnn
{

  void
  AbstractPObj::impl_activate ()
  {
  }
  
  void
  AbstractPObj::impl_deactivate ()
  {
  }

  FatProcess*
  AbstractPObj::create_GObj_prop (BoolPropertyProxy **prop, CouplingWithData **cprop, bool *rawp, const string& name, int notify_mask)
  {
    *prop = new BoolPropertyProxy (this, name, *rawp, notify_mask);
    /*FatProcess *update = UpdateDrawing::instance ()->get_damaged ();
    *cprop = new Coupling (*prop, ACTIVATION, update, ACTIVATION);
    if (this->somehow_activating ()) {
      //auto _frame = get_frame ();
      //(*cprop)->enable(_frame);
    }
    else
      (*cprop)->disable ();*/
    return *prop;
  }

  FatProcess*
  AbstractPObj::create_GObj_prop (IntPropertyProxy **prop, CouplingWithData **cprop, int *rawp, const string& name, int notify_mask)
  {
    *prop = new IntPropertyProxy (this, name, *rawp, notify_mask);
    /*FatProcess *update = UpdateDrawing::instance ()->get_damaged ();
    *cprop = new Coupling (*prop, ACTIVATION, update, ACTIVATION);
    if (this->somehow_activating ()) {
      //auto _frame = get_frame ();
      //(*cprop)->enable(_frame);
    }
    else
      (*cprop)->disable ();*/
    return *prop;
  }

  FatProcess*
  AbstractPObj::create_GObj_prop (DoublePropertyProxy **prop, CouplingWithData **cprop, double *rawp, const string& name, int notify_mask)
  {
    *prop = new DoublePropertyProxy (this, name, *rawp, notify_mask);
    /*FatProcess *update = UpdateDrawing::instance ()->get_damaged ();
    *cprop = new Coupling (*prop, ACTIVATION, update, ACTIVATION);
    if (this->somehow_activating ()) {
      //auto _frame = get_frame ();
      //(*cprop)->enable(_frame);
    }
    else
      (*cprop)->disable ();*/
    return *prop;
  }

  FatProcess*
  AbstractPObj::create_GObj_prop (TextPropertyProxy **prop, CouplingWithData **cprop, string *rawp, const string& name, int notify_mask)
  {
    *prop = new TextPropertyProxy (this, name, *rawp, notify_mask);
    /*FatProcess *update = UpdateDrawing::instance ()->get_damaged ();
    *cprop = new Coupling (*prop, ACTIVATION, update, ACTIVATION);
    if (this->somehow_activating ()) {
      //auto _frame = get_frame ();
      //(*cprop)->enable(_frame);
    }
    else
      (*cprop)->disable ();*/
    return *prop;
  }

  void
  World::StepAction::impl_activate ()
  {
    double x,y,z,dt;

    World* w = (World*) get_parent ();
    w->get_properties_values(x,y,z,dt);

    w->get_impl ()->step (dt);
    for (auto p : w->get_phy_objects ()) {
      p->update ();
    }
  }

  World::World (ParentProcess* parent, const string& name, double x, double y, double z) :
  AbstractPropWorld (parent, name, x, y, z, 0.016), _dt (nullptr)
  {
    _step = new Spike (this, "spike");
    add_symbol ("step", _step);
    _step_action = new StepAction (this, "step_action");
    _cstep = new Coupling (_step, ACTIVATION, _step_action, ACTIVATION, true);
    graph_add_edge (_step, _step_action);

    _world_impl = PhysicsBackend::instance ()->create_world (this, x, y, z);
    if (get_parent () && get_parent ()->state_dependency () != nullptr)
      graph_add_edge (get_parent ()->state_dependency (), _step_action);

    finalize_construction (parent, name);
  }

  World::~World ()
  {
    if (get_parent () && get_parent ()->state_dependency () != nullptr)
      graph_remove_edge (get_parent ()->state_dependency (), _step_action);
    graph_remove_edge (_step, _step_action);
    delete _cstep;
    delete _step_action;
    delete _step;
    delete _dt;
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


  PhyObj::PhyObj (ParentProcess* parent, const string& name)
  : PhyObj (parent, name, 0,0,0,0)
  {}

  PhyObj::PhyObj (ParentProcess* parent, const string& name, double x, double y, double z, double mass) :
      AbstractPropPhyObj (parent, name, x, y, z, 0,0,0, 0,0,0, mass, 1.0, 0.3),
      /*FatProcess (name),*/
      _x (nullptr), _y (nullptr), _z (nullptr), _dx (nullptr), _dy (nullptr), _dz (nullptr),
      _roll (nullptr), _pitch (nullptr), _yall (nullptr),
      /*_mass (nullptr), _density (nullptr), _friction (nullptr),
          _cx (
          nullptr), _cy (nullptr), _cz (nullptr), _cdx (nullptr), _cdy (nullptr), _cdz (nullptr),*/
          _update_from_engine (false),
          /*raw_props
        { .x = x, .y = y, .z = z, .dx = 0, .dy = 0, .dz = 0, .roll = 0, .pitch = 0, .yall = 0, .mass = mass, .density =
            1.0f, .friction = 0.3f },*/
            _world (nullptr),
            _impl (nullptr)
  {
    _position_action = new D3PhyObjUpdatePosition (this, "position_action");
    _velocity_action = new D3PhyObjUpdateVelocity (this, "velocity_action");

    _collision = new Spike (this, "collision");
  }

  PhyObj::~PhyObj ()
  {
    delete _position_action;
    delete _velocity_action;
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
      FatProcess *cur_parent = get_parent ();
      FatProcess *cur_child = this;
      while (!found && cur_parent != nullptr) {
        if (cur_parent->get_process_type () == CONTAINER_T) {
          Container *cont = dynamic_cast<Container*> (cur_parent);
          for (auto c : cont->children ()) {
            if (c == cur_child)
              break;
            else if (c->get_process_type () == WORLD_T && c->somehow_activating ()) {
              _world = dynamic_cast<World*> (c);
              found = true;
            }
          }
        }
        do {
          cur_child = cur_parent;
          cur_parent = cur_parent->get_parent ();
        } while (cur_parent != nullptr && cur_parent->get_process_type () != CONTAINER_T);
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

  FatChildProcess*
  PhyObj::find_child_impl (const string& name)
  {
      auto * res = AbstractPropPhyObj::find_child_impl (name);
      #define phyattr(x) if (name ==#x) _##x = dynamic_cast<DoublePropertyProxy*>(res); else
      phyattr(x)
      phyattr(y)
      phyattr(z)
      phyattr(dx)
      phyattr(dy)
      phyattr(dz)
      phyattr(roll)
      phyattr(pitch)
      phyattr(yall)
      #undef phyattr
      {}
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
  PhyObj::get_position_values (double& x, double& y, double& z)
  {
    x = raw_props.x;
    y = raw_props.y;
    z = raw_props.z;
  }

  void
  PhyObj::get_velocity_values (double& dx, double& dy, double& dz)
  {
    dx = raw_props.dx;
    dy = raw_props.dy;
    dz = raw_props.dz;
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

    #define phyattr(x) if (_##x) _##x->schedule_activation ();
    phyattr(x)
    phyattr(y)
    phyattr(z)
    phyattr(dx)
    phyattr(dy)
    phyattr(dz)
    phyattr(roll)
    phyattr(pitch)
    phyattr(yall)
    #undef phyattr
    
    _update_from_engine = false;
  }

  

  Plane::Plane (ParentProcess* parent, const string& name, double a, double b, double c, double d) :
      AbstractPropPlane (parent, name, a, b, c, d)
      //plane_props
      //  { .a = a, .b = b, .c = c, .d = d }, _a (nullptr), _b (nullptr), _c (nullptr), _d (nullptr)
  {
    finalize_construction (parent, name);
  }

  Plane::~Plane ()
  {
    // delete _a;
    // delete _b;
    // delete _c;
    // delete _d;
  }

  // FatProcess*
  // Plane::find_child (const string& n)
  // {
  //   FatProcess *res = PhyObj::find_child (n);
  //   if (res)
  //     return res;
  //   if (n == "a") {
  //     _a = new DoublePropertyProxy (this, "a", _plane_props.a);
  //     res = _a;
  //   }
  //   if (n == "b") {
  //     _b = new DoublePropertyProxy (this, "b", _plane_props.b);
  //     res = _b;
  //   }
  //   if (n == "c") {
  //     _c = new DoublePropertyProxy (this, "c", _plane_props.c);
  //     res = _c;
  //   }
  //   if (n == "d") {
  //     _d = new DoublePropertyProxy (this, "d", _plane_props.d);
  //     res = _d;
  //   }
  //   return res;
  // }

  void
  Plane::impl_activate ()
  {
    PhyObj::impl_activate ();
    PhysicsBackend::instance ()->create_plane (this, _world,
      AbstractPropPlane::raw_props.a, AbstractPropPlane::raw_props.b, AbstractPropPlane::raw_props.c, AbstractPropPlane::raw_props.d);
  }

  void
  Plane::impl_deactivate ()
  {
    PhyObj::impl_deactivate ();
    PhysicsBackend::instance ()->destroy_plane (this, _world);
  }

  Box::Box (ParentProcess* parent, const string& name, double x, double y, double z, double w, double h, double d, double mass) :
      AbstractPropBox (parent, name, w, h, d)
      //, w (w), h (h), d (d), _w (nullptr), _h (nullptr), _d (nullptr)
  {
    PhyObj::raw_props.x = x;
    PhyObj::raw_props.y = y;
    PhyObj::raw_props.z = z;
    PhyObj::raw_props.mass = mass;
    finalize_construction (parent, name);
  }

  Box::~Box ()
  {
    // delete _w;
    // delete _h;
    // delete _d;
  }

  void
  Box::impl_activate ()
  {
    PhyObj::impl_activate ();
    PhysicsBackend::instance ()->create_box (this, _world,
        PhyObj::raw_props.x, PhyObj::raw_props.y, PhyObj::raw_props.z,
        AbstractPropBox::raw_props.w, AbstractPropBox::raw_props.h, AbstractPropBox::raw_props.d,
        PhyObj::raw_props.mass
    );
  }

  void
  Box::impl_deactivate ()
  {
    PhyObj::impl_deactivate ();
    PhysicsBackend::instance ()->destroy_body (this);
  }

  // FatProcess*
  // Box::find_child (const string& n)
  // {
  //   FatProcess* res = PhyObj::find_child (n);
  //   if (res)
  //     return res;
  //   if (n == "width") {
  //     _w = new DoublePropertyProxy (this, "width", w);
  //     res = _w;
  //   }
  //   if (n == "height") {
  //     _h = new DoublePropertyProxy (this, "height", h);
  //     res = _h;
  //   }
  //   if (n == "depth") {
  //     _d = new DoublePropertyProxy (this, "depth", d);
  //   }
  //   return res;
  // }

  Sphere::Sphere (ParentProcess* parent, const string& name, double x, double y, double z, double radius, double mass) :
    AbstractPropSphere (parent, name, radius) 
      //PhyObj (parent, name, x, y, z, mass), radius (radius), _radius (nullptr)
  {
    PhyObj::raw_props.x = x;
    PhyObj::raw_props.y = y;
    PhyObj::raw_props.z = z;
    PhyObj::raw_props.mass = mass;
    finalize_construction (parent, name);
  }

  Sphere::~Sphere ()
  {
    //delete _radius;
  }
  void
  Sphere::impl_activate ()
  {
    PhyObj::impl_activate ();
    PhysicsBackend::instance ()->create_sphere (this, _world,
        PhyObj::raw_props.x, PhyObj::raw_props.y, PhyObj::raw_props.z,
        AbstractPropSphere::raw_props.radius,
        PhyObj::raw_props.mass
    );
  }

  void
  Sphere::impl_deactivate ()
  {
    PhyObj::impl_deactivate ();
    PhysicsBackend::instance ()->destroy_body (this);
  }

  // FatProcess*
  // Sphere::find_child (const string& n)
  // {
  //   FatProcess* res = PhyObj::find_child (n);
  //   if (res)
  //     return res;
  //   if (n == "r") {
  //     _radius = new DoublePropertyProxy (this, "r", radius);
  //     res = _radius;
  //   }
  //   return res;
  // }
}
