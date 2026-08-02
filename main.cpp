#include "math_helpers.h"

#include "object.h"
#include "object_list.h"
#include "sphere.h"

Color RayColor(const Ray& r, const Object& world) {
    HitRecord rec;
    if (world.Hit(r, Interval(0, infinity), rec)) {
        return 0.5 * (rec.normal + Color(1, 1, 1));
    }

    Vec3 unitDirection = UnitVector(r.Direction());
    auto a = 0.5 * (unitDirection.Y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

int main() {
    // Image creation details
    auto aspectRatio = 16.0 / 9.0;
    int imageWidth = 400;

    // Calculate height from aspect ratio and width
    int imageHeight = int(imageWidth / aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    // Create objects for the world
    ObjectList world;

    world.Add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.Add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    // Camera set up
    auto focalLength = 1.0;
    auto viewportHeight = 2.0;
    auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);
    auto cameraCenter = Point3(0, 0, 0);

    // Calculate across and down vectors
    auto viewportU = Vec3(viewportWidth, 0, 0);
    auto viewportV = Vec3(0, -viewportHeight, 0);

    // Calculate pixel data
    auto pixelDeltaU = viewportU / imageWidth;
    auto pixelDeltaV = viewportV / imageHeight;

    // Calculate the location of upper left hand pixel
    auto viewportUpperLeft = cameraCenter
                           - Vec3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
    auto pixel00Location = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

    // Render work
    std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    for (int j = 0; j < imageHeight; j++) {
        std::clog << "\rScanlines remaining: " << (imageHeight - j) << "" << std::flush;
        for (int i = 0; i < imageWidth; i++) {
            auto pixelCenter = pixel00Location + (i * pixelDeltaU) + (j * pixelDeltaV);
            auto rayDirection = pixelCenter - cameraCenter;
            Ray r(cameraCenter, rayDirection);

            Color pixelColor = RayColor(r, world);
            WriteColor(std::cout, pixelColor);
        }
    }
}
