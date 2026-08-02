#ifndef OBJECT_H
#define OBJECT_H

class HitRecord {
public:
    Point3 p;
    Vec3 normal;
    double t{};

    // Information regarding back facing vs. front facing and setting normals
    bool frontFace;
    void SetFaceNormal(const Ray& r, const Vec3& outwardNormal) {
        frontFace = Dot(r.Direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Object {
public:
    virtual ~Object() = default;

    virtual bool Hit(const Ray& r, Interval rayT, HitRecord& hitRecord) const = 0;
};

#endif
