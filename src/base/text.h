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
#include "core/tree/text_property.h"

#include <iostream>

namespace djnn
{
  using namespace std;

  class TextPrinter : public Process
  {
  private:
    class TextPrinterAction : public Action
    {
    public:
      TextPrinterAction (Process *p, const string &n, TextProperty* input) : Action (p, n), _input (input) {}
      virtual ~TextPrinterAction () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      TextProperty* _input;
    };
  public:
    TextPrinter ();
    TextPrinter (Process *p, const string &n);
    void impl_activate () override { c_input.enable(); };
    void impl_deactivate () override { c_input.disable (); };
    virtual ~TextPrinter ();
    void serialize (const string& type) override;
  private:
    void init ();
    TextProperty _input;
    TextPrinterAction _action;
    Coupling c_input;
  };

#define NEW_TEXT 0

#if NEW_TEXT

  class TextCatenatorAction;
  class TextComparatorAction;
  template <> const char name_info<TextCatenatorAction>::serialize [];
  template <> const char name_info<TextComparatorAction>::serialize [];

  template <typename Action>
  class TextBinaryOperator : public Process
  {
  public:
    TextBinaryOperator (Process *p, const string &n, const string& l_val, const string& r_val)
    : Process (n),
      _left(this, "left", l_val),
      _right(this, "right", r_val),
      _result(this, "result", l_val+r_val),
      _action(this, "action", *this),
      _c_left(&_left, ACTIVATION, &_action, ACTIVATION),
      _c_right(&_right, ACTIVATION, &_action, ACTIVATION)
    {
      init_binary_couplings(_left, _right, _result, _action, _c_left, _c_right);
      Process::finalize_construction (p);
    }
    TextBinaryOperator (Process *p, const string &n)
    : Process (n),
      _left(this, "left", ""),
      _right(this, "right", ""),
      _result(this, "result", ""),
      _action(this, "action", *this),
      _c_left(&_left, ACTIVATION, &_action, ACTIVATION),
      _c_right(&_right, ACTIVATION, &_action, ACTIVATION)
    {
      init_binary_couplings(_left, _right, _result, _action, _c_left, _c_right);
      Process::finalize_construction (p);
    }
    virtual ~TextBinaryOperator () {
      uninit_binary_couplings(this, _left, _right, _result, _action, _c_left, _c_right);
    }
    void impl_activate () override { _c_left.enable(); _c_right.enable (); _action.activate (); }
    void impl_deactivate () override { _c_left.disable (); _c_right.disable (); _action.deactivate ();};
    void serialize (const string& type) override {
      AbstractSerializer::pre_serialize(this, type);
      AbstractSerializer::serializer->start ("base:" + string(name_info<Action>::serialize));
      AbstractSerializer::serializer->text_attribute ("id", get_name ());
      AbstractSerializer::serializer->cpptype_attribute ("left", _left.get_value ());
      AbstractSerializer::serializer->cpptype_attribute ("right", _right.get_value ());
      AbstractSerializer::serializer->end ();
      AbstractSerializer::post_serialize(this);
    }

  protected:
    void set_parent (Process* p) override {
      // in case of re-parenting remove edge dependency in graph
      if (get_parent ()) {
         remove_state_dependency (get_parent (), &_action);
      }
      add_state_dependency (p, &_action);
      Process::set_parent (p);
    }
  public:
    TextProperty _left;
    TextProperty _right;
    TextProperty _result;
    Action _action;
    Coupling _c_left, _c_right;
  };

  class TextCatenatorAction : public Action
    {
    public:
      TextCatenatorAction (Process* parent, const string &name, TextBinaryOperator<TextCatenatorAction>& tbo) :
      Action (parent, name), _tbo(tbo) { finalize_construction(parent); }
      virtual ~TextCatenatorAction () {}
      void impl_activate () override
      {
        const string& head = _tbo._left.get_value ();
        const string& tail = _tbo._right.get_value ();
        string out = head + tail;
        _tbo._result.set_value (out, true);
      }
      void impl_deactivate () override {}
    private:
      TextBinaryOperator<TextCatenatorAction>& _tbo;
    };

  class TextComparatorAction : public Action
    {
    public:
      TextComparatorAction (Process* parent, const string &name, TextBinaryOperator<TextComparatorAction>& tbo) :
      Action (parent, name), _tbo(tbo) { finalize_construction(parent); }
      virtual ~TextComparatorAction () {}
      void impl_activate ()
      {
        const string& left = _tbo._left.get_value ();
        const string& right = _tbo._right.get_value ();
        _tbo._result.set_value (left.compare (right) == 0, true);
      }
      void impl_deactivate () {}
      private:
      TextBinaryOperator<TextComparatorAction>& _tbo;
    };

  typedef TextBinaryOperator<TextCatenatorAction> TextCatenator;
  typedef TextBinaryOperator<TextComparatorAction> TextComparator;


#else

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
      void impl_activate () override
      {
        string head = ((TextProperty*)_left)->get_value ();
        string tail = ((TextProperty*)_right)->get_value ();
        string out = head + tail;
        _result->set_value (out, true);
      }
      void impl_deactivate () override {}
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
      void impl_activate ()
      {
        string left = ((TextProperty*)_left)->get_value ();
        string right = ((TextProperty*)_right)->get_value ();
        _result->set_value (left.compare (right) == 0, true);
      }
      void impl_deactivate () {}
    };
  public:
    TextComparator (Process *p, const string &name, const string &left, const string &right);
    virtual ~TextComparator () {}
    void serialize (const string& type) override;
  };
#endif

  class TextAccumulator : public Process
  {
  private:
    class AccumulateAction : public Action {
      public:
        AccumulateAction (Process *p, const string &n, TextAccumulator& ta) : Action (p, n), _ta (ta) {}
        virtual ~AccumulateAction () {}
        void impl_activate () override {
          std::string new_state = _ta._state.get_value () + _ta._input.get_value ();
          _ta._state.set_value (new_state, true);
        }
        void impl_deactivate () override {}
      private:
        TextAccumulator& _ta;
    };
    class DeleteAction : public Action {
    public:
      DeleteAction (Process *p, const string &n, TextAccumulator& ta) : Action (p, n), _ta (ta) {}
      virtual ~DeleteAction () {}
      void impl_activate () override {
        int sz = _ta._state.get_value ().size ();
        std::string new_state = "";
        if (sz > 1)
          new_state = _ta._state.get_value ().substr (0, sz - 1);
        _ta._state.set_value (new_state, true);
      }
      void impl_deactivate () override {}
    private:
      TextAccumulator& _ta;
    };
  public:
    TextAccumulator (Process *p, const string &n, const string &init = "");
    virtual ~TextAccumulator ();
    void impl_activate () override;
    void impl_deactivate () override;
    void serialize (const string &type) override;
  private:
    void set_parent (Process* p) override;
    TextProperty _input, _state;
    Spike _del;
    AccumulateAction _acc_action;
    DeleteAction _del_action;
    Coupling _c_acc, _c_del;
  };

  class DoubleFormatter : public Process
  {
  private:
    class DoubleFormatterAction : public Action
    {
    public:
      DoubleFormatterAction (Process* p, const string &n, DoubleFormatter& df) : Action (p, n), _df(df) { Process::finalize_construction (p); }
      virtual ~DoubleFormatterAction () {}
      void impl_activate () override {
        int decimal = _df._decimal.get_value ();
        double value = _df._input.get_value();
        string res = to_string (value);
        std::size_t found = res.find('.');
        if (found != string::npos) {
          if (decimal == 0) {
            res = res.substr (0, found);
          } else {
          res = res.substr (0, found + decimal + 1);
          }
        }
        _df._output.set_value (res, true);
      }
      void impl_deactivate () override {}
    private:
      DoubleFormatter& _df;
    };
  public:
    DoubleFormatter (double initial, int decimal);
    DoubleFormatter (Process* parent, const string &name, double initial, int decimal);
    virtual ~DoubleFormatter ();
    void impl_activate () override;
    void impl_deactivate () override;
    void serialize (const string& type) override;
  private:
    void set_parent (Process* p) override;
    void init (double initial, int decimal);
    DoubleProperty _input;
    IntProperty _decimal;
    TextProperty _output;
    DoubleFormatterAction _action;
    Coupling _c_input, _c_decimal;
  };
}
