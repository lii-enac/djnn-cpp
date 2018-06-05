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

#include "operators.h"
#include <iostream>

namespace djnn
{
  using namespace std;

  class And : public BinaryOperator
  {
  private:
    class AndAction : public BinaryOperatorAction
    {
    public:
      AndAction (Process* parent, const string &name, shared_ptr<AbstractProperty> left, shared_ptr<AbstractProperty> right,
                 shared_ptr<AbstractProperty> result) :
          BinaryOperatorAction (parent, name, left, right, result) { Process::finalize (); }
      virtual ~AndAction () {}
      void activate ()
      {
        if (_parent->get_state () > activated)
          return;
        _result.get()->set_value (((BoolProperty*) _left.get())->get_value () && ((BoolProperty*) _right.get())->get_value (), true);
      }
      void deactivate () {}
    };
  public:
    And (Process *p, const string &n, int l_val, int r_val);
    virtual ~And () {}
  };

  class Or : public BinaryOperator
  {
  private:
    class OrAction : public BinaryOperatorAction
    {
    public:
      OrAction (Process* parent, const string &name, shared_ptr<AbstractProperty> left, shared_ptr<AbstractProperty> right,
                shared_ptr<AbstractProperty> result) :
                  BinaryOperatorAction (parent, name, left, right, result) { Process::finalize (); }
      virtual ~OrAction () {}
      void activate ()
      {
        if (_parent->get_state () > activated)
          return;
        _result.get()->set_value (((BoolProperty*) _left.get())->get_value () != ((BoolProperty*) _right.get())->get_value (), true);
      }
      void deactivate () {}
    };
  public:
    Or (Process *p, const string &n, int l_val, int r_val);
    virtual ~Or () {}
  };

  class XOr : public BinaryOperator
  {
  private:
    class XOrAction : public BinaryOperatorAction
    {
    public:
      XOrAction (Process* parent, const string &name, shared_ptr<AbstractProperty> left, shared_ptr<AbstractProperty> right,
                 shared_ptr<AbstractProperty> result) :
                   BinaryOperatorAction (parent, name, left, right, result) { Process::finalize (); }
      virtual ~XOrAction () {}
      void activate ()
      {
        if (_parent->get_state () > activated)
          return;
        _result.get()->set_value (((BoolProperty*) _left.get())->get_value () != ((BoolProperty*) _right.get())->get_value (), true);
      }
      void deactivate () {}
    };
  public:
    XOr (Process *p, const string &n, int l_val, int r_val);
    virtual ~XOr () {}
  };

  class Not : public UnaryOperator
  {
  private:
    class NotAction : public UnaryOperatorAction
    {
    public:
      NotAction (Process *p, const string &n, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize (); }
      virtual ~NotAction () {}
      void activate () {
        if (_parent->get_state () > activated)
                  return;
        _output.get()->set_value (!((BoolProperty*) _input.get())->get_value (), true);
      }
      void deactivate () {}
    };
  public:
    Not (Process *p, const string &n, int in_val);
    virtual ~Not () {}
  };
}
