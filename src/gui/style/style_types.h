/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

namespace djnn
{

  enum djnCapStyle
  {
    DJN_BUTT_CAP, DJN_ROUND_CAP, DJN_SQUARE_CAP
  };

  enum djnJoinStyle
  {
    DJN_MITER_JOIN, DJN_ROUNDCAP_JOIN, DJN_BEVEL_JOIN
  };

  enum djnFillCoords
  {
    DJN_RENDERING_COORDS, DJN_LOCAL_COORDS
  };

  enum djnFillSpread
  {
    DJN_PAD_FILL, DJN_REPEAT_FILL, DJN_REFLECT_FILL
  };

  enum djnTextEncoding
  {
    DJN_UTF8, DJN_LATIN1, DJN_ASCII
  };

  enum djnLengthUnit
  {
    DJN_NO_UNIT,
    DJN_EM,
    DJN_EX,
    DJN_PC,
    DJN_PT,
    DJN_PX,
    DJN_CM,
    DJN_MM,
    DJN_IN,
    DJN_PERCENT
  };

  enum djnFontSlope
  {
    DJN_NORMAL_FONT, DJN_ITALIC_FONT, DJN_OBLIQUE_FONT
  };

  enum djnAnchorType
  {
    DJN_START_ANCHOR, DJN_MIDDLE_ANCHOR, DJN_END_ANCHOR
  };

  enum djnFillRuleType
  {
    DJN_NON_ZERO_FILL, DJN_EVENT_ODD_FILL
  };

}

