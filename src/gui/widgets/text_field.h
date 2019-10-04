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
 */

#pragma once

#include "../../core/ontology/process.h"
#include "../../core/control/action.h"
#include "../../core/tree/text_property.h"
#include "../../core/tree/double_property.h"
#include "../../core/tree/int_property.h"
#include "../shapes/shapes.h"
#include "../transformation/transformations.h"

namespace djnn {

  class TextField : public Process
  {
  private:
    class CutAction : public Action
    {
     public:
       CutAction (Process *p, const string &n) : Action (p, n) {}
       ~CutAction () {}
       void impl_activate () override { ((TextField*)_parent)->cut (); };
       void impl_deactivate () override {};
    };
    class PointerMoveAction : public Action
    {
     public:
       PointerMoveAction (Process *p, const string &n) : Action (p, n) {}
       ~PointerMoveAction () {}
       void impl_activate () override { ((TextField*)_parent)->pointer_move (); };
       void impl_deactivate () override {};
    };
    class ClearAction : public Action
    {
     public:
       ClearAction (Process *p, const string &n) : Action (p, n) {}
       ~ClearAction () {}
       void impl_activate () override { ((TextField*)_parent)->clear (); };
       void impl_deactivate () override {};
    };
    class DeleteAction : public Action
    {
    public:
      DeleteAction (Process *p, const string &n) : Action (p, n) {}
      ~DeleteAction () {}
      void impl_activate () override { ((TextField*)_parent)->del ();};
      void impl_deactivate () override {};
    };

    class SupprAction : public Action
    {
    public:
      SupprAction (Process *p, const string &n) : Action (p, n) {}
      ~SupprAction () {}
      void impl_activate () override { ((TextField*)_parent)->suppr (); };
      void impl_deactivate () override {};
    };

    class AddStringAction : public Action
    {
    public:
      AddStringAction (Process *p, const string &n) : Action (p, n) {}
      ~AddStringAction () {}
      void impl_activate () override { ((TextField*)_parent)->add_string (); };
      void impl_deactivate () override {};
    };
    class MoveLeftAction : public Action
    {
    public:
      MoveLeftAction (Process *p, const string &n) : Action (p, n) {}
      ~MoveLeftAction () {}
      void impl_activate () override { ((TextField*)_parent)->move_left ();};
      void impl_deactivate () override {};
    };
    class MoveRightAction : public Action
    {
    public:
      MoveRightAction (Process *p, const string &n) : Action (p, n) {}
      ~MoveRightAction () {}
      void impl_activate () override { ((TextField*)_parent)->move_right ();};
      void impl_deactivate () override {};
    };
    class UpdateCursorPositionAction : public Action
    {
    public:
      UpdateCursorPositionAction (Process *p, const string &n) : Action (p, n) {}
      ~UpdateCursorPositionAction () {}
      void impl_activate () override { ((TextField*)_parent)->update_cursor_pos_from_press ();};
      void impl_deactivate () override {};
    };
    class StartSelectionAction : public Action
    {
    public:
      StartSelectionAction (Process *p, const string &n) : Action (p, n) {}
      ~StartSelectionAction () {}
      void impl_activate () override { ((TextField*)_parent)->start_selection ();};
      void impl_deactivate () override {};
    };
    class EndSelectionAction : public Action
    {
    public:
      EndSelectionAction (Process *p, const string &n) : Action (p, n) {}
      ~EndSelectionAction () {}
      void impl_activate () override { ((TextField*)_parent)->end_selection ();};
      void impl_deactivate () override {};
    };
    class SelectAllAction : public Action
    {
    public:
      SelectAllAction (Process *p, const string &n) : Action (p, n) {}
      ~SelectAllAction () {}
      void impl_activate () override { ((TextField*)_parent)->select_all ();};
      void impl_deactivate () override {};
  };
  public:
    TextField (Process *p, const string &n, Process* text, Process* shape);
    virtual ~TextField ();
    void impl_activate () override;
    void impl_deactivate () override;
    void cut ();
    void clear ();
    void del ();
    void suppr ();
    void add_string ();
    void pointer_move ();
    void move_left ();
    void move_right ();
    int update_cursor_pos_from_index ();
    void update_cursor_pos_from_press ();
    void start_selection ();
    void end_selection ();
    void select_all ();
  private:
    void delete_text (int from, int to);
    Process  *_move_left, *_move_right, *_del, *_suppr, *_clear, *_cut, *_start_selection, *_end_selection, *_pointer_move, *_select_all;
    Text *_text;
    TextProperty *_new_text;
    IntProperty *_x_start, *_x_end;
    DoubleProperty *_press_x, *_press_y, *_move_x, *_move_y;
    Homography* _inverted_matrix;
    Coupling *_c_on_add, *_c_on_del, *_c_on_suppr, *_c_move_left, *_c_move_right, *_c_on_press, *_c_on_move, *_c_clear, *_c_cut, *_c_start_sel, *_c_end_sel, *_c_select_all;
    ClearAction *_clear_action;
    DeleteAction *_delete_action;
    SupprAction *_suppr_action;
    AddStringAction *_add_string_action;
    MoveLeftAction *_move_left_action;
    MoveRightAction *_move_right_action;
    CutAction *_cut_action;
    PointerMoveAction *_pointer_move_action;
    UpdateCursorPositionAction *_update_cursor_pos_action;
    StartSelectionAction *_start_selection_action;
    EndSelectionAction *_end_selection_action;
    SelectAllAction *_select_all_action;
    IntProperty *_index;
    int _start_select, _end_select;
    bool _is_selecting;
  };
}
