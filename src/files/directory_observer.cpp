/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "core/utils/filesystem.h"
#include "files.h"
#include "core/core-dev.h"
#include "exec_env/global_mutex.h"
#include "exec_env/main_loop.h"
#include "core/execution/graph.h"
#include "core/serializer/serializer.h"

#include "core/utils/iostream.h"

namespace djnn
{
  DirectoryObserver::DirectoryObserver (CoreProcess *parent, const string& name, const string& path)
  : FatProcess (name), ExternalSource(name),
    _path (this, "path", path),
    _update (this, "update"),
    _s_added (this, "files_added"),
    _s_removed (this, "files_removed"),
    _delete (this, "delete"),
    _rename (this, "rename"),
    _change (this, "change"),
    _update_action (this, "update_action"),
    _change_path_action (this, "change_path_action"),
    _c_path (&_path, ACTIVATION, &_change_path_action, ACTIVATION),
    _c_update (&_update, ACTIVATION, &_update_action, ACTIVATION),
    _files (this, "files"),
    _added_files (this, "added_files"),
    _removed_files (this, "removed_files")
  {
    graph_add_edge (&_update_action, &_s_added);
    graph_add_edge (&_update_action, &_s_removed);
    finalize_construction (parent, name);
  }

  DirectoryObserver::~DirectoryObserver ()
  {
    graph_remove_edge (&_update_action, &_s_added);
    graph_remove_edge (&_update_action, &_s_removed);
  }

  void
  DirectoryObserver::impl_activate ()
  {
    _c_path.enable ();
    _c_update.enable ();
    change_path ();
  }

  void
  DirectoryObserver::impl_deactivate ()
  {
    _c_path.disable ();
    _c_update.disable ();
    please_stop ();
  }

  void
  DirectoryObserver::change_path ()
  {
    please_stop ();
    iterate ();
    start_thread ();
  }

  void
  DirectoryObserver::run ()
  {
    set_please_stop (false);
    DirectoryObserverData *data = p_init_directory_watcher (_path.get_value ());
    //while (!thread_local_cancelled && !get_please_stop ()) {
    while (!get_please_stop ()) {
      djn_dir_event ret = p_run_directory_watcher (data);
      //if(thread_local_cancelled) {
      if (get_please_stop ()) {
        return;
      }
      bool exec = false;
      djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
      //if (!thread_local_cancelled && !get_please_stop () && ret != DJN_UNKNOWN && ret != DJN_ERROR) {
      if (!get_please_stop () && ret != DJN_UNKNOWN && ret != DJN_ERROR) {
        switch (ret) {
          case DJN_CHANGE:
            _change.schedule_activation ();
            exec = true;
            break;
          case DJN_RENAME:
            _rename.schedule_activation ();
            exec = true;
            break;
          case DJN_DELETE:
            _delete.schedule_activation ();
            exec = true;
            break;
          default:;
        }
        if (exec)
          GRAPH_EXEC;
      }
      djnn::release_exclusive_access (DBG_REL);
    }
  }


  void
  DirectoryObserver::iterate ()
  {
    vector<filesystem::path> new_files;
    vector<string> old_files;
    bool added = false;
    bool removed = false;
    _added_files.remove_all ();
    _removed_files.remove_all ();
    #if DJNN_STL_STD || DJNN_STL_DJNN
    const auto& path = _path.get_value();
    #elif DJNN_STL_EASTL
    const std::string path = _path.get_value().c_str();
    #endif
    for (auto &p: filesystem::directory_iterator(path)) {
      new_files.push_back (p);
    }
    for (auto &p : _files.children ()) {
      old_files.push_back (((File*)p)->get_filename ().c_str());
    }
    bool found = false;
    for (auto nf: new_files) {
      found = false;
      for (auto of: old_files) {
        if (nf.filename() == of) {
          found = true;
          break;
        }
      }
      if (!found) {
        std::string full_path = nf.root_directory().string() + nf.relative_path().string();
        CoreProcess * file = new File (&_files, "",full_path.c_str(), nf.filename().string().c_str(), filesystem::is_directory (nf));
        _added_files.add_one (file);
        added = true;
      }
    }
    for (auto of: old_files) {
      found = false;
      for (auto nf: new_files) {
        if (nf.filename() == of) {
          found = true;
          break;
        }
      }
      if (!found) {
        for (auto p : _files.children()) {
          if (of == ((File*)p)->get_filename ().c_str()) {
            p->deactivate();
            _files.remove_child (p);
            _removed_files.add_one (p);
            p->schedule_deletion();
            removed = true;
          }
        }
      }
    }
    if (added)
      _s_added.set_activation_flag (ACTIVATION);
    if (removed)
      _s_removed.set_activation_flag (ACTIVATION);
  }
}
