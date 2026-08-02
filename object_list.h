#ifndef OBJECT_LIST_H
#define OBJECT_LIST_H

#include "object.h"

#include <vector>

class ObjectList : public Object {
public:
    std::vector<shared_ptr<Object>> objects;

    ObjectList() = default;
    ObjectList(shared_ptr<Object> object) { Add(object); }

    void Clear() { objects.clear(); }

    void Add(shared_ptr<Object> object) {
        objects.push_back(object);
    }

    bool Hit(const Ray& r, Interval rayT, HitRecord& hitRecord) const override {
        HitRecord tempRecord;
        bool hitAnything = false;
        auto closestSoFar = rayT.max;

        for (const auto& object : objects) {
            if (object->Hit(r, Interval(rayT.min, closestSoFar), tempRecord)) {
                hitAnything = true;
                closestSoFar = tempRecord.t;
                hitRecord = tempRecord;
            }
        }

        return hitAnything;
    }
};

#endif
