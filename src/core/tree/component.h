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

#include "process.h"
#include <list>

namespace djnn {
  using namespace std;

  class Container : public Process
  {
  typedef std::vector<Process*> children_t;
  public:
    Container ();
    Container (Process* parent, const string& name);
    void add_child (Process* c, const string& name) override;
    void remove_child (Process* c) override;
    void remove_child (const string& name) override;
    void draw () override;
    void activate () override;
    void deactivate () override;
    void print_children ();
    virtual ~Container ();
    children_t children () { return _children; }
  protected:
    children_t _children;
  };

  class Component : public Container
  {
  public:
    Component () : Container () {}
    Component (Process* parent, const string& name) : Container (parent, name) { Process::finalize (); }
    Process* clone () override;
    void serialize (const string& type) override;
  };

  class AssignmentSequence : public Container
  {
  public:
    AssignmentSequence ();
    AssignmentSequence (Process *p, const string &n, bool is_model);
    virtual ~AssignmentSequence () {};
    void add_child (Process* c, const string& name) override;
    void draw () override {}
    void activate () override;
    void post_activate () override;
    void deactivate () override {};
  };
}
