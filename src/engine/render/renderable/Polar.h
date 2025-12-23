#ifndef LED_SEGMENTS_SPECS_POLAR_H
#define LED_SEGMENTS_SPECS_POLAR_H

#include "FastLED.h"
#include <functional>

class PolarContext;
using PolarCoords = std::function<void(uint16_t pixelIndex, PolarContext &context)>;

class PolarContext {
    uint16_t _lastSegmentSize = 0;
    uint16_t _angleUnit = 0;

public:
    uint16_t segmentIndex;
    uint16_t segmentSize;
    uint16_t cumulativePixels;
    fract16 angle;
    fract16 radius;

    PolarContext(
        uint16_t segmentIndex = 0,
        uint16_t segmentSize = 0,
        uint16_t cumulativePixels = 0,
        fract16 angle = 0,
        fract16 radius = 0
    ) : segmentIndex(segmentIndex),
        segmentSize(segmentSize),
        cumulativePixels(cumulativePixels),
        angle(angle),
        radius(radius) {
    }

    fract16 angleUnit() {
        if (segmentSize != _lastSegmentSize) {
            _angleUnit = segmentSize == 0 ? 0 : UINT16_MAX / segmentSize;
            _lastSegmentSize = segmentSize;
        }
        return _angleUnit;
    }

    void reset() {
        segmentIndex = 0;
        segmentSize = 0;
        cumulativePixels = 0;
        angle = 0;
        radius = 0;
    }

    void print() {
        Serial.print("segmentIndex: ");
        Serial.print(segmentIndex);
        Serial.print(" segmentSize: ");
        Serial.print(segmentSize);
        Serial.print(" cumulativePixels: ");
        Serial.print(cumulativePixels);
        Serial.print(" angle: ");
        Serial.print(angle);
        Serial.print(" radius: ");
        Serial.println(radius);
    }
};


#endif //LED_SEGMENTS_SPECS_POLAR_H
