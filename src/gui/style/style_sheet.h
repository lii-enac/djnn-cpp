#pragma once

#include "core/tree/component.h"

namespace djnn {
    class StyleSheet : public Container
  {
  public:
    StyleSheet (ParentProcess* parent, const djnn::string& name);
    virtual ~StyleSheet () override {}
    StyleSheet* clone () override;
    const djnn::string& classname () { return _classname; }
    int id () { return _id; }
    static int get_id (const djnn::string& classname);
    static void draw_style (const vector<int>& classes);
  private:
    djnn::string _classname;
    int _id;
  };

}
