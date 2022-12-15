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
#include "gui/style/abstract_style.h"


namespace djnn
{
  class TextAnchor : public AbstractStyle
  {
  public:
    TextAnchor (ParentProcess* parent, const string& name, int anchor);
    virtual ~TextAnchor ();
    void draw () override;
    TextAnchor* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
    void get_properties_values (int& anchor);
    const vector<string>& get_properties_name () const override;
    virtual FatChildProcess* find_child_impl (const string&) override;
		AbstractIntProperty* anchor () { return (AbstractIntProperty*) find_child_impl ("anchor"); }

  protected:
    struct raw_props_t { int anchor; };
    raw_props_t raw_props;
    Coupling *_canchor;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
