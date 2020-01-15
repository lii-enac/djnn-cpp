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

#include "component.h"

#include "core/core.h"
#include "core/control/assignment.h"
#include "core/utils/error.h"
#include "core/execution/graph.h"
#include "core/tree/component_observer.h"
#include "core/serializer/serializer.h"

#include "gui/picking/analytical_picking_context.h"

#include <algorithm>
//#include <boost/range/adaptor/reversed.hpp>

#include <iostream>

//#define DEBUG
//#define DEBUG_DEACTIVATE

namespace djnn
{
  using namespace std;

  Container::Container (Process* parent, const string& name) :
      Process (name)
  {
    Container* c = dynamic_cast<Container*> (parent);
    if (c)
      c->init_context (_cur_context);
    for (auto s: structure_observer_list) {
      s->add_container (this);
    }
  }

#ifdef DEBUG
  static int nb_space = 0;
#endif

  void Container::clean_up_content () 
  {
#ifdef DEBUG
    for (int space = 0; space < nb_space; space++ ) cerr << "\t";
    ++nb_space ;
    cerr << "--- " << get_name () << " :" << endl;
#endif

    /* delete and remove children from this container */
    int sz = _children.size ();
    for (int i = sz - 1; i >= 0; i--) {
#ifdef DEBUG
      for (int space=0; space < nb_space; space++ ) cerr << "\t";
      cerr << i << ". " << _children[i]->get_name () << endl ;
#endif
      /* remove child from structure_observer (if in structure_observer) */
      for (auto s: structure_observer_list) 
        s->remove_child_from_container (this, _children[i]);
      delete _children[i]; 
      _children.pop_back();
    }

#ifdef DEBUG
    --nb_space ;
#endif
  }

  Container::~Container ()
  {
    if (!_children.empty ())
      clean_up_content ();

    /* remove container from structure_observer_list */
    for (auto s: structure_observer_list)
      s->remove_container (this);
  }

  void
  Container::add_child (Process* child, const std::string& name)
  {
    if (child == nullptr) {
      error (this, " add_child: trying to add '" +  name + "' to the parent '" + this->get_name () + "'  but could NOT find it\n");
      return;
    }
    _children.push_back (child);
    /* WARNING should we authorize multiple parenthood? */
    if (child->get_parent () != nullptr && child->get_parent () != this) {
      child->get_parent ()->remove_child (child);
    }
    child->set_parent (this);
    add_symbol (name, child);
    
    if (get_activation_state () == ACTIVATED && !child->is_model ()) {
      child->activate ();
    } else if (child->get_activation_state () == ACTIVATED) {
      child->deactivate ();
    }
    for (auto s: structure_observer_list) {
      s->add_child_to_container (this, child, _children.size () - 1);
    }
  }

  void
  Container::move_child (Process *child_to_move, child_position_e spec, Process *child2)
  {
    if (child2 == 0) {
      if (spec == FIRST) {
        Container::remove_child_from_children_only (child_to_move);
        _children.insert (_children.begin (), child_to_move);
        for (auto s: structure_observer_list) {
          s->move_child_to (this, child_to_move, 0, spec, 0);
        }
        return;
      } else if (spec == LAST) {
        Container::remove_child_from_children_only (child_to_move);
        _children.push_back (child_to_move);
        for (auto s: structure_observer_list) {
          s->move_child_to (this, child_to_move, 0, spec, _children.size () - 1);
        }
      } else
        return;
    }
    auto it = std::find (_children.begin (), _children.end (), child2);
    if (it == _children.end ()) {
      return;
    } else {
      auto index = std::distance (_children.begin (), it);
      if (spec == BEFORE) {
        Container::remove_child_from_children_only (child_to_move);
        _children.insert (_children.begin () + index, child_to_move);
        for (auto s: structure_observer_list) {
          s->move_child_to (this, child_to_move, index, spec, index);
        }
      } else if (spec == AFTER) {
        Container::remove_child_from_children_only (child_to_move);
        _children.insert (_children.begin () + index, child_to_move);
        for (auto s: structure_observer_list) {
          s->move_child_to (this, child_to_move, index, spec, index);
        }
      } else {
        cout << "spec = " << spec << endl;
        warning (this, "Undefined spec to move child " + child_to_move->get_name ());
      }
    }
  }

  void
  Container::remove_child_from_children_only (Process* c)
  {
    _children.erase (std::remove (_children.begin (), _children.end (), c), _children.end ());
    for (auto s: structure_observer_list) {
      s->remove_child_from_container (this, c);
    }
  }

  /* note: 
    remove child but do not delete child 
  */
  void
  Container::remove_child (Process* c)
  {
    Process::remove_child (c);
    remove_child_from_children_only (c);
  }

  /* note: 
    remove child but do not delete child 
  */
  void
  Container::remove_child (const std::string& name)
  {
    std::map<std::string, Process*>::iterator it = symtable ().find (name);
    if (it != symtable ().end ()) {
      Process* c = it->second;
      Container::remove_child(c);
    } else
      warning (nullptr,  " symbol " + name + " not found in Component " + get_name () + "\n");
  }

  void
  Container::swap_children (int i, int j)
  {
    Process *buff = _children[j];
    _children[j] = _children[i];
    _children[i] = buff;
    for (auto s: structure_observer_list) {
      s->swap_children (this, i, j);
    }
  }

  void
  Container::impl_activate ()
  {
    ComponentObserver::instance ().start_component ();
    /* WARNING Here we don't use C++ iterator as we want to allow
     * the dynamic modification of children list */
    unsigned int i = 0;
    while (i < _children.size ()) {
      if (!_children[i]->is_model ()) {
        _children[i]->activate ();
      }
      i++;
    }
    ComponentObserver::instance ().end_component ();
  }

  void
  Container::impl_deactivate ()
  {
    for(auto it = _children.rbegin(); it != _children.rend(); ++it) {
      auto& c = *it;
#ifdef DEBUG_DEACTIVATE
      cerr << __FUNCTION__ << " name: " << c->get_name () << endl;
#endif
      c->deactivate ();
    }
  }

  void
  Container::update_drawing ()
  {
    if (get_activation_flag () == DEACTIVATION)
      return;
    for (auto c : _children) {
      c->update_drawing ();
    }
  }

  void
  Container::draw ()
  {
    //std::cerr << this << " " << __FUNCTION__ << " " << __FILE__ << " " << __LINE__ <<  std::endl;
    if (get_activation_flag () == DEACTIVATION)
      return;
    ComponentObserver::instance ().start_draw ();
    for (auto c : _children) {
      c->draw ();
    }
    ComponentObserver::instance ().end_draw ();
  }

  void
  Container::pick ()
  {
    //std::cerr << this << " " << __FUNCTION__ << " " << __FILE__ << " " << __LINE__ <<  std::endl;
    if (get_activation_flag () == DEACTIVATION)
      return;
    //ComponentObserver::instance ().start_pick ();
    for (auto c : _children) {
      c->pick ();
    }
    //ComponentObserver::instance ().end_pick ();
  }

  AbstractGShape*
  Container::pick_analytical (PickAnalyticalContext& pac)
  {
    PickAnalyticalContext pac_stacked = pac;
    if (get_activation_flag () == DEACTIVATION) {
      return nullptr;
    }
    AbstractGShape * picked = nullptr;
    for (auto p : _children) {
      AbstractGShape * picked_ = p->pick_analytical (pac_stacked);
      if(pac_stacked.clipped) {
        return picked;
      }
      if(picked_) picked = picked_;
    }
    return picked;
  }


  Process*
  Container::clone ()
  {
    Process* clone = new Container (nullptr, get_name ());
    for (auto c : _children) {
      clone->add_child (c->clone (), this->find_component_name(c));
    }
    return clone;
  }

  void
  Container::print_children ()
  {
    cout << get_name () << "'s children:\n";
    for (auto c : _children) {
      cout << c->get_name () << endl;
    }
    cout << endl;
  }

  Process*
  Component::clone ()
  {
    Process* clone = new Component (nullptr, get_name ());
    for (auto c : _children) {
      clone->add_child (c->clone (), this->find_component_name(c));
    }
    return clone;
  }


  void
  Component::serialize (const string& format) {

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:component");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());

    for (auto c : _children)
        c->serialize (format);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }


  AssignmentSequence::AssignmentSequence (Process *parent, const string &name, bool isModel) :
      Container (parent, name)
  {
    set_is_model (isModel);
    Process::finalize_construction (parent, name);
  }

  void
  AssignmentSequence::impl_activate ()
  {
    for (auto c : _children) {
      c->activate ();
    }
  }

  void
  AssignmentSequence::add_child (Process* c, const std::string& name)
  {
    if (c == nullptr)
      return;

    _children.push_back (c);
    /* WARNING should we authorize multiple parenthood? */
    if (c->get_parent () != nullptr && c->get_parent () != this) {
      c->get_parent ()->remove_child (c);
    }
    c->set_parent (this);
    add_symbol (name, c);
  }

  void
  AssignmentSequence::post_activate ()
  {
    set_activation_state (DEACTIVATED);
  }

  void
  AssignmentSequence::serialize (const string& format) {

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:assignmentsequence");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());

    for (auto c : _children)
        c->serialize (format);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
}

