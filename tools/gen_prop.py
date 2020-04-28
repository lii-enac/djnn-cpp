#!/usr/bin/env python3

# cd - ; python3 gen_prop.py  ; cd -

import re

copyright = """/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 *  !! this file has been automatically generated - do NOT modify !!
 *
 */

"""

decl_string = """
#pragma once

namespace djnn
{
  class %(CLASS)s : public %(INHERITS)s
  {
  public:
    %(CLASS)s (Process *parent, const std::string& name, %(DECL_PROPS_CALL_DECL)s);
    virtual ~%(CLASS)s ();
    %(DECL_DRAW)s
    %(DECL_CLONE)s
    void get_properties_values (%(DECL_PROPS_REF_CALL)s);
    virtual Process* find_child (const std::string&) override;
%(PROP_GETTERS)s
  protected:
    struct raw_props_t { %(DECL_PROPS_STRUCT)s; };
    raw_props_t raw_props;
    Coupling %(DECL_PROPS_COUPLING_POINTER)s;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
"""

getter_string = """Abstract%(PROP_TYPE)sProperty* %(PROP_NAME)s () { return (Abstract%(PROP_TYPE)sProperty*) find_child ("%(PROP_NAME)s"); }"""

def_string = """
#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display.h"
#include "display/abstract_display.h"
#include "display/window.h"
#include "gui/shapes/shapes.h"
#include "gui/style/style.h"
#include "core/ontology/process.h"
#include "core/ontology/coupling.h"

#include "audio/style/audio_style.h"


#include "%(CLASS_LOWER)s.h"

namespace djnn
{
  %(CLASS)s::%(CLASS)s (Process *parent, const std::string& name, %(DECL_PROPS_CALL_DEF)s) :
    %(INHERITS)s (parent, name%(FOLLOW_PARENT_PROPS_CALL)s),
    raw_props{%(RAW_PROPS_INIT)s},
    %(COUPLINGS_INIT)s
  {
    %(SET_ORIGIN)s
    %(FINALIZE_CONSTRUCTION)s
  }

  %(CLASS)s::~%(CLASS)s ()
  {
    %(DELETE_COUPLINGS)s;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (children_size () > %(ORIGIN_IN_SYMTABLE)s) {
      symtable_t::iterator it;

      %(DELETE_DYN_PROPS)s
    }
  }
 
  Process*
  %(CLASS)s::find_child (const std::string& name)
  {
    Process* res = %(INHERITS)s::find_child(name);
    if(res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    Coupling ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef std::string text;
    text* rawp_Text = nullptr;
    int notify_mask = notify_none;
    %(CREATE_PROPERTIES)s
    return nullptr;

    if(prop_Double) {
      DoublePropertyProxy* prop = nullptr; // do not cache
      res = create_GObj_prop(&prop, coupling, rawp_Double, name, notify_mask);
    }
    else if(prop_Int) {
      IntPropertyProxy* prop = nullptr; // do not cache
      res = create_GObj_prop(&prop, coupling, rawp_Int, name, notify_mask);
    }
    else if(prop_Text) {
      TextPropertyProxy* prop = nullptr; // do not cache
      res = create_GObj_prop(&prop, coupling, rawp_Text, name, notify_mask);
    }

    return res;
  }

  void
  %(CLASS)s::get_properties_values (%(DECL_PROPS_REF_CALL)s)
  {
    %(DEF_PROPS_REF_SET)s;
    %(GET_PROPS_FROM_PARENT)s
  }

  void
  %(CLASS)s::impl_activate ()
  {
    %(INHERITS)s::impl_activate ();
    auto _frame = frame ();
    %(DEF_COUPLINGS_ENABLE)s;
  }

  void
  %(CLASS)s::impl_deactivate ()
  {
    %(DEF_COUPLINGS_DISABLE)s;
    %(INHERITS)s::impl_deactivate ();
  }

  %(DEF_DRAW)s

  %(DEF_CLONE)s

} /* namespace djnn */
"""

def_draw = """
  void
  %(CLASS)s::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->%(DRAW_METHOD)s_%(CLASS_LOWER)s (this);
    }
  }
"""

def_clone = """
  Process*
  %(CLASS)s::clone ()
  {
    return new %(CLASS)s (nullptr, get_name (), %(RAW_PROP_PARAMS)s);
  }
"""

get_or_create_property_string = """
    if(name=="%(PROP_NAME)s") {
      coupling=&_c%(PROP_NAME)s;
      rawp_%(PROP_TYPE)s=&raw_props.%(PROP_NAME)s;
      notify_mask = notify_damaged_%(PROP_DAMAGE)s;
      prop_%(PROP_TYPE)s=true;
    } else"""


class Prop:

    def __init__(self, name, type_, default_value, damage):
        self.name = name
        self.type = type_
        self.default_value = default_value
        self.damage = damage
        self.origin = "x,y"

    def as_cpp_type_str(self):
        if self.type == 'text':
          return 'std::string'
        else:
          return self.type

    def as_cpp_type_str_for_function_signature(self):
        if self.type == 'text':
          return 'const std::string&'
        else:
          return self.type

    def __str__(self):
      return "prop " + self.as_cpp_type_str() + " " + self.name


def flatten(l):
  return [item for sublist in l for item in sublist]

class DjnnClass:

    parent_prop_pos_beg = 0
    parent_prop_pos_end = 1

    def __init__(self, name, inherits, path, origin="x, y", finalize_construction=True, parent_prop=None, parent_prop_pos=parent_prop_pos_beg, emit_clone=None):
        self.name = name
        self.inherits = inherits
        self.props = []
        self.path = path
        self.origin = origin
        self.draw_method = "draw" if self.origin else "load"
        self.finalize_construction = finalize_construction
        self.parent_prop = parent_prop
        self.parent_prop_pos = parent_prop_pos
        if emit_clone == None:
          self.emit_clone = finalize_construction
        else:
          self.emit_clone = emit_clone

    def get_parent_prop_list(self):
      res = []
      if self.parent_prop:
        dpcd = self.parent_prop.get_all_prop_list()
        if self.parent_prop_pos == DjnnClass.parent_prop_pos_beg:
          dpcd.extend(res)
          res = dpcd
        else:
          res.extend(dpcd)
      return res

    def get_all_prop_list(self):
      res = self.props.copy()
      if self.parent_prop:
        dpcd = self.parent_prop.get_all_prop_list()
        if self.parent_prop_pos == DjnnClass.parent_prop_pos_beg:
          dpcd.extend(res)
          res = dpcd
        else:
          res.extend(dpcd)
      return res

    #def prop_call_decl(self):
    #  dpcd = self.get_parent_prop_list ()
    #  DECL_PROPS_CALL_DECL = ', '.join([p.as_cpp_type_str() + ' ' + p.name + (("=" + p.default_value) if p.default_value != None else "") for p in dpcd])   
    #  return DECL_PROPS_CALL_DECL



first_cap_re = re.compile('(.)([A-Z][a-z]+)')
all_cap_re = re.compile('([a-z0-9])([A-Z])')
def camel_case_to_snake_case(name):
    s1 = first_cap_re.sub(r'\1_\2', name)
    return all_cap_re.sub(r'\1_\2', s1).lower()

def just_do_it(dc, finalize_construction=True):
    # print(dc.name, dc.inherits)
    all_props = dc.get_all_prop_list ()
    parent_props = dc.get_parent_prop_list ()

    #if dc.name == "LinearGradient":
    #  print(all_props)
    #  print(parent_props)

    PARENT_PROPS_CALL = ', '.join([p.name for p in parent_props])

    DECL_PROPS_CALL_DECL = ', '.join([p.as_cpp_type_str_for_function_signature() + ' ' + p.name + (("=" + p.default_value) if p.default_value != None else "") for p in all_props])
    # print (DECL_PROPS_CALL)
    DECL_PROPS_REF_CALL = ', '.join([p.as_cpp_type_str() + '& ' + p.name for p in all_props])
    # print (DECL_PROPS_REF_CALL)
    DECL_PROPS_STRUCT = '; '.join([p.as_cpp_type_str() + ' ' + p.name for p in dc.props])
    # print (DECL_PROPS_STRUCT)
    DECL_PROPS_COUPLING_POINTER = ', '.join(['*_c' + p.name for p in dc.props])
    # print (DECL_PROPS_COUPLING_POINTER)

    PROP_GETTERS = ''
    CREATE_PROPERTIES = ''
    for prop in dc.props:
        # print(prop.name, prop.type)
        d = {'PROP_NAME': prop.name,
            'PROP_TYPE': prop.type.capitalize(),
            'PROP_DAMAGE': prop.damage
            }
        # print(d)
        PROP_GETTERS += "\t\t"+getter_string % d + "\n"
        CREATE_PROPERTIES += get_or_create_property_string % d

    # print (CREATE_PROPERTIES)

    DECL_PROPS_CALL_DEF = ', '.join([p.as_cpp_type_str_for_function_signature() + ' ' + p.name for p in all_props])
  
    RAW_PROPS_INIT = ', '.join(['.' + p.name + '=' + p.name for p in dc.props])
    # print (RAW_PROPS_INIT)
    join_str = '\n\t\t'
    COUPLINGS_INIT = (', ').join(['_c' + p.name + ' (nullptr)' for p in dc.props])
    # print (COUPLINGS_INIT)
    DELETE_COUPLINGS = (';'+join_str).join(['delete _c' + p.name for p in dc.props])
    # print (DELETE_COUPLINGS)
    DELETE_DYN_PROPS = ('\n'+join_str+'\t').join([ 'it = find_child_iterator ("' + p.name + '");\n\t\t\tif (it != children_end ())\n\t\t\t\tdelete it->second;' for p in dc.props])
    #print (DELETE_DYN_PROPS)
    DEF_PROPS_REF_SET = (';'+join_str).join([p.name + ' = raw_props.'  + p.name for p in dc.props])
    # print (DEF_PROPS_REF_SET)
    if(dc.inherits and dc.get_parent_prop_list() != []):
      DECL_PROPS_CALL = ', '.join([p.name for p in dc.get_parent_prop_list()])
      GET_PROPS_FROM_PARENT = dc.inherits+"::get_properties_values("+DECL_PROPS_CALL+");"
    else:
      GET_PROPS_FROM_PARENT = ""
    #
    DEF_COUPLINGS_ENABLE = (';'+join_str).join([ 'if(_c' + p.name + ') _c'+p.name+'->enable (_frame)' for p in dc.props])
    #DEF_COUPLINGS_ENABLE = (';'+join_str).join([ 'if(_c' + p.name + ') _c'+p.name+'->enable (&*frame_sp)' for p in dc.props])
    # print (DEF_COUPLINGS_ENABLE)
    DEF_COUPLINGS_DISABLE = (';'+join_str).join([ 'if(_c' + p.name + ') _c'+p.name+'->disable ()' for p in dc.props])
    RAW_PROP_PARAMS = ', '.join(['raw_props.' + p.name for p in dc.props])
    # print (RAW_PROP_PARAMS)

    SET_ORIGIN = ''
    ORIGIN_IN_SYMTABLE = '0'
    #if(len(dc.props)>2):
    #    SET_ORIGIN = "set_origin (" + dc.props[0].name + ', ' + dc.props[1].name + ');'
    if(dc.origin):
      SET_ORIGIN = "set_origin (" + dc.origin + ');'
      ORIGIN_IN_SYMTABLE = '2'
      
    FINALIZE_CONSTRUCTION = ''
    if(dc.finalize_construction):
        FINALIZE_CONSTRUCTION = "finalize_construction (parent, name);"
    # print (SET_ORIGIN)

    DECL_CLONE = ''
    if(dc.emit_clone):
      DECL_CLONE = "Process* clone () override;"

    DECL_DRAW = ''
    if(dc.finalize_construction):
      DECL_DRAW = 'void draw () override;'


    d = {
        'CLASS': dc.name,
        'CLASS_LOWER': camel_case_to_snake_case (dc.name),
        'INHERITS': dc.inherits,
        'PARENT_PROPS_CALL': PARENT_PROPS_CALL,
        'FOLLOW_PARENT_PROPS_CALL': (', ' + PARENT_PROPS_CALL) if len(PARENT_PROPS_CALL) else '',
        'DECL_PROPS_CALL_DECL': DECL_PROPS_CALL_DECL,
        'DECL_PROPS_REF_CALL': DECL_PROPS_REF_CALL,
        'DECL_PROPS_STRUCT': DECL_PROPS_STRUCT,
        'DECL_PROPS_COUPLING_POINTER': DECL_PROPS_COUPLING_POINTER,
        'PROP_GETTERS': PROP_GETTERS,
        'CREATE_PROPERTIES': CREATE_PROPERTIES,
        'DECL_PROPS_CALL_DEF': DECL_PROPS_CALL_DEF,
        'RAW_PROPS_INIT': RAW_PROPS_INIT,
        'COUPLINGS_INIT': COUPLINGS_INIT,
        'DELETE_COUPLINGS': DELETE_COUPLINGS,
        'DELETE_DYN_PROPS': DELETE_DYN_PROPS,
        'DEF_PROPS_REF_SET': DEF_PROPS_REF_SET,
        'GET_PROPS_FROM_PARENT': GET_PROPS_FROM_PARENT,
        'DEF_COUPLINGS_ENABLE': DEF_COUPLINGS_ENABLE,
        'DEF_COUPLINGS_DISABLE': DEF_COUPLINGS_DISABLE,
        'RAW_PROP_PARAMS': RAW_PROP_PARAMS,
        'SET_ORIGIN': SET_ORIGIN,
        'ORIGIN_IN_SYMTABLE': ORIGIN_IN_SYMTABLE,
        'FINALIZE_CONSTRUCTION': FINALIZE_CONSTRUCTION,
        'DECL_CLONE': DECL_CLONE,
        'DECL_DRAW': DECL_DRAW,
        'DRAW_METHOD': dc.draw_method,
    }

    d['DEF_CLONE'] = ''
    if(dc.emit_clone):
      d['DEF_CLONE'] = def_clone % d
    d['DEF_DRAW'] = ''
    if(dc.finalize_construction):
      d['DEF_DRAW'] = def_draw % d

    decl_content = copyright + decl_string % d
    def_content = copyright + def_string % d

    filename = camel_case_to_snake_case (dc.name)
    path = dc.path + '/gen/' + filename
    open (path+'.h', 'w').write(decl_content)
    open (path+'.cpp', 'w').write(def_content)




# ------------------------

dcs = []

# -- shapes

dc = DjnnClass("AbstractPropRectangle", "AbstractGShape", "../src/gui/shapes", finalize_construction=False)
dc.props.append(Prop('x', 'double', None, "transform"))
dc.props.append(Prop('y', 'double', None, "transform"))
dc.props.append(Prop('width', 'double', None, "geometry"))
dc.props.append(Prop('height', 'double', None, "geometry"))
dc.props.append(Prop('rx', 'double', "0", "geometry"))
dc.props.append(Prop('ry', 'double', "0", "geometry"))
dcs.append(dc)

dc = DjnnClass("AbstractPropCircle", "AbstractGShape", "../src/gui/shapes", origin='cx,cy', finalize_construction=False)
dc.props.append(Prop('cx', 'double', None, "transform"))
dc.props.append(Prop('cy', 'double', None, "transform"))
dc.props.append(Prop('r', 'double', None, "geometry"))
dcs.append(dc)

dc = DjnnClass("AbstractPropEllipse", "AbstractGShape", "../src/gui/shapes", origin='cx,cy', finalize_construction=False)
dc.props.append(Prop('cx', 'double', None, "transform"))
dc.props.append(Prop('cy', 'double', None, "transform"))
dc.props.append(Prop('rx', 'double', None, "geometry"))
dc.props.append(Prop('ry', 'double', None, "geometry"))
dcs.append(dc)

dc = DjnnClass("AbstractPropLine", "AbstractGShape", "../src/gui/shapes", origin='x1,y1', finalize_construction=False)
dc.props.append(Prop('x1', 'double', None, "transform"))
dc.props.append(Prop('y1', 'double', None, "transform"))
dc.props.append(Prop('x2', 'double', None, "geometry"))
dc.props.append(Prop('y2', 'double', None, "geometry"))
dcs.append(dc)

dc = DjnnClass("AbstractPropRectangleClip", "AbstractGShape", "../src/gui/shapes", finalize_construction=False)
dc.props.append(Prop('x', 'double', None, "transform"))
dc.props.append(Prop('y', 'double', None, "transform"))
dc.props.append(Prop('width', 'double', None, "geometry"))
dc.props.append(Prop('height', 'double', None, "geometry"))
dcs.append(dc)

dc = DjnnClass("AbstractImage", "AbstractGShape", "../src/gui/shapes", finalize_construction=False)
#dc.props.append(Prop('path', 'text', None, "geometry"))
dc.props.append(Prop('x', 'double', None, "transform"))
dc.props.append(Prop('y', 'double', None, "transform"))
dc.props.append(Prop('width', 'double', None, "geometry"))
dc.props.append(Prop('height', 'double', None, "geometry"))
dcs.append(dc)
apg = dc

dc = DjnnClass("AbstractPathImage", "AbstractImage", "../src/gui/shapes", finalize_construction=False, parent_prop = apg, parent_prop_pos = DjnnClass.parent_prop_pos_end)
dc.props.append(Prop('path', 'text', None, "geometry"))
dcs.append(dc)

dc = DjnnClass("AbstractDataImage", "AbstractImage", "../src/gui/shapes", finalize_construction=False, parent_prop = apg, parent_prop_pos = DjnnClass.parent_prop_pos_end)
dc.props.append(Prop('data', 'text', None, "geometry"))
dcs.append(dc)


# -- styles

# dc = DjnnClass("AbstractColor", "AbstractStyle", "../src/gui/style", origin=None, finalize_construction=False)
# dc.props.append(Prop('r', 'double', None, "style"))
# dc.props.append(Prop('g', 'double', None, "style"))
# dc.props.append(Prop('b', 'double', None, "style"))
# dcs.append(dc)

dc = DjnnClass("FillRule", "AbstractStyle", "../src/gui/style", origin=None)
dc.props.append(Prop('rule', 'int', None, "geometry"))
dcs.append(dc)

dc = DjnnClass("Texture", "AbstractStyle", "../src/gui/style", origin=None)
dc.props.append(Prop('path', 'text', None, "style"))
dcs.append(dc)

dc = DjnnClass("AbstractOpacity", "AbstractStyle", "../src/gui/style", origin=None, finalize_construction=False)
dc.props.append(Prop('a', 'double', None, "style"))
dcs.append(dc)

dc = DjnnClass("OutlineWidth", "AbstractStyle", "../src/gui/style", origin=None)
dc.props.append(Prop('width', 'double', None, "geometry"))
dcs.append(dc)

dc = DjnnClass("OutlineCapStyle", "AbstractStyle", "../src/gui/style", origin=None)
dc.props.append(Prop('cap', 'int', None, "geometry"))
dcs.append(dc)

dc = DjnnClass("OutlineJoinStyle", "AbstractStyle", "../src/gui/style", origin=None)
dc.props.append(Prop('join', 'int', None, "geometry"))
dcs.append(dc)

dc = DjnnClass("OutlineMiterLimit", "AbstractStyle", "../src/gui/style", origin=None)
dc.props.append(Prop('limit', 'int', None, "geometry"))
dcs.append(dc)

dc = DjnnClass("DashOffset", "AbstractStyle", "../src/gui/style", origin=None)
dc.props.append(Prop('offset', 'double', None, "geometry"))
dcs.append(dc)

dc = DjnnClass("AbstractPropGradientStop", "AbstractStyle", "../src/gui/style", origin=None, finalize_construction=False)
dc.props.append(Prop('r', 'double', None, "style"))
dc.props.append(Prop('g', 'double', None, "style"))
dc.props.append(Prop('b', 'double', None, "style"))
dc.props.append(Prop('a', 'double', None, "style"))
dc.props.append(Prop('offset', 'double', None, "style"))
dcs.append(dc)

dc = DjnnClass("AbstractPropGradient", "AbstractStyle", "../src/gui/style", origin=None, finalize_construction=False)
dc.props.append(Prop('spread', 'int', None, "style"))
dc.props.append(Prop('coords', 'int', None, "style"))
dcs.append(dc)
apg = dc

dc = DjnnClass("AbstractPropLinearGradient", "AbstractGradient", "../src/gui/style", origin=None, finalize_construction=False, parent_prop = apg, parent_prop_pos = DjnnClass.parent_prop_pos_end)
dc.props.append(Prop('x1', 'double', None, "style"))
dc.props.append(Prop('y1', 'double', None, "style"))
dc.props.append(Prop('x2', 'double', None, "style"))
dc.props.append(Prop('y2', 'double', None, "style"))
dcs.append(dc)

dc = DjnnClass("AbstractPropRadialGradient", "AbstractGradient", "../src/gui/style", origin=None, finalize_construction=False, parent_prop = apg, parent_prop_pos = DjnnClass.parent_prop_pos_end)
dc.props.append(Prop('cx', 'double', None, "style"))
dc.props.append(Prop('cy', 'double', None, "style"))
dc.props.append(Prop('r', 'double', None, "style"))
dc.props.append(Prop('fx', 'double', None, "style"))
dc.props.append(Prop('fy', 'double', None, "style"))
dcs.append(dc)

dc = DjnnClass("AbstractPropFontSize", "AbstractStyle", "../src/gui/style", origin=None, finalize_construction=False)
dc.props.append(Prop('unit', 'int', None, "geometry"))
dc.props.append(Prop('size', 'double', None, "geometry"))
dcs.append(dc)

dc = DjnnClass("AbstractPropFontWeight", "AbstractStyle", "../src/gui/style", origin=None, finalize_construction=False)
dc.props.append(Prop('weight', 'int', None, "geometry"))
dcs.append(dc)

dc = DjnnClass("AbstractPropFontStyle", "AbstractStyle", "../src/gui/style", origin=None, finalize_construction=False)
dc.props.append(Prop('style', 'int', None, "geometry"))
dcs.append(dc)

dc = DjnnClass("AbstractPropFontFamily", "AbstractStyle", "../src/gui/style", origin=None, finalize_construction=False)
dc.props.append(Prop('family', 'text', None, "geometry"))
dcs.append(dc)

dc = DjnnClass("TextAnchor", "AbstractStyle", "../src/gui/style", origin=None)
dc.props.append(Prop('anchor', 'int', None, "transform"))
dcs.append(dc)

dc = DjnnClass("AbstractTranslation", "AbstractTransformation", "../src/gui/transformation", origin=None, finalize_construction=False)
dc.props.append(Prop('tx', 'double', None, "transform"))
dc.props.append(Prop('ty', 'double', None, "transform"))
dcs.append(dc)

dc = DjnnClass("AbstractRotation", "AbstractTransformation", "../src/gui/transformation", origin=None, finalize_construction=False)
dc.props.append(Prop('a', 'double', None, "transform"))
dc.props.append(Prop('cx', 'double', None, "transform"))
dc.props.append(Prop('cy', 'double', None, "transform"))
dcs.append(dc)

dc = DjnnClass("AbstractScaling", "AbstractTransformation", "../src/gui/transformation", origin=None, finalize_construction=False)
dc.props.append(Prop('sx', 'double', None, "transform"))
dc.props.append(Prop('sy', 'double', None, "transform"))
dc.props.append(Prop('cx', 'double', None, "transform"))
dc.props.append(Prop('cy', 'double', None, "transform"))
dcs.append(dc)

dc = DjnnClass("AbstractSkew", "AbstractTransformation", "../src/gui/transformation", origin=None, finalize_construction=False)
dc.props.append(Prop('a', 'double', None, "transform"))
dcs.append(dc)

dc = DjnnClass("AbstractPropHomography", "AbstractTransformation", "../src/gui/transformation", origin=None, finalize_construction=False)
for i in range(1,5):
  for j in range(1,5):
    dc.props.append(Prop('m'+str(i)+str(j), 'double', str(1 if i==j else 0), "transform"))
dcs.append(dc)

dc = DjnnClass("AbstractVolume", "AbstractAudioStyle", "../src/audio/style", origin=None, finalize_construction=False)
dc.props.append(Prop('volume', 'double', None, "style"))
dcs.append(dc)


for dc in dcs:
    just_do_it(dc)

