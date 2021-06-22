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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stéphane Conversy <stephane.convery@enac.fr>
 *
 */

#include <regex>

#include "core/ontology/coupling.h"
#include "core/tree/text_property.h"
#include "core/control/action.h"

namespace djnn {

  class Regex : public FatProcess
  {
  private:
    class RegexAction : public Action {
    public:
      RegexAction (ParentProcess* parent, const std::string& name, Regex& reg) : Action (parent, name), _reg (reg) {}
      virtual ~RegexAction () {}
      void impl_activate () override;
    private:
      Regex& _reg;
    };
  public:
    Regex (ParentProcess* parent, const std::string& name, const std::string& Regex = "");
    virtual ~Regex ();
    void impl_activate () override;
    void impl_deactivate () override;

 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const std::string& format) override;
#endif
    
    FatProcess* find_child_impl (const std::string&) override;

  private:
    void set_parent (ParentProcess* parent) override;
    TextProperty _input;
    std::string _init;
    std::regex _regex;
    RegexAction _reg_action;
    Coupling _c_reg;
    map <int, TextProperty*> _in_map;
  };

}