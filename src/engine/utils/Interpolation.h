// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Pierre Thomain

#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <cmath>

namespace LEDSegments {

class Interpolation {
public:
    template<typename T>
    static T easeOutQuad(T x) {
        return 1 - (1 - x) * (1 - x);
    }

    template<typename T>
    static T easeInOutQuad(T x) {
        return x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2;
    }

    template<typename T>
    static T easeInQuad(T x) {
        return x * x;
    }

    template<typename T>
    static T easeInQuart(T x) {
        return x * x * x * x;
    }

    template<typename T>
    static T fastSlowFast(T x) {
        return 4*x*x*x - 6*x*x + 3*x;
    }

    template<typename T>
    static T fastSlowFastHalfSpeed(T x) {
        return 2*x*x*x - 3*x*x + 2*x;
    }

    template<typename T>
    static T easeOutQuart(T x) {
        return 1 - pow(1 - x, 4);
    }
};

} // namespace LEDSegments

#endif // INTERPOLATION_H
