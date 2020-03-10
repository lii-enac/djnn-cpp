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
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"

namespace djnn
{
  extern Process *mainloop;
  void init_exec_env();
  void clear_exec_env();
}


#include "exec_env/exit.h"
#include "exec_env/main_loop.h"
