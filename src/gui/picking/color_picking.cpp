/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2014-2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stéphane Chatty <chatty@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 */
#include "color_picking.h"

#include "core/utils/iostream.h"
#include "core/utils/utils-dev.h"
#include "utils/debug.h"

namespace djnn {

ColorPickingView::ColorPickingView (Window* win)
    : Picking (win)
{
    init ();
}

ColorPickingView::~ColorPickingView ()
{
}

void
ColorPickingView::init ()
{
    _pick_color = 0xff000000; // for production
    //_pick_color = 0xff203040; // for debugging

    // --------- code before 07/2021
    //_color_map.clear (); // no correct anymore since Layers

    /*for (auto it = _color_map.cbegin(); it != _color_map.cend();) {
      if (!it->second.cache()) {
        if (it->first > _pick_color)
          _pick_color = it->first;
        _color_map.erase(it++);    // or "it = m.erase(it)" since C++11
      } else {
        ++it;
      }
    }*/

    // if (nocache) {
    //   _pick_color.clear();
    // } else {
    // -----------------

    // ---- code after 07/2021
    for (auto it = _color_map.cbegin (); it != _color_map.cend ();) {
        if (it->second->cache ()) {
            if (_pick_color < it->first) {
                _pick_color = it->first;
            }
            ++it;
        } else {
            it = _color_map.erase (it); // or "it = m.erase(it)" since C++11
        }
    }

    seed = 0; // no correct anymore since Layers

    next_color ();
}

double
ColorPickingView::myrandom ()
{
    seed = (seed * 9301 + 49297) % 233280;
    return seed / (233280.0);
}

void
ColorPickingView::next_color ()
{
// 11.2021 MP : bug when on picking with myrandom color
#if 1
    // if ( _DEBUG_SEE_COLOR_PICKING_VIEW == 0 )
    ++_pick_color;
    //_pick_color+=100;
#else
    // else {
    //  DEBUG MODE
    //  repeatable random, helpful for debugging
    //  (maxed out to 200 to see color on white bg)
    //  (mined out to 100 to see color on black bg)
    int r       = 100 + myrandom () * 155;
    int g       = 100 + myrandom () * 155;
    int b       = 100 + myrandom () * 155;
    _pick_color = ((((((0xff << 8) + r) << 8) + g) << 8) + b);
#endif
    //}

    // std::cerr << hex << _pick_color << dec << __FL__;
}

void
ColorPickingView::add_pick_shape (PickUI* pshape, bool cache)
{
    // if the object has to be cached then maybe it is already there, so remove it first
    // WARNING 1 - should we better do an update
    // WARNING 2 - how to be sure that there is no remaining invalid objects?
    if (cache)
        remove_pick_shape (pshape);

    // reset cache if it changed
    pshape->cache (cache);
    _color_map.insert (djnnstl::pair<unsigned int, PickUI*> (_pick_color, pshape));
    // save pick_color in PickUI interface
    pshape->color (_pick_color);
    next_color ();
}

void
ColorPickingView::remove_pick_shape (PickUI* pshape)
{
    // use the saved color in pickUI interface to accelerate find in _color_map
    auto it = _color_map.find (pshape->color ());
    if (it != _color_map.end ())
        _color_map.erase (it);
}

PickUI*
ColorPickingView::pick (double x, double y)
{
    int  color = get_pixel (x, y);
    auto it    = _color_map.find (color);

    // debug
    // std::cout << "pick color: " << std::hex << color << " obj: " << it->second << std::endl <<std::endl;

    // We added guard on has_ui because background_windows doesn't have ui all the time event if it is always in the _color_map
    if (it != _color_map.end () && it->second->has_ui ()) {
        return it->second;
    }
    return nullptr;
}

void
ColorPickingView::object_deactivated (PickUI* gobj)
{

    Picking::object_deactivated (gobj);

    // FIXME : should we delete object from picking_view when deactivated
    auto it = _color_map.find (gobj->color ());
    if (it != _color_map.end ())
        _color_map.erase (it);
}

} /* namespace djnn */
