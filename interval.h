#ifndef INTERVAL_H
#define INTERVAL_H

class Interval {
public:
    double min, max;

    // Default constructor (will make an empty interval
    Interval() : min(+infinity), max (-infinity) {}

    // Parametrized constructor that takes in a min and max value
    Interval(double min, double max) : min(min), max(max) {}

    [[nodiscard]] double Size() const {
        return max - min;
    }

    [[nodiscard]] bool Contains(double x) const {
        return min <= x && x <= max;
    }

    [[nodiscard]] bool Surrounds(double x) const {
        return min < x && x < max;
    }

    static const Interval empty, universe;
};

const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);

#endif
