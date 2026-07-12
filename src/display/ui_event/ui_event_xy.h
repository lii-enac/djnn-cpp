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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */
EVENT(move, FatSpike, nullptr_)
EVENT(press, FatSpike, nullptr_)
EVENT(release, FatSpike, nullptr_)
EVENT(wheel, Component, wheel)

EVENT(mouse_move, FatSpike, nullptr_)
EVENT(mouse_press, FatSpike, nullptr_)
EVENT(mouse_release, FatSpike, nullptr_)
EVENT(mouse_wheel, FatSpike, nullptr_)

EVENT(stylus_pen_move, FatSpike, nullptr_)
EVENT(stylus_pen_press, FatSpike, nullptr_)
EVENT(stylus_pen_release, FatSpike, nullptr_)

EVENT(stylus_eraser_move, FatSpike, nullptr_)
EVENT(stylus_eraser_press, FatSpike, nullptr_)
EVENT(stylus_eraser_release, FatSpike, nullptr_)

// EVENT(left_press, FatSpike, nullptr_)
// EVENT(left_release, FatSpike, nullptr_)
// EVENT(right_press, FatSpike, nullptr_)
// EVENT(right_release, FatSpike, nullptr_)
// EVENT(middle_press, FatSpike, nullptr_)
// EVENT(middle_release, FatSpike, nullptr_)