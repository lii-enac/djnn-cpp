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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 *  !! this file has been automatically generated - do NOT modify !!
 *
 */

#pragma once

#include "core/ontology/coupling.h"
#include "core/ontology/process.h"

namespace djnn {
class AbstractPropBox : public PhyObj {
  public:
    AbstractPropBox (CoreProcess* parent, const string& name, double w, double h, double d);
    virtual ~AbstractPropBox ();

    void                    get_properties_values (double& w, double& h, double& d);
    const vector<string>&   get_properties_name () const override;
    virtual CoreProcess*    find_child_impl (const string&) override;
    AbstractDoubleProperty* w () { return (AbstractDoubleProperty*)find_child_impl ("w"); }
    AbstractDoubleProperty* h () { return (AbstractDoubleProperty*)find_child_impl ("h"); }
    AbstractDoubleProperty* d () { return (AbstractDoubleProperty*)find_child_impl ("d"); }

  protected:
    struct raw_props_t {
        double w;
        double h;
        double d;
    };
    raw_props_t raw_props;
    Coupling *  _cw, *_ch, *_cd;
    void        impl_activate () override;
    void        impl_deactivate () override;
};
} // namespace djnn
