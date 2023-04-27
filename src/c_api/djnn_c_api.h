// has been generated by gen_prop.py
#pragma once

//#include "core/core-dev.h" // _DEBUG*
#include "core/enums.h"
#include "core/ontology/enums.h"
#include "base/enums.h"
#include "display/display-dev.h"
#include "animation/enums.h"
#include "gui/style/style_types.h"

namespace djnnstl {

}

extern "C" {
char * strdup(const char *s1);
void free(void *ptr);
}

struct mystring {
  char* s;
  mystring () : s(0) {}
  mystring (const char* _s) : s(strdup(_s)) {}
  ~mystring() { free ((void*)s); }
  operator const char*() const { return s; }
  mystring& operator += (const char*);
  mystring& operator = (const char*);
  bool operator == (const char*) const;
  bool operator == (const mystring&) const;
#ifdef DJNN_C_API_IMPL
  operator const djnnstl::string () const { return djnnstl::string(s); }
#endif
};
  // mystring (const char* _s) : s(_s) {}
  // mystring (const mystring& other) : s(strdup(other.s)) {}
  // mystring (mystring&& other) : s(other.s) { other.s = 0; }

mystring operator+ (const mystring&, const char*);
mystring operator+ (const mystring&, const mystring&);

using apistring = mystring;
//using apistring = const char*;
using string = mystring;

namespace djnn {

  class CoreProcess;
  class FatProcess;
  class NativeExpressionAction;

  using Process = CoreProcess;

  using ProcessForNative = CoreProcess;
  typedef void (NativeCode) (ProcessForNative*);

  class NativeExpressionActionProxy {
    public:
	  NativeExpressionActionProxy (CoreProcess* parent, const char* name, bool string_setter, bool is_model=false);
    ~NativeExpressionActionProxy ();

    virtual void impl_activate () = 0;
    void finalize_construction (CoreProcess* parent, const char* name);

    bool _string_setter;
    bool isModel;
    NativeExpressionAction * _nea;
  };

  typedef void (NativeCollectionCodeProxy) (CoreProcess*, CoreProcess*[], unsigned long);

  extern int mouse_tracking;
  extern CoreProcess* GenericMouse;
  //extern CoreProcess* GenericKeyboard;

  void graph_exec ();

  extern int
    _DEBUG_SEE_COLOR_PICKING_VIEW,
    _DEBUG_GRAPH_CYCLE_DETECT,
    _DEBUG_SEE_ACTIVATION_SEQUENCE,
    _DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_TIME_US,
    _DEBUG_SEE_ACTIVATION_SEQUENCE_ONLY_TARGETED,
    _AUTHORIZE_CYCLE,
    _DEBUG_SEE_RECOMPUTE_PIXMAP_AND_PAINTEVENT,
    _DEBUG_SEE_RECOMPUTE_PIXMAP_ONLY,
    _DEBUG_SEE_COMPONENTS_DESTRUCTION_INFO_LEVEL,
    _DEBUG_NO_TOUCH_EVENT
    ;
}

//extern "C" {
void djnn_init_core();
void djnn_clear_core();

void djnn_init_exec_env();
void djnn_clear_exec_env();

void djnn_init_base();
void djnn_clear_base();

void djnn_init_display();
void djnn_clear_display();

void djnn_init_gui();
void djnn_clear_gui();

void djnn_init_input();
void djnn_clear_input();

void djnn_init_audio();
void djnn_clear_audio();

void djnn_init_physics();
void djnn_clear_physics();

void djnn_init_animation();
void djnn_clear_animation();

void djnn_init_comms();
void djnn_clear_comms();

void djnn_init_files();
void djnn_clear_files();

void djnn_activate (djnn::CoreProcess* p);
void djnn_notify_activation (djnn::CoreProcess* p);
void djnn_deactivate (djnn::CoreProcess* p);
djnn::CoreProcess* djnn_find (djnn::CoreProcess* parent, const char* path);
djnn::CoreProcess* djnn_find (djnn::CoreProcess* parent, int pos);
djnn::CoreProcess* djnn_find_optional (djnn::CoreProcess* parent, const char* path);
djnn::CoreProcess* djnn_get_parent (djnn::CoreProcess* p);
inline djnn::CoreProcess* find (djnn::CoreProcess* parent, const char* path) { return djnn_find (parent, path); }
inline djnn::CoreProcess* find_optional (djnn::CoreProcess* parent, const char* path) { return djnn_find_optional (parent, path); }
inline djnn::CoreProcess*  find (djnn::CoreProcess* p) { return p; } // helper for smalac

mystring to_string(int);
int stoi(const mystring&);

//void djnn_delete (djnn::CoreProcess* p);
void djnn_schedule_deletion (djnn::CoreProcess* p);

djnn::CoreProcess* clone (djnn::CoreProcess *p);

void djnn_add_child (djnn::CoreProcess *parent, djnn::CoreProcess *child, const char* name);
void djnn_remove_child (djnn::CoreProcess *parent, djnn::CoreProcess *child);

void alias_children (djnn::CoreProcess* parent, djnn::FatProcess *to);
void alias (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* from);
void merge_children (djnn::CoreProcess* parent1, const char* sy1, djnn::CoreProcess* parent2, const char* sy2);
void djnn_move_child (djnn::CoreProcess* parent, djnn::CoreProcess* child_to_move, djnn::child_position_e spec, djnn::CoreProcess* child = nullptr);


// internal c-like API
inline djnn::FatProcess*  djnn_find (djnn::FatProcess* p) { return p; } // helper for smalac
djnn::FatProcess*  djnn_find (djnn::CoreProcess* p);
void djnn_remove_from_parentless_name (djnn::CoreProcess* child);


djnn::process_type_e djnn_get_process_type (djnn::CoreProcess* p);
djnn::PropertyType djnn_get_property_type (djnn::CoreProcess* p);
  
void    djnn_add_state_dependency (djnn::CoreProcess* parent, djnn::CoreProcess *p);
void djnn_remove_state_dependency (djnn::CoreProcess* parent, djnn::CoreProcess *p);

void djnn_synchronizer_add_source (djnn::CoreProcess* sync, djnn::CoreProcess* source, const char* path);
void djnn_add_native_edge (djnn::CoreProcess* sync, djnn::NativeExpressionActionProxy* src, djnn::CoreProcess* dst);
void djnn_add_native_edge (djnn::NativeExpressionActionProxy* naep, djnn::NativeExpressionActionProxy* src, djnn::CoreProcess* dst);
void djnn_finalize_construction (djnn::NativeExpressionActionProxy* self, djnn::CoreProcess* parent, const char* name);

//djnn::CoreProcess* djnn_find_child(djnn::CoreProcess* p, const mystring& n); // { return p->find_child (n); }
djnn::CoreProcess* djnn_get_activation_source(djnn::CoreProcess* p);
void* djnn_get_native_user_data(djnn::CoreProcess* p);

void djnn_set_value (djnn::CoreProcess* p, bool v, bool immediate);
void djnn_set_value (djnn::CoreProcess* p, int v, bool immediate);
void djnn_set_value (djnn::CoreProcess* p, double v, bool immediate);
void djnn_set_value (djnn::CoreProcess* p, const char* v, bool immediate);
double djnn_get_double_value (djnn::CoreProcess* p);
const mystring djnn_get_string_value (djnn::CoreProcess* p);


void djnn_dump(djnn::CoreProcess* p);
djnn::CoreProcess* djnn_mainloop_instance();

djnn::CoreProcess* load_from_XML(const char* path);
djnn::CoreProcess* load_from_XML_once(const char* path);


djnn::CoreProcess* djnn_new_Rectangle(djnn::CoreProcess* parent, const char* name, double x, double y, double width, double height, double rx=0, double ry=0); // { return new djnn::Rectangle(parent, name, x, y, width, height, rx, ry); }
djnn::CoreProcess* djnn_new_Circle(djnn::CoreProcess* parent, const char* name, double cx, double cy, double r); // { return new djnn::Circle(parent, name, cx, cy, r); }
djnn::CoreProcess* djnn_new_Ellipse(djnn::CoreProcess* parent, const char* name, double cx, double cy, double rx, double ry); // { return new djnn::Ellipse(parent, name, cx, cy, rx, ry); }
djnn::CoreProcess* djnn_new_Line(djnn::CoreProcess* parent, const char* name, double x1, double y1, double x2, double y2); // { return new djnn::Line(parent, name, x1, y1, x2, y2); }
djnn::CoreProcess* djnn_new_RectangleClip(djnn::CoreProcess* parent, const char* name, double x, double y, double width, double height); // { return new djnn::RectangleClip(parent, name, x, y, width, height); }
djnn::CoreProcess* djnn_new_PathImage(djnn::CoreProcess* parent, const char* name, const apistring& path, double x, double y, double width, double height); // { return new djnn::PathImage(parent, name, path, x, y, width, height); }
djnn::CoreProcess* djnn_new_DataImage (djnn::CoreProcess* parent, const char* name, const apistring& data, int format, double x, double y, double w, double h);
djnn::CoreProcess* djnn_new_DataImage (djnn::CoreProcess* parent, const char* name, const apistring& data, double x, double y, double w, double h);
djnn::CoreProcess* djnn_new_DataImage (djnn::CoreProcess* parent, const char* name, double x, double y, double w, double h);
djnn::CoreProcess* djnn_new_Image (djnn::CoreProcess* parent, const char* name, const apistring& path, double x, double y, double w, double h);

//djnn::CoreProcess* djnn_new_DataImage(djnn::CoreProcess* parent, const char* name, const apistring& data, int format, double x, double y, double width, double height); // { return new djnn::DataImage(parent, name, data, format, x, y, width, height); }
//djnn::CoreProcess* djnn_new_Color(djnn::CoreProcess* parent, const char* name, int r, int g, int b, int value); // { return new djnn::Color(parent, name, r, g, b, value); }
djnn::CoreProcess* djnn_new_FillRule(djnn::CoreProcess* parent, const char* name, int rule); // { return new djnn::FillRule(parent, name, rule); }
djnn::CoreProcess* djnn_new_Texture(djnn::CoreProcess* parent, const char* name, const apistring& path); // { return new djnn::Texture(parent, name, path); }
djnn::CoreProcess* djnn_new_FillOpacity(djnn::CoreProcess* parent, const char* name, double a); // { return new djnn::FillOpacity(parent, name, a); }
djnn::CoreProcess* djnn_new_OutlineOpacity(djnn::CoreProcess* parent, const char* name, double a); // { return new djnn::OutlineOpacity(parent, name, a); }
//djnn::CoreProcess* djnn_new_FillColor_rgb(djnn::CoreProcess* parent, const char* name, int r, int g, int b); // { return new djnn::FillColor(parent, name, r, g, b, value, r, g, b, value); }
//djnn::CoreProcess* djnn_new_FillColor_value(djnn::CoreProcess* parent, const char* name, int value); // { return new djnn::FillColor(parent, name, r, g, b, value, r, g, b, value); }
djnn::CoreProcess* djnn_new_FillColor(djnn::CoreProcess* parent, const char* name, int r, int g, int b); // { return new djnn::FillColor(parent, name, r, g, b, value, r, g, b, value); }
djnn::CoreProcess* djnn_new_FillColor(djnn::CoreProcess* parent, const char* name, int value); // { return new djnn::FillColor(parent, name, r, g, b, value, r, g, b, value); }
djnn::CoreProcess* djnn_new_OutlineWidth(djnn::CoreProcess* parent, const char* name, double width); // { return new djnn::OutlineWidth(parent, name, width); }
djnn::CoreProcess* djnn_new_OutlineCapStyle(djnn::CoreProcess* parent, const char* name, int cap); // { return new djnn::OutlineCapStyle(parent, name, cap); }
djnn::CoreProcess* djnn_new_OutlineJoinStyle(djnn::CoreProcess* parent, const char* name, int join); // { return new djnn::OutlineJoinStyle(parent, name, join); }
djnn::CoreProcess* djnn_new_OutlineMiterLimit(djnn::CoreProcess* parent, const char* name, int limit); // { return new djnn::OutlineMiterLimit(parent, name, limit); }
djnn::CoreProcess* djnn_new_DashOffset(djnn::CoreProcess* parent, const char* name, double offset); // { return new djnn::DashOffset(parent, name, offset); }
djnn::CoreProcess* djnn_new_GradientStop(djnn::CoreProcess* parent, const char* name, double r, double g, double b, double a, double offset); // { return new djnn::GradientStop(parent, name, r, g, b, a, offset); }
djnn::CoreProcess* djnn_new_LinearGradient(djnn::CoreProcess* parent, const char* name, double x1, double y1, double x2, double y2, int spread, int coords); // { return new djnn::LinearGradient(parent, name, x1, y1, x2, y2, spread, coords); }
djnn::CoreProcess* djnn_new_RadialGradient(djnn::CoreProcess* parent, const char* name, double cx, double cy, double r, double fx, double fy, int spread, int coords); // { return new djnn::RadialGradient(parent, name, cx, cy, r, fx, fy, spread, coords); }
djnn::CoreProcess* djnn_new_FontSize(djnn::CoreProcess* parent, const char* name, int unit, double size); // { return new djnn::FontSize(parent, name, unit, size); }
djnn::CoreProcess* djnn_new_FontWeight(djnn::CoreProcess* parent, const char* name, int weight); // { return new djnn::FontWeight(parent, name, weight); }
djnn::CoreProcess* djnn_new_FontStyle(djnn::CoreProcess* parent, const char* name, int style); // { return new djnn::FontStyle(parent, name, style); }
djnn::CoreProcess* djnn_new_FontFamily(djnn::CoreProcess* parent, const char* name, const apistring& family); // { return new djnn::FontFamily(parent, name, family); }
djnn::CoreProcess* djnn_new_TextAnchor(djnn::CoreProcess* parent, const char* name, int anchor); // { return new djnn::TextAnchor(parent, name, anchor); }
djnn::CoreProcess* djnn_new_Translation(djnn::CoreProcess* parent, const char* name, double tx, double ty); // { return new djnn::Translation(parent, name, tx, ty); }
djnn::CoreProcess* djnn_new_Rotation(djnn::CoreProcess* parent, const char* name, double a, double cx, double cy); // { return new djnn::Rotation(parent, name, a, cx, cy); }
djnn::CoreProcess* djnn_new_Scaling(djnn::CoreProcess* parent, const char* name, double sx, double sy, double cx, double cy); // { return new djnn::Scaling(parent, name, sx, sy, cx, cy); }
djnn::CoreProcess* djnn_new_Homography(djnn::CoreProcess* parent, const char* name, double m11=1, double m12=0, double m13=0, double m14=0, double m21=0, double m22=1, double m23=0, double m24=0, double m31=0, double m32=0, double m33=1, double m34=0, double m41=0, double m42=0, double m43=0, double m44=1); // { return new djnn::Homography(parent, name, m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44); }
djnn::CoreProcess* djnn_new_Volume(djnn::CoreProcess* parent, const char* name, double volume); // { return new djnn::Volume(parent, name, volume); }
//djnn::CoreProcess* djnn_new_Plane(djnn::CoreProcess* parent, const char* name, double a, double b, double c, double d); // { return new djnn::Plane(parent, name, a, b, c, d); }
djnn::CoreProcess* djnn_new_Binding(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* src, djnn::activation_flag_e src_flag, djnn::CoreProcess* dst, djnn::activation_flag_e dst_flag); // { return new djnn::Binding(parent, name, src, src_flag, dst, dst_flag); }
djnn::CoreProcess* djnn_new_Assignment(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* src, djnn::CoreProcess* dst, bool is_model); // { return new djnn::Assignment(parent, name, src, dst, is_model); }
djnn::CoreProcess* djnn_new_Synchronizer(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* dst, const apistring& spec); // { return new djnn::Synchronizer(parent, name, dst, spec); }
djnn::CoreProcess* djnn_new_Activator(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* src); // { return new djnn::Activator(parent, name, src); }
//djnn::CoreProcess* djnn_new_NativeAction(djnn::CoreProcess* parent, const char* name, const apistring& action, void* data, bool is_model); // { return new djnn::NativeAction(parent, name, action, data, is_model); }
djnn::CoreProcess* djnn_new_NativeAction(djnn::CoreProcess* parent, const char* name, djnn::NativeCode action, void* data, bool is_model); // { return new djnn::NativeAction(parent, name, action, data, is_model); }
djnn::CoreProcess* djnn_new_Blank(djnn::CoreProcess* parent, const char* name); // { return new djnn::Blank(parent, name); }
djnn::CoreProcess* djnn_new_Component(djnn::CoreProcess* parent, const char* name, bool is_model=false); // { return new djnn::Component(parent, name); }
djnn::CoreProcess* djnn_new_AssignmentSequence(djnn::CoreProcess* parent, const char* name, bool is_model); // { return new djnn::AssignmentSequence(parent, name, is_model); }
djnn::CoreProcess* djnn_new_List(djnn::CoreProcess* parent, const char* name); // { return new djnn::List(parent, name); }
djnn::CoreProcess* djnn_new_ListIterator(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* list, djnn::CoreProcess* action, bool is_model); // { return new djnn::ListIterator(parent, name, list, action, is_model); }
djnn::CoreProcess* djnn_new_Set(djnn::CoreProcess* parent, const char* name); // { return new djnn::Set(parent, name); }
djnn::CoreProcess* djnn_new_SetIterator(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* set, djnn::CoreProcess* action, bool is_model); // { return new djnn::SetIterator(parent, name, set, action, is_model); }
djnn::CoreProcess* djnn_new_Spike(djnn::CoreProcess* parent, const char* name); // { return new djnn::Spike(parent, name); }
djnn::CoreProcess* djnn_new_RefProperty(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* ref); // { return new djnn::RefProperty(parent, name, ref); }
djnn::CoreProcess* djnn_new_RemoteProperty(djnn::CoreProcess* parent, const char* name, const apistring& spec); // { return new djnn::RemoteProperty(parent, name, spec); }
djnn::CoreProcess* djnn_new_IntProperty(djnn::CoreProcess* parent, const char* name, int value); // { return new djnn::IntProperty(parent, name, value); }
djnn::CoreProcess* djnn_new_DoubleProperty(djnn::CoreProcess* parent, const char* name, double value); // { return new djnn::DoubleProperty(parent, name, value); }
djnn::CoreProcess* djnn_new_TextProperty(djnn::CoreProcess* parent, const char* name, const apistring& value); // { return new djnn::TextProperty(parent, name, value); }
djnn::CoreProcess* djnn_new_BoolProperty(djnn::CoreProcess* parent, const char* name, bool value); // { return new djnn::BoolProperty(parent, name, value); }
djnn::CoreProcess* djnn_new_Timer(djnn::CoreProcess* parent, const char* name, int delay); // { return new djnn::Timer(parent, name, delay); }
djnn::CoreProcess* djnn_new_Exit(djnn::CoreProcess* parent, const char* name, int value, bool is_model); // { return new djnn::Exit(parent, name, value, is_model); }
djnn::CoreProcess* djnn_new_Connector(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* src, djnn::CoreProcess* dst, bool copy_on_activation); // { return new djnn::Connector(parent, name, src, dst, copy_on_activation); }
djnn::CoreProcess* djnn_new_Clock(djnn::CoreProcess* parent, const char* name, int period); // { return new djnn::Clock(parent, name, period); }
djnn::CoreProcess* djnn_new_Counter(djnn::CoreProcess* parent, const char* name, int init, int delta); // { return new djnn::Counter(parent, name, init, delta); }
djnn::CoreProcess* djnn_new_Deref(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* ref_prop, const apistring& path, djnn::djnn_dir_t dir = djnn::DJNN_IGNORE); // { return new djnn::Deref(parent, name, ref_prop, path, dir); }
djnn::CoreProcess* djnn_new_DerefBool(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* ref_prop, const apistring& path, djnn::djnn_dir_t dir); // { return new djnn::DerefDouble(parent, name, ref_prop, path, dir); }
djnn::CoreProcess* djnn_new_DerefInt(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* ref_prop, const apistring& path, djnn::djnn_dir_t dir); // { return new djnn::DerefDouble(parent, name, ref_prop, path, dir); }
djnn::CoreProcess* djnn_new_DerefDouble(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* ref_prop, const apistring& path, djnn::djnn_dir_t dir); // { return new djnn::DerefDouble(parent, name, ref_prop, path, dir); }
djnn::CoreProcess* djnn_new_DerefString(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* ref_prop, const apistring& path, djnn::djnn_dir_t dir); // { return new djnn::DerefString(parent, name, ref_prop, path, dir); }
djnn::CoreProcess* djnn_new_Dictionary(djnn::CoreProcess* parent, const char* name); // { return new djnn::Dictionary(parent, name); }
djnn::CoreProcess* djnn_new_Finder(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* container, const apistring& path); // { return new djnn::Finder(parent, name, container, path); }
djnn::CoreProcess* djnn_new_FSM(djnn::CoreProcess* parent, const char* name); // { return new djnn::FSM(parent, name); }
djnn::CoreProcess* djnn_new_FSMTransition(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* from, djnn::CoreProcess* to, djnn::CoreProcess* trigger, djnn::CoreProcess* action); // { return new djnn::FSMTransition(parent, name, from, to, trigger, action); }
djnn::CoreProcess* djnn_new_FSMTransition(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* from, djnn::CoreProcess* to, djnn::CoreProcess* trigger, const apistring& tspec, djnn::CoreProcess* action, const apistring& aspec); // { return new djnn::FSMTransition(parent, name, from, to, trigger, action); }
djnn::CoreProcess* djnn_new_FSMState(djnn::CoreProcess* parent, const char* name); // { return new djnn::FSMState(parent, name); }
djnn::CoreProcess* djnn_new_HermiteCurve(djnn::CoreProcess* parent, const char* name, double p1, double p2, double t1, double t2); // { return new djnn::HermiteCurve(parent, name, p1, p2, t1, t2); }
djnn::CoreProcess* djnn_new_LogPrinter(djnn::CoreProcess* parent, const char* name, const apistring& label); // { return new djnn::LogPrinter(parent, name, label); }
djnn::CoreProcess* djnn_new_SwitchList(djnn::CoreProcess* parent, const char* name, bool loop); // { return new djnn::SwitchList(parent, name, loop); }
djnn::CoreProcess* djnn_new_SwitchRange(djnn::CoreProcess* parent, const char* name, double initial); // { return new djnn::SwitchRange(parent, name, initial); }
djnn::CoreProcess* djnn_new_Switch(djnn::CoreProcess* parent, const char* name, const apistring& initial); // { return new djnn::Switch(parent, name, initial); }
djnn::CoreProcess* djnn_new_TextPrinter(djnn::CoreProcess* parent, const char* name); // { return new djnn::TextPrinter(parent, name); }
djnn::CoreProcess* djnn_new_Regex(djnn::CoreProcess* parent, const char* name, const apistring& regex); // { return new djnn::Regex(parent, name, regex); }
djnn::CoreProcess* djnn_new_TextAccumulator(djnn::CoreProcess* parent, const char* name, const apistring& init); // { return new djnn::TextAccumulator(parent, name, init); }
djnn::CoreProcess* djnn_new_TextComparator(djnn::CoreProcess* parent, const char* name); // { return new djnn::TextComparator(parent, name); }
djnn::CoreProcess* djnn_new_TextCatenator(djnn::CoreProcess* parent, const char* name); // { return new djnn::TextCatenator(parent, name); }
djnn::CoreProcess* djnn_new_WallClock(djnn::CoreProcess* parent, const char* name); // { return new djnn::WallClock(parent, name); }
djnn::CoreProcess* djnn_new_Window(djnn::CoreProcess* parent, const char* name, const apistring& title, double x, double y, double width, double height); // { return new djnn::Window(parent, name, title, x, y, width, height); }
djnn::CoreProcess* djnn_new_OutlineColor(djnn::CoreProcess* parent, const char* name, double r, double g, double b); // { return new djnn::OutlineColor(parent, name, r, g, b); }
djnn::CoreProcess* djnn_new_Text(djnn::CoreProcess* parent, const char* name, double x, double y, const apistring& text); // { return new djnn::Text(parent, name, x, y, text); }

djnn::CoreProcess* djnn_new_NoOutline(djnn::CoreProcess* parent, const char* name);
djnn::CoreProcess* djnn_new_Polygon(djnn::CoreProcess* parent, const char* name);
djnn::CoreProcess* djnn_new_Polyline(djnn::CoreProcess* parent, const char* name);
djnn::CoreProcess* djnn_new_PolyPoint(djnn::CoreProcess* parent, const char* name, double x, double y);
djnn::CoreProcess* djnn_new_Synchronizer(djnn::CoreProcess* parent, const char* name, djnn::NativeExpressionActionProxy* s, const apistring& sname);
djnn::CoreProcess* djnn_new_DoubleFormatter(djnn::CoreProcess* parent, const char* name, double initial, int decimal);
djnn::CoreProcess* djnn_new_Incr (djnn::CoreProcess* parent, const char* name, bool is_model);
djnn::CoreProcess* djnn_new_EasingGenerator (djnn::CoreProcess* parent, const char* name, int func_t);
djnn::CoreProcess* djnn_new_ScreenToLocal (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* shape);
djnn::CoreProcess* djnn_new_Pow (djnn::CoreProcess* parent, const char* name, double x, double p);
djnn::CoreProcess* djnn_new_BoundedValue (djnn::CoreProcess* parent, const char* name, double min, double max, double v);
djnn::CoreProcess* djnn_new_OutlineColor(djnn::CoreProcess* parent, const char* name, int r, int g, int b);
djnn::CoreProcess* djnn_new_OutlineColor(djnn::CoreProcess* parent, const char* name, int value);
djnn::CoreProcess* djnn_new_TextField (djnn::CoreProcess* parent, const char* name, int x, int y, int width, int height, const apistring& text = "", bool enable_edit_on_activation = true);
djnn::CoreProcess* djnn_new_Path (djnn::CoreProcess* parent, const char* name, const apistring& path = "");
djnn::CoreProcess* djnn_new_MaxList (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess *container, const apistring& spec);
djnn::CoreProcess* djnn_new_SumList (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess *container, const apistring& spec);
djnn::CoreProcess* djnn_new_ProductList (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess *container, const apistring& spec);
djnn::CoreProcess* djnn_new_LazyConnector (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* src, djnn::CoreProcess* dst, bool copy_on_activation=true);
djnn::CoreProcess* djnn_new_SwitchRangeBranch (djnn::CoreProcess* parent, const char* name, double lower, bool left_open, double upper, bool right_open);
djnn::CoreProcess* djnn_new_PathMove (djnn::CoreProcess* parent, const char* name, double x, double y);
djnn::CoreProcess* djnn_new_PathLine (djnn::CoreProcess* parent, const char* name, double x, double y);
djnn::CoreProcess* djnn_new_PathQuadratic (djnn::CoreProcess* parent, const char* name, double x1, double y1, double x, double y);
djnn::CoreProcess* djnn_new_PathCubic (djnn::CoreProcess* parent, const char* name, double x1, double y1, double x2, double y2, double x, double y);
djnn::CoreProcess* djnn_new_PathArc (djnn::CoreProcess* parent, const char* name,  double rx, double ry, double rotx, double fl, double swfl, double x, double y);
djnn::CoreProcess* djnn_new_PathClosure (djnn::CoreProcess* parent, const char* name);
djnn::CoreProcess* djnn_new_ZOrderedGroup (djnn::CoreProcess* parent, const char* name);
djnn::CoreProcess* djnn_new_GraphEdgeAdder (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* src, djnn::CoreProcess* dst);
djnn::CoreProcess* djnn_new_DashArray (djnn::CoreProcess* parent, const char* name, double patterns[]);
djnn::CoreProcess* djnn_new_NoDashArray (djnn::CoreProcess* parent, const char* name);
djnn::CoreProcess* djnn_new_NoFill (djnn::CoreProcess* parent, const char* name);
djnn::CoreProcess* djnn_new_PickFill (djnn::CoreProcess* parent, const char* name);
djnn::CoreProcess* djnn_new_NoPickFill (djnn::CoreProcess* parent, const char* name);
djnn::CoreProcess* djnn_new_PickOutline (djnn::CoreProcess* parent, const char* name);
djnn::CoreProcess* djnn_new_Adder (djnn::CoreProcess* parent, const char* name, double a, double b);
djnn::CoreProcess* djnn_new_Subtractor (djnn::CoreProcess* parent, const char* name, double a, double b);
djnn::CoreProcess* djnn_new_Multiplier (djnn::CoreProcess* parent, const char* name, double a, double b);
djnn::CoreProcess* djnn_new_Divider (djnn::CoreProcess* parent, const char* name, double a, double b);
djnn::CoreProcess* djnn_new_AdderAccumulator (djnn::CoreProcess* parent, const char* name, double input, double clamp_min, double clamp_max);
djnn::CoreProcess* djnn_new_Sample (djnn::CoreProcess* parent, const char* name, const apistring& path);
djnn::CoreProcess* djnn_new_Cosine (djnn::CoreProcess* parent, const char* name, double v);
djnn::CoreProcess* djnn_new_Sine (djnn::CoreProcess* parent, const char* name, double v);
djnn::CoreProcess* djnn_new_Abs (djnn::CoreProcess* parent, const char* name, double v);
djnn::CoreProcess* djnn_new_ClampMin (djnn::CoreProcess* parent, const char* name, double v, double a);
djnn::CoreProcess* djnn_new_ClampMax (djnn::CoreProcess* parent, const char* name, double v, double a);
djnn::CoreProcess* djnn_new_IvyAccess (djnn::CoreProcess* parent, const char* name, const apistring& bus="224.1.2.3:2010", const apistring& appname="NO_NAME", const apistring& ready="READY", bool isModel=false);
djnn::CoreProcess* djnn_new_ProcExporter (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* tree, int port);
djnn::CoreProcess* djnn_new_RemoteProc (djnn::CoreProcess* parent, const char* name, const apistring& addr="224.1.2.3", int port = 2010);
//djnn::CoreProcess* djnn_new_NativeCollectionAction (djnn::CoreProcess* parent, const char* name, NativeCollectionCodeProxy *action, djnn::CoreProcess* coll, void* data, bool isModel);
djnn::CoreProcess* djnn_new_LazyAssignment (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* src, djnn::CoreProcess* dst, bool is_model = false); // { return new djnn::Assignment(parent, name, src, dst, is_model); }
djnn::CoreProcess* djnn_new_MultilineEditor (djnn::CoreProcess* parent, const char* name, int x, int y, int width, int height, const apistring& text = "", bool enable_edit_on_activation = true, bool wrap_text = false);
djnn::CoreProcess* djnn_new_Layer (djnn::CoreProcess* parent, const char* name, double pad=0);
djnn::CoreProcess* djnn_new_Layer (djnn::CoreProcess* parent, const char* name, double x, double y, double w, double h, double pad=0);
djnn::CoreProcess* djnn_new_Sorter (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* container, const apistring& spec);
djnn::CoreProcess* djnn_new_FileWriter (djnn::CoreProcess* parent, const char* name, const apistring& filename);

#define FAST_COMP_DECL(Proc) djnn::CoreProcess* djnn_new_##Proc (djnn::CoreProcess* parent, const char* name);
#define FAST_COMP_IMPL(Proc) djnn::CoreProcess* djnn_new_##Proc (djnn::CoreProcess* parent, const char* name) { return new djnn::Proc (parent, name); }

FAST_COMP_DECL(SwitchList);
FAST_COMP_DECL(ProcessCollector);
FAST_COMP_DECL(NoPickOutline);
FAST_COMP_DECL(Identity);


//djnn::CoreProcess* djnn_new_

using my_size_t = unsigned long; // prevent inclusion of size_t header

void MultiConnector (djnn::CoreProcess* parent, djnn::CoreProcess* src, const char* src_props[], my_size_t src_size, djnn::CoreProcess* dst, const char* dst_props[], my_size_t dst_size, bool copy_on_activation=true);
void MultiConnector (djnn::CoreProcess* parent, djnn::CoreProcess* src, djnn::CoreProcess* dst, bool copy_on_activation=true);
void MultiAssignment (djnn::CoreProcess* parent, djnn::CoreProcess* src, const char* src_props[], my_size_t src_size, djnn::CoreProcess* dst, const char* dst_props[], my_size_t dst_size, bool copy_on_activation=true);
void MultiAssignment (djnn::CoreProcess* parent, djnn::CoreProcess* src, djnn::CoreProcess* dst, bool copy_on_activation=true);
//}
