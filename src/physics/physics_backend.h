/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019-2024)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
#pragma once

namespace djnn {
class AbstractPhysicsBackend;

class PhysicsBackend {
  public:
    static AbstractPhysicsBackend* instance ();
    static void                    init ();

  private:
    class Impl;
    static Impl* _instance;
};
} // namespace djnn
