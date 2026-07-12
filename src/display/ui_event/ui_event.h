/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2026)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
EVENT(move, FatSpike, parent)
EVENT(press, FatSpike, parent)
EVENT(release, FatSpike, parent)
EVENT(wheel, Component, parent)
EVENT(double_click, FatSpike, parent)

EVENT(local_move, FatSpike, parent)
EVENT(local_press, FatSpike, parent)
EVENT(local_release, FatSpike, parent)
EVENT(local_wheel, FatSpike, parent)

EVENT2(mouse_move, FatSpike, mouse, move)
EVENT2(mouse_press, FatSpike, mouse, press)
EVENT2(mouse_release, FatSpike, mouse, release)
EVENT2(mouse_wheel, FatSpike, mouse, wheel)

EVENT2(mouse_move_local, FatSpike, parent, move_local)
EVENT2(mouse_press_local, FatSpike, parent, press_local)
EVENT2(mouse_release_local, FatSpike, parent, release_local)
EVENT2(mouse_wheel_local, FatSpike, parent, wheel_local)

EVENT2(stylus_pen_move, FatSpike, stylus_pen, move)
EVENT2(stylus_pen_press, FatSpike, stylus_pen, press)
EVENT2(stylus_pen_release, FatSpike, stylus_pen, release)

EVENT2(stylus_eraser_move, FatSpike, stylus_eraser, move)
EVENT2(stylus_eraser_press, FatSpike, stylus_eraser, press)
EVENT2(stylus_eraser_release, FatSpike, stylus_eraser, release)

// added for the sake of macros...
EVENT2(stylus_pen_move_local, FatSpike, stylus_pen, move_local)
EVENT2(stylus_pen_press_local, FatSpike, stylus_pen, press_local)
EVENT2(stylus_pen_release_local, FatSpike, stylus_pen, release_local)

EVENT2(stylus_eraser_move_local, FatSpike, stylus_eraser, move_local)
EVENT2(stylus_eraser_press_local, FatSpike, stylus_eraser, press_local)
EVENT2(stylus_eraser_release_local, FatSpike, stylus_eraser, relase_local)
//

EVENT(enter, FatSpike, parent)
EVENT(leave, FatSpike, parent)
EVENT(mouse_enter, FatSpike, parent)
EVENT(mouse_leave, FatSpike, parent)

EVENT2(left_press, FatSpike, left, press)
EVENT2(left_release, FatSpike, left, release)
EVENT2(left_double_click, FatSpike, left, double_click)
EVENT2(right_press, FatSpike, right, press)
EVENT2(right_release, FatSpike, right, release)
EVENT2(middle_press, FatSpike, middle, press)
EVENT2(middle_release, FatSpike, middle, release)

EVENT2(stylus_pen_enter, FatSpike, stylus_pen, enter)
EVENT2(stylus_pen_leave, FatSpike, stylus_pen, leave)
EVENT2(stylus_eraser_enter, FatSpike, stylus_eraser, enter)
EVENT2(stylus_eraser_leave, FatSpike, stylus_eraser, leave)