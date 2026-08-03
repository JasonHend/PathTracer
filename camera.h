#ifndef CAMERA_H
#define CAMERA_H

#include <algorithm>

#include "object.h"

class Camera {
public:
    // Aspect to calculate image height
    double aspectRatio = 1.0;
    int imageWidth = 100;

    // Determines how many comparisons to do to surrounding pixels
    int samplesPerPixel = 10;

    void Render(const Object& world) {
        Initialize();

        std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

        for (int j = 0; j < imageHeight; j++) {
            std::clog << "\rScanlines remaining: " << (imageHeight - j) << "" << std::flush;
            for (int i = 0; i < imageWidth; i++) {
                Color pixelColor(0, 0, 0);
                for (int sample = 0; sample < samplesPerPixel; sample++) {
                    Ray r = GetRay(i, j);
                    pixelColor += RayColor(r, world);
                }
                WriteColor(std::cout, pixelSamplesScale * pixelColor);
            }
        }
    }

private:
    int imageHeight;
    // Changes color interloping based on number of samples
    double pixelSamplesScale;
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

        pixelSamplesScale = 1.0 / samplesPerPixel;

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

    // Gets a ray emitted from the camera to the offset (i, j)
    [[nodiscard]] Ray GetRay(int i, int j) const {
        auto offset = SampleSquare();
        auto pixelSample = pixel00Location
                            + ((i + offset.X()) * pixelDeltaU)
                            + ((j + offset.Y()) * pixelDeltaV);

        auto rayOrigin = center;
        auto rayDirection = pixelSample - rayOrigin;

        return {rayOrigin, rayDirection};
    }

    // Chooses a random sample in the unit square [-0.5, -0.5], [0.5, 0.5]
    [[nodiscard]] Vec3 SampleSquare() const {
        return {RandomDouble() - 0.5, RandomDouble() - 0.5, RandomDouble() - 0.5};
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
