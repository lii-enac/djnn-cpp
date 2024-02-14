/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2023)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 */

#pragma once

#include "core/control/action.h"
#include "core/control/spike.h"
#include "core/ontology/coupling.h"
#include "core/ontology/process.h"
#include "core/property/text_property.h"
#include "gui/shape/abstract_gshape.h"

namespace djnn {
class FileDialog : public AbstractGShape
{
  private:
    class OpenAction : public Action
    {
      public:
        OpenAction (CoreProcess* parent, const string& name) : Action (parent, name) {}
        virtual ~OpenAction () {}
        void impl_activate () override { ((FileDialog*)get_parent ())->open_dialog (); }
    };
    class SaveAction : public Action
    {
      public:
        SaveAction (CoreProcess* parent, const string& name) : Action (parent, name) {}
        virtual ~SaveAction () {}
        void impl_activate () override { ((FileDialog*)get_parent ())->save_dialog (); }
    };

  public:
    FileDialog (CoreProcess* parent, const string& name, const string& title = string ());
    virtual ~FileDialog ();
    void                impl_activate () override;
    void                impl_deactivate () override;
    void                open_dialog ();
    void                save_dialog ();
    const TextProperty& title () { return _title; }
    const TextProperty& filter () { return _filter; }
    const TextProperty& from () { return _from; }
    void                set_file_selected (const string& path) { _file_selected.set_value (path, true); }

  private:
    Spike        _open, _save;
    TextProperty _title, _filter, _from, _file_selected;
    OpenAction   _open_action;
    SaveAction   _save_action;
    Coupling     _c_open, _c_save;
};
} // namespace djnn