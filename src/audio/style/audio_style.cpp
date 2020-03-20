/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "../abstract_sobj.h"
//#include "backend.h"
//#include "abstract_backend.h"

//#include "display/window.h"
//#include "display/update_drawing.h"

#include <iostream>

namespace djnn
{
  Process*
  AbstractSObj::create_GObj_prop (BoolPropertyProxy **prop, Coupling **cprop, bool *rawp, const std::string& name, int notify_mask)
  {
    *prop = new BoolPropertyProxy (this, name, *rawp, notify_mask);
    /*Process *update = UpdateDrawing::instance ()->get_damaged ();
    *cprop = new Coupling (*prop, ACTIVATION, update, ACTIVATION);
    if (this->somehow_activating ()) {
      //auto _frame = frame ();
      //(*cprop)->enable(_frame);
    }
    else
      (*cprop)->disable ();*/
    return *prop;
  }

  Process*
  AbstractSObj::create_GObj_prop (IntPropertyProxy **prop, Coupling **cprop, int *rawp, const std::string& name, int notify_mask)
  {
    *prop = new IntPropertyProxy (this, name, *rawp, notify_mask);
    /*Process *update = UpdateDrawing::instance ()->get_damaged ();
    *cprop = new Coupling (*prop, ACTIVATION, update, ACTIVATION);
    if (this->somehow_activating ()) {
      //auto _frame = frame ();
      //(*cprop)->enable(_frame);
    }
    else
      (*cprop)->disable ();*/
    return *prop;
  }

  Process*
  AbstractSObj::create_GObj_prop (DoublePropertyProxy **prop, Coupling **cprop, double *rawp, const std::string& name, int notify_mask)
  {
    *prop = new DoublePropertyProxy (this, name, *rawp, notify_mask);
    /*Process *update = UpdateDrawing::instance ()->get_damaged ();
    *cprop = new Coupling (*prop, ACTIVATION, update, ACTIVATION);
    if (this->somehow_activating ()) {
      //auto _frame = frame ();
      //(*cprop)->enable(_frame);
    }
    else
      (*cprop)->disable ();*/
    return *prop;
  }

  Process*
  AbstractSObj::create_GObj_prop (TextPropertyProxy **prop, Coupling **cprop, std::string *rawp, const std::string& name, int notify_mask)
  {
    *prop = new TextPropertyProxy (this, name, *rawp, notify_mask);
    /*Process *update = UpdateDrawing::instance ()->get_damaged ();
    *cprop = new Coupling (*prop, ACTIVATION, update, ACTIVATION);
    if (this->somehow_activating ()) {
      //auto _frame = frame ();
      //(*cprop)->enable(_frame);
    }
    else
      (*cprop)->disable ();*/
    return *prop;
  }
}
