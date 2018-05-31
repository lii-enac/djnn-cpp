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
    djnButtCap, djnRoundCap, djnSquareCap
  };

  enum djnJoinStyle
  {
    djnMiterJoin, djnRoundCapJoin, djnBevelJoin
  };

  enum djnFillCoords
  {
    djnRenderingCoords, djnLocalCoords
  };

  enum djnFillSpread
  {
    djnPadFill, djnRepeatFill, djnReflectFill
  };

  enum djnTextEncoding
  {
    djnUtf8, djnLatin1, djnAscii
  };

  enum djnLengthUnit
  {
    djnNoLengthUnit,
    djnEmLength,
    djnExLength,
    djnPcLength,
    djnPtLength,
    djnPxLength,
    djnCmLength,
    djnMmLength,
    djnInLength,
    djnPercentLength
  };

  enum djnFontSlope
  {
    djnNormalFont, djnItalicFont, djnObliqueFont
  };

  enum djnAnchorType
  {
    djnStartAnchor, djnMiddleAnchor, djnEndAnchor
  };

  enum djnFillRuleType
  {
    djnNonZeroFill, djnEvenOddFill
  };

}

