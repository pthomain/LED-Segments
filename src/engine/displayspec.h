#ifndef LED_SEGMENTS_DISPLAYSPEC_H
#define LED_SEGMENTS_DISPLAYSPEC_H

#include <cstdint>
#include "FastLED.h"
#include "utils/utils.h"

class DisplaySpec {
private:

    uint16_t _maxSegmentSize = 0;

    void calculateMaxSegmentSize() {
        for (uint8_t layoutIndex = 0; layoutIndex < nbLayouts(); layoutIndex++) {
            for (uint8_t segmentIndex = 0; segmentIndex < nbSegments(layoutIndex); segmentIndex++) {
                _maxSegmentSize = max(_maxSegmentSize, segmentSize(layoutIndex, segmentIndex));
            }
        }
    }

public:

    explicit DisplaySpec() {}

    virtual uint16_t nbLeds() const = 0;

    virtual uint16_t nbLayouts() const = 0;

    virtual String layoutName(const uint16_t layoutIndex) const = 0;

    virtual uint16_t maxSegmentSize() const final {
        if (_maxSegmentSize == 0) const_cast<DisplaySpec *>(this)->calculateMaxSegmentSize();
        return _maxSegmentSize;
    };

    virtual uint16_t nbSegments(const uint16_t layoutIndex) const = 0;

    virtual uint16_t segmentSize(const uint16_t layoutIndex, const uint16_t segmentIndex) const = 0;

    virtual void setColour(
            const uint16_t layoutIndex,
            const uint16_t segmentIndex,
            const uint16_t pixelIndex,
            const uint16_t frameIndex,
            CRGB *outputArray,
            const CRGB colour
    ) const = 0;

    virtual ~DisplaySpec() = default;
};

#endif //LED_SEGMENTS_DISPLAYSPEC_H
