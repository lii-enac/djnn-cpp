/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020-2024)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */
#include "audio/abstract_sobj.h"

namespace djnn {
void
AbstractSObj::impl_activate ()
{
    // Backend::instance ()->activate_gobj (this);
}

void
AbstractSObj::impl_deactivate ()
{
}

FatProcess*
AbstractSObj::create_GObj_prop (BoolPropertyProxy** prop, CouplingWithData** cprop, bool* rawp, const string& name, int notify_mask)
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
AbstractSObj::create_GObj_prop (IntPropertyProxy** prop, CouplingWithData** cprop, int* rawp, const string& name, int notify_mask)
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
AbstractSObj::create_GObj_prop (DoublePropertyProxy** prop, CouplingWithData** cprop, double* rawp, const string& name, int notify_mask)
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
AbstractSObj::create_GObj_prop (TextPropertyProxy** prop, CouplingWithData** cprop, string* rawp, const string& name, int notify_mask)
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
FatProcess*
AbstractSObj::create_GObj_prop (TextPropertyProxy** prop, CouplingWithData** cprop, string** rawp, const string& name, int notify_mask)
{
    *prop = new TextPropertyProxy (this, name, **rawp, notify_mask);
    // create_Gobj_update_coupling(reinterpret_cast<CoreProcess**>(prop), cprop);
    return *prop;
}
} // namespace djnn