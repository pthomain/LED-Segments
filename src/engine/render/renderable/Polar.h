#ifndef LED_SEGMENTS_SPECS_POLAR_H
#define LED_SEGMENTS_SPECS_POLAR_H

#include "FastLED.h"
#include <functional>

class PolarContext;
using PolarCoords = std::function<void(uint16_t pixelIndex, PolarContext &context)>;

class PolarContext {
    uint16_t _lastSegmentSize = 0;
    uint8_t _angleUnit = 0;

public:
    uint16_t segmentIndex;
    uint16_t segmentSize;
    uint16_t cumulativePixels;
    uint8_t angle;
    uint8_t radius;

    PolarContext(
        uint16_t segmentIndex = 0,
        uint16_t segmentSize = 0,
        uint16_t cumulativePixels = 0,
        uint8_t angle = 0,
        uint8_t radius = 0
    ) : segmentIndex(segmentIndex),
        segmentSize(segmentSize),
        cumulativePixels(cumulativePixels),
        angle(angle),
        radius(radius) {
    }

    uint8_t angleUnit() {
        if (segmentSize != _lastSegmentSize) {
            _angleUnit = segmentSize == 0 ? 0 : map16_to_8(max(1, 65535 / segmentSize));
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
