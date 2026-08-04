#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

using Color = Vec3;

// Square root conversion for gamma correction
inline double LinearToGamma(double linearComponent) {
    if (linearComponent > 0)
        return std::sqrt(linearComponent);

    return 0;
}

inline void WriteColor(std::ostream& out, const Color& pixelColor) {
    auto r = pixelColor.X();
    auto g = pixelColor.Y();
    auto b = pixelColor.Z();

    // Apply gamma correction to the pixel colors
    r = LinearToGamma(r);
    g = LinearToGamma(g);
    b = LinearToGamma(b);

    // Translate from [0,1] range to [0,255]
    static const Interval intensity(0.000, 0.999);
    int rByte = static_cast<int>(256 * intensity.Clamp(r));
    int gByte = static_cast<int>(256 * intensity.Clamp(g));
    int bByte = static_cast<int>(256 * intensity.Clamp(b));

    // Output pixel color components
    out << rByte << ' ' << gByte << ' ' << bByte << '\n';
}

#endif
