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

#include "abstract_gobj.h"

namespace djnn
{

  bool gui_initialized = false;
  std::shared_ptr<UpdateDrawing> UpdateDrawing::_instance;
  std::once_flag UpdateDrawing::onceFlag;

  UpdateDrawing*
  UpdateDrawing::instance ()
  {
    std::call_once (UpdateDrawing::onceFlag, [] () {
      _instance.reset(new UpdateDrawing);
    });

    return _instance.get ();
  }

  void
  UpdateDrawing::activate ()
  {
    for (auto w : _win_list) {
      if (w != nullptr) {
        w->update ();
        w->set_refresh (false);
      }
    }
    _win_list.clear ();
  }

  void
  UpdateDrawing::set_activation_flag (int flag)
  {
    Process::set_activation_flag (flag);
    Window *frame = dynamic_cast<Window*> (get_data ());
    if (frame == nullptr)
      return;
    if (!frame->refresh ()) {
      _win_list.push_back (frame);
      frame->set_refresh (true);
    }
  }

  void
  UpdateDrawing::init ()
  {
    Graph::instance ().add_output_node (instance ());
    gui_initialized = true;
  }

  void
  UpdateDrawing::clear ()
  {
    if (gui_initialized) {
      _instance->set_data (nullptr);
      _instance->_win_list.clear ();
      gui_initialized = false;
    }
  }

  void
  AbstractGObj::activate ()
  {
    if (_frame == nullptr || _frame->get_state () <= activated) {
      /*  this algorithm is a little bit tricky. We want to find the closest running frame
       *  on the left side of the current object (cur_child). For this, we take its parent (cur_parent) and go through its
       *  children in order to find a frame. If no frame is found when the list iteration process arrived to (cur_child),
       *  then we set (cur_child) to its parent (cur_parent), and (cur_parent) is set to (cur_parent->parent).
       *  May be there is a place for simplification */
      bool found = false;
      Process *cur_parent = _parent;
      Process *cur_child = this;
      while (!found && cur_parent != nullptr) {
        if (cur_parent->get_cpnt_type () == COMPONENT) {
          Container *cont = dynamic_cast<Container*> (cur_parent);
          for (auto c : cont->children ()) {
            if (c == cur_child)
              break;
            else if (c->get_cpnt_type () == WINDOW && c->get_state () <= activated) {
              _frame = dynamic_cast<Window*> (c);
              found = true;
            }
          }
        }
        do {
          cur_child = cur_parent;
          cur_parent = cur_parent->get_parent ();
        } while (cur_parent != nullptr && cur_parent->get_cpnt_type () != COMPONENT);
      }
      if (!found) {
        std::cout << "Warning no running frame found\n";
        return;
      }
    }
    UpdateDrawing::instance ()->set_data (_frame);
    UpdateDrawing::instance ()->set_activation_flag (ACTIVATION);
  }

  void
  AbstractGObj::deactivate ()
  {
    if (_frame != nullptr) {
      UpdateDrawing::instance ()->set_data (_frame);
      UpdateDrawing::instance ()->set_activation_flag (ACTIVATION);
    }
  }
}
