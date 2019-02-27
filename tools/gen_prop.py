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
 */

"""

decl_string = """
namespace djnn
{
  class %(CLASS)s : public %(INHERITS)s
  {
  public:
    %(CLASS)s (Process *p, const std::string& n, %(DECL_PROPS_CALL_DECL)s);
    %(CLASS)s (%(DECL_PROPS_CALL_DECL)s);
    virtual ~%(CLASS)s ();
    void draw () override;
    Process* clone () override;
    void get_properties_values (%(DECL_PROPS_REF_CALL)s);
    virtual Process* find_component (const string&) override;
%(PROP_GETTERS)s
  private:
    struct raw_props_t { %(DECL_PROPS_STRUCT)s; };
    raw_props_t raw_props;
    Coupling %(DECL_PROPS_COUPLING_POINTER)s;
    void activate () override;
    void deactivate () override;
  };
}
"""

getter_string = """Abstract%(PROP_TYPE)sProperty* %(PROP_NAME)s () { return (Abstract%(PROP_TYPE)sProperty*) find_component("%(PROP_NAME)s"); }"""

def_string = """
#include "../backend.h"
#include "../abstract_backend.h"
#include "shapes.h"
#include "../../core/control/coupling.h"

namespace djnn
{
  %(CLASS)s::%(CLASS)s (Process *p, const std::string& n, %(DECL_PROPS_CALL_DEF)s):
    %(INHERITS)s (p, n),
    raw_props{%(RAW_PROPS_INIT)s},
    %(COUPLINGS_INIT)s
  {
    %(SET_ORIGIN)s
    Process::finalize ();
  }

  %(CLASS)s::%(CLASS)s (%(DECL_PROPS_CALL_DEF)s):
    %(INHERITS)s (), 
    raw_props{%(RAW_PROPS_INIT)s},
    %(COUPLINGS_INIT)s
  {
    %(SET_ORIGIN)s
  }

  %(CLASS)s::~%(CLASS)s ()
  {
    %(DELETE_COUPLINGS)s;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (_symtable.size () > 2) {
      std::map<std::string, Process*>::iterator it;

      %(DELETE_DYN_PROPS)s
    }
  }
 
  Process*
  %(CLASS)s::find_component (const string& name)
  {
    Process* res = %(INHERITS)s::find_component(name);
    if(res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    Coupling ** coupling;
    double* rawp_Double;
    int* rawp_Int;
    typedef string text;
    text* rawp_Text;
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
  }

  void
  %(CLASS)s::activate ()
  {
    AbstractGObj::activate ();
    %(DEF_COUPLINGS_ENABLE)s;
  }

  void
  %(CLASS)s::deactivate ()
  {
    AbstractGObj::deactivate ();
    %(DEF_COUPLINGS_DISABLE)s;
  }

  void
  %(CLASS)s::draw ()
  {
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_%(CLASS_LOWER)s (this);
    }
  }

  Process*
  %(CLASS)s::clone ()
  {
    return new %(CLASS)s (%(RAW_PROP_PARAMS)s);
  }
} /* namespace djnn */
"""

get_or_create_property_string = """
    if(name=="%(PROP_NAME)s") {
      coupling=&_c%(PROP_NAME)s;
      rawp_%(PROP_TYPE)s=&raw_props.%(PROP_NAME)s;
      notify_mask = notify_damaged_%(PROP_DAMAGE)s;
      prop_%(PROP_TYPE)s=true;
    } else"""


def just_do_it(dc):
    # print(dc.name, dc.inherits)
    DECL_PROPS_CALL_DECL = ', '.join([p.type + ' ' + p.name + (("=" + p.default_value) if p.default_value != None else "") for p in dc.props])
    # print (DECL_PROPS_CALL)
    DECL_PROPS_REF_CALL = ', '.join([p.type + '& ' + p.name for p in dc.props])
    # print (DECL_PROPS_REF_CALL)
    DECL_PROPS_STRUCT = '; '.join([p.type + ' ' + p.name for p in dc.props])
    # print (DECL_PROPS_STRUCT)
    DECL_PROPS_COUPLING_POINTER = ', '.join(['_c' + p.name for p in dc.props])
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

    # print (PROP_GETTERS)
    # print (CREATE_PROPERTIES)

    DECL_PROPS_CALL_DEF = ', '.join([p.type + ' ' + p.name for p in dc.props])
    RAW_PROPS_INIT = ', '.join(['.' + p.name + '=' + p.name for p in dc.props])
    # print (RAW_PROPS_INIT)
    join_str = '\n\t\t'
    COUPLINGS_INIT = (', ').join(['_c' + p.name + ' (nullptr)' for p in dc.props])
    # print (COUPLINGS_INIT)
    DELETE_COUPLINGS = (';'+join_str).join(['delete _c' + p.name for p in dc.props])
    # print (DELETE_COUPLINGS)
    DELETE_DYN_PROPS = ('\n'+join_str+'\t').join([ 'it = _symtable.find ("' + p.name + '");\n\t\t\tif (it != _symtable.end ())\n\t\t\t\tdelete it->second;' for p in dc.props])
    #print (DELETE_DYN_PROPS)
    DEF_PROPS_REF_SET = (';'+join_str).join([p.name + ' = raw_props.'  + p.name for p in dc.props])
    # print (DEF_PROPS_REF_SET)
    DEF_COUPLINGS_ENABLE = (';'+join_str).join([ 'if(_c' + p.name + ') _c'+p.name+'->enable (_frame)' for p in dc.props])
    # print (DEF_COUPLINGS_ENABLE)
    DEF_COUPLINGS_DISABLE = (';'+join_str).join([ 'if(_c' + p.name + ') _c'+p.name+'->disable ()' for p in dc.props])
    RAW_PROP_PARAMS = ', '.join(['raw_props.' + p.name for p in dc.props])
    # print (RAW_PROP_PARAMS)

    SET_ORIGIN = ''
    if(len(dc.props)>2):
        SET_ORIGIN = "set_origin(" + dc.props[0].name + ', ' + dc.props[1].name + ');'
    # print (SET_ORIGIN)

    d = {
        'CLASS': dc.name,
        'CLASS_LOWER': dc.name.lower(),
        'INHERITS': dc.inherits,
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
        'DEF_COUPLINGS_ENABLE': DEF_COUPLINGS_ENABLE,
        'DEF_COUPLINGS_DISABLE': DEF_COUPLINGS_DISABLE,
        'RAW_PROP_PARAMS': RAW_PROP_PARAMS,
        'SET_ORIGIN': SET_ORIGIN
    }

    decl_content = copyright + decl_string % d
    def_content = copyright + def_string % d

    filename = dc.name.lower()
    path = dc.path + '/' + filename
    # path = filename
    open (path+'.h', 'w').write(decl_content)
    open (path+'.cpp', 'w').write(def_content)    


class Prop:

    def __init__(self, name, type_, default_value, damage):
        self.name = name
        self.type = type_
        self.default_value = default_value
        self.damage = damage


class DjnnClass:

    def __init__(self, name, inherits, path):
        self.name = name
        self.inherits = inherits
        self.props = []
        self.path = path


# ------------------------

dcs = []

dc = DjnnClass("Rectangle", "AbstractGShape", "../src/gui/shapes")
dc.props.append(Prop('x', 'double', None, "transform"))
dc.props.append(Prop('y', 'double', None, "transform"))
dc.props.append(Prop('width', 'double', None, "geometry"))
dc.props.append(Prop('height', 'double', None, "geometry"))
dc.props.append(Prop('rx', 'double', "0", "geometry"))
dc.props.append(Prop('ry', 'double', "0", "geometry"))
#dcs.append(dc)

dc = DjnnClass("Image", "AbstractGShape", "src/gui/shapes")
dc.props.append(Prop('x', 'double', None, "transform"))
dc.props.append(Prop('y', 'double', None, "transform"))
dc.props.append(Prop('width', 'double', None, "geometry"))
dc.props.append(Prop('height', 'double', None, "geometry"))
dc.props.append(Prop('path', 'text', "0", "geometry"))
#dcs.append(dc)

dc = DjnnClass("Circle", "AbstractGShape", "../src/gui/shapes")
dc.props.append(Prop('cx', 'double', None, "transform"))
dc.props.append(Prop('cy', 'double', None, "transform"))
dc.props.append(Prop('r', 'double', None, "geometry"))
dcs.append(dc)


for dc in dcs:
    just_do_it(dc)

