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

#endif