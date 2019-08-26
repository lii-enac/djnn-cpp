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
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "../core/ontology/process.h"
#include "../core/ontology/coupling.h"
#include "../core/tree/text_property.h"
#include "../core/control/action.h"

namespace djnn
{

  extern std::vector<string> loadedModules; 

  void init_utils ();
  std::string strTrim (std::string s);

  class Properties : public Process
  {
  private:
    class PropertiesAction : public Action
    {
    public:
      PropertiesAction (Process *p, const string &n) : Action (p, n) { Process::finalize_construction (p); }
      void impl_activate () override;
      void impl_deactivate () override {}
    };
  public:
    Properties (Process *p, const string &n, const string& filename = "");
    virtual ~Properties ();
    void impl_activate () override;
    void impl_deactivate () override;
    void serialize (const string& type) override;
    void add_property ();
  private:
    TextProperty *_input;
    Coupling* _c_input;
    PropertiesAction* _action;
  };
}
