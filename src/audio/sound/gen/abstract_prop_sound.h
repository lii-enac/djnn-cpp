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
#include "audio/abstract_sobj.h"

namespace djnn {
class AbstractPropSound : public AbstractSObj
{
  public:
    AbstractPropSound (CoreProcess* parent, const string& name, double gain, double lowpass_gain, double lowpass_freq, double x, double y, double z, double pitch_mul, int loop);
    virtual ~AbstractPropSound ();

    void                                    get_properties_values (double& gain, double& lowpass_gain, double& lowpass_freq, double& x, double& y, double& z, double& pitch_mul, int& loop);
    const djnnstl::vector<djnnstl::string>& get_properties_name () const override;
    virtual CoreProcess*                    find_child_impl (const string&) override;
    AbstractDoubleProperty*                 gain () { return (AbstractDoubleProperty*)find_child_impl ("gain"); }
    AbstractDoubleProperty*                 lowpass_gain () { return (AbstractDoubleProperty*)find_child_impl ("lowpass_gain"); }
    AbstractDoubleProperty*                 lowpass_freq () { return (AbstractDoubleProperty*)find_child_impl ("lowpass_freq"); }
    AbstractDoubleProperty*                 x () { return (AbstractDoubleProperty*)find_child_impl ("x"); }
    AbstractDoubleProperty*                 y () { return (AbstractDoubleProperty*)find_child_impl ("y"); }
    AbstractDoubleProperty*                 z () { return (AbstractDoubleProperty*)find_child_impl ("z"); }
    AbstractDoubleProperty*                 pitch_mul () { return (AbstractDoubleProperty*)find_child_impl ("pitch_mul"); }
    AbstractIntProperty*                    loop () { return (AbstractIntProperty*)find_child_impl ("loop"); }

  protected:
    struct raw_props_t
    {
        double gain;
        double lowpass_gain;
        double lowpass_freq;
        double x;
        double y;
        double z;
        double pitch_mul;
        int    loop;
    };
    raw_props_t raw_props;
    Coupling *  _cgain, *_clowpass_gain, *_clowpass_freq, *_cx, *_cy, *_cz, *_cpitch_mul, *_cloop;
    void        impl_activate () override;
    void        impl_deactivate () override;
};
} // namespace djnn
