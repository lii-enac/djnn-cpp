#pragma once

#include "core/tree/component.h"

namespace djnn {
    class StyleSheet : public Container
  {
  public:
    StyleSheet (ParentProcess* parent, const string& name);
    virtual ~StyleSheet () override {}
    StyleSheet* clone () override;
    const string& classname () { return _classname; }
    int id () { return _id; }
    static int get_id (const string& classname);
    static void draw_style (const vector<int>& classes);
  private:
    string _classname;
    int _id;
  };

}
