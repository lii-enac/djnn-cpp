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

  class TextPrinter : public Process
  {
  private:
    class TextPrinterAction : public Process
    {
    public:
      TextPrinterAction (Process *p, const string &n, shared_ptr<TextProperty> input) : Process(p, n), _input (input) {}
      virtual ~TextPrinterAction () {}
      void activate () override;
      void deactivate () override {}
    private:
      shared_ptr<TextProperty> _input;
    };
  public:
    TextPrinter ();
    TextPrinter (Process *p, const string &n);
    void activate () { c_input.get()->enable(); };
    void deactivate () { c_input.get()->disable (); };
    virtual ~TextPrinter ();
  private:
    void init ();
    shared_ptr<TextProperty> input;
    shared_ptr<Process> _action;
    unique_ptr<Coupling> c_input;
  };

  class TextCatenator : public BinaryOperator
  {
  private:
    class TextCatenatorAction : public BinaryOperatorAction
    {
    public:
      TextCatenatorAction (Process* parent, const string &name, shared_ptr<AbstractProperty> left, shared_ptr<AbstractProperty> right,
                           shared_ptr<AbstractProperty> result) :
		  BinaryOperatorAction (parent, name, left, right, result) {}
      virtual ~TextCatenatorAction () {}
      void activate ()
      {
        if (_parent->get_state () != activated)
          return;
        string head = ((TextProperty*)_left.get())->get_value ();
        string tail = ((TextProperty*)_right.get())->get_value ();
        string out = head + tail;
        _result.get()->set_value (out, true);
      }
      void deactivate () {}
    };
  public:
    TextCatenator (Process *p, const string &name);
    virtual ~TextCatenator () {}
  };

  class TextComparator : public BinaryOperator
  {
  private:
    class TextComparatorAction : public BinaryOperatorAction
    {
    public:
      TextComparatorAction (Process* parent, const string &name, shared_ptr<AbstractProperty> left, shared_ptr<AbstractProperty> right,
                           shared_ptr<AbstractProperty> result) :
      BinaryOperatorAction (parent, name, left, right, result) {}
      virtual ~TextComparatorAction () {}
      void activate ()
      {
        if (_parent->get_state () != activated)
          return;
        string left = ((TextProperty*)_left.get())->get_value ();
        string right = ((TextProperty*)_right.get())->get_value ();
        _result.get()->set_value (left.compare (right) == 0, true);
      }
      void deactivate () {}
    };
  public:
    TextComparator (Process *p, const string &name, const string &left, const string &right);
    virtual ~TextComparator () {}
  };

  class DoubleFormatter : public Process
  {
  private:
    class DoubleFormatterAction : public Process
    {
    public:
      DoubleFormatterAction (Process* p, const string &n, shared_ptr<DoubleProperty> in, shared_ptr<IntProperty> dec, shared_ptr<TextProperty> out) : Process (p, n), _input (in), _decimal (dec), _output (out) { Process::finalize (); }
      virtual ~DoubleFormatterAction () {}
      void activate () override {
        int decimal = _decimal.get()->get_value ();
        double value = _input.get()->get_value();
        string res = to_string (value);
        std::size_t found = res.find('.');
        if (found == string::npos) {
          _output.get()->set_value (res, true);
        }
        else {
          res = res.substr (0, found + decimal + 1);
          _output.get()->set_value (res, true);
        }
      }
      void deactivate () override {}
    private:
      shared_ptr<DoubleProperty> _input;
      shared_ptr<IntProperty> _decimal;
      shared_ptr<TextProperty> _output;
    };
  public:
    DoubleFormatter (double initial, int decimal);
    DoubleFormatter (Process* parent, const string &name, double initial, int decimal);
    virtual ~DoubleFormatter ();
    void activate () override;
    void deactivate () override;
  private:
    void init (double initial, int decimal);
    shared_ptr<DoubleProperty> _input;
    shared_ptr<IntProperty> _decimal;
    shared_ptr<TextProperty> _output;
    unique_ptr<Coupling> _c_input, _c_decimal;
    shared_ptr<Process> _action;
  };
}
