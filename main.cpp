#include "math_helpers.h"

#include "camera.h"
#include "object.h"
#include "object_list.h"
#include "sphere.h"

int main() {
    ObjectList world;

    world.Add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.Add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    Camera mainCamera;

    mainCamera.aspectRatio = 16.0 / 9.0;
    mainCamera.imageWidth = 400;
    mainCamera.samplesPerPixel = 100;
    mainCamera.maxDepth = 50;

    mainCamera.Render(world);
}
