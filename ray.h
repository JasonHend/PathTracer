#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
public:
    Ray() = default;

    Ray(const Point3& origin, const Vec3& direction) : origin(origin), direction(direction) {}

    [[nodiscard]] const Point3& Origin() const { return origin; }
    [[nodiscard]] const Vec3& Direction() const { return direction; }

    [[nodiscard]] Point3 At(double t) const {
        return origin + t * direction;
    }

private:
    Point3 origin;
    Vec3 direction;
};

#endif