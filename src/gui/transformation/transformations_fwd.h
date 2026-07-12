/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020-2024)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
#pragma once

namespace djnn {
class AbstractTransformation;
class Identity;
class Translation;
class GradientTranslation;
class Rotation;
class GradientRotation;
class Scaling;
class GradientScaling;
class SkewX;
class GradientSkewX;
class SkewY;
class GradientSkewY;
class AbstractHomography;
class Homography;
class GradientHomography;
class SimpleGradientTransform;
class ScreenToLocal;
class LocalToScreen;
} // namespace djnn