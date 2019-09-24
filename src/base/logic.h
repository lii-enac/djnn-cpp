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
 *
 */

#pragma once

#include "operators.h"

namespace djnn
{
  using namespace std;

  class And : public BinaryOperator
  {
  private:
    class AndAction : public BinaryOperatorAction
    {
    public:
      AndAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
                 AbstractProperty* result) :
          BinaryOperatorAction (parent, name, left, right, result) { Process::finalize_construction (); }
      virtual ~AndAction () {}
      void impl_activate ()
      {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        _result->set_value (((BoolProperty*) _left)->get_value () && ((BoolProperty*) _right)->get_value (), true);
      }
      void impl_deactivate () {}
    };
  public:
    And (Process *p, const string &n, int l_val, int r_val);
    virtual ~And () {}
    void serialize (const string& type) override;
  };

  class Or : public BinaryOperator
  {
  private:
    class OrAction : public BinaryOperatorAction
    {
    public:
      OrAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
                AbstractProperty* result) :
                  BinaryOperatorAction (parent, name, left, right, result) { Process::finalize_construction (); }
      virtual ~OrAction () {}
      void impl_activate ()
      {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        _result->set_value (((BoolProperty*) _left)->get_value () != ((BoolProperty*) _right)->get_value (), true);
      }
      void impl_deactivate () {}
    };
  public:
    Or (Process *p, const string &n, int l_val, int r_val);
    virtual ~Or () {}
    void serialize (const string& type) override;
  };

  class XOr : public BinaryOperator
  {
  private:
    class XOrAction : public BinaryOperatorAction
    {
    public:
      XOrAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
                 AbstractProperty* result) :
                   BinaryOperatorAction (parent, name, left, right, result) { Process::finalize_construction (); }
      virtual ~XOrAction () {}
      void impl_activate ()
      {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        _result->set_value (((BoolProperty*) _left)->get_value () != ((BoolProperty*) _right)->get_value (), true);
      }
      void impl_deactivate () {}
    };
  public:
    XOr (Process *p, const string &n, int l_val, int r_val);
    virtual ~XOr () {}
    void serialize (const string& type) override;
  };

  class Not : public UnaryOperator
  {
  private:
    class NotAction : public UnaryOperatorAction
    {
    public:
      NotAction (Process *p, const string &n, AbstractProperty* input, AbstractProperty* output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (); }
      virtual ~NotAction () {}
      void impl_activate () {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
                  return;
        _output->set_value (!((BoolProperty*) _input)->get_value (), true);
      }
      void impl_deactivate () {}
    };
  public:
    Not (Process *p, const string &n, int in_val);
    virtual ~Not () {}
    void serialize (const string& type) override;
  };
}
