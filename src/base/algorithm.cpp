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
#include "core/execution/graph.h"
#include "core/utils/error.h"
#include "core/serializer/serializer.h"
#include <iostream>
#include <algorithm>

namespace djnn
{

  Sorter::Sorter (Process *parent, const std::string &name, Process *container, const std::string& spec) :
      Process (name),
      _ascending (this, "ascending", true),
      _spec (this, "spec", spec),
      _action (this, "sort"),
      _sort (this, "sort"),
      _c_sort_action (&_sort, ACTIVATION, &_action, ACTIVATION, true),
      _c_spec_action (&_spec, ACTIVATION, &_action, ACTIVATION, true)
  {

    _container = dynamic_cast<Container*> (container);
    if (_container == nullptr)
      error (this, "Wrong argument: only containers can be sorted");
    Process::finalize_construction (parent, name);
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

  AbstractProperty*
  Sorter::get_and_check (Process *p)
  {
    AbstractProperty* r = dynamic_cast<AbstractProperty*> (p->find_component (_spec.get_value ()));
    if (r == nullptr) {
      error (this, "Unable to sort children: properties not found or with incorrect type");
      return nullptr;
    }
    return r;
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
  Sorter::merge (int p, int q, int r)
  {
    int n1 = q - p + 1, i , j ,k;
    int n2 = r - q;
    Container::children_t children = _container->children ();
    Process* L[n1];
    Process* R[n2];
    for (i = 0; i < n1; i++)
    {
      L[i] = children[p+i];
    }
    for (j = 0; j < n2; j++)
    {
      R[j] = children [q+j+1];
    }
    i=0,j=0;

    for (k = p; i < n1 && j < n2; k++)
    {
      if(compare (get_and_check (L[i]), get_and_check (R[j])))
      {
        _container->set_child (L[i], k);
        i++;
      }
      else
      {
        _container->set_child (R[j], k);
        j++;
      }
    }
    while (i < n1)
    {
      _container->set_child (L[i], k);
      i++; k++;
    }
    while(j<n2)
    {
      _container->set_child (R[j], k);
      j++; k++;
    }
  }

  void
  Sorter::merge_sort (int p, int r)
  {
    int q;

    if (p < r)
    {
      q = (p + r) / 2;
      merge_sort (p, q);
      merge_sort (q + 1, r);
      merge (p, q, r);
    }
  }

  void
  Sorter::sort ()
  {
    Container::children_t children = _container->children ();
    //quick_sort (0, children.size () - 1);
    merge_sort (0, children.size () - 1);
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

  ListOperator::ListOperator (Process *parent, const std::string &name, Process *container, const std::string& spec) :
      Process (name),
      _spec (this, "spec", spec),
      _update_list (this, "updateListAction"),
      _action (this, "action"),
      _c_spec_action (&_spec, ACTIVATION, &_action, ACTIVATION, true)
  {

    _container = dynamic_cast<Container*> (container);
    if (_container == nullptr)
      error (this, "Wrong argument: only containers can be used on List Operator");
    _c_update_list_action = new Coupling (container->find_component ("size"), ACTIVATION, &_update_list, ACTIVATION);
    Graph::instance ().add_edge (container->find_component ("size"), &_update_list);
    Process::finalize_construction (parent, name);
  }

  ListOperator::~ListOperator()
  {
    Graph::instance ().remove_edge (_container->find_component ("size"), &_action);
    delete _c_update_list_action;
  }

  void
  ListOperator::impl_activate ()
  {
    update_list ();
    _c_update_list_action->enable ();
    _c_spec_action.enable ();
    for (auto c: _coupling_list) {
      c->enable ();
    }
  }

  void
  ListOperator::impl_deactivate ()
  {
    _c_update_list_action->disable ();
    _c_spec_action.disable ();
    for (auto c: _coupling_list) {
      c->disable ();
    }
  }

  void
  ListOperator::update_list ()
  {
    for (auto c: _coupling_list) {
      Process *s = c->get_src();
      Graph::instance ().remove_edge (s, &_action);
      delete c;
    }
    _coupling_list.clear ();
    for (auto c: _container->children ()) {
      AbstractProperty *s = dynamic_cast<AbstractProperty*> (c->find_component (_spec.get_value ()));
      if (s == nullptr) {
        warning (this, "Wrong property in ListOperator " + get_name ());
        continue;
      }
      Coupling *cpl = new Coupling (s, ACTIVATION, &_action, ACTIVATION);
      Graph::instance ().add_edge (s, &_action);
      _coupling_list.push_back (cpl);
    }
    do_action ();
  }

  void
  SumList::do_action ()
  {
    double sum = 0;
    for (auto c: _container->children ()) {
      sum += ((AbstractProperty*)(c->find_component (_spec.get_value ())))->get_double_value();
    }
    _output.set_value (sum, true);
  }

  void
  SumList::serialize (const std::string& type)
  {
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:sum-list");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    string buf;
    AbstractSerializer::compute_path (get_parent (), _container, buf);
    AbstractSerializer::serializer->text_attribute ("container", buf);
    AbstractSerializer::serializer->text_attribute ("spec", _spec.get_value());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  void
  ProductList::do_action ()
  {
    double product = 0;
    for (auto c: _container->children ()) {
      product *= ((AbstractProperty*)(c->find_component (_spec.get_value ())))->get_double_value();
    }
    _output.set_value (product, true);
  }

  void
  ProductList::serialize (const std::string& type)
  {
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:product-list");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    string buf;
    AbstractSerializer::compute_path (get_parent (), _container, buf);
    AbstractSerializer::serializer->text_attribute ("container", buf);
    AbstractSerializer::serializer->text_attribute ("spec", _spec.get_value());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  void
  MaxList::do_action ()
  {
    double vmax = 0;
    for (auto c: _container->children ()) {
      vmax = max (((AbstractProperty*)(c->find_component (_spec.get_value ())))->get_double_value(), vmax);
    }
    _output.set_value (vmax, true);
  }

  void
  MaxList::serialize (const std::string& type)
  {
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:max-list");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    string buf;
    AbstractSerializer::compute_path (get_parent (), _container, buf);
    AbstractSerializer::serializer->text_attribute ("container", buf);
    AbstractSerializer::serializer->text_attribute ("spec", _spec.get_value());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  void
  MinList::do_action ()
  {
    double vmin = 0;
    for (auto c: _container->children ()) {
      vmin = min (((AbstractProperty*)(c->find_component (_spec.get_value ())))->get_double_value(), vmin);
    }
    _output.set_value (vmin, true);
  }

  void
  MinList::serialize (const std::string& type)
  {
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:min-list");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    string buf;
    AbstractSerializer::compute_path (get_parent (), _container, buf);
    AbstractSerializer::serializer->text_attribute ("container", buf);
    AbstractSerializer::serializer->text_attribute ("spec", _spec.get_value());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
}
