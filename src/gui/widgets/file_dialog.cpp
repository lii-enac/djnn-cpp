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

#include "file_dialog.h"
#include "gui/backend.h"
#include "gui/abstract_backend.h"

#include "exec_env/global_mutex.h"
#include "core/core-dev.h"

namespace djnn
{
  FileDialog::FileDialog(CoreProcess *parent, const string &name, const string &title)
  : AbstractGShape(parent, name),
  _open(this, "open"),
  _save(this, "save"),
  _title(this, "title", title),
  _filter(this, "filter", ""),
  _from(this, "from", ""),
  _file_selected(this, "file_selected", ""),
  _open_action(this, "open_action"),
  _save_action(this, "save_action"),
  _c_open(&_open, ACTIVATION, &_open_action, ACTIVATION),
  _c_save(&_save, ACTIVATION, &_save_action, ACTIVATION)
  {
    finalize_construction(parent, name);
  }

  FileDialog::~FileDialog()
  {
  }

  void
  FileDialog::impl_activate()
  {
    AbstractGShape::impl_activate();
    _c_open.enable();
    _c_save.enable();
  }

  void
  FileDialog::impl_deactivate()
  {
    AbstractGShape::impl_deactivate();
    _c_open.disable();
    _c_save.disable();
  }

  void
  FileDialog::open_dialog()
  {
    Backend::instance()->open_dialog(this);
  }

  void
  FileDialog::save_dialog()
  {
    Backend::instance()->save_dialog(this);
  }
}