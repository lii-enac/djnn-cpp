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

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"



namespace djnn
{
  class OutlineMiterLimit : public AbstractStyle
  {
  public:
    OutlineMiterLimit (ParentProcess* parent, const std::string& name, int limit);
    virtual ~OutlineMiterLimit ();
    void draw () override;
    OutlineMiterLimit* clone () override;
    void get_properties_values (int& limit);
    const std::vector<std::string>& get_properties_name () const override;
    virtual FatChildProcess* find_child_impl (const std::string&) override;
		AbstractIntProperty* limit () { return (AbstractIntProperty*) find_child_impl ("limit"); }

  protected:
    struct raw_props_t { int limit; };
    raw_props_t raw_props;
    Coupling *_climit;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
