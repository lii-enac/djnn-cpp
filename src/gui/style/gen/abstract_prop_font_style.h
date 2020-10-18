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
  class AbstractPropFontStyle : public AbstractStyle
  {
  public:
    AbstractPropFontStyle (ParentProcess* parent, const std::string& name, int style);
    virtual ~AbstractPropFontStyle ();
    
    
    void get_properties_values (int& style);
    std::vector<std::string> get_properties_name () override {
      std::vector<std::string> res;
      res.push_back ("style");
      return res;
    }
    virtual FatChildProcess* find_child_impl (const std::string&) override;
		AbstractIntProperty* style () { return (AbstractIntProperty*) find_child_impl ("style"); }

  protected:
    struct raw_props_t { int style; };
    raw_props_t raw_props;
    CouplingWithData *_cstyle;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
