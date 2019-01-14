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
#include "../core/tree/text_property.h"
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
      TextPrinterAction (Process *p, const string &n, TextProperty* input) : Process(p, n), _input (input) {}
      virtual ~TextPrinterAction () {}
      void activate () override;
      void deactivate () override {}
    private:
      TextProperty* _input;
    };
  public:
    TextPrinter ();
    TextPrinter (Process *p, const string &n);
    void activate () override { c_input->enable(); };
    void deactivate () override { c_input->disable (); };
    virtual ~TextPrinter ();
    void serialize (const string& type) override;
  private:
    void init ();
    TextProperty* _input;
    Process* _action;
    Coupling* c_input;
  };

  class TextCatenator : public BinaryOperator
  {
  private:
    class TextCatenatorAction : public BinaryOperatorAction
    {
    public:
      TextCatenatorAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
                           AbstractProperty* result) :
		  BinaryOperatorAction (parent, name, left, right, result) {}
      virtual ~TextCatenatorAction () {}
      void activate () override
      {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        string head = ((TextProperty*)_left)->get_value ();
        string tail = ((TextProperty*)_right)->get_value ();
        string out = head + tail;
        _result->set_value (out, true);
      }
      void deactivate () override {}
    };
  public:
    TextCatenator (Process *p, const string &name);
    virtual ~TextCatenator () {}
    void serialize (const string& type) override;
  };

  class TextComparator : public BinaryOperator
  {
  private:
    class TextComparatorAction : public BinaryOperatorAction
    {
    public:
      TextComparatorAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
                           AbstractProperty* result) :
      BinaryOperatorAction (parent, name, left, right, result) {}
      virtual ~TextComparatorAction () {}
      void activate ()
      {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        string left = ((TextProperty*)_left)->get_value ();
        string right = ((TextProperty*)_right)->get_value ();
        _result->set_value (left.compare (right) == 0, true);
      }
      void deactivate () {}
    };
  public:
    TextComparator (Process *p, const string &name, const string &left, const string &right);
    virtual ~TextComparator () {}
    void serialize (const string& type) override;
  };

  class TextAccumulator : public Process
  {
  private:
    class AccumulateAction : public Process {
      public:
        AccumulateAction (Process *p, const string &n, TextProperty *input, TextProperty *state) : Process (p, n), _input (input), _state (state) {}
        virtual ~AccumulateAction () {}
        void activate () override {
          std::string new_state = _state->get_value () + _input->get_value ();
          _state->set_value (new_state, true);
        }
        void deactivate () override {}
      private:
        TextProperty *_input, *_state;
    };
    class DeleteAction : public Process {
    public:
      DeleteAction (Process *p, const string &n, TextProperty *state) : Process (p, n), _state (state) {}
      virtual ~DeleteAction () {}
      void activate () override {
        int sz = _state->get_value ().size ();
        std::string new_state = "";
        if (sz > 1)
          new_state = _state->get_value ().substr (0, sz - 1);
        _state->set_value (new_state, true);
      }
      void deactivate () override {}
    private:
      TextProperty *_state;
    };
  public:
    TextAccumulator (Process *p, const string &n, const string &init = "");
    virtual ~TextAccumulator ();
    void activate () override;
    void deactivate () override;
    void serialize (const string &type) override;
  private:
    TextProperty *_input, *_state;
    Spike *_del;
    AccumulateAction *_acc_action;
    DeleteAction *_del_action;
    Coupling *_c_acc, *_c_del;
  };

  class DoubleFormatter : public Process
  {
  private:
    class DoubleFormatterAction : public Process
    {
    public:
      DoubleFormatterAction (Process* p, const string &n, DoubleProperty* in, IntProperty* dec, TextProperty* out) : Process (p, n), _input (in), _decimal (dec), _output (out) { Process::finalize (); }
      virtual ~DoubleFormatterAction () {}
      void activate () override {
        int decimal = _decimal->get_value ();
        double value = _input->get_value();
        string res = to_string (value);
        std::size_t found = res.find('.');
        if (found != string::npos) {
          if (decimal == 0) {
            res = res.substr (0, found);
          } else {
          res = res.substr (0, found + decimal + 1);
          }
        }
        _output->set_value (res, true);
      }
      void deactivate () override {}
    private:
      DoubleProperty* _input;
      IntProperty* _decimal;
      TextProperty* _output;
    };
  public:
    DoubleFormatter (double initial, int decimal);
    DoubleFormatter (Process* parent, const string &name, double initial, int decimal);
    virtual ~DoubleFormatter ();
    void activate () override;
    void deactivate () override;
    void serialize (const string& type) override;
  private:
    void init (double initial, int decimal);
    DoubleProperty* _input;
    IntProperty* _decimal;
    TextProperty* _output;
    Coupling *_c_input, *_c_decimal;
    Process *_action;
  };
}
