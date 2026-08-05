#ifndef MATERIAL_H
#define MATERIAL_H

#include "object.h"
#include "vec3.h"

class Material {
public:
    virtual ~Material() = default;

    virtual  bool Scatter(
        const Ray& rayIn, const HitRecord& hitRecord, Color& attenuation, Ray& scattered
    ) const {
        return false;
    }
};

class Lambertian : public Material {
public:
    Lambertian(const Color& albedo) : albedo(albedo) {}

    bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, Color& attenuation, Ray& scattered)
    const override {
        auto scatterDirection = hitRecord.normal + RandomUnitVector();

        // Make sure the scatter is going somewhere and not just zero
        if (scatterDirection.NearZero())
            scatterDirection = hitRecord.normal;

        scattered = Ray(hitRecord.p, scatterDirection);
        attenuation = albedo;
        return true;
    }

private:
    Color albedo;
};

class Metal : public Material {
public:
    Metal(const Color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, Color& attenuation, Ray& scattered)
    const override {
        Vec3 reflected = Reflect(rayIn.Direction(), hitRecord.normal);
        reflected = UnitVector(reflected) + (fuzz * RandomUnitVector());
        scattered = Ray(hitRecord.p, reflected);
        attenuation = albedo;
        return (Dot(scattered.Direction(), hitRecord.normal) > 0);
    }

private:
    Color albedo;
    // Determines the radius of the sphere at which reflected rays will be scattered after reflectance,
    // resulting in a more physically correct metallic rendering.
    double fuzz;
};

#endif
