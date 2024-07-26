/* generated with gen_prop.py */

function djnn_init_js_api () {

process_type_e = {
    UNDEFINED_T: 0,
    PROPERTY_T: 1,
    ACTION_T: 2,
    NATIVE_ACTION_T: 3,
    NATIVE_COLLECTION_ACTION_T: 4,
    CONTAINER_T: 5,
    FSM_T: 6,
    WINDOW_T: 7,
    GOBJ_T: 8,
    WORLD_T: 9,
    DEFS_T: 10,
    LAYER_T: 11,
    SYNCHRONIZER_T: 12
};
activation_flag_e = {
    NONE_ACTIVATION: 0,
    ACTIVATION: 1,
    DEACTIVATION: 2
};
djnn_dir_t  = {
    DJNN_GET_ON_CHANGE: 0,
    DJNN_SET_ON_CHANGE: 1,
    DJNN_IGNORE: 2
};
if (Object.freeze) {
  Object.freeze(process_type_e);
  Object.freeze(activation_flag_e);
  Object.freeze(djnn_dir_t);
}


init_core = Module.cwrap('djnn_init_core', 'null', []);
clear_core = Module.cwrap('djnn_clear_core', 'null', []);
init_exec_env = Module.cwrap('djnn_init_exec_env', 'null', []);
clear_exec_env = Module.cwrap('djnn_clear_exec_env', 'null', []);
init_base = Module.cwrap('djnn_init_base', 'null', []);
clear_base = Module.cwrap('djnn_clear_base', 'null', []);
init_display = Module.cwrap('djnn_init_display', 'null', []);
clear_display = Module.cwrap('djnn_clear_display', 'null', []);
init_gui = Module.cwrap('djnn_init_gui', 'null', []);
clear_gui = Module.cwrap('djnn_clear_gui', 'null', []);
init_audio = Module.cwrap('djnn_init_audio', 'null', []);
clear_audio = Module.cwrap('djnn_clear_audio', 'null', []);
init_physics = Module.cwrap('djnn_init_physics', 'null', []);
clear_physics = Module.cwrap('djnn_clear_physics', 'null', []);

activate = Module.cwrap('djnn_activate', 'null', ['number']);
get_process_type = Module.cwrap('djnn_get_process_type', 'number', ['number']);
find_child = Module.cwrap('djnn_find_child', 'number', ['number', 'string']);
get_activation_source = Module.cwrap('djnn_get_activation_source', 'number', ['number']);
get_native_user_data = Module.cwrap('djnn_get_native_user_data', 'number', ['number']);
set_value = Module.cwrap('djnn_set_value', 'null', ['number','number','number']);
get_value = Module.cwrap('djnn_get_value', 'number', ['number']);
get_string_value = Module.cwrap('djnn_get_string_value', 'text', ['number']);
dump = Module.cwrap('djnn_dump', 'null', ['number']);
mainloop_instance = Module.cwrap('djnn_mainloop_instance', 'number', []);
FillColor = function (p, n, r, g, b) {
    return typeof g !== "undefined" ? FillColor_rgb(p,n,r,g,b) : FillColor_value(p,n,r);
  }
Rectangle = Module.cwrap('djnn_new_Rectangle', 'number', ['number', 'string', 'number', 'number', 'number', 'number', 'number', 'number']);
Circle = Module.cwrap('djnn_new_Circle', 'number', ['number', 'string', 'number', 'number', 'number']);
Ellipse = Module.cwrap('djnn_new_Ellipse', 'number', ['number', 'string', 'number', 'number', 'number', 'number']);
Line = Module.cwrap('djnn_new_Line', 'number', ['number', 'string', 'number', 'number', 'number', 'number']);
RectangleClip = Module.cwrap('djnn_new_RectangleClip', 'number', ['number', 'string', 'number', 'number', 'number', 'number']);
PathImage = Module.cwrap('djnn_new_PathImage', 'number', ['number', 'string', 'string', 'number', 'number', 'number', 'number']);
DataImage = Module.cwrap('djnn_new_DataImage', 'number', ['number', 'string', 'string*', 'number', 'number', 'number', 'number', 'number']);
FillRule = Module.cwrap('djnn_new_FillRule', 'number', ['number', 'string', 'number']);
Texture = Module.cwrap('djnn_new_Texture', 'number', ['number', 'string', 'string']);
OutlineWidth = Module.cwrap('djnn_new_OutlineWidth', 'number', ['number', 'string', 'number']);
OutlineCapStyle = Module.cwrap('djnn_new_OutlineCapStyle', 'number', ['number', 'string', 'number']);
OutlineJoinStyle = Module.cwrap('djnn_new_OutlineJoinStyle', 'number', ['number', 'string', 'number']);
OutlineMiterLimit = Module.cwrap('djnn_new_OutlineMiterLimit', 'number', ['number', 'string', 'number']);
DashOffset = Module.cwrap('djnn_new_DashOffset', 'number', ['number', 'string', 'number']);
GradientStop = Module.cwrap('djnn_new_GradientStop', 'number', ['number', 'string', 'number', 'number', 'number', 'number', 'number']);
LinearGradient = Module.cwrap('djnn_new_LinearGradient', 'number', ['number', 'string', 'number', 'number', 'number', 'number', 'number', 'number']);
RadialGradient = Module.cwrap('djnn_new_RadialGradient', 'number', ['number', 'string', 'number', 'number', 'number', 'number', 'number', 'number', 'number']);
FontSize = Module.cwrap('djnn_new_FontSize', 'number', ['number', 'string', 'number', 'number']);
FontWeight = Module.cwrap('djnn_new_FontWeight', 'number', ['number', 'string', 'number']);
FontStyle = Module.cwrap('djnn_new_FontStyle', 'number', ['number', 'string', 'number']);
FontFamily = Module.cwrap('djnn_new_FontFamily', 'number', ['number', 'string', 'string']);
TextAnchor = Module.cwrap('djnn_new_TextAnchor', 'number', ['number', 'string', 'number']);
Translation = Module.cwrap('djnn_new_Translation', 'number', ['number', 'string', 'number', 'number']);
Rotation = Module.cwrap('djnn_new_Rotation', 'number', ['number', 'string', 'number', 'number', 'number']);
Scaling = Module.cwrap('djnn_new_Scaling', 'number', ['number', 'string', 'number', 'number', 'number', 'number']);
Homography = Module.cwrap('djnn_new_Homography', 'number', ['number', 'string', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number']);
Volume = Module.cwrap('djnn_new_Volume', 'number', ['number', 'string', 'number']);
Plane = Module.cwrap('djnn_new_Plane', 'number', ['number', 'string', 'number', 'number', 'number', 'number']);
Binding = Module.cwrap('djnn_new_Binding', 'number', ['number', 'string', 'number', 'number', 'number', 'number']);
Assignment = Module.cwrap('djnn_new_Assignment', 'number', ['number', 'string', 'number', 'number', 'number']);
Synchronizer = Module.cwrap('djnn_new_Synchronizer', 'number', ['number', 'string', 'number', 'string']);
Activator = Module.cwrap('djnn_new_Activator', 'number', ['number', 'string', 'number']);
NativeAction = Module.cwrap('djnn_new_NativeAction', 'number', ['number', 'string', 'string', 'number', 'number']);
Blank = Module.cwrap('djnn_new_Blank', 'number', ['number', 'string', '']);
Component = Module.cwrap('djnn_new_Component', 'number', ['number', 'string', '']);
AssignmentSequence = Module.cwrap('djnn_new_AssignmentSequence', 'number', ['number', 'string', 'number']);
List = Module.cwrap('djnn_new_List', 'number', ['number', 'string', '']);
ListIterator = Module.cwrap('djnn_new_ListIterator', 'number', ['number', 'string', 'number', 'number', 'number']);
Set = Module.cwrap('djnn_new_Set', 'number', ['number', 'string', '']);
SetIterator = Module.cwrap('djnn_new_SetIterator', 'number', ['number', 'string', 'number', 'number', 'number']);
Spike = Module.cwrap('djnn_new_Spike', 'number', ['number', 'string', '']);
RefProperty = Module.cwrap('djnn_new_RefProperty', 'number', ['number', 'string', 'number']);
RemoteProperty = Module.cwrap('djnn_new_RemoteProperty', 'number', ['number', 'string', 'string']);
IntProperty = Module.cwrap('djnn_new_IntProperty', 'number', ['number', 'string', 'number']);
DoubleProperty = Module.cwrap('djnn_new_DoubleProperty', 'number', ['number', 'string', 'number']);
TextProperty = Module.cwrap('djnn_new_TextProperty', 'number', ['number', 'string', 'string']);
BoolProperty = Module.cwrap('djnn_new_BoolProperty', 'number', ['number', 'string', 'number']);
Timer = Module.cwrap('djnn_new_Timer', 'number', ['number', 'string', 'number']);
Exit = Module.cwrap('djnn_new_Exit', 'number', ['number', 'string', 'number', 'number']);
Connector = Module.cwrap('djnn_new_Connector', 'number', ['number', 'string', 'number', 'number', 'number']);
Clock = Module.cwrap('djnn_new_Clock', 'number', ['number', 'string', 'number']);
Counter = Module.cwrap('djnn_new_Counter', 'number', ['number', 'string', 'number', 'number']);
Deref = Module.cwrap('djnn_new_Deref', 'number', ['number', 'string', 'number', 'string', 'number']);
DerefDouble = Module.cwrap('djnn_new_DerefDouble', 'number', ['number', 'string', 'number', 'string', 'number']);
DerefString = Module.cwrap('djnn_new_DerefString', 'number', ['number', 'string', 'number', 'string', 'number']);
Dictionary = Module.cwrap('djnn_new_Dictionary', 'number', ['number', 'string', '']);
Finder = Module.cwrap('djnn_new_Finder', 'number', ['number', 'string', 'number', 'string']);
FSM = Module.cwrap('djnn_new_FSM', 'number', ['number', 'string', '']);
FSMTransition = Module.cwrap('djnn_new_FSMTransition', 'number', ['number', 'string', 'number', 'number', 'number', 'number']);
FSMState = Module.cwrap('djnn_new_FSMState', 'number', ['number', 'string', '']);
HermiteCurve = Module.cwrap('djnn_new_HermiteCurve', 'number', ['number', 'string', 'number', 'number', 'number', 'number']);
LogPrinter = Module.cwrap('djnn_new_LogPrinter', 'number', ['number', 'string', 'string']);
SwitchList = Module.cwrap('djnn_new_SwitchList', 'number', ['number', 'string', 'number']);
SwitchRange = Module.cwrap('djnn_new_SwitchRange', 'number', ['number', 'string', 'number']);
Switch = Module.cwrap('djnn_new_Switch', 'number', ['number', 'string', 'string']);
TextPrinter = Module.cwrap('djnn_new_TextPrinter', 'number', ['number', 'string', '']);
Regex = Module.cwrap('djnn_new_Regex', 'number', ['number', 'string', 'string']);
TextAccumulator = Module.cwrap('djnn_new_TextAccumulator', 'number', ['number', 'string', 'string']);
TextComparator = Module.cwrap('djnn_new_TextComparator', 'number', ['number', 'string', '']);
TextCatenator = Module.cwrap('djnn_new_TextCatenator', 'number', ['number', 'string', '']);
WallClock = Module.cwrap('djnn_new_WallClock', 'number', ['number', 'string', '']);
Window = Module.cwrap('djnn_new_Window', 'number', ['number', 'string', 'string', 'number', 'number', 'number', 'number']);
FillColor_rgb = Module.cwrap('djnn_new_FillColor_rgb', 'number', ['number', 'string', 'number', 'number', 'number']);
FillColor_value = Module.cwrap('djnn_new_FillColor_value', 'number', ['number', 'string', 'number']);
OutlineColor = Module.cwrap('djnn_new_OutlineColor', 'number', ['number', 'string', 'number', 'number', 'number']);
Text = Module.cwrap('djnn_new_Text', 'number', ['number', 'string', 'number', 'number', 'string']);
}

