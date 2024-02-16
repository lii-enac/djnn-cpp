#pragma once

namespace djnn {
// process types
enum
    // class
    process_type_e {
        UNDEFINED_T,
        PROPERTY_T,
        ACTION_T,
        NATIVE_ACTION_T,
        NATIVE_COLLECTION_ACTION_T,
        CONTAINER_T,
        FSM_T,
        WINDOW_T,
        GOBJ_T,
        WORLD_T,
        DEFS_T,
        LAYER_T,
        SYNCHRONIZER_T,
        Z_ORDERED_GROUP_T,
        PROCESS_COLLECTOR_T
    };
// using enum process_type_e;

enum
    // class
    activation_flag_e {
        NONE_ACTIVATION,
        ACTIVATION,
        DEACTIVATION
    };
// using enum activation_flag_e;

enum
    // class
    activation_state_e {
        ACTIVATING,
        ACTIVATED,
        DEACTIVATING,
        DEACTIVATED
    };
// using enum activation_state_e;

enum notify_mask {
    notify_none              = 0,
    notify_generic           = 1 << 0,
    notify_damaged_geometry  = 1 << 1,
    notify_damaged_transform = 1 << 2,
    notify_damaged_style     = 1 << 3
};

enum
    // class
    child_position_e {
        FIRST,
        BEFORE,
        AFTER,
        LAST
    };

// using enum child_position_e;
} // namespace djnn