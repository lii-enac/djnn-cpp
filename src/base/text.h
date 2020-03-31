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

//#include <iostream>
#include <regex>

namespace djnn
{

  class TextPrinter : public Process
  {
  private:
    class TextPrinterAction : public Action
    {
    public:
      TextPrinterAction (Process *parent, const std::string& name, TextProperty* input) : Action (parent, name), _input (input) {}
      virtual ~TextPrinterAction () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      TextProperty* _input;
    };
  public:
    TextPrinter (Process *parent, const std::string& name);
    void impl_activate () override { c_input.enable(); };
    void impl_deactivate () override { c_input.disable (); };
    virtual ~TextPrinter ();
#ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const std::string& format) override;
#endif
  private:
    void init ();
    TextProperty _input;
    TextPrinterAction _action;
    Coupling c_input;
  };

  class TextCatenatorAction;
  class TextComparatorAction;
  template <> const char name_info<TextCatenatorAction>::left [];
  template <> const char name_info<TextCatenatorAction>::right [];
  template <> const char name_info<TextCatenatorAction>::serialize [];

  template <> const char name_info<TextComparatorAction>::left [];
  template <> const char name_info<TextComparatorAction>::right [];
  template <> const char name_info<TextComparatorAction>::serialize [];

  template <typename Action, typename Result>
  class TextBinaryOperator : public Process
  {
  public:
    TextBinaryOperator (Process *parent, const std::string& name, const std::string& l_val, const std::string& r_val)
    : Process (name),
      _left(this, name_info<Action>::left, l_val),
      _right(this, name_info<Action>::right, r_val),
      _result(this, "output", Action::perform(l_val,r_val)),
      _action(this, "action", *this),
      _c_left(&_left, ACTIVATION, &_action, ACTIVATION),
      _c_right(&_right, ACTIVATION, &_action, ACTIVATION)
    {
      init_binary_couplings(_left, _right, _result, _action, _c_left, _c_right);
      Process::finalize_construction (parent, name);
    }
    TextBinaryOperator (Process *parent, const std::string& name)
    : Process (name),
      _left(this, name_info<Action>::left, ""),
      _right(this, name_info<Action>::right, ""),
      _result(this, "output", Action::default_value()),
      _action(this, "action", *this),
      _c_left(&_left, ACTIVATION, &_action, ACTIVATION),
      _c_right(&_right, ACTIVATION, &_action, ACTIVATION)
    {
      init_binary_couplings(_left, _right, _result, _action, _c_left, _c_right);
      Process::finalize_construction (parent, name);
    }
    virtual ~TextBinaryOperator () {
      uninit_binary_couplings(this, _left, _right, _result, _action, _c_left, _c_right);
    }
    void impl_activate () override { _c_left.enable(); _c_right.enable (); _action.activate (); }
    void impl_deactivate () override { _c_left.disable (); _c_right.disable (); _action.deactivate (); }

#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& type) override {
      AbstractSerializer::pre_serialize(this, type);
      AbstractSerializer::serializer->start ("base:" + std::string(name_info<Action>::serialize));
      AbstractSerializer::serializer->text_attribute ("id", get_name ());
      AbstractSerializer::serializer->cpptype_attribute ("left", _left.get_value ());
      AbstractSerializer::serializer->cpptype_attribute ("right", _right.get_value ());
      AbstractSerializer::serializer->end ();
      AbstractSerializer::post_serialize(this);
    }
#endif
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
    Result _result;
    Action _action;
    Coupling _c_left, _c_right;
  };

  class TextCatenatorAction : public Action
    {
    public:
      TextCatenatorAction (Process* parent, const std::string& name, TextBinaryOperator<TextCatenatorAction, TextProperty>& tbo) :
      Action (parent, name), _tbo(tbo) { finalize_construction(parent, name); }
      virtual ~TextCatenatorAction () {}
      void impl_activate () override
      {
        const std::string& head = _tbo._left.get_value ();
        const std::string& tail = _tbo._right.get_value ();
        //string out = head + tail;
        //_tbo._result.set_value (out, true);
        _tbo._result.set_value (perform(head, tail), true);
      }
      void impl_deactivate () override {}
      static std::string perform(const std::string& l, const std::string& r) { return l+r; }
      static std::string default_value () {return "";}

    private:
      TextBinaryOperator<TextCatenatorAction, TextProperty>& _tbo;
    };

  class TextComparatorAction : public Action
    {
    public:
      TextComparatorAction (Process* parent, const std::string& name, TextBinaryOperator<TextComparatorAction, BoolProperty>& tbo) :
      Action (parent, name), _tbo(tbo) { finalize_construction(parent, name); }
      virtual ~TextComparatorAction () {}
      void impl_activate ()
      {
        const std::string& left = _tbo._left.get_value ();
        const std::string& right = _tbo._right.get_value ();
        //_tbo._result.set_value (left.compare (right) == 0, true);
        _tbo._result.set_value (perform(left, right), true);
      }
      void impl_deactivate () {}
      static bool perform(const std::string& l, const std::string& r) { return l.compare(r) ==0; }
      static bool default_value () {return false;}

      private:
      TextBinaryOperator<TextComparatorAction, BoolProperty>& _tbo;
    };

  typedef TextBinaryOperator<TextCatenatorAction, TextProperty> TextCatenator;
  typedef TextBinaryOperator<TextComparatorAction, BoolProperty> TextComparator;


  class TextAccumulator : public Process
  {
  private:
    class AccumulateAction : public Action {
      public:
        AccumulateAction (Process *parent, const std::string& name, TextAccumulator& ta) : Action (parent, name), _ta (ta) {}
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
      DeleteAction (Process *parent, const std::string& name, TextAccumulator& ta) : Action (parent, name), _ta (ta) {}
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
    TextAccumulator (Process *parent, const std::string& name, const std::string& init = "");
    virtual ~TextAccumulator ();
    void impl_activate () override;
    void impl_deactivate () override;
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const std::string& format) override;
#endif
  private:
    void set_parent (Process* p) override;
    TextProperty _input, _state;
    Spike _del;
    AccumulateAction _acc_action;
    DeleteAction _del_action;
    Coupling _c_acc, _c_del;
  };

  class Regex : public Process
  {
  private:
    class RegexAction : public Action {
      public:
        RegexAction (Process *parent, const std::string& name, Regex& reg) : Action (parent, name), _reg (reg) {}
        virtual ~RegexAction () {}
        void impl_activate () override;
        void impl_deactivate () override {}
      private:
        Regex& _reg;
    };
  public:
    Regex (Process *parent, const std::string& name, const std::string& Regex = "");
    virtual ~Regex ();
    void impl_activate () override;
    void impl_deactivate () override;

 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const std::string& format) override;
#endif
  protected:
    Process* find_child (const std::string&) override;
  private:
    void set_parent (Process* p) override;
    TextProperty _input;
    std::string _init;
    std::regex _regex;
    RegexAction _reg_action;
    Coupling _c_reg;
    std::map <int, TextProperty*> _in_map;
  };

  class DoubleFormatter : public Process
  {
  private:
    class DoubleFormatterAction : public Action
    {
    public:
      DoubleFormatterAction (Process* parent, const std::string& name, DoubleFormatter& df) : Action (parent, name), _df(df) { Process::finalize_construction (parent, name); }
      virtual ~DoubleFormatterAction () {}
      void impl_activate () override {
        int decimal = _df._decimal.get_value ();
        double value = _df._input.get_value();
        std::string res = std::to_string (value);
        std::size_t found = res.find('.');
        if (found != std::string::npos) {
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
    DoubleFormatter (Process* parent, const std::string& name, double initial, int decimal);
    virtual ~DoubleFormatter ();
    void impl_activate () override;
    void impl_deactivate () override;
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const std::string& format) override;
#endif
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
