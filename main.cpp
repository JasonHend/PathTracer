#include "math_helpers.h"

#include "camera.h"
#include "object.h"
#include "object_list.h"
#include "sphere.h"

int main() {
    ObjectList world;

    auto groundMaterial = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.Add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMaterial));

    // Create randomly placed spheres around the scene
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto chooseMaterial = RandomDouble();
            Point3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

            // Don't make objects that won't be rendered
            if ((center - Point3(4, 0.2, 0)).Length() > 0.9) {
                shared_ptr<Material> sphereMaterial;

                if (chooseMaterial < 0.8) {
                    auto albedo = Color::Random() * Color::Random();
                    sphereMaterial = make_shared<Lambertian>(albedo);
                    world.Add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                } else if (chooseMaterial < 0.95) {
                    auto albedo = Color::Random(0.5, 1);
                    auto fuzz = RandomDouble(0, 0.5);
                    sphereMaterial = make_shared<Metal>(albedo, fuzz);
                    world.Add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                } else {
                    sphereMaterial = make_shared<Dielectric>(1.5);
                    world.Add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
            }

            // Create static materials
            auto material1 = make_shared<Dielectric>(1.5);
            world.Add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

            auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
            world.Add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

            auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
            world.Add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));
        }
    }

    Camera mainCamera;

    mainCamera.aspectRatio = 16.0 / 9.0;
    mainCamera.imageWidth = 1200;
    mainCamera.samplesPerPixel = 500;
    mainCamera.maxDepth = 50;
    mainCamera.verticalFOV = 20;
    mainCamera.lookFrom = {13, 2, 3};
    mainCamera.lookAt = {0, 0, 0};
    mainCamera.upVector = {0, 1, 0};
    mainCamera.defocusAngle = 0.6;
    mainCamera.focusDistance = 10.0;

    mainCamera.lambertian = true;

    mainCamera.Render(world);
}
