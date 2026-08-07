#include "math_helpers.h"

#include "camera.h"
#include "object.h"
#include "object_list.h"
#include "sphere.h"

int main() {
    ObjectList world;

    auto materialGround = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto materialCenter = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto materialLeft = make_shared<Dielectric>(1.50);
    auto materialBubble = make_shared<Dielectric>(1.00 / 1.50);
    auto materialRight = make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

    world.Add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, materialGround));
    world.Add(make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, materialCenter));
    world.Add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, materialLeft));
    world.Add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.4, materialBubble));
    world.Add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, materialRight));

    Camera mainCamera;

    mainCamera.aspectRatio = 16.0 / 9.0;
    mainCamera.imageWidth = 400;
    mainCamera.samplesPerPixel = 100;
    mainCamera.maxDepth = 50;
    mainCamera.verticalFOV = 90;
    mainCamera.lookFrom = {-2, 2, 1};
    mainCamera.lookAt = {0, 0, -1};
    mainCamera.upVector = {0, 1, 0};

    mainCamera.lambertian = true;

    mainCamera.Render(world);
}
