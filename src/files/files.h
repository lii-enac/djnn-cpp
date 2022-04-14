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

#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/property/text_property.h"
#include "core/property/bool_property.h"
#include "core/control/spike.h"
#include "core/control/blank.h"
#include "core/tree/list.h"
#include "core/control/action.h"
#include "exec_env/external_source.h"
#include "files/file_writer.h"
#include "files/files-dev.h"
#include "base/process_handler.h"

namespace djnn
{
  typedef enum djn_dir_event {
    DJN_CHANGE,
    DJN_DELETE,
    DJN_RENAME,
    DJN_UNKNOWN,
    DJN_ERROR
  } djn_dir_event;

  struct DirectoryObserverData;
  extern vector<string> loadedModules; 

  class FileReader : public FatProcess
  {
  private:
    class FileReaderAction : public Action
    {
    public:
      FileReaderAction (ParentProcess* parent, const string& name) : Action (parent, name) {}
      void impl_activate () override;
    };
  public:
    FileReader (ParentProcess* parent, const string& name, const string& filename);
    virtual ~FileReader ();
    void impl_activate () override;
    void impl_deactivate () override;
#ifndef DJNN_NO_SERIALIZE    
    void serialize (const string& type) override;
#endif
    void read ();
  private:
    TextProperty *_input, *_output;
    Coupling* _c_input;
    FileReaderAction* _action;
  };

  class DirectoryObserver : public FatProcess, public ExternalSource
  {
  private:
     class UpdateAction : public Action
     {
     public:
       UpdateAction (ParentProcess *parent, const string& name) : Action (parent, name) {}
       void impl_activate () override { ((DirectoryObserver*)get_parent ())->iterate (); };
       void impl_deactivate () override {}
     };
     class ChangePathAction : public Action
     {
     public:
       ChangePathAction (ParentProcess *parent, const string& name) : Action (parent, name) {}
       void impl_activate () override { ((DirectoryObserver*)get_parent ())->change_path (); };
       void impl_deactivate () override {}
     };
  public:
    DirectoryObserver (ParentProcess *parent, const string& name, const string& path);
    virtual ~DirectoryObserver ();
    void impl_activate () override;
    void impl_deactivate () override;
    void iterate ();
    void change_path ();
  private:
    void run () override;
    TextProperty _path;
    Spike _update, _s_added, _s_removed;
    Blank _delete, _rename, _change;
    UpdateAction _update_action;
    ChangePathAction _change_path_action;
    Coupling _c_path, _c_update;
    List _files;
    ProcessCollector _added_files, _removed_files;
  };

  class File : public FatProcess
  {
  public:
    File (ParentProcess *parent, const string &name, const string &path, const string &filename, bool is_dir) :
          FatProcess (name), _path (this, "full_path", path), _filename (this, "filename", filename),
          _is_dir (this, "is_dir", is_dir) { finalize_construction (parent, name); }
    virtual ~File () {}
    void impl_activate () override {};
    void impl_deactivate () override {}
    const string& get_filename () { return _filename.get_value(); }
    const string& get_path () { return _path.get_value(); }
    bool is_dir () { return _is_dir.get_value (); }
  private:
    TextProperty _path, _filename;
    BoolProperty _is_dir;
  };
  DirectoryObserverData* p_init_directory_watcher (const string& path);
  djn_dir_event p_run_directory_watcher (DirectoryObserverData* data);
}
