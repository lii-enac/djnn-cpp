namespace djnn {
#ifndef DJNN_NO_DEBUG // still required to link smala programs
                      // DEBUG OPTIONS
int
    _DEBUG_SEE_COLOR_PICKING_VIEW                 = 0,
    _DEBUG_GRAPH_CYCLE_DETECT                     = 0,
    _DEBUG_SEE_ACTIVATION_SEQUENCE                = 0,
    _DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_TIME_US = 1000,
    _DEBUG_SEE_ACTIVATION_SEQUENCE_ONLY_TARGETED  = 0,
    _AUTHORIZE_CYCLE                              = 0,
    _DEBUG_SEE_RECOMPUTE_PIXMAP_AND_PAINTEVENT    = 0,
    _DEBUG_SEE_RECOMPUTE_PIXMAP_ONLY              = 0,
    _DEBUG_SEE_COMPONENTS_DESTRUCTION_INFO_LEVEL  = 0,
    _DEBUG_NO_TOUCH_EVENT                         = 1;

const char* _DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_LOCATION = "";
#endif
} // namespace djnn