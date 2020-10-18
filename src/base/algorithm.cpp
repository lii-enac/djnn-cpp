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
#include "core/utils/djnn_dynamic_cast.h"
#include "core/serializer/serializer.h"

#include <algorithm>


namespace djnn
{

  Sorter::Sorter (ParentProcess* parent, const std::string& name, CoreProcess *container, const std::string& spec) :
      FatProcess (name),
      _ascending (this, "ascending", true),
      _spec (this, "spec", spec),
      _sort_action (this, "sort"),
      _c_spec_action (&_spec, ACTIVATION, &_sort_action, ACTIVATION)
  {
    _container = djnn_dynamic_cast<Container*> (container);
    if (_container == nullptr)
      error (this, "Wrong argument: only containers can be sorted");

    Graph::instance ().add_edge (&_spec, &_sort_action);

    finalize_construction (parent, name);
  }

  Sorter::~Sorter () {
    Graph::instance ().remove_edge (&_spec, &_sort_action);
  }

  void
  Sorter::impl_activate ()
  {
    _c_spec_action.enable ();
  }

  void
  Sorter::impl_deactivate ()
  {
    _c_spec_action.disable ();
  }

  AbstractProperty*
  Sorter::get_and_check (CoreProcess *p)
  {
    AbstractProperty* r = nullptr;
    if(p)
      r = djnn_dynamic_cast<AbstractProperty*> (p->find_child_impl (_spec.get_value ()));
    if (r == nullptr) {
      error (this, "Unable to sort children: properties not found or with incorrect type");
      return nullptr;
    }
    return r;
  }

  bool
  compare_number (const std::pair<double, int> &l, const std::pair<double, int> &r)
  {
    return l.first < r.first;
  }

  bool
  compare_string (const std::pair<std::string, int> &l, const std::pair<std::string, int> &r)
  {
    return l.first < r.first;
  }

  void
  Sorter::sort ()
  {
    auto & children = _container->children ();
    int sz = children.size ();
    if (sz == 0)
      return;
    Container::ordered_children_t cpy (children);
    AbstractProperty* p = get_and_check (children[0]);
    int type = p->get_prop_type ();
    switch (type)
      {
      case Boolean:
      case Integer:
      case Double:
    	{
    	  std::vector<std::pair<double, int>> to_sort;
    	  int i = 0;
    	  for (auto c : children) {
    	    AbstractProperty* prop = get_and_check (c);
    	    if (prop->get_prop_type () != type) {
    	      error (this, "Cannot compare properties of different types");
    	    }
    	    to_sort.push_back (std::pair<double, int> (prop->get_double_value (), i++));
    	  }
    	  std::stable_sort (to_sort.begin (), to_sort.end (), compare_number);
    	  i = 0;
    	  for (auto v : to_sort) {
    	    if (_ascending.get_value ())
    	      _container->set_child (cpy[v.second], i);
    	    else
    	      _container->set_child (cpy[v.second], sz - i - 1);
    	    i++;
    	  }
    	  break;
    	}
      case String:
    	{
    	  std::vector<std::pair<std::string, int>> to_sort;
    	  int i = 0;
    	  for (auto c : children) {
    	    AbstractProperty* prop = get_and_check (c);
    	    if (prop->get_prop_type () != type) {
    	      error (this, "Cannot compare properties of different types");
    	    }
    	    to_sort.push_back (std::pair<std::string, int> (prop->get_string_value (), i++));
    	  }
    	  std::stable_sort (to_sort.begin (), to_sort.end (), compare_string);
    	  i = 0;
    	  for (auto v : to_sort) {
    	    if (_ascending.get_value ())
    	      _container->set_child (cpy[v.second], i);
    	    else
    	      _container->set_child (cpy[v.second], sz - i - 1);
    	    i++;
    	  }
    	  break;
    	}
      default:
	       return;
      }
  }

#ifndef DJNN_NO_SERIALIZE
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
#endif

  ListOperator::ListOperator (ParentProcess* parent, const std::string& name, CoreProcess *container, const std::string& spec) :
      FatProcess (name),
      _spec (this, "spec", spec),
      _update_list (this, "updateListAction"),
      _spec_action (this, "specAction"),
      _c_spec_action (&_spec, ACTIVATION, &_spec_action, ACTIVATION, true)
  {

    _container = djnn_dynamic_cast<Container*> (container);
    if (_container == nullptr)
      error (this, "Wrong argument: only containers can be used on List Operator");
    _c_update_list_action = new Coupling (container->find_child_impl ("size"), ACTIVATION, &_update_list, ACTIVATION);
    Graph::instance ().add_edge (container->find_child_impl ("size"), &_update_list);
    finalize_construction (parent, name);
  }

  ListOperator::~ListOperator()
  {
    for (auto c: _coupling_list) {
      auto * s = c->get_src();
      Graph::instance ().remove_edge (s, &_spec_action);
      delete c;
    }

    Graph::instance ().remove_edge (_container->find_child_impl ("size"), &_update_list);
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
      auto * s = c->get_src();
      Graph::instance ().remove_edge (s, &_spec_action);
      delete c;
    }
    _coupling_list.clear ();
    for (auto c: _container->children ()) {
      AbstractProperty *s = djnn_dynamic_cast<AbstractProperty*> (c->find_child_impl (_spec.get_value ()));
      if (s == nullptr) {
        warning (this, "Wrong property in ListOperator " + get_name ());
        continue;
      }
      Coupling *cpl = new Coupling (s, ACTIVATION, &_spec_action, ACTIVATION);
      Graph::instance ().add_edge (s, &_spec_action);
      _coupling_list.push_back (cpl);
    }
    do_action ();
  }

  void
  SumList::do_action ()
  {
    double sum = 0;
    for (auto c: _container->children ()) {
      sum += ((AbstractProperty*)(c->find_child_impl (_spec.get_value ())))->get_double_value();
    }
    _output.set_value (sum, true);
  }

#ifndef DJNN_NO_SERIALIZE
  void
  SumList::serialize (const std::string& type)
  {
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:sum-list");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    std::string buf;
    AbstractSerializer::compute_path (get_parent (), _container, buf);
    AbstractSerializer::serializer->text_attribute ("container", buf);
    AbstractSerializer::serializer->text_attribute ("spec", _spec.get_value());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
#endif

  void
  ProductList::do_action ()
  {
    double product = 1;
    for (auto c: _container->children ()) {
      product *= ((AbstractProperty*)(c->find_child_impl (_spec.get_value ())))->get_double_value();
    }
    _output.set_value (product, true);
  }

#ifndef DJNN_NO_SERIALIZE
  void
  ProductList::serialize (const std::string& type)
  {
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:product-list");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    std::string buf;
    AbstractSerializer::compute_path (get_parent (), _container, buf);
    AbstractSerializer::serializer->text_attribute ("container", buf);
    AbstractSerializer::serializer->text_attribute ("spec", _spec.get_value());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
#endif

  void
  MaxList::do_action ()
  {
    double vmax = 0;
    for (auto c: _container->children ()) {
      vmax = std::max (((AbstractProperty*)(c->find_child_impl (_spec.get_value ())))->get_double_value(), vmax);
    }
    _output.set_value (vmax, true);
  }

#ifndef DJNN_NO_SERIALIZE
  void
  MaxList::serialize (const std::string& type)
  {
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:max-list");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    std::string buf;
    AbstractSerializer::compute_path (get_parent (), _container, buf);
    AbstractSerializer::serializer->text_attribute ("container", buf);
    AbstractSerializer::serializer->text_attribute ("spec", _spec.get_value());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
#endif

  void
  MinList::do_action ()
  {
    double vmin = std::numeric_limits<double>::max(); // set to double max value
    for (auto c: _container->children ()) {
      vmin = std::min (((AbstractProperty*)(c->find_child_impl (_spec.get_value ())))->get_double_value(), vmin);
    }
    _output.set_value (vmin, true);
  }

#ifndef DJNN_NO_SERIALIZE
  void
  MinList::serialize (const std::string& type)
  {
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:min-list");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    std::string buf;
    AbstractSerializer::compute_path (get_parent (), _container, buf);
    AbstractSerializer::serializer->text_attribute ("container", buf);
    AbstractSerializer::serializer->text_attribute ("spec", _spec.get_value());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
#endif

}
