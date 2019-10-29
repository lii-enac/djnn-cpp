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

#if NEW_OP

  typedef NewBinaryOperatorAction<BoolProperty, BoolProperty, BoolProperty, std::logical_and<double>, double, double> AndAction;
  typedef NewBinaryOperator      <BoolProperty, BoolProperty, BoolProperty, std::logical_and<double>, double, double> And;

  typedef NewBinaryOperatorAction<BoolProperty, BoolProperty, BoolProperty, std::logical_or<double>, double, double> OrAction;
  typedef NewBinaryOperator      <BoolProperty, BoolProperty, BoolProperty, std::logical_or<double>, double, double> Or;

  typedef NewBinaryOperatorAction<BoolProperty, BoolProperty, BoolProperty, std::not_equal_to<double>, double, double> XOrAction;
  typedef NewBinaryOperator      <BoolProperty, BoolProperty, BoolProperty, std::not_equal_to<double>, double, double> XOr;

#else

  class And : public BinaryOperator
  {
  private:
    class AndAction : public BinaryOperatorAction
    {
    public:
      AndAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
                 AbstractProperty* result) :
          BinaryOperatorAction (parent, name, left, right, result) { Process::finalize_construction (parent); }
      virtual ~AndAction () {}
      void impl_activate ()
      {
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
                  BinaryOperatorAction (parent, name, left, right, result) { Process::finalize_construction (parent); }
      virtual ~OrAction () {}
      void impl_activate ()
      {
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
                   BinaryOperatorAction (parent, name, left, right, result) { Process::finalize_construction (parent); }
      virtual ~XOrAction () {}
      void impl_activate ()
      {
        _result->set_value (((BoolProperty*) _left)->get_value () != ((BoolProperty*) _right)->get_value (), true);
      }
      void impl_deactivate () {}
    };
  public:
    XOr (Process *p, const string &n, int l_val, int r_val);
    virtual ~XOr () {}
    void serialize (const string& type) override;
  };
#endif

  class Not : public UnaryOperator
  {
  private:
    class NotAction : public UnaryOperatorAction
    {
    public:
      NotAction (Process *parent, const string &n, AbstractProperty* input, AbstractProperty* output) :
        UnaryOperatorAction (parent, n, input, output) { Process::finalize_construction (parent); }
      virtual ~NotAction () {}
      void impl_activate () {
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
