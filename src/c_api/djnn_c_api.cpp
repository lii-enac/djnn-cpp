// generated with gen_prop.py

#include "core/core.h"
#include "exec_env/exec_env.h"
#include "base/base.h"
#include "display/display.h"
#include "gui/gui.h"
#include "audio/audio.h"
#include "physics/physics.h"

namespace djnn {
class Image;
class FillColor;

// following for js
using PathImage       = Image;
using FillColor_rgb   = FillColor;
using FillColor_value = FillColor;
} // namespace djnn
extern "C"
{
    void
    djnn_init_core ()
    {
        djnn::init_core ();
    }
    void
    djnn_clear_core ()
    {
        djnn::clear_core ();
    }
    void
    djnn_init_exec_env ()
    {
        djnn::init_exec_env ();
    }
    void
    djnn_clear_exec_env ()
    {
        djnn::clear_exec_env ();
    }
    void
    djnn_init_base ()
    {
        djnn::init_base ();
    }
    void
    djnn_clear_base ()
    {
        djnn::clear_base ();
    }
    void
    djnn_init_display ()
    {
        djnn::init_display ();
    }
    void
    djnn_clear_display ()
    {
        djnn::clear_display ();
    }
    void
    djnn_init_gui ()
    {
        djnn::init_gui ();
    }
    void
    djnn_clear_gui ()
    {
        djnn::clear_gui ();
    }
    void
    djnn_init_audio ()
    {
        djnn::init_audio ();
    }
    void
    djnn_clear_audio ()
    {
        djnn::clear_audio ();
    }
    void
    djnn_init_physics ()
    {
        djnn::init_physics ();
    }
    void
    djnn_clear_physics ()
    {
        djnn::clear_physics ();
    }
    void
    djnn_activate (djnn::CoreProcess* p)
    {
        p->activate ();
    }
    djnn::CoreProcess*
    djnn_find_child (djnn::CoreProcess* p, const char* n)
    {
        return p->find_child (n);
    }
    djnn::process_type_e
    djnn_get_process_type (djnn::CoreProcess* p)
    {
        return p->get_process_type ();
    }
    djnn::CoreProcess*
    djnn_get_activation_source (djnn::CoreProcess* p)
    {
        return p->get_activation_source ();
    }
    void*
    djnn_get_native_user_data (djnn::CoreProcess* p)
    {
        return djnn::get_native_user_data (p);
    }
    void
    djnn_set_value (djnn::CoreProcess* p, double v, bool immediate)
    {
        ((djnn::AbstractProperty*)p)->set_value (v, immediate);
    }
    double
    djnn_get_value (djnn::CoreProcess* p)
    {
        return ((djnn::AbstractProperty*)p)->get_double_value ();
    }
    const char*
    djnn_get_string_value (djnn::CoreProcess* p)
    {
        return ((djnn::AbstractProperty*)p)->get_string_value ().c_str ();
    }
    void
    djnn_dump (djnn::CoreProcess* p)
    {
        p->dump ();
    }
    djnn::MainLoop*
    djnn_mainloop_instance ()
    {
        return &djnn::MainLoop::instance ();
    }
    djnn::Rectangle*
    djnn_new_Rectangle (djnn::ParentProcess* parent, const char* name, double x, double y, double width, double height, double rx = 0, double ry = 0)
    {
        return new djnn::Rectangle (parent, name, x, y, width, height, rx, ry);
    }
    djnn::Circle*
    djnn_new_Circle (djnn::ParentProcess* parent, const char* name, double cx, double cy, double r)
    {
        return new djnn::Circle (parent, name, cx, cy, r);
    }
    djnn::Ellipse*
    djnn_new_Ellipse (djnn::ParentProcess* parent, const char* name, double cx, double cy, double rx, double ry)
    {
        return new djnn::Ellipse (parent, name, cx, cy, rx, ry);
    }
    djnn::Line*
    djnn_new_Line (djnn::ParentProcess* parent, const char* name, double x1, double y1, double x2, double y2)
    {
        return new djnn::Line (parent, name, x1, y1, x2, y2);
    }
    djnn::RectangleClip*
    djnn_new_RectangleClip (djnn::ParentProcess* parent, const char* name, double x, double y, double width, double height)
    {
        return new djnn::RectangleClip (parent, name, x, y, width, height);
    }
    djnn::PathImage*
    djnn_new_PathImage (djnn::ParentProcess* parent, const char* name, const char* path, double x, double y, double width, double height)
    {
        return new djnn::PathImage (parent, name, path, x, y, width, height);
    }
    djnn::DataImage*
    djnn_new_DataImage (djnn::ParentProcess* parent, const char* name, const char* data, int format, double x, double y, double width, double height)
    {
        return new djnn::DataImage (parent, name, data, format, x, y, width, height);
    }
    djnn::FillRule*
    djnn_new_FillRule (djnn::ParentProcess* parent, const char* name, int rule)
    {
        return new djnn::FillRule (parent, name, rule);
    }
    djnn::Texture*
    djnn_new_Texture (djnn::ParentProcess* parent, const char* name, const char* path)
    {
        return new djnn::Texture (parent, name, path);
    }
    djnn::OutlineWidth*
    djnn_new_OutlineWidth (djnn::ParentProcess* parent, const char* name, double width)
    {
        return new djnn::OutlineWidth (parent, name, width);
    }
    djnn::OutlineCapStyle*
    djnn_new_OutlineCapStyle (djnn::ParentProcess* parent, const char* name, int cap)
    {
        return new djnn::OutlineCapStyle (parent, name, cap);
    }
    djnn::OutlineJoinStyle*
    djnn_new_OutlineJoinStyle (djnn::ParentProcess* parent, const char* name, int join)
    {
        return new djnn::OutlineJoinStyle (parent, name, join);
    }
    djnn::OutlineMiterLimit*
    djnn_new_OutlineMiterLimit (djnn::ParentProcess* parent, const char* name, int limit)
    {
        return new djnn::OutlineMiterLimit (parent, name, limit);
    }
    djnn::DashOffset*
    djnn_new_DashOffset (djnn::ParentProcess* parent, const char* name, double offset)
    {
        return new djnn::DashOffset (parent, name, offset);
    }
    djnn::GradientStop*
    djnn_new_GradientStop (djnn::ParentProcess* parent, const char* name, double r, double g, double b, double a, double offset)
    {
        return new djnn::GradientStop (parent, name, r, g, b, a, offset);
    }
    djnn::LinearGradient*
    djnn_new_LinearGradient (djnn::ParentProcess* parent, const char* name, double x1, double y1, double x2, double y2, int spread, int coords)
    {
        return new djnn::LinearGradient (parent, name, x1, y1, x2, y2, spread, coords);
    }
    djnn::RadialGradient*
    djnn_new_RadialGradient (djnn::ParentProcess* parent, const char* name, double cx, double cy, double r, double fx, double fy, int spread, int coords)
    {
        return new djnn::RadialGradient (parent, name, cx, cy, r, fx, fy, spread, coords);
    }
    djnn::FontSize*
    djnn_new_FontSize (djnn::ParentProcess* parent, const char* name, int unit, double size)
    {
        return new djnn::FontSize (parent, name, unit, size);
    }
    djnn::FontWeight*
    djnn_new_FontWeight (djnn::ParentProcess* parent, const char* name, int weight)
    {
        return new djnn::FontWeight (parent, name, weight);
    }
    djnn::FontStyle*
    djnn_new_FontStyle (djnn::ParentProcess* parent, const char* name, int style)
    {
        return new djnn::FontStyle (parent, name, style);
    }
    djnn::FontFamily*
    djnn_new_FontFamily (djnn::ParentProcess* parent, const char* name, const char* family)
    {
        return new djnn::FontFamily (parent, name, family);
    }
    djnn::TextAnchor*
    djnn_new_TextAnchor (djnn::ParentProcess* parent, const char* name, int anchor)
    {
        return new djnn::TextAnchor (parent, name, anchor);
    }
    djnn::Translation*
    djnn_new_Translation (djnn::ParentProcess* parent, const char* name, double tx, double ty)
    {
        return new djnn::Translation (parent, name, tx, ty);
    }
    djnn::Rotation*
    djnn_new_Rotation (djnn::ParentProcess* parent, const char* name, double a, double cx, double cy)
    {
        return new djnn::Rotation (parent, name, a, cx, cy);
    }
    djnn::Scaling*
    djnn_new_Scaling (djnn::ParentProcess* parent, const char* name, double sx, double sy, double cx, double cy)
    {
        return new djnn::Scaling (parent, name, sx, sy, cx, cy);
    }
    djnn::Homography*
    djnn_new_Homography (djnn::ParentProcess* parent, const char* name, double m11 = 1, double m12 = 0, double m13 = 0, double m14 = 0, double m21 = 0, double m22 = 1, double m23 = 0, double m24 = 0, double m31 = 0, double m32 = 0, double m33 = 1, double m34 = 0, double m41 = 0, double m42 = 0, double m43 = 0, double m44 = 1)
    {
        return new djnn::Homography (parent, name, m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
    }
    djnn::Volume*
    djnn_new_Volume (djnn::ParentProcess* parent, const char* name, double volume)
    {
        return new djnn::Volume (parent, name, volume);
    }
    djnn::Plane*
    djnn_new_Plane (djnn::ParentProcess* parent, const char* name, double a, double b, double c, double d)
    {
        return new djnn::Plane (parent, name, a, b, c, d);
    }
    djnn::Binding*
    djnn_new_Binding (djnn::ParentProcess* parent, const char* name, djnn::CoreProcess* src, djnn::activation_flag_e src_flag, djnn::CoreProcess* dst, djnn::activation_flag_e dst_flag)
    {
        return new djnn::Binding (parent, name, src, src_flag, dst, dst_flag);
    }
    djnn::Assignment*
    djnn_new_Assignment (djnn::ParentProcess* parent, const char* name, djnn::CoreProcess* src, djnn::CoreProcess* dst, bool is_model)
    {
        return new djnn::Assignment (parent, name, src, dst, is_model);
    }
    djnn::Synchronizer*
    djnn_new_Synchronizer (djnn::ParentProcess* parent, const char* name, djnn::CoreProcess* dst, const char* spec)
    {
        return new djnn::Synchronizer (parent, name, dst, spec);
    }
    djnn::Activator*
    djnn_new_Activator (djnn::ParentProcess* parent, const char* name, djnn::CoreProcess* src)
    {
        return new djnn::Activator (parent, name, src);
    }
    djnn::NativeAction*
    djnn_new_NativeAction (djnn::ParentProcess* parent, const char* name, const char* action, void* data, bool is_model)
    {
        return new djnn::NativeAction (parent, name, action, data, is_model);
    }
    djnn::Blank*
    djnn_new_Blank (djnn::ParentProcess* parent, const char* name)
    {
        return new djnn::Blank (parent, name);
    }
    djnn::Component*
    djnn_new_Component (djnn::ParentProcess* parent, const char* name)
    {
        return new djnn::Component (parent, name);
    }
    djnn::AssignmentSequence*
    djnn_new_AssignmentSequence (djnn::ParentProcess* parent, const char* name, bool is_model)
    {
        return new djnn::AssignmentSequence (parent, name, is_model);
    }
    djnn::List*
    djnn_new_List (djnn::ParentProcess* parent, const char* name)
    {
        return new djnn::List (parent, name);
    }
    djnn::ListIterator*
    djnn_new_ListIterator (djnn::ParentProcess* parent, const char* name, djnn::CoreProcess* list, djnn::CoreProcess* action, bool is_model)
    {
        return new djnn::ListIterator (parent, name, list, action, is_model);
    }
    djnn::Set*
    djnn_new_Set (djnn::ParentProcess* parent, const char* name)
    {
        return new djnn::Set (parent, name);
    }
    djnn::SetIterator*
    djnn_new_SetIterator (djnn::ParentProcess* parent, const char* name, djnn::CoreProcess* set, djnn::CoreProcess* action, bool is_model)
    {
        return new djnn::SetIterator (parent, name, set, action, is_model);
    }
    djnn::Spike*
    djnn_new_Spike (djnn::ParentProcess* parent, const char* name)
    {
        return new djnn::Spike (parent, name);
    }
    djnn::RefProperty*
    djnn_new_RefProperty (djnn::ParentProcess* parent, const char* name, djnn::CoreProcess* ref)
    {
        return new djnn::RefProperty (parent, name, ref);
    }
    djnn::RemoteProperty*
    djnn_new_RemoteProperty (djnn::ParentProcess* parent, const char* name, const char* spec)
    {
        return new djnn::RemoteProperty (parent, name, spec);
    }
    djnn::IntProperty*
    djnn_new_IntProperty (djnn::ParentProcess* parent, const char* name, int value)
    {
        return new djnn::IntProperty (parent, name, value);
    }
    djnn::DoubleProperty*
    djnn_new_DoubleProperty (djnn::ParentProcess* parent, const char* name, double value)
    {
        return new djnn::DoubleProperty (parent, name, value);
    }
    djnn::TextProperty*
    djnn_new_TextProperty (djnn::ParentProcess* parent, const char* name, const char* value)
    {
        return new djnn::TextProperty (parent, name, value);
    }
    djnn::BoolProperty*
    djnn_new_BoolProperty (djnn::ParentProcess* parent, const char* name, bool value)
    {
        return new djnn::BoolProperty (parent, name, value);
    }
    djnn::Timer*
    djnn_new_Timer (djnn::ParentProcess* parent, const char* name, int delay)
    {
        return new djnn::Timer (parent, name, delay);
    }
    djnn::Exit*
    djnn_new_Exit (djnn::ParentProcess* parent, const char* name, int value, bool is_model)
    {
        return new djnn::Exit (parent, name, value, is_model);
    }
    djnn::Connector*
    djnn_new_Connector (djnn::ParentProcess* parent, const char* name, djnn::CoreProcess* src, djnn::CoreProcess* dst, bool copy_on_activation)
    {
        return new djnn::Connector (parent, name, src, dst, copy_on_activation);
    }
    djnn::Clock*
    djnn_new_Clock (djnn::ParentProcess* parent, const char* name, int period)
    {
        return new djnn::Clock (parent, name, period);
    }
    djnn::Counter*
    djnn_new_Counter (djnn::ParentProcess* parent, const char* name, int init, int delta)
    {
        return new djnn::Counter (parent, name, init, delta);
    }
    djnn::Deref*
    djnn_new_Deref (djnn::ParentProcess* parent, const char* name, djnn::CoreProcess* ref_prop, const char* path, djnn::djnn_dir_t dir)
    {
        return new djnn::Deref (parent, name, ref_prop, path, dir);
    }
    djnn::DerefDouble*
    djnn_new_DerefDouble (djnn::ParentProcess* parent, const char* name, djnn::CoreProcess* ref_prop, const char* path, djnn::djnn_dir_t dir)
    {
        return new djnn::DerefDouble (parent, name, ref_prop, path, dir);
    }
    djnn::DerefString*
    djnn_new_DerefString (djnn::ParentProcess* parent, const char* name, djnn::CoreProcess* ref_prop, const char* path, djnn::djnn_dir_t dir)
    {
        return new djnn::DerefString (parent, name, ref_prop, path, dir);
    }
    djnn::Dictionary*
    djnn_new_Dictionary (djnn::ParentProcess* parent, const char* name)
    {
        return new djnn::Dictionary (parent, name);
    }
    djnn::Finder*
    djnn_new_Finder (djnn::ParentProcess* parent, const char* name, djnn::FatProcess* container, const char* path)
    {
        return new djnn::Finder (parent, name, container, path);
    }
    djnn::FSM*
    djnn_new_FSM (djnn::ParentProcess* parent, const char* name)
    {
        return new djnn::FSM (parent, name);
    }
    djnn::FSMTransition*
    djnn_new_FSMTransition (djnn::ParentProcess* parent, const char* name, djnn::CoreProcess* from, djnn::CoreProcess* to, djnn::CoreProcess* trigger, djnn::CoreProcess* action)
    {
        return new djnn::FSMTransition (parent, name, from, to, trigger, action);
    }
    djnn::FSMState*
    djnn_new_FSMState (djnn::ParentProcess* parent, const char* name)
    {
        return new djnn::FSMState (parent, name);
    }
    djnn::HermiteCurve*
    djnn_new_HermiteCurve (djnn::ParentProcess* parent, const char* name, double p1, double p2, double t1, double t2)
    {
        return new djnn::HermiteCurve (parent, name, p1, p2, t1, t2);
    }
    djnn::LogPrinter*
    djnn_new_LogPrinter (djnn::ParentProcess* parent, const char* name, const char* label)
    {
        return new djnn::LogPrinter (parent, name, label);
    }
    djnn::SwitchList*
    djnn_new_SwitchList (djnn::ParentProcess* parent, const char* name, bool loop)
    {
        return new djnn::SwitchList (parent, name, loop);
    }
    djnn::SwitchRange*
    djnn_new_SwitchRange (djnn::ParentProcess* parent, const char* name, double initial)
    {
        return new djnn::SwitchRange (parent, name, initial);
    }
    djnn::Switch*
    djnn_new_Switch (djnn::ParentProcess* parent, const char* name, const char* initial)
    {
        return new djnn::Switch (parent, name, initial);
    }
    djnn::TextPrinter*
    djnn_new_TextPrinter (djnn::ParentProcess* parent, const char* name)
    {
        return new djnn::TextPrinter (parent, name);
    }
    djnn::Regex*
    djnn_new_Regex (djnn::ParentProcess* parent, const char* name, const char* regex)
    {
        return new djnn::Regex (parent, name, regex);
    }
    djnn::TextAccumulator*
    djnn_new_TextAccumulator (djnn::ParentProcess* parent, const char* name, const char* init)
    {
        return new djnn::TextAccumulator (parent, name, init);
    }
    djnn::TextComparator*
    djnn_new_TextComparator (djnn::ParentProcess* parent, const char* name)
    {
        return new djnn::TextComparator (parent, name);
    }
    djnn::TextCatenator*
    djnn_new_TextCatenator (djnn::ParentProcess* parent, const char* name)
    {
        return new djnn::TextCatenator (parent, name);
    }
    djnn::WallClock*
    djnn_new_WallClock (djnn::ParentProcess* parent, const char* name)
    {
        return new djnn::WallClock (parent, name);
    }
    djnn::Window*
    djnn_new_Window (djnn::ParentProcess* parent, const char* name, const char* title, double x, double y, double width, double height)
    {
        return new djnn::Window (parent, name, title, x, y, width, height);
    }
    djnn::FillColor_rgb*
    djnn_new_FillColor_rgb (djnn::ParentProcess* parent, const char* name, double r, double g, double b)
    {
        return new djnn::FillColor_rgb (parent, name, r, g, b);
    }
    djnn::FillColor_value*
    djnn_new_FillColor_value (djnn::ParentProcess* parent, const char* name, int value)
    {
        return new djnn::FillColor_value (parent, name, value);
    }
    djnn::OutlineColor*
    djnn_new_OutlineColor (djnn::ParentProcess* parent, const char* name, double r, double g, double b)
    {
        return new djnn::OutlineColor (parent, name, r, g, b);
    }
    djnn::Text*
    djnn_new_Text (djnn::ParentProcess* parent, const char* name, double x, double y, const char* text)
    {
        return new djnn::Text (parent, name, x, y, text);
    }
}
