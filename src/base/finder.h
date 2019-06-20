/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "../core/tree/component.h"
#include "../core/tree/spike.h"
#include "../core/tree/text_property.h"
#include "../core/tree/ref_property.h"

namespace djnn
{

  class Finder : public Process
  {
  private:
    class FinderAction : public Action
    {
    public:
      FinderAction (Process *p, const string &n) : Action (p, n) { Process::finalize_construction (); }
      virtual ~FinderAction () {}
      void impl_activate () override;
      void impl_deactivate () override {};
    };
    public:
      Finder (Process *p, const string &n, Process *container, const string& path);
      virtual ~Finder ();
      void impl_activate () override;
      void impl_deactivate () override;
      void serialize (const string& type) override;
    private:
      Container* _container;
      TextProperty *_path, *_key;
      RefProperty *_result;
      Spike *_found, *_not_found;
      FinderAction *_action;
      Coupling *_cfind;
  };
}
