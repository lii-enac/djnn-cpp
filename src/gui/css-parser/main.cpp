/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include <iostream>
#include <fstream>

#include "driver.h"
#include "core/core.h"
#include "base/base.h"
#include "display/display.h"
#include "gui/gui.h"

using namespace djnn;

int main(int argc, char *argv[])
{
  init_core ();
  init_base ();
  init_display ();
  init_gui ();

  Component root (nullptr, "");
  Window f (&root, "f", "my frame", 0, 0, 500, 500);
  css::Driver driver;
  std::fstream infile (argv[1]);
  if (!infile.good())
  {
    std::cerr << "Could not open file: " << argv[1] << std::endl;
    return 0;
  }

  bool result = driver.parse_stream(infile, argv[1], &root);
  Rectangle r (&root, "r", 50, 50, 100, 100, 5, 5);
  r.add_style_class ("st0");
  root.activate ();
  MainLoop::instance ().activate ();
  return result;
}
