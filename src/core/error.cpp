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
 *
 */

#include "../core/error.h"

namespace djnn
{
  void
  error (const std::string &msg)
  {
    std::cerr << "Error: " << msg << std::endl;
    exit (0);
  }

  void
  warning (const std::string &msg)
  {
    std::cerr << "Warning: " << msg << std::endl;
  }

}
