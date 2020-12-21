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

#include "ode_backend.h"

namespace djnn
{
  ODEBackend *ODEBackend::_instance;
  std::once_flag ODEBackend::onceFlag;

  ODEBackend*
  ODEBackend::instance ()
  {
    std::call_once (ODEBackend::onceFlag, [] () {
      _instance = new ODEBackend ();
    });

    return _instance;
  }

  ODEBackend::ODEBackend ()
  {
  }

  ODEBackend::~ODEBackend ()
  {
  }

  ODEWorld::ODEWorld (double dx, double dy, double dz)
  {
    _world_id = dWorldCreate ();
    _space_id = dHashSpaceCreate (0);
    dWorldSetCFM (_world_id, 1e-5);
    dWorldSetGravity (_world_id, dx, dy, dz);
    _joint_group_id = dJointGroupCreate (0);
  }

  ODEWorld::~ODEWorld ()
  {
    dJointGroupDestroy (_joint_group_id);
    dSpaceDestroy (_space_id);
    dWorldDestroy (_world_id);
  }

  static void
  nearCallback (void *data, dGeomID o1, dGeomID o2)
  {
    dBodyID b1 = dGeomGetBody (o1);
    dBodyID b2 = dGeomGetBody (o2);

    PhyObj* phy_obj_1 = (PhyObj*) dGeomGetData (o1);
    PhyObj* phy_obj_2 = (PhyObj*) dGeomGetData (o2);

    dWorldID world = ((ODEWorld*) phy_obj_1->get_world ()->get_impl ())->get_world ();
    dJointGroupID group = ((ODEWorld*) phy_obj_1->get_world ()->get_impl ())->get_joint_group ();
    dContact contact;
    contact.surface.mode = dContactBounce | dContactSoftCFM;
    contact.surface.mu = dInfinity;
    contact.surface.bounce = 0.9;
    contact.surface.bounce_vel = 0.1;
    contact.surface.soft_cfm = 0.001;
    if (/*int numc =*/ dCollide (o1, o2, 1, &contact.geom, sizeof(dContact))) {
      dJointID c = dJointCreateContact (world, group, &contact);
      dJointAttach (c, b1, b2);
      phy_obj_1->collision ();
      phy_obj_2->collision ();
    }
  }

  void
  ODEWorld::step (double dt)
  {
    dSpaceCollide (_space_id, 0, nearCallback);
    dWorldStep (_world_id, dt);
    dJointGroupEmpty (_joint_group_id);
  }

  WorldImpl*
  ODEBackend::create_world (World *world, double x, double y, double z)
  {
    return new ODEWorld (x, y, z);
  }

  void
  ODEBackend::create_plane (PhyObj* phy_obj, World *world, double a, double b, double c, double d)
  {
    ODEWorld* w = (ODEWorld*) world->get_impl ();
    dSpaceID _space_id = w->get_space ();
    dGeomID geom = dCreatePlane (_space_id, a, b, c, d);
    dGeomSetData (geom, phy_obj);
    new ODEPhyObj (nullptr, geom);
  }

  void
  ODEBackend::destroy_plane (PhyObj* phy_obj, World *world)
  {
    ODEPhyObj *obj = (ODEPhyObj*) phy_obj->get_impl ();
    dGeomDestroy (obj->get_geom ());
    delete obj;
  }

  void
  ODEBackend::update_position (PhyObj* phy_obj)
  {
    dBodyID body = ((ODEPhyObj*) phy_obj->get_impl ())->get_body ();
    double x,y,z;
    phy_obj->get_position_values(x,y,z);
    dBodySetPosition (body, x, y, z);
  }

  void
  ODEBackend::get_position (PhyObj* phy_obj, double &x, double &y, double &z)
  {
    const dReal *pos;
    dBodyID body = ((ODEPhyObj*) phy_obj->get_impl ())->get_body ();
    pos = dBodyGetPosition (body);
    x = pos[0];
    y = pos[1];
    z = pos[2];
  }

  void
  ODEBackend::get_angle (PhyObj* phy_obj, double &roll, double &pitch, double &yaw)
  {
    const dReal *q;
    dBodyID body = ((ODEPhyObj*) phy_obj->get_impl ())->get_body ();
    q = dBodyGetRotation (body);
    double w = q[0];
    double x = q[1];
    double y = q[2];
    double z = q[3];
    // Taken from Wikipedia: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Quaternion_to_Euler_Angles_Conversion
    // roll (x-axis rotation)
    double sinr_cosp = +2.0 * (w * x + y * z);
    double cosr_cosp = +1.0 - 2.0 * (x * x + y * y);
    roll = atan2 (sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = +2.0 * (w * y - z * x);
    if (fabs (sinp) >= 1)
      pitch = copysign (M_PI / 2, sinp); // use 90 degrees if out of range
    else
      pitch = asin (sinp);

    // yaw (z-axis rotation)
    double siny_cosp = +2.0 * (w * z + x * y);
    double cosy_cosp = +1.0 - 2.0 * (y * y + z * z);
    yaw = atan2 (siny_cosp, cosy_cosp);
  }

  void
  ODEBackend::get_velocity (PhyObj* phy_obj, double &dx, double &dy, double &dz)
  {
    const dReal *v;
    dBodyID body = ((ODEPhyObj*) phy_obj->get_impl ())->get_body ();
    v = dBodyGetLinearVel (body);
    dx = v[0];
    dy = v[1];
    dz = v[2];
  }

  void
  ODEBackend::update_velocity (PhyObj* phy_obj)
  {
    dBodyID body = ((ODEPhyObj*) phy_obj->get_impl ())->get_body ();
    double dx,dy,dz;
    phy_obj->get_velocity_values (dx,dy,dz);
    dBodySetLinearVel (body, dx, dy, dz);
  }

  void
  ODEBackend::create_sphere (PhyObj* phy_obj, World *world, double x, double y, double z, double radius, double mass)
  {
    ODEWorld* w = (ODEWorld*) world->get_impl ();
    dWorldID world_id = w->get_world ();
    dBodyID body = dBodyCreate (world_id);
    dGeomID geom = dCreateSphere (w->get_space (), radius);
    dMass m_sphere;
    dMassSetZero (&m_sphere);
    dMassSetSphere (&m_sphere, mass, radius);
    dBodySetMass (body, &m_sphere);
    dGeomSetBody (geom, body);
    dBodySetPosition (body, x, y, z);
    dGeomSetData (geom, phy_obj);
    phy_obj->set_impl (new ODEPhyObj (body, geom));
  }

  void
  ODEBackend::create_box (PhyObj* phy_obj, World *world, double x, double y, double z, double lx, double ly, double lz, double mass)
  {
    ODEWorld* ode_w = (ODEWorld*) world->get_impl ();
    dWorldID world_id = ode_w->get_world ();
    dBodyID body = dBodyCreate (world_id);
    dGeomID geom = dCreateBox (ode_w->get_space (), lx, ly, lz);
    dMass m_box;
    dMassSetZero (&m_box);
    dMassSetBox (&m_box, mass, lx, ly, lz);
    dBodySetMass (body, &m_box);
    dGeomSetBody (geom, body);
    dBodySetPosition (body, x, y, z);
    dGeomSetData (geom, phy_obj);
    phy_obj->set_impl (new ODEPhyObj (body, geom));
  }

  void
  ODEBackend::destroy_body (PhyObj* phy_obj)
  {
    dBodyDestroy (((ODEPhyObj*) phy_obj->get_impl ())->get_body ());
    dGeomDestroy (((ODEPhyObj*) phy_obj->get_impl ())->get_geom ());
    delete phy_obj;
  }

} /* namespace djnn */
