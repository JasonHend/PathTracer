#ifndef VEC3_H
#define VEC3_H

class Vec3 {
    public:
    // Array to hold axis values
    double e[3];

    // Base constructor
    Vec3() : e{0, 0, 0} {}

    // Overload for setting discretionary values
    Vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    // Accessors for axes
    [[nodiscard]] double X() const { return e[0]; }
    [[nodiscard]] double Y() const { return e[1]; }
    [[nodiscard]] double Z() const { return e[2]; }

    // Negation operation and accessors
    [[nodiscard]] Vec3 operator-() const { return {-e[0], -e[1], -e[2]}; }
    [[nodiscard]] double operator[](int i) const { return e[i]; }
    [[nodiscard]] double& operator[](int i) {return e[i]; }

    Vec3& operator+=(const Vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vec3& operator/=(double t) {
        return *this *= 1 / t;
    }

    [[nodiscard]] double Length() const {
        return std::sqrt(LengthSquared());
    }

    [[nodiscard]] double LengthSquared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    // Checks if a vector's value is close to zero
    [[nodiscard]] bool NearZero() const {
        auto s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < 2);
    }

    // Creates a random vector of doubles
    [[nodiscard]] static Vec3 Random() {
       return {RandomDouble(), RandomDouble(), RandomDouble()};
    }

    // Creates a random vector where each value falls between [min, max)
    [[nodiscard]] static Vec3 Random(double min, double max) {
        return {RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max)};
    }
};

// Create an alias to describe points in 3D space
using Point3 = Vec3;

// Utility functions

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
    return {u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]};
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
    return {u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]};
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return {u.e[0] * v[0], u.e[1] * v[1], u.e[2] * v[2]};
}

inline Vec3 operator*(const Vec3& v, double t) {
    return {t * v.e[0], t * v.e[1], t * v.e[2]};
}

inline Vec3 operator*(double t, const Vec3& v) {
    return {t * v.e[0], t * v.e[1], t * v.e[2]};
}

inline Vec3 operator/(const Vec3& v, double t) {
    return (1 / t) * v;
}

inline double Dot(const Vec3& u, const Vec3& v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline Vec3 Cross(const Vec3& u, const Vec3& v) {
    return {u.e[1] * v.e[2] - u.e[2] * v.e[1],
               u.e[2] * v.e[0] - u.e[0] * v.e[2],
               u.e[0] * v.e[1] - u.e[1] * v.e[0]};
}

inline Vec3 UnitVector(const Vec3& v) {
    return v / v.Length();
}

// Creates a unit vector from a random point in the unit sphere
inline Vec3 RandomUnitVector() {
    while (true) {
        auto p = Vec3::Random(-1, 1);
        auto lengthSquared = p.LengthSquared();
        if (1e-160 < lengthSquared && lengthSquared <= 1)
            return p / sqrt(lengthSquared);
    }
}

// Determines if the vector is facing the correct way (out from the object)
inline Vec3 RandomOnHemisphere(const Vec3& normal) {
    // Get a unit sphere vector and take the dot product
    Vec3 onUnitSphere = RandomUnitVector();
    // If the dot is positive, it's going in the right direction, if not, negate
    if (Dot(onUnitSphere, normal) > 0.0)
        return onUnitSphere;
    else
        return -onUnitSphere;
}

// Reflectance function utilizing the dot product of an incoming vector and the normal vector
inline Vec3 Reflect(const Vec3& v, const Vec3& n) {
    return v - 2 * Dot(v, n) * n;
}

// Refracting formula, splitting a ray into it's perpendicular and parallel quantities and utilizing the law of cosines
inline Vec3 Refract(const Vec3& uv, const Vec3& n, double etaiOverEtat) {
    auto cosTheta = std::fmin(Dot(-uv, n), 1.0);
    Vec3 rayOutPerp = etaiOverEtat * (uv + cosTheta * n);
    Vec3 rayOutParallel = -std::sqrt(std::fabs(1.0 - rayOutPerp.LengthSquared())) * n;
    return rayOutPerp + rayOutParallel;
}

#endif