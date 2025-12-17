/*

function wrapper
example usage in smala:

%{
bool
my_contains(const string& s, const string& to_find, int i)
{
  return s.find(to_find);
}
%}

String toto("")

TFuncWrapper wcontains(my_contains)
toto =:> wcontains.input1
"ot" =:> wcontains.input2
wcontains.output =:> tp.input

*/


// c++20
#pragma once

#include <tuple>
#include <utility>
#include <type_traits>
#include <vector>
#include <string>
#include <functional>

#include "core/property/property_trait.h"

namespace djnn {

// ============================================================
//  function_traits (C++20)
// ============================================================
template<typename T>
struct function_traits;

// fonction libre
template<typename R, typename... Args>
struct function_traits<R(*)(Args...)> {
  using return_type = R;
  using args_tuple  = std::tuple<Args...>;
};

// lambda / fonctor
template<typename L>
struct function_traits {
private:
  using call_t = function_traits<decltype(&L::operator())>;
public:
  using return_type = typename call_t::return_type;
  using args_tuple  = typename call_t::args_tuple;
};

// ============================================================
//  Property associée à un argument
// ============================================================
template<typename Arg>
using prop_for_arg_t =
  typename PropertyTrait<std::remove_cvref_t<Arg>>::property_type;

// ============================================================
//  ActionParentMethod (inchangé)
// ============================================================
// template <typename PARENT, void (PARENT::*Method)()>
// class ActionParentMethod : public Action {
// public:
//   ActionParentMethod(CoreProcess* parent,
//                      const djnnstl::string& name)
//     : Action(parent, name)
//   {
//     finalize_construction(parent, name);
//   }

//   void impl_activate() override {
//     (static_cast<PARENT*>(get_parent())->*Method)();
//   }
// };

// ============================================================
//  TFuncWrapper — C++20
// ============================================================
template<typename FUNC>
class TFuncWrapper : public FatProcess {
private:
  using Traits     = function_traits<std::remove_cvref_t<FUNC>>;
  using ReturnType = typename Traits::return_type;
  using ArgsTuple  = typename Traits::args_tuple;

  static constexpr std::size_t N = std::tuple_size_v<ArgsTuple>;

  template<std::size_t I>
  using Arg_t =
    std::remove_cvref_t<std::tuple_element_t<I, ArgsTuple>>;

  template<std::size_t I>
  using Prop_t = prop_for_arg_t<Arg_t<I>>;

  // ---- do_action doit être déclaré avant WrapperAction
  void do_action();

  using WrapperAction =
    ActionParentMethod<TFuncWrapper, &TFuncWrapper::do_action>;

  // ==========================================================
  //  Tuple de pointeurs vers propriétés internes
  // ==========================================================
  template<typename Seq>
  struct PropsFromSeq;

  template<std::size_t... I>
  struct PropsFromSeq<std::index_sequence<I...>> {
    using type = std::tuple<Prop_t<I>*...>;
  };

  using Props =
    typename PropsFromSeq<std::make_index_sequence<N>>::type;

  // ==========================================================
  //  Membres
  // ==========================================================
  FUNC _func;
  Props _props;
  typename PropertyTrait<ReturnType>::property_type _output;
  WrapperAction _action;
  std::vector<Binding*> _bindings;

  // ==========================================================
  //  Création / destruction des propriétés internes
  // ==========================================================
  template<std::size_t... I>
  Props make_props(const djnnstl::string& name,
                   std::index_sequence<I...>) {
    return Props{
      new Prop_t<I>(
        this,
        "input" + std::to_string(I),
        Arg_t<I>{}
      )...
    };
  }

  template<std::size_t... I>
  void delete_props(std::index_sequence<I...>) {
    (delete std::get<I>(_props), ...);
  }

  // ==========================================================
  //  Bindings internes (input → action)
  // ==========================================================
  template<std::size_t I>
  void create_binding() {
    _bindings.push_back(
      new Binding(
        this,
        "bind_input" + std::to_string(I),
        std::get<I>(_props),
        &_action
      )
    );
  }

  template<std::size_t... I>
  void create_bindings(std::index_sequence<I...>) {
    (create_binding<I>(), ...);
  }

  void activate_bindings(bool on) {
    for (Binding* b : _bindings) {
      if (on) b->activate();
      else    b->deactivate();
    }
  }

  void delete_bindings() {
    for (Binding* b : _bindings) delete b;
  }

  // ==========================================================
  //  Appel de la fonction
  // ==========================================================
  ReturnType call_func() {
    if constexpr (N == 0) {
      return std::invoke(_func);
    } else {
      return std::apply(
        [this](auto*... p) {
          return std::invoke(_func, p->get_value()...);
        },
        _props
      );
    }
  }

public:
  // ==========================================================
  //  Construction / destruction
  // ==========================================================
  TFuncWrapper(CoreProcess* parent,
               const djnnstl::string& name,
               FUNC func)
    : FatProcess(name)
    , _func(func)
    , _props(make_props(name, std::make_index_sequence<N>{}))
    , _output(this, "output", ReturnType{})
    , _action(this, "action")
  {
    if constexpr (N > 0)
      create_bindings(std::make_index_sequence<N>{});

    finalize_construction(parent, name);
  }

  ~TFuncWrapper() override {
    delete_bindings();
    delete_props(std::make_index_sequence<N>{});
  }

protected:
  void impl_activate() override {
    activate_bindings(true);
  }

  void impl_deactivate() override {
    activate_bindings(false);
  }
};

// ============================================================
//  do_action — définition hors classe
// ============================================================
template<typename FUNC>
void TFuncWrapper<FUNC>::do_action() {
  _output.set_value(call_func(), true);
}

}

// c++23
// #pragma once

// #include <tuple>
// #include <utility>
// #include <type_traits>
// #include <vector>
// #include <string>
// #include <functional>

// namespace djnn {

// // ============================================================
// //  function_traits
// // ============================================================
// template<typename T>
// struct function_traits;

// // fonction libre
// template<typename R, typename... Args>
// struct function_traits<R(*)(Args...)> {
//   using return_type = R;
//   using args_tuple  = std::tuple<Args...>;
// };

// // lambda / callable
// template<typename L>
// struct function_traits {
// private:
//   using call_t = function_traits<decltype(&L::operator())>;
// public:
//   using return_type = typename call_t::return_type;
//   using args_tuple  = typename call_t::args_tuple;
// };

// // ============================================================
// //  Property associée à un argument
// // ============================================================
// template<typename Arg>
// using prop_for_arg_t =
//   typename PropertyTrait<std::remove_cvref_t<Arg>>::property_type;

// // ============================================================
// //  ActionParentMethod (inchangé)
// // ============================================================
// // template <typename PARENT, void (PARENT::*Method)()>
// // class ActionParentMethod : public Action {
// // public:
// //   ActionParentMethod (CoreProcess* parent, const djnnstl::string& name)
// //     : Action(parent, name)
// //   {
// //     finalize_construction(parent, name);
// //   }

// //   void impl_activate () override {
// //     (dynamic_cast<PARENT*>(get_parent())->*Method)();
// //   }
// // };

// // ============================================================
// //  TFuncWrapper
// // ============================================================
// template<typename FUNC>
// class TFuncWrapper : public FatProcess {
// private:
//   using Traits     = function_traits<std::remove_cvref_t<FUNC>>;
//   using ReturnType = typename Traits::return_type;
//   using ArgsTuple  = typename Traits::args_tuple;

//   static constexpr std::size_t N = std::tuple_size_v<ArgsTuple>;

//   template<std::size_t I>
//   using Arg_t = std::remove_cvref_t<std::tuple_element_t<I, ArgsTuple>>;

//   template<std::size_t I>
//   using Prop_t = prop_for_arg_t<Arg_t<I>>;

//   // ---- do_action doit être visible ici
//   void do_action();

//   using WrapperAction =
//     ActionParentMethod<TFuncWrapper, &TFuncWrapper::do_action>;

//   // ==========================================================
//   //  Tuple de pointeurs vers propriétés internes
//   // ==========================================================
//   template<typename Seq>
//   struct PropsFromSeq;

//   template<std::size_t... I>
//   struct PropsFromSeq<std::index_sequence<I...>> {
//     using type = std::tuple<Prop_t<I>*...>;
//   };

//   using Props =
//     typename PropsFromSeq<std::make_index_sequence<N>>::type;

//   // ==========================================================
//   //  Membres
//   // ==========================================================
//   FUNC _func;
//   Props _props;
//   typename PropertyTrait<ReturnType>::property_type _output;
//   WrapperAction _action;
//   std::vector<Binding*> _bindings;

//   // ==========================================================
//   //  Création / destruction des propriétés internes
//   // ==========================================================
//   template<std::size_t... I>
//   Props make_props(const djnnstl::string& name,
//                    std::index_sequence<I...>) {
//     return Props{
//       new Prop_t<I>(
//         this,
//         "input" + std::to_string(I),
//         Arg_t<I>{}
//       )...
//     };
//   }

//   template<std::size_t... I>
//   void delete_props(std::index_sequence<I...>) {
//     (delete std::get<I>(_props), ...);
//   }

//   // ==========================================================
//   //  Bindings internes (input → action)
//   // ==========================================================
//   template<std::size_t I>
//   void create_binding() {
//     _bindings.push_back(
//       new Binding(
//         this,
//         "bind_input" + std::to_string(I),
//         std::get<I>(_props),
//         &_action
//       )
//     );
//   }

//   template<std::size_t... I>
//   void create_bindings(std::index_sequence<I...>) {
//     (create_binding<I>(), ...);
//   }

//   void activate_bindings(bool on) {
//     for (auto* b : _bindings)
//       on ? b->activate() : b->deactivate();
//   }

//   void delete_bindings() {
//     for (auto* b : _bindings) delete b;
//   }

//   // ==========================================================
//   //  Appel de la fonction
//   // ==========================================================
//   ReturnType call_func() {
//     if constexpr (N == 0) {
//       return std::invoke(_func);
//     } else {
//       return std::apply(
//         [this](auto*... p) {
//           return std::invoke(_func, p->get_value()...);
//         },
//         _props
//       );
//     }
//   }

// public:
//   // ==========================================================
//   //  Constructeur / destructeur
//   // ==========================================================
//   TFuncWrapper(CoreProcess* parent,
//                const djnnstl::string& name,
//                FUNC func)
//     : FatProcess(name)
//     , _func(func)
//     , _props(make_props(name, std::make_index_sequence<N>{}))
//     , _output(this, "output", ReturnType{})
//     , _action(this, "action")
//   {
//     if constexpr (N > 0)
//       create_bindings(std::make_index_sequence<N>{});

//     finalize_construction(parent, name);
//   }

//   ~TFuncWrapper() override {
//     delete_bindings();
//     delete_props(std::make_index_sequence<N>{});
//   }

// protected:
//   void impl_activate() override {
//     activate_bindings(true);
//   }

//   void impl_deactivate() override {
//     activate_bindings(false);
//   }
// };

// // ============================================================
// //  do_action (définition hors classe)
// // ============================================================
// template<typename FUNC>
// void TFuncWrapper<FUNC>::do_action() {
//   _output.set_value(call_func(), true);
// }

// }
