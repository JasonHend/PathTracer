#ifndef INTERVAL_H
#define INTERVAL_H

class Interval {
public:
    double min, max;

    // Default constructor (will make an empty interval
    Interval() : min(+infinity), max (-infinity) {}

    // Parametrized constructor that takes in a min and max value
    Interval(double min, double max) : min(min), max(max) {}

    // Returns the overall length of the array
    [[nodiscard]] double Size() const {
        return max - min;
    }

    // Checks to see if a value x is within [min, max]
    [[nodiscard]] bool Contains(double x) const {
        return min <= x && x <= max;
    }

    // Checks to see if a value x is within (min, max)
    [[nodiscard]] bool Surrounds(double x) const {
        return min < x && x < max;
    }

    // Returns a value that does not go outside the bounds of x
    [[nodiscard]] double Clamp(double x) const {
        if (min > x) return min;
        if (max < x) return max;
        return x;
    }

    static const Interval empty, universe;
};

const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);

#endif
