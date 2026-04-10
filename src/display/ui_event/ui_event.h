EVENT(move, Spike, parent)
EVENT(press, Spike, parent)
EVENT(release, Spike, parent)
EVENT(wheel, Component, parent)

EVENT(local_move, Spike, parent)
EVENT(local_press, Spike, parent)
EVENT(local_release, Spike, parent)
EVENT(local_wheel, Spike, parent)

EVENT(mouse_move, Spike, mouse)
EVENT(mouse_press, Spike, mouse)
EVENT(mouse_release, Spike, mouse)
EVENT(mouse_wheel, Spike, mouse)

EVENT(mouse_move_local, Spike, parent)
EVENT(mouse_press_local, Spike, parent)
EVENT(mouse_release_local, Spike, parent)
EVENT(mouse_wheel_local, Spike, parent)

EVENT(stylus_pen_move, Spike, stylus_pen)
EVENT(stylus_pen_press, Spike, stylus_pen)
EVENT(stylus_pen_release, Spike, stylus_pen)

EVENT(stylus_eraser_move, Spike, stylus_eraser)
EVENT(stylus_eraser_press, Spike, stylus_eraser)
EVENT(stylus_eraser_release, Spike, stylus_eraser)

// added for the sake of macros...
EVENT(stylus_pen_move_local, Spike, stylus_pen)
EVENT(stylus_pen_press_local, Spike, stylus_pen)
EVENT(stylus_pen_release_local, Spike, stylus_pen)

EVENT(stylus_eraser_move_local, Spike, stylus_eraser)
EVENT(stylus_eraser_press_local, Spike, stylus_eraser)
EVENT(stylus_eraser_release_local, Spike, stylus_eraser)
//

EVENT(enter, Spike, parent)
EVENT(leave, Spike, parent)
EVENT(mouse_enter, Spike, parent)
EVENT(mouse_leave, Spike, parent)

EVENT2(left_press, Spike, left, press)
EVENT2(left_release, Spike, left, release)
EVENT2(right_press, Spike, right, press)
EVENT2(right_release, Spike, right, release)
EVENT2(middle_press, Spike, middle, press)
EVENT2(middle_release, Spike, middle, release)

EVENT(stylus_pen_enter, Spike, stylus_pen)
EVENT(stylus_pen_leave, Spike, stylus_pen)
EVENT(stylus_eraser_enter, Spike, stylus_eraser)
EVENT(stylus_eraser_leave, Spike, stylus_eraser)