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

namespace djnn
{
  class AbstractPropFontWeight : public AbstractStyle
  {
  public:
    AbstractPropFontWeight (ParentProcess* parent, const std::string& name, int weight);
    virtual ~AbstractPropFontWeight ();
    
    
    void get_properties_values (int& weight);
    std::vector<std::string> get_properties_name () override {
      std::vector<std::string> res;
      res.push_back ("weight");
      return res;
    }
    virtual FatChildProcess* find_child_impl (const std::string&) override;
		AbstractIntProperty* weight () { return (AbstractIntProperty*) find_child_impl ("weight"); }

  protected:
    struct raw_props_t { int weight; };
    raw_props_t raw_props;
    CouplingWithData *_cweight;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
