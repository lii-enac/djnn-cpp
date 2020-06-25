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

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/tree/text_property.h"
#include "core/control/action.h"

#include "utils/debug.h"

#include <functional>

namespace djnn
{

  extern std::vector<std::string> loadedModules; 

  void init_utils ();
  std::string strTrim (std::string s);

  class Properties : public FatProcess
  {
  private:
    class PropertiesAction : public Action
    {
    public:
      PropertiesAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
      void impl_activate () override;
    };
  public:
    Properties (ParentProcess* parent, const std::string& name, const std::string& filename = "");
    virtual ~Properties ();
    void impl_activate () override;
    void impl_deactivate () override;
    void add_property ();

#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& type) override;
#endif

  private:
    TextProperty *_input;
    Coupling* _c_input;
    PropertiesAction* _action;
  };

  inline
  void
  depth_first_traverse(CoreProcess *p, std::function<void(FatProcess*)> f)
  {
    auto * fp = dynamic_cast<FatProcess*>(p);
    if(!fp) return;
    f(fp);
    for (auto child: fp->children ()) depth_first_traverse(child.second, f);
  }

  void run_stats(FatProcess *p);

  void display_creation_stats ();

}
