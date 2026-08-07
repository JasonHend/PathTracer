#ifndef CAMERA_H
#define CAMERA_H

#include <algorithm>

#include "material.h"
#include "object.h"

class Camera {
public:
    // Aspect to calculate image height
    double aspectRatio = 1.0;
    int imageWidth = 100;

    // Determines how many comparisons to do to surrounding pixels
    int samplesPerPixel = 10;

    // Limit on how many times we will bounce a ray off a surface
    int maxDepth = 10;

    double verticalFOV = 90;

    // Directional vectors for camera orientation
    Point3 lookFrom = Point3(0, 0, 0);
    Point3 lookAt = Point3(0, 0, -1);
    Vec3 upVector = Vec3(0, 1, 0);

    // Defocus values
    double defocusAngle = 0;
    double focusDistance = 10;

    // Determines whether to use lambertian diffuse
    bool lambertian = false;

    void Render(const Object& world) {
        Initialize();

        std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

        for (int j = 0; j < imageHeight; j++) {
            std::clog << "\rScanlines remaining: " << (imageHeight - j) << "" << std::flush;
            for (int i = 0; i < imageWidth; i++) {
                Color pixelColor(0, 0, 0);
                for (int sample = 0; sample < samplesPerPixel; sample++) {
                    Ray r = GetRay(i, j);
                    pixelColor += RayColor(r, maxDepth, world);
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

    // Camera orientation vectors
    Vec3 u, v, w;

    // Defocus disk UVs for unit points
    Vec3 defocusDiskU;
    Vec3 defocusDiskV;

    // Initialization and ray coloration functions
    void Initialize() {
        // Calculate height from aspect ratio and width
        imageHeight = int(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;

        pixelSamplesScale = 1.0 / samplesPerPixel;

        center = lookFrom;

        // Camera set up
        auto theta = DegreesToRadians(verticalFOV);
        auto h = std::tan(theta / 2);
        auto viewportHeight = 2 * h * focusDistance;
        auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);

        // Calculate unit vectors based on the orientational vectors
        w = UnitVector(lookFrom - lookAt);
        u = UnitVector(Cross(upVector, w));
        v = UnitVector(Cross(w, u));

        // Calculate across and down vectors
        auto viewportU = viewportWidth * u;
        auto viewportV = viewportHeight * - v;

        // Calculate pixel data
        pixelDeltaU = viewportU / imageWidth;
        pixelDeltaV = viewportV / imageHeight;

        // Calculate the location of upper left hand pixel
        auto viewportUpperLeft =
            center - (focusDistance * w) - viewportU / 2 - viewportV / 2;
        pixel00Location = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

        // Calculate the defocus disk UVs
        auto defocusRadius = focusDistance * std::tan(DegreesToRadians(defocusAngle / 2));
        defocusDiskU = u * defocusRadius;
        defocusDiskV = v * defocusRadius;
    }

    // Gets a ray emitted from the camera to the offset (i, j)
    [[nodiscard]] Ray GetRay(int i, int j) const {
        auto offset = SampleSquare();
        auto pixelSample = pixel00Location
                            + ((i + offset.X()) * pixelDeltaU)
                            + ((j + offset.Y()) * pixelDeltaV);

        auto rayOrigin = (defocusAngle <= 0) ? center : DefocusDiskSample();
        auto rayDirection = pixelSample - rayOrigin;

        return {rayOrigin, rayDirection};
    }

    // Chooses a random sample in the unit square [-0.5, -0.5], [0.5, 0.5]
    [[nodiscard]] Vec3 SampleSquare() const {
        return {RandomDouble() - 0.5, RandomDouble() - 0.5, RandomDouble() - 0.5};
    }

    // Chooses a random point in the camera defocus disk
    [[nodiscard]] Point3 DefocusDiskSample() const {
        auto p = RandomInUnitDisk();
        return center + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
    }

    [[nodiscard]] Color RayColor(const Ray& r, int depth, const Object& world) const {
        // If we've hit the max depth that we can handle, no more light will be gathered
        if (depth <= 0)
            return {0, 0, 0};

        HitRecord hitRecord;
        if (world.Hit(r, Interval(0.001, infinity), hitRecord)) {
            // Utilize the reflectance function in material to calculate a bounced ray from the hit object
            Ray scattered;
            Color attenuation;
            if (hitRecord.material->Scatter(r, hitRecord, attenuation, scattered))
                return attenuation * RayColor(scattered, depth - 1, world);
            return {0, 0, 0};
        }

        // If we do not register a hit, render the "skybox"
        Vec3 unitDirection = UnitVector(r.Direction());
        auto a = 0.5 * (unitDirection.Y() + 1.0);
        return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }
};

#endif
