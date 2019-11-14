/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#pragma once

#include "operators.h"

namespace djnn
{
  using namespace std;

  template <> const char name_info<std::logical_and<bool>>::left[];
  template <> const char name_info<std::logical_and<bool>>::right[];
  template <> const char name_info<std::logical_and<bool>>::serialize[];
  typedef BinaryOperatorAction<BoolProperty, BoolProperty, BoolProperty, std::logical_and<bool>, bool, bool> AndAction;
  typedef BinaryOperator      <BoolProperty, BoolProperty, BoolProperty, std::logical_and<bool>, bool, bool> And;

  template <> const char name_info<std::logical_or<bool>>::left[];
  template <> const char name_info<std::logical_or<bool>>::right[];
  template <> const char name_info<std::logical_or<bool>>::serialize[];
  typedef BinaryOperatorAction<BoolProperty, BoolProperty, BoolProperty, std::logical_or<bool>, bool, bool> OrAction;
  typedef BinaryOperator      <BoolProperty, BoolProperty, BoolProperty, std::logical_or<bool>, bool, bool> Or;

  template <> const char name_info<std::not_equal_to<bool>>::left[];
  template <> const char name_info<std::not_equal_to<bool>>::right[];
  template <> const char name_info<std::not_equal_to<bool>>::serialize[];
  typedef BinaryOperatorAction<BoolProperty, BoolProperty, BoolProperty, std::not_equal_to<bool>, bool, bool> XOrAction;
  typedef BinaryOperator      <BoolProperty, BoolProperty, BoolProperty, std::not_equal_to<bool>, bool, bool> XOr;

  template <> const char name_info<std::logical_not<bool>>::left[];
  template <> const char name_info<std::logical_not<bool>>::right[];
  template <> const char name_info<std::logical_not<bool>>::serialize[];
  typedef UnaryOperatorAction<DoubleProperty, DoubleProperty,  std::logical_not<bool>, bool> NotAction;
  typedef UnaryOperator      <DoubleProperty, DoubleProperty,  std::logical_not<bool>, bool> Not;

}
