/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "algorithm.h"
#include "core/utils/error.h"
#include "core/serializer/serializer.h"
#include <iostream>
#include <algorithm>

namespace djnn
{

  Sorter::Sorter (Process *p, const std::string &name, Process *container, const std::string& spec) :
      Process (name),
      _ascending (BoolProperty (this, "ascending", true)),
      _spec (TextProperty (this, "spec", spec)),
      _action (SortAction (this, "sort")),
      _sort (Spike (this, "sort")),
      _c_sort_action (Coupling (&_sort, ACTIVATION, &_action, ACTIVATION, true)),
      _c_spec_action (Coupling (&_spec, ACTIVATION, &_action, ACTIVATION, true))
  {
    _container = dynamic_cast<Container*> (container);
    if (_container == nullptr)
      error (this, "Wrong argument: only containers can be sorted");
    Process::finalize_construction (p);
  }

  void
  Sorter::impl_activate ()
  {
    _c_sort_action.enable ();
    _c_spec_action.enable ();
  }

  void
  Sorter::impl_deactivate ()
  {
    _c_sort_action.disable ();
    _c_spec_action.disable ();
  }

  bool
  Sorter::compare (AbstractProperty* left, AbstractProperty* right)
  {
    if (left->get_prop_type () != right->get_prop_type()) {
      error (nullptr, "Cannot compare properties of different types");
      return false;
    }
    bool ascending = _ascending.get_value ();
    switch (left->get_prop_type()) {
      case Boolean:
      case Integer:
      case Double:
        if (ascending)
          return left->get_double_value () < right->get_double_value();
        else
          return left->get_double_value () > right->get_double_value();
      case String: {
        string s_left = left->get_string_value ();
        string s_right = right->get_string_value ();
        int sz = s_left.length() <= s_right.length() ? s_left.length() : s_right.length();
        for (int i = 0; i < sz; i++) {
          if (s_left[i] < s_right[i])
            return ascending;
          else if (s_left[i] > s_right[i])
            return !ascending;
        }
        if (s_left.length () <= s_right.length())
          return ascending;
        return !ascending;
      }
      case Reference:
        return true; // How should we sort references?
    }
    return false;
  }

  AbstractProperty*
  Sorter::get_and_check (int i) {
    Container::children_t children = _container->children ();
    AbstractProperty* p = dynamic_cast<AbstractProperty*>(children[i]->find_component (_spec.get_value()));
    if (p == nullptr) {
      error (this, "Unable to sort children: properties not found or with incorrect type");
      return nullptr;
    }
    return p;
  }

  int
  Sorter::partition (int i_first, int i_last) {
    AbstractProperty* pivot = get_and_check (i_first + (i_last - i_first)/2);
    int i = i_first - 1;
    int j = i_last + 1;
    while (true) {
      do {
        i = i + 1;
      } while (compare (get_and_check (i), pivot));
      do {
        j = j - 1;
      } while (compare (pivot, get_and_check (j)));
      if (i>=j)
        return j;
      _container->swap_children (i, j);
    }
  }

  void
  Sorter::quick_sort (int i_first, int i_last)
  {
    if (i_first < i_last) {
      int  i_pivot = partition (i_first, i_last);
      quick_sort (i_first, i_pivot);
      quick_sort (i_pivot + 1, i_last);
    }
  }

  void
  Sorter::sort ()
  {
    Container::children_t children = _container->children ();
    quick_sort (0, children.size () - 1);
  }

  void
  Sorter::serialize (const std::string& type)
  {
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:sort");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("spec", _spec.get_value());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
}
