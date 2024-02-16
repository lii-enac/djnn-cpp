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
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#pragma once

namespace djnn {
class AbstractBackend;

class Backend {
  public:
    static AbstractBackend* instance ();
    static void             init ();
    static void             clear ();

  private:
    class Impl;
    static Impl* _instance;
};
} // namespace djnn
