#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"

class Camera {
public:
    // Aspect to calculate image height
    double aspectRatio = 1.0;
    int imageWidth = 100;

    void Render(const Object& world) {
        Initialize();

        std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

        for (int j = 0; j < imageHeight; j++) {
            std::clog << "\rScanlines remaining: " << (imageHeight - j) << "" << std::flush;
            for (int i = 0; i < imageWidth; i++) {
                auto pixelCenter = pixel00Location + (i * pixelDeltaU) + (j * pixelDeltaV);
                auto rayDirection = pixelCenter - center;
                Ray r(center, rayDirection);

                Color pixelColor = RayColor(r, world);
                WriteColor(std::cout, pixelColor);
            }
        }
    }

private:
    int imageHeight;
    Point3 center;
    Point3 pixel00Location;

    // Pixel offsets in horizontal and vertical
    Vec3 pixelDeltaU;
    Vec3 pixelDeltaV;

    // Initialization and ray coloration functions
    void Initialize() {
        // Calculate height from aspect ratio and width
        imageHeight = int(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;

        center = Point3(0, 0, 0);

        // Camera set up
        auto focalLength = 1.0;
        auto viewportHeight = 2.0;
        auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);

        // Calculate across and down vectors
        auto viewportU = Vec3(viewportWidth, 0, 0);
        auto viewportV = Vec3(0, -viewportHeight, 0);

        // Calculate pixel data
        pixelDeltaU = viewportU / imageWidth;
        pixelDeltaV = viewportV / imageHeight;

        // Calculate the location of upper left hand pixel
        auto viewportUpperLeft =
            center - Vec3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
        pixel00Location = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
    }

    [[nodiscard]] Color RayColor(const Ray& r, const Object& world) const {
        HitRecord hitRecord;
        if (world.Hit(r, Interval(0, infinity), hitRecord)) {
            return 0.5 * (hitRecord.normal + Color(1, 1, 1));
        }

        Vec3 unitDirection = UnitVector(r.Direction());
        auto a = 0.5 * (unitDirection.Y() + 1.0);
        return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }
};

#endif
