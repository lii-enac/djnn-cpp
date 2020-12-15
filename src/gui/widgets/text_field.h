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

#include "core/ontology/process.h"
#include "core/control/action.h"
#include "core/tree/text_property.h"
#include "core/tree/double_property.h"
#include "core/tree/int_property.h"
#include "gui/shapes/text.h"

namespace djnn {

  class Homography;

  class TextField : public FatProcess
  {
  private:
    class CutAction : public Action
    {
     public:
       CutAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
       ~CutAction () {}
       void impl_activate () override { ((TextField*)get_parent ())->cut (); };
    };
    class PointerMoveAction : public Action
    {
     public:
       PointerMoveAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
       ~PointerMoveAction () {}
       void impl_activate () override { ((TextField*)get_parent ())->pointer_move (); };
    };
    class ClearAction : public Action
    {
     public:
       ClearAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
       ~ClearAction () {}
       void impl_activate () override { ((TextField*)get_parent ())->clear (); };
    };
    class DeleteAction : public Action
    {
    public:
      DeleteAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
      ~DeleteAction () {}
      void impl_activate () override { ((TextField*)get_parent ())->del ();};
    };

    class SupprAction : public Action
    {
    public:
      SupprAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
      ~SupprAction () {}
      void impl_activate () override { ((TextField*)get_parent ())->suppr (); };
    };

    class AddStringAction : public Action
    {
    public:
      AddStringAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
      ~AddStringAction () {}
      void impl_activate () override { ((TextField*)get_parent ())->add_string (); };
    };
    class MoveLeftAction : public Action
    {
    public:
      MoveLeftAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
      ~MoveLeftAction () {}
      void impl_activate () override { ((TextField*)get_parent ())->move_left ();};
    };
    class MoveRightAction : public Action
    {
    public:
      MoveRightAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
      ~MoveRightAction () {}
      void impl_activate () override { ((TextField*)get_parent ())->move_right ();};
    };
    class UpdateCursorPositionAction : public Action
    {
    public:
      UpdateCursorPositionAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
      ~UpdateCursorPositionAction () {}
      void impl_activate () override { ((TextField*)get_parent ())->update_cursor_pos_from_press ();};
    };
    class StartSelectionAction : public Action
    {
    public:
      StartSelectionAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
      ~StartSelectionAction () {}
      void impl_activate () override { ((TextField*)get_parent ())->start_selection ();};
    };
    class EndSelectionAction : public Action
    {
    public:
      EndSelectionAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
      ~EndSelectionAction () {}
      void impl_activate () override { ((TextField*)get_parent ())->end_selection ();};
    };
    class SelectAllAction : public Action
    {
    public:
      SelectAllAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
      ~SelectAllAction () {}
      void impl_activate () override { ((TextField*)get_parent ())->select_all ();};
  };
  public:
    TextField (ParentProcess* parent, const std::string& name, CoreProcess* text, CoreProcess* shape);
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
    FatProcess  *_move_left, *_move_right, *_del, *_suppr, *_clear, *_cut, *_start_selection, *_end_selection, *_pointer_move, *_select_all;
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
