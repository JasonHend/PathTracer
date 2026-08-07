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

class Dielectric : public Material {
public:
    Dielectric(double refractionIndex) : refractionIndex(refractionIndex) {}

    bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, Color& attenuation, Ray& scattered)
    const override {
        attenuation = {1.0, 1.0, 1.0};
        double ri = hitRecord.frontFace ? (1.0 / refractionIndex) : refractionIndex;

        Vec3 unitDirection = UnitVector(rayIn.Direction());
        double cosTheta = std::fmin(Dot(-unitDirection, hitRecord.normal), 1.0);
        double sinTheta = std::sqrt(1 - (cosTheta * cosTheta));

        bool cannotRefract = ri * sinTheta > 1.0;
        Vec3 direction;

        if (cannotRefract || Reflectance(cosTheta, ri) > RandomDouble())
            direction = Reflect(unitDirection, hitRecord.normal);
        else
            direction = Refract(unitDirection, hitRecord.normal, ri);

        scattered = {hitRecord.p, direction};
        return true;
    }

private:
    // Refraction index that will determine the amount at which an incoming ray will be refracted
    // when hitting an object with this material
    double refractionIndex;

    // Smooths out the result from refracted rays that utilize the schlick approximation
    static double Reflectance(double cosine, double refractionIndex) {
        auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

#endif
