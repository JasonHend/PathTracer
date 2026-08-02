#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

class Sphere : public Object {
public:
    Sphere(const Point3& center, double radius): center(center), radius(std::fmax(0, radius)) {}

    // Utilizes simplified quadratic formula to determine if a ray has hit the sphere
    bool Hit(const Ray& r, Interval rayT, HitRecord& hitRecord) const override {
        Vec3 oc = center - r.Origin();
        auto a = r.Direction().LengthSquared();
        auto h = Dot(r.Direction(), oc);
        auto c = oc.LengthSquared() - radius * radius;

        auto discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find roots
        auto root = (h - sqrtd) / a;
        if (!rayT.Surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!rayT.Surrounds(root))
                return false;
        }

        hitRecord.t = root;
        hitRecord.p = r.At(hitRecord.t);
        Vec3 outwardNormal = (hitRecord.p - center) / radius;
        hitRecord.SetFaceNormal(r, outwardNormal);

        return true;
    }

private:
    Point3 center;
    double radius;
};

#endif
