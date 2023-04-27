// generated with gen_prop.py


#include "core/core.h"
#include "exec_env/exec_env.h"
#include "base/base.h"
#include "display/display.h"
#include "gui/gui.h"
#include "input/input.h"
#include "audio/audio.h"
//#include "physics/physics.h"
#include "utils/utils.h"
#include "comms/comms.h"
#include "animation/animation.h"
#include "files/files.h"

#define DJNN_C_API_IMPL
#include "djnn_c_api.h"


mystring& mystring::operator += (const char* a) { djnnstl::string str(this->s); str+=a; s=strdup(str.c_str()); return *this; }
mystring& mystring::operator = (const char* a) { free(s); s=strdup(a); return *this; }
bool mystring::operator == (const char* a) const { return strcmp (s,a) == 0; }
bool mystring::operator == (const mystring& a) const { return strcmp (s,a.s) == 0; }
mystring operator+ (const mystring& a, const char* b) { djnnstl::string str(a.s); str += b; return mystring(str.c_str()); }
mystring operator+ (const mystring& a, const mystring& b) { djnnstl::string str(a.s); str += b.s; return mystring(str.c_str()); }
const mystring djnn_get_string_value (djnn::CoreProcess* p) { return mystring (((djnn::AbstractProperty*)p)->get_string_value ().c_str()); }

using apistring = mystring;
//using apistring = const char*;

namespace djnn {
  class NativeExpressionActionProxy;

  class MyAction : public NativeExpressionAction {
    public:
    MyAction (CoreProcess* parent, const char* name, bool isModel, NativeExpressionActionProxy * neap)
    : NativeExpressionAction (parent, name, isModel), _neap (neap) {}
    protected:
    virtual void impl_activate () override;
    virtual void impl_deactivate () override {}
    NativeExpressionActionProxy *_neap;
  };

  NativeExpressionActionProxy::NativeExpressionActionProxy (CoreProcess *parent, const char* name, bool string_setter, bool is_model)
  : _string_setter (string_setter)
  {
    _nea = new MyAction (parent, name, is_model, this);
  }
  void MyAction::impl_activate () { _neap->impl_activate (); }

}

//extern "C" {
void djnn_init_core() { djnn::init_core(); }
void djnn_clear_core() { djnn::clear_core(); }

void djnn_init_exec_env() { djnn::init_exec_env(); }
void djnn_clear_exec_env() { djnn::clear_exec_env(); }

void djnn_init_base() { djnn::init_base(); }
void djnn_clear_base() { djnn::clear_base(); }

void djnn_init_display() { djnn::init_display(); }
void djnn_clear_display() { djnn::clear_display(); }

void djnn_init_gui() { djnn::init_gui(); }
void djnn_clear_gui() { djnn::clear_gui(); }

void djnn_init_input() { djnn::init_input(); }
void djnn_clear_input() { djnn::clear_input(); }

void djnn_init_audio() { djnn::init_audio(); }
void djnn_clear_audio() { djnn::clear_audio(); }

// void djnn_init_physics() { djnn::init_physics(); }
// void djnn_clear_physics() { djnn::clear_physics(); }

void djnn_init_animation() { djnn::init_animation(); }
void djnn_clear_animation() { djnn::clear_animation(); }

void djnn_init_comms() { djnn::init_comms(); }
void djnn_clear_comms() { djnn::clear_comms(); }

void djnn_init_files() { djnn::init_files(); }
void djnn_clear_files() { djnn::clear_files(); }


void djnn_activate(djnn::CoreProcess* p) { p->activate (); }
void djnn_notify_activation (djnn::CoreProcess* p) { p->notify_activation (); }
void djnn_deactivate(djnn::CoreProcess* p) { p->deactivate (); }

djnn::CoreProcess* djnn_find (djnn::CoreProcess* parent, const char* path) { return parent->find_child (path); }
djnn::CoreProcess* djnn_find (djnn::CoreProcess* parent, int pos) { return parent->find_child (pos); }
djnn::CoreProcess* djnn_find_child(djnn::CoreProcess* p, const char* n) { return p->find_child (n); }
djnn::CoreProcess* djnn_find_optional (djnn::CoreProcess* parent, const char* path) { return parent->find_optional_child (path); }
//djnn::FatProcess*  find (djnn::CoreProcess* p) { return dynamic_cast<djnn::FatProcess*>(p); } // helper for smalac

mystring to_string(int i) { return djnnstl::to_string(i).c_str(); }
int stoi(const mystring& s) { return djnnstl::stoi (s.s); }

djnn::CoreProcess* djnn_get_parent (djnn::CoreProcess* p) { return p->get_parent (); }

//void djnn_delete (djnn::CoreProcess* p) { delete p; }
void djnn_schedule_deletion (djnn::CoreProcess* p) { p->schedule_deletion (); }

djnn::CoreProcess* clone (djnn::CoreProcess *p) { return p->clone (); } // FIXME will make code size grow :-/...

void djnn_add_child (djnn::CoreProcess *parent, djnn::CoreProcess *child, const char* name) { parent->add_child (child, name); }
void djnn_remove_child (djnn::CoreProcess *parent, djnn::CoreProcess *child) { parent->remove_child (child); }

void alias_children (djnn::CoreProcess* parent, djnn::FatProcess *to) { djnn::alias_children (parent, to); }
void alias (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* from) { djnn::alias (parent, name, from); }
void merge_children (djnn::CoreProcess* parent1, const char* sym1, djnn::CoreProcess* parent2, const char* sym2) { djnn::merge_children (parent1, sym1, parent2, sym2); }
void djnn_move_child (djnn::CoreProcess* parent, djnn::CoreProcess* child_to_move, djnn::child_position_e spec, djnn::CoreProcess* child) { parent->move_child (child_to_move, spec, child); }

// internal c-like API
//inline djnn::FatProcess*  djnn_find (djnn::FatProcess* p) { return p; } // helper for smalac
djnn::FatProcess*  djnn_find (djnn::CoreProcess* p) { return dynamic_cast<djnn::FatProcess*>(p); } // helper for smalac
void djnn_remove_from_parentless_name (djnn::CoreProcess* child) { return djnn::remove_from_parentless_name (child); }

djnn::process_type_e djnn_get_process_type (djnn::CoreProcess* p) { return p->get_process_type (); }
djnn::property_type_e djnn_get_property_type (djnn::CoreProcess* p) { return dynamic_cast<djnn::AbstractSimpleProperty*>(p)->get_property_type (); }

//vector<djnn::CoreProcess*> djnn_get_process_collector_list (djnn::CoreProcess* p) { return dynamic_cast<djnn::ProcessCollector*>(p)->get_list (); }
//vector<djnn::CoreProcess*> djnn_get_children (djnn::CoreProcess* p) { return p->children (); } 
  
void    djnn_add_state_dependency (djnn::CoreProcess* parent, djnn::CoreProcess *p) { djnn::add_state_dependency(parent, p); }
void djnn_remove_state_dependency (djnn::CoreProcess* parent, djnn::CoreProcess *p) { djnn::remove_state_dependency(parent, p); }

void djnn_synchronizer_add_source (djnn::CoreProcess* sync, djnn::CoreProcess* source, const char* path) { dynamic_cast<djnn::Synchronizer*>(sync)->add_source (source, path); }
void djnn_add_native_edge (djnn::CoreProcess* sync, djnn::NativeExpressionActionProxy* src, djnn::CoreProcess* dst) { dynamic_cast<djnn::Synchronizer*>(sync)->add_native_edge(src->_nea, dst); }
void djnn_add_native_edge (djnn::NativeExpressionActionProxy* naep, djnn::NativeExpressionActionProxy* src, djnn::CoreProcess* dst) { dynamic_cast<djnn::NativeExpressionAction*>(naep->_nea)->add_native_edge(src->_nea, dst); }
void djnn_finalize_construction (djnn::NativeExpressionActionProxy* self, djnn::CoreProcess* parent, const char* name) { self->_nea->finalize_construction (parent, name); }


djnn::CoreProcess* djnn_get_activation_source(djnn::CoreProcess* p) { return p->get_activation_source (); }
void* djnn_get_native_user_data(djnn::CoreProcess* p) { return djnn::get_native_user_data(p); }
void djnn_set_value (djnn::CoreProcess* p, bool v, bool immediate) { dynamic_cast<djnn::AbstractSimpleProperty*>(p)->set_value (v, immediate); }
void djnn_set_value (djnn::CoreProcess* p, int v, bool immediate) { dynamic_cast<djnn::AbstractSimpleProperty*>(p)->set_value (v, immediate); }
void djnn_set_value (djnn::CoreProcess* p, double v, bool immediate) { dynamic_cast<djnn::AbstractSimpleProperty*>(p)->set_value (v, immediate); }
//void djnn_set_value (djnn::CoreProcess* p, const char* v, bool immediate) { dynamic_cast<djnn::AbstractSimpleProperty*>(p)->set_value (djnnstl::string(v), immediate); }
void djnn_set_value (djnn::CoreProcess* p, const char* v, bool immediate) { dynamic_cast<djnn::AbstractSimpleProperty*>(p)->set_value (v, immediate); }
double djnn_get_double_value (djnn::CoreProcess* p) { return ((djnn::AbstractProperty*)p)->get_double_value (); }

djnn::CoreProcess* djnn_get_ref (djnn::CoreProcess* p) { return djnn::getRef (p); }
void djnn_set_ref (djnn::CoreProcess* p, djnn::CoreProcess* v) { djnn::setRef (p, v); }

void djnn_dump(djnn::CoreProcess* p) { p->dump (); }
djnn::CoreProcess* djnn_mainloop_instance() { return &djnn::MainLoop::instance (); }

djnn::CoreProcess* load_from_XML(const char* path) { return djnn::load_from_XML (path); }
djnn::CoreProcess* load_from_XML_once(const char* path) { return djnn::load_from_XML_once (path); }

int djnn__error (const djnn::CoreProcess *p, const char* msg, const char* ctxinfo=nullptr) { error (p,msg); return -1; }
void djnn__warning (const djnn::CoreProcess *p, const char* msg, const char* ctxinfo=nullptr) { warning (p,msg); }
int djnn__exit(int ret) { ::exit(ret); return 1; }

djnn::CoreProcess* djnn_new_Rectangle(djnn::CoreProcess* parent, const char* name, double x, double y, double width, double height, double rx, double ry) { return new djnn::Rectangle(parent, name, x, y, width, height, rx, ry); }
djnn::CoreProcess* djnn_new_Circle(djnn::CoreProcess* parent, const char* name, double cx, double cy, double r) { return new djnn::Circle(parent, name, cx, cy, r); }
djnn::CoreProcess* djnn_new_Ellipse(djnn::CoreProcess* parent, const char* name, double cx, double cy, double rx, double ry) { return new djnn::Ellipse(parent, name, cx, cy, rx, ry); }
djnn::CoreProcess* djnn_new_Line(djnn::CoreProcess* parent, const char* name, double x1, double y1, double x2, double y2) { return new djnn::Line(parent, name, x1, y1, x2, y2); }
djnn::CoreProcess* djnn_new_RectangleClip(djnn::CoreProcess* parent, const char* name, double x, double y, double width, double height) { return new djnn::RectangleClip(parent, name, x, y, width, height); }
djnn::CoreProcess* djnn_new_PathImage(djnn::CoreProcess* parent, const char* name, const apistring& path, double x, double y, double width, double height) { return new djnn::Image(parent, name, path, x, y, width, height); }
//djnn::CoreProcess* djnn_new_DataImage (djnn::CoreProcess* parent, const char* name, const apistring& data, int format, double x, double y, double w, double h) { return new djnn::DataImage (parent, name, (djnnstl::string*)data, format, x, y, w, h); }
//djnn::CoreProcess* djnn_new_DataImage (djnn::CoreProcess* parent, const char* name, const apistring& data, double x, double y, double w, double h)  { return new djnn::DataImage (parent, name, (djnnstl::string*)data, x, y, w, h); }
djnn::CoreProcess* djnn_new_DataImage (djnn::CoreProcess* parent, const char* name, double x, double y, double w, double h)  { return new djnn::DataImage (parent, name, x, y, w, h); }
djnn::CoreProcess* djnn_new_Image (djnn::CoreProcess* parent, const char* name, const apistring& path, double x, double y, double w, double h)  { return new djnn::Image (parent, name, path, x, y, w, h); }

//djnn::CoreProcess* djnn_new_Color(djnn::CoreProcess* parent, const char* name, int r, int g, int b, int value) { return new djnn::Color(parent, name, r, g, b, value); }
djnn::CoreProcess* djnn_new_FillRule(djnn::CoreProcess* parent, const char* name, int rule) { return new djnn::FillRule(parent, name, rule); }
djnn::CoreProcess* djnn_new_Texture(djnn::CoreProcess* parent, const char* name, const apistring& path) { return new djnn::Texture(parent, name, path); }
djnn::CoreProcess* djnn_new_FillOpacity(djnn::CoreProcess* parent, const char* name, double a) { return new djnn::FillOpacity(parent, name, a); }
djnn::CoreProcess* djnn_new_OutlineOpacity(djnn::CoreProcess* parent, const char* name, double a) { return new djnn::OutlineOpacity(parent, name, a); }
//djnn::CoreProcess* djnn_new_FillColor_rgb(djnn::CoreProcess* parent, const char* name, int r, int g, int b) { return new djnn::FillColor(parent, name, r, g, b); }
//djnn::CoreProcess* djnn_new_FillColor_value(djnn::CoreProcess* parent, const char* name, int value) { return new djnn::FillColor(parent, name, value); }
djnn::CoreProcess* djnn_new_FillColor(djnn::CoreProcess* parent, const char* name, int r, int g, int b) { return new djnn::FillColor(parent, name, r, g, b); }
djnn::CoreProcess* djnn_new_FillColor(djnn::CoreProcess* parent, const char* name, int value) { return new djnn::FillColor(parent, name, value); }
djnn::CoreProcess* djnn_new_OutlineWidth(djnn::CoreProcess* parent, const char* name, double width) { return new djnn::OutlineWidth(parent, name, width); }
djnn::CoreProcess* djnn_new_OutlineCapStyle(djnn::CoreProcess* parent, const char* name, int cap) { return new djnn::OutlineCapStyle(parent, name, cap); }
djnn::CoreProcess* djnn_new_OutlineJoinStyle(djnn::CoreProcess* parent, const char* name, int join) { return new djnn::OutlineJoinStyle(parent, name, join); }
djnn::CoreProcess* djnn_new_OutlineMiterLimit(djnn::CoreProcess* parent, const char* name, int limit) { return new djnn::OutlineMiterLimit(parent, name, limit); }
djnn::CoreProcess* djnn_new_DashOffset(djnn::CoreProcess* parent, const char* name, double offset) { return new djnn::DashOffset(parent, name, offset); }
djnn::CoreProcess* djnn_new_GradientStop(djnn::CoreProcess* parent, const char* name, double r, double g, double b, double a, double offset) { return new djnn::GradientStop(parent, name, r, g, b, a, offset); }
djnn::CoreProcess* djnn_new_LinearGradient(djnn::CoreProcess* parent, const char* name, double x1, double y1, double x2, double y2, int spread, int coords) { return new djnn::LinearGradient(parent, name, x1, y1, x2, y2, spread, coords); }
djnn::CoreProcess* djnn_new_RadialGradient(djnn::CoreProcess* parent, const char* name, double cx, double cy, double r, double fx, double fy, int spread, int coords) { return new djnn::RadialGradient(parent, name, cx, cy, r, fx, fy, spread, coords); }
djnn::CoreProcess* djnn_new_FontSize(djnn::CoreProcess* parent, const char* name, int unit, double size) { return new djnn::FontSize(parent, name, unit, size); }
djnn::CoreProcess* djnn_new_FontWeight(djnn::CoreProcess* parent, const char* name, int weight) { return new djnn::FontWeight(parent, name, weight); }
djnn::CoreProcess* djnn_new_FontStyle(djnn::CoreProcess* parent, const char* name, int style) { return new djnn::FontStyle(parent, name, style); }
djnn::CoreProcess* djnn_new_FontFamily(djnn::CoreProcess* parent, const char* name, const apistring& family) { return new djnn::FontFamily(parent, name, family); }
djnn::CoreProcess* djnn_new_TextAnchor(djnn::CoreProcess* parent, const char* name, int anchor) { return new djnn::TextAnchor(parent, name, anchor); }
djnn::CoreProcess* djnn_new_Translation(djnn::CoreProcess* parent, const char* name, double tx, double ty) { return new djnn::Translation(parent, name, tx, ty); }
djnn::CoreProcess* djnn_new_Rotation(djnn::CoreProcess* parent, const char* name, double a, double cx, double cy) { return new djnn::Rotation(parent, name, a, cx, cy); }
djnn::CoreProcess* djnn_new_Scaling(djnn::CoreProcess* parent, const char* name, double sx, double sy, double cx, double cy) { return new djnn::Scaling(parent, name, sx, sy, cx, cy); }
djnn::CoreProcess* djnn_new_Homography(djnn::CoreProcess* parent, const char* name, double m11, double m12, double m13, double m14, double m21, double m22, double m23, double m24, double m31, double m32, double m33, double m34, double m41, double m42, double m43, double m44) { return new djnn::Homography(parent, name, m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44); }
djnn::CoreProcess* djnn_new_Volume(djnn::CoreProcess* parent, const char* name, double volume) { return new djnn::Volume(parent, name, volume); }
//djnn::CoreProcess* djnn_new_Plane(djnn::CoreProcess* parent, const char* name, double a, double b, double c, double d) { return new djnn::Plane(parent, name, a, b, c, d); }
djnn::CoreProcess* djnn_new_Binding(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* src, djnn::activation_flag_e src_flag, djnn::CoreProcess* dst, djnn::activation_flag_e dst_flag) { return new djnn::Binding(parent, name, src, src_flag, dst, dst_flag); }
djnn::CoreProcess* djnn_new_Assignment(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* src, djnn::CoreProcess* dst, bool is_model) { return new djnn::Assignment(parent, name, src, dst, is_model); }
djnn::CoreProcess* djnn_new_Synchronizer(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* dst, const apistring& spec) { return new djnn::Synchronizer(parent, name, dst, spec); }
djnn::CoreProcess* djnn_new_Activator(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* src) { return new djnn::Activator(parent, name, src); }
//djnn::CoreProcess* djnn_new_NativeAction(djnn::CoreProcess* parent, const char* name, djnn::NativeCode * action, void* data, bool is_model) { return new djnn::NativeAction(parent, name, action, data, is_model); }
djnn::CoreProcess* djnn_new_NativeAction(djnn::CoreProcess* parent, const char* name, djnn::NativeCode * action, void* data, bool is_model) { return new djnn::NativeAction(parent, name, action, data, is_model); }
djnn::CoreProcess* djnn_new_Blank(djnn::CoreProcess* parent, const char* name) { return new djnn::Blank(parent, name); }
djnn::CoreProcess* djnn_new_Component(djnn::CoreProcess* parent, const char* name, bool is_model) { return new djnn::Component (parent, name, is_model); }
djnn::CoreProcess* djnn_new_AssignmentSequence(djnn::CoreProcess* parent, const char* name, bool is_model) { return new djnn::AssignmentSequence (parent, name, is_model); }
djnn::CoreProcess* djnn_new_List(djnn::CoreProcess* parent, const char* name) { return new djnn::List(parent, name); }
djnn::CoreProcess* djnn_new_ListIterator(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* list, djnn::CoreProcess* action, bool is_model) { return new djnn::ListIterator(parent, name, list, action, is_model); }
djnn::CoreProcess* djnn_new_Set(djnn::CoreProcess* parent, const char* name) { return new djnn::Set(parent, name); }
djnn::CoreProcess* djnn_new_SetIterator(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* set, djnn::CoreProcess* action, bool is_model) { return new djnn::SetIterator(parent, name, set, action, is_model); }
djnn::CoreProcess* djnn_new_Spike(djnn::CoreProcess* parent, const char* name) { return new djnn::Spike(parent, name); }
djnn::CoreProcess* djnn_new_RefProperty(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* ref) { return new djnn::RefProperty(parent, name, ref); }
djnn::CoreProcess* djnn_new_RemoteProperty(djnn::CoreProcess* parent, const char* name, const apistring& spec) { return new djnn::RemoteProperty(parent, name, spec); }
djnn::CoreProcess* djnn_new_IntProperty(djnn::CoreProcess* parent, const char* name, int value) { return new djnn::IntProperty(parent, name, value); }
djnn::CoreProcess* djnn_new_DoubleProperty(djnn::CoreProcess* parent, const char* name, double value) { return new djnn::DoubleProperty(parent, name, value); }
djnn::CoreProcess* djnn_new_TextProperty(djnn::CoreProcess* parent, const char* name, const apistring& value) { return new djnn::TextProperty(parent, name, value); }
djnn::CoreProcess* djnn_new_BoolProperty(djnn::CoreProcess* parent, const char* name, bool value) { return new djnn::BoolProperty(parent, name, value); }
djnn::CoreProcess* djnn_new_Timer(djnn::CoreProcess* parent, const char* name, int delay) { return new djnn::Timer(parent, name, delay); }
djnn::CoreProcess* djnn_new_Exit(djnn::CoreProcess* parent, const char* name, int value, bool is_model) { return new djnn::Exit(parent, name, value, is_model); }
djnn::CoreProcess* djnn_new_Connector(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* src, djnn::CoreProcess* dst, bool copy_on_activation) { return new djnn::Connector(parent, name, src, dst, copy_on_activation); }
djnn::CoreProcess* djnn_new_Clock(djnn::CoreProcess* parent, const char* name, int period) { return new djnn::Clock(parent, name, period); }
djnn::CoreProcess* djnn_new_Counter(djnn::CoreProcess* parent, const char* name, int init, int delta) { return new djnn::Counter(parent, name, init, delta); }
djnn::CoreProcess* djnn_new_Deref(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* ref_prop, const apistring& path, djnn::djnn_dir_t dir) { return new djnn::Deref(parent, name, ref_prop, path, dir); }
djnn::CoreProcess* djnn_new_DerefBool(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* ref_prop, const apistring& path, djnn::djnn_dir_t dir) { return new djnn::DerefBool(parent, name, ref_prop, path, dir); }
djnn::CoreProcess* djnn_new_DerefInt(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* ref_prop, const apistring& path, djnn::djnn_dir_t dir) { return new djnn::DerefInt(parent, name, ref_prop, path, dir); }
djnn::CoreProcess* djnn_new_DerefDouble(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* ref_prop, const apistring& path, djnn::djnn_dir_t dir) { return new djnn::DerefDouble(parent, name, ref_prop, path, dir); }
djnn::CoreProcess* djnn_new_DerefString(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* ref_prop, const apistring& path, djnn::djnn_dir_t dir) { return new djnn::DerefString(parent, name, ref_prop, path, dir); }
djnn::CoreProcess* djnn_new_Dictionary(djnn::CoreProcess* parent, const char* name) { return new djnn::Dictionary(parent, name); }
djnn::CoreProcess* djnn_new_Finder(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* container, const apistring& path) { return new djnn::Finder(parent, name, container, path); }
djnn::CoreProcess* djnn_new_FSM(djnn::CoreProcess* parent, const char* name) { return new djnn::FSM(parent, name); }
djnn::CoreProcess* djnn_new_FSMTransition(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* from, djnn::CoreProcess* to, djnn::CoreProcess* trigger, djnn::CoreProcess* action) { return new djnn::FSMTransition(parent, name, from, to, trigger, action); }
djnn::CoreProcess* djnn_new_FSMTransition(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* from, djnn::CoreProcess* to, djnn::CoreProcess* trigger, const apistring& tspec, djnn::CoreProcess* action, const apistring& aspec) { return new djnn::FSMTransition(parent, name, from, to, trigger, tspec, action, aspec); }

djnn::CoreProcess* djnn_new_FSMState(djnn::CoreProcess* parent, const char* name) { return new djnn::FSMState(parent, name); }
djnn::CoreProcess* djnn_new_HermiteCurve(djnn::CoreProcess* parent, const char* name, double p1, double p2, double t1, double t2) { return new djnn::HermiteCurve(parent, name, p1, p2, t1, t2); }
djnn::CoreProcess* djnn_new_LogPrinter(djnn::CoreProcess* parent, const char* name, const apistring& label) { return new djnn::LogPrinter(parent, name, label); }
djnn::CoreProcess* djnn_new_SwitchList(djnn::CoreProcess* parent, const char* name, bool loop) { return new djnn::SwitchList(parent, name, loop); }
djnn::CoreProcess* djnn_new_SwitchRange(djnn::CoreProcess* parent, const char* name, double initial) { return new djnn::SwitchRange(parent, name, initial); }
djnn::CoreProcess* djnn_new_Switch(djnn::CoreProcess* parent, const char* name, const apistring& initial) { return new djnn::Switch(parent, name, initial); }
djnn::CoreProcess* djnn_new_TextPrinter(djnn::CoreProcess* parent, const char* name) { return new djnn::TextPrinter(parent, name); }
//djnn::CoreProcess* djnn_new_Regex(djnn::CoreProcess* parent, const char* name, const apistring& regex) { return new djnn::Regex(parent, name, regex); }
djnn::CoreProcess* djnn_new_TextAccumulator(djnn::CoreProcess* parent, const char* name, const apistring& init) { return new djnn::TextAccumulator(parent, name, init); }
djnn::CoreProcess* djnn_new_TextComparator(djnn::CoreProcess* parent, const char* name) { return new djnn::TextComparator(parent, name); }
djnn::CoreProcess* djnn_new_TextCatenator(djnn::CoreProcess* parent, const char* name) { return new djnn::TextCatenator(parent, name); }
djnn::CoreProcess* djnn_new_WallClock(djnn::CoreProcess* parent, const char* name) { return new djnn::WallClock(parent, name); }
djnn::CoreProcess* djnn_new_Window(djnn::CoreProcess* parent, const char* name, const apistring& title, double x, double y, double width, double height) { return new djnn::Window(parent, name, title, x, y, width, height); }
djnn::CoreProcess* djnn_new_OutlineColor(djnn::CoreProcess* parent, const char* name, double r, double g, double b) { return new djnn::OutlineColor(parent, name, r, g, b); }
djnn::CoreProcess* djnn_new_Text(djnn::CoreProcess* parent, const char* name, double x, double y, const apistring& text) { return new djnn::Text(parent, name, x, y, text); }

djnn::CoreProcess* djnn_new_NoOutline(djnn::CoreProcess* parent, const char* name) { return new djnn::NoOutline (parent, name); }
djnn::CoreProcess* djnn_new_Polygon(djnn::CoreProcess* parent, const char* name) { return new djnn::Polygon (parent, name); }
djnn::CoreProcess* djnn_new_Polyline(djnn::CoreProcess* parent, const char* name) { return new djnn::Polyline (parent, name); }
djnn::CoreProcess* djnn_new_PolyPoint(djnn::CoreProcess* parent, const char* name, double x, double y) { return new djnn::PolyPoint (parent, name, x, y); }
djnn::CoreProcess* djnn_new_Synchronizer(djnn::CoreProcess* parent, const char* name, djnn::NativeExpressionActionProxy* s, const apistring& sname) { return new djnn::Synchronizer (parent, name, s->_nea, sname); }
djnn::CoreProcess* djnn_new_DoubleFormatter(djnn::CoreProcess* parent, const char* name, double initial, int decimal) { return new djnn::DoubleFormatter (parent, name, initial, decimal); }
djnn::CoreProcess* djnn_new_Incr (djnn::CoreProcess* parent, const char* name, bool is_model) { return new djnn::Incr (parent, name, is_model); }
djnn::CoreProcess* djnn_new_EasingGenerator (djnn::CoreProcess* parent, const char* name, int func_t) { return new djnn::EasingGenerator (parent, name, func_t); }
djnn::CoreProcess* djnn_new_ScreenToLocal (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* shape) { return new djnn::ScreenToLocal (parent, name, shape); }
djnn::CoreProcess* djnn_new_Pow (djnn::CoreProcess* parent, const char* name, double x, double p) { return new djnn::Pow (parent, name, x, p); }
djnn::CoreProcess* djnn_new_BoundedValue (djnn::CoreProcess* parent, const char* name, double min, double max, double v) { return new djnn::BoundedValue (parent, name, min, max, v); }
djnn::CoreProcess* djnn_new_OutlineColor(djnn::CoreProcess* parent, const char* name, int r, int g, int b) { return new djnn::OutlineColor(parent, name, r, g, b); }
djnn::CoreProcess* djnn_new_OutlineColor(djnn::CoreProcess* parent, const char* name, int value) { return new djnn::OutlineColor(parent, name, value); }
djnn::CoreProcess* djnn_new_TextField (djnn::CoreProcess* parent, const char* name, int x, int y, int width, int height, const apistring& text, bool enable_edit_on_activation) { return new djnn::TextField (parent, name, x, y, width, height, text, enable_edit_on_activation); }
djnn::CoreProcess* djnn_new_Path (djnn::CoreProcess* parent, const char* name, const apistring& path) { return new djnn::Path (parent, name, path); }
djnn::CoreProcess* djnn_new_MaxList (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess *container, const apistring& spec) { return new djnn::MaxList (parent, name, container, spec); }
djnn::CoreProcess* djnn_new_SumList (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess *container, const apistring& spec) { return new djnn::SumList (parent, name, container, spec); }
djnn::CoreProcess* djnn_new_ProductList (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess *container, const apistring& spec) { return new djnn::ProductList (parent, name, container, spec); }
//djnn::CoreProcess* djnn_new_DivideList (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess *container, const apistring& spec) { return new djnn::DivideList (parent, name, container, spec); }
djnn::CoreProcess* djnn_new_LazyConnector (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* src, djnn::CoreProcess* dst, bool copy_on_activation) { return new djnn::LazyConnector (parent, name, src, dst, copy_on_activation); }
djnn::CoreProcess* djnn_new_SwitchRangeBranch (djnn::CoreProcess* parent, const char* name, double lower, bool left_open, double upper, bool right_open) { return new djnn::SwitchRangeBranch (parent, name, lower, left_open, upper, right_open); }
djnn::CoreProcess* djnn_new_PathMove (djnn::CoreProcess* parent, const char* name, double x, double y) { return new djnn::PathMove (parent, name, x, y); }
djnn::CoreProcess* djnn_new_PathLine (djnn::CoreProcess* parent, const char* name, double x, double y) { return new djnn::PathLine (parent, name, x, y); }
djnn::CoreProcess* djnn_new_PathQuadratic (djnn::CoreProcess* parent, const char* name, double x1, double y1, double x, double y) { return new djnn::PathQuadratic (parent, name, x1, y1, x, y); }
djnn::CoreProcess* djnn_new_PathCubic (djnn::CoreProcess* parent, const char* name, double x1, double y1, double x2, double y2, double x, double y) { return new djnn::PathCubic (parent, name, x1, y1, x2, y2, x, y); }
djnn::CoreProcess* djnn_new_PathArc (djnn::CoreProcess* parent, const char* name,  double rx, double ry, double rotx, double fl, double swfl, double x, double y) { return new djnn::PathArc (parent, name, rx, ry, rotx, fl, swfl,x , y); }
djnn::CoreProcess* djnn_new_PathClosure (djnn::CoreProcess* parent, const char* name) { return new djnn::PathClosure (parent, name); }
djnn::CoreProcess* djnn_new_ZOrderedGroup (djnn::CoreProcess* parent, const char* name) { return new djnn::ZOrderedGroup (parent, name); }
djnn::CoreProcess* djnn_new_GraphEdgeAdder (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* src, djnn::CoreProcess* dst) { return new djnn::GraphEdgeAdder (parent, name, src, dst); }
djnn::CoreProcess* djnn_new_DashArray (djnn::CoreProcess* parent, const char* name, double patterns[]) { return new djnn::DashArray (parent, name, djnnstl::vector(patterns, patterns)); }
djnn::CoreProcess* djnn_new_NoDashArray (djnn::CoreProcess* parent, const char* name) { return new djnn::NoDashArray (parent, name); }
djnn::CoreProcess* djnn_new_NoFill (djnn::CoreProcess* parent, const char* name) { return new djnn::NoFill(parent, name); }
djnn::CoreProcess* djnn_new_NoPickFill (djnn::CoreProcess* parent, const char* name) { return new djnn::NoPickFill(parent, name); }
djnn::CoreProcess* djnn_new_PickFill (djnn::CoreProcess* parent, const char* name) { return new djnn::PickFill(parent, name); }
djnn::CoreProcess* djnn_new_PickOutline (djnn::CoreProcess* parent, const char* name) { return new djnn::PickOutline(parent, name); }
djnn::CoreProcess* djnn_new_Adder (djnn::CoreProcess* parent, const char* name, double a, double b) { return new djnn::Adder(parent, name, a, b); }
djnn::CoreProcess* djnn_new_Subtractor (djnn::CoreProcess* parent, const char* name, double a, double b) { return new djnn::Subtractor(parent, name, a, b); }
djnn::CoreProcess* djnn_new_Multiplier (djnn::CoreProcess* parent, const char* name, double a, double b) { return new djnn::Multiplier(parent, name, a, b); }
djnn::CoreProcess* djnn_new_Divider (djnn::CoreProcess* parent, const char* name, double a, double b) { return new djnn::Divider(parent, name, a, b); }
djnn::CoreProcess* djnn_new_AdderAccumulator (djnn::CoreProcess* parent, const char* name, double input, double clamp_min, double clamp_max) { return new djnn::AdderAccumulator (parent, name, input, clamp_min, clamp_max); }
djnn::CoreProcess* djnn_new_Sample (djnn::CoreProcess* parent, const char* name, const apistring& path) { return new djnn::Sample (parent, name, path); }
djnn::CoreProcess* djnn_new_Cosine (djnn::CoreProcess* parent, const char* name, double v) { return new djnn::Cosine (parent, name, v); }
djnn::CoreProcess* djnn_new_Sine (djnn::CoreProcess* parent, const char* name, double v) { return new djnn::Sine (parent, name, v); }
djnn::CoreProcess* djnn_new_Abs (djnn::CoreProcess* parent, const char* name, double v) { return new djnn::Abs (parent, name, v); }
djnn::CoreProcess* djnn_new_ClampMin (djnn::CoreProcess* parent, const char* name, double v, double a) { return new djnn::ClampMin (parent, name, v, a); }
djnn::CoreProcess* djnn_new_ClampMax (djnn::CoreProcess* parent, const char* name, double v, double a) { return new djnn::ClampMax (parent, name, v, a); }
djnn::CoreProcess* djnn_new_IvyAccess (djnn::CoreProcess* parent, const char* name, const apistring& bus, const apistring& appname, const apistring& ready, bool isModel) { return new djnn::IvyAccess (parent, name, bus, appname, ready, isModel); }
djnn::CoreProcess* djnn_new_ProcExporter (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* tree, int port) { return new djnn::ProcExporter (parent, name, tree, port); }
djnn::CoreProcess* djnn_new_RemoteProc (djnn::CoreProcess* parent, const char* name, const apistring& addr, int port) { return new djnn::RemoteProc (parent, name, addr, port); }
//djnn::CoreProcess* djnn_new_NativeCollectionAction (djnn::CoreProcess* parent, const char* name, djnn::NativeCollectionCode* action, unsigned long size, djnn::CoreProcess* coll, void* data, bool isModel) { return new djnn::NativeCollectionAction (parent, name, , coll, data, isModel); }
djnn::CoreProcess* djnn_new_LazyAssignment(djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* src, djnn::CoreProcess* dst, bool is_model) { return new djnn::LazyAssignment(parent, name, src, dst, is_model); }
djnn::CoreProcess* djnn_new_MultilineEditor (djnn::CoreProcess* parent, const char* name, int x, int y, int width, int height, const apistring& text, bool enable_edit_on_activation, bool wrap_text) { return new djnn::MultilineEditor (parent, name, x, y, width, height, text, enable_edit_on_activation, wrap_text); }
djnn::CoreProcess* djnn_new_Layer (djnn::CoreProcess* parent, const char* name, double pad) { return new djnn::Layer (parent, name, pad); }
djnn::CoreProcess* djnn_new_Layer (djnn::CoreProcess* parent, const char* name, double x, double y, double w, double h, double pad) { return new djnn::Layer (parent, name, x, y, w, h, pad); }
djnn::CoreProcess* djnn_new_Sorter (djnn::CoreProcess* parent, const char* name, djnn::CoreProcess* container, const apistring& spec) { return new djnn::Sorter (parent, name, container, spec); }
djnn::CoreProcess* djnn_new_FileWriter (djnn::CoreProcess* parent, const char* name, const apistring& filename) { return new djnn::FileWriter (parent, name, filename); }

#define FAST_COMP_IMPL(Proc) djnn::CoreProcess* djnn_new_##Proc (djnn::CoreProcess* parent, const char* name) { return new djnn::Proc (parent, name); }
FAST_COMP_IMPL(SwitchList);
FAST_COMP_IMPL(ProcessCollector);
FAST_COMP_IMPL(NoPickOutline);
FAST_COMP_IMPL(Identity);


void MultiConnector (djnn::CoreProcess* parent, djnn::CoreProcess* src, const char* src_props[], size_t src_size, djnn::CoreProcess* dst, const char* dst_props[], size_t dst_size, bool copy_on_activation) { return djnn::MultiConnector (parent, src, src_props, src_size, dst, dst_props, dst_size, copy_on_activation); }
void MultiConnector (djnn::CoreProcess* parent, djnn::CoreProcess* src, djnn::CoreProcess* dst, bool copy_on_activation) { djnn::MultiConnector (parent, src, dst, copy_on_activation); }
void MultiAssignment (djnn::CoreProcess* parent, djnn::CoreProcess* src, const char* src_props[], size_t src_size, djnn::CoreProcess* dst, const char* dst_props[], size_t dst_size, bool copy_on_activation) { return djnn::MultiAssignment (parent, src, src_props, src_size, dst, dst_props, dst_size, copy_on_activation); }
void MultiAssignment (djnn::CoreProcess* parent, djnn::CoreProcess* src, djnn::CoreProcess* dst, bool copy_on_activation);
//}
