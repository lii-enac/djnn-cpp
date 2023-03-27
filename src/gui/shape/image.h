#pragma once

#include "gui/shape/abstract_gshape.h"
#include "gen/abstract_image.h"
#include "gen/abstract_path_image.h"
#include "gen/abstract_data_image.h"

#include "core/control/action.h"

namespace djnn {
  class ImageWatcher;

  class Image : public AbstractPathImage
  {
  public:
    Image (CoreProcess* parent, const string& name, const string& path, double x, double y, double w, double h);
    virtual ~Image ();
    void draw () override;
    Image* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
    virtual CoreProcess* find_child_impl (const string&) override;
    void* cache () { return _cache;}
    void set_cache (void * cache) { _cache = cache;}
    bool invalid_cache () { return _invalid_cache;}
    void set_invalid_cache (bool v) { _invalid_cache = v;}
    //void press (double x, double y) override; // should not have it here : MP 15.10.2021
    //void move (double x, double y) override; // should not have it here : MP 15.10.2021
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
  private:
    Coupling *_cwatcher;
    ImageWatcher *_watcher;
    IntProperty _pixel;
    void impl_activate () override;
    void impl_deactivate () override;
    void *_cache;
    bool _invalid_cache;
  };

  class DataImageWatcher;
  class DataImage : public AbstractDataImage
    {
    public:
      DataImage (CoreProcess* parent, const string& name, string* data, int format, double x, double y, double w, double h);
      DataImage (CoreProcess* parent, const string& name, string* data, double x, double y, double w, double h);
      DataImage (CoreProcess* parent, const string& name, double x, double y, double w, double h);
      virtual ~DataImage ();
      void draw () override;
      DataImage* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
      virtual CoreProcess* find_child_impl (const string&) override;
      void* cache () { return _cache;}
      void set_cache (void * cache) { _cache = cache;}
      bool invalid_cache () { return _invalid_cache;}
      void set_invalid_cache (bool v) { _invalid_cache = v;}
      //void get_bounding_box (double& x, double& y, double& w, double& h) const override;
      //double sdf (double x, double y) const override;
      string*& get_data_ref() { return raw_props.data; }
    private:
      Coupling *_cwatcher;
      DataImageWatcher *_watcher;
      void impl_activate () override;
      void impl_deactivate () override;
      void *_cache;
      bool _invalid_cache;
    };

  class ImageWatcher : public Action
  {
  public:
    ImageWatcher (Image *i) :
        Action (i, "ImageWatcher"), _img (i) {}
    virtual ~ImageWatcher () {}
    void impl_activate () override
    {
      _img->set_invalid_cache (true);
    }
  private:
    Image * _img;
  };

  class DataImageWatcher : public Action
    {
    public:
      DataImageWatcher (DataImage *i) :
          Action (i, "DataImageWatcher"), _img (i) {}
      virtual ~DataImageWatcher () {}
      void impl_activate () override
      {
        _img->set_invalid_cache (true);
      }
    private:
      DataImage * _img;
    };
}