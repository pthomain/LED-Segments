#ifndef LED_SEGMENTS_DISPLAYSPEC_H
#define LED_SEGMENTS_DISPLAYSPEC_H

#include <cstdint>
#include <engine/effect/Effect.h>
#include "utils/Utils.h"
#include "engine/displayspec/LayoutCatalog.h"

class DisplaySpec {
    uint16_t _maxSegmentSize = 0;

    void calculateMaxSegmentSize() {
        for (uint8_t layoutIndex = 0; layoutIndex < nbLayouts(); layoutIndex++) {
            for (uint8_t segmentIndex = 0; segmentIndex < nbSegments(layoutIndex); segmentIndex++) {
                _maxSegmentSize = max(_maxSegmentSize, nbPixels(layoutIndex, segmentIndex));
            }
        }
    }

protected:
    const LayoutCatalog _catalog;

public:
    explicit DisplaySpec(const LayoutCatalog &catalog): _catalog(catalog) {
    }

    virtual uint16_t nbLeds() const = 0;

    virtual uint16_t nbLayouts() const = 0;

    virtual String layoutName(const uint16_t layoutIndex) const = 0;

    virtual bool isCircular() const { return false; }

    virtual uint16_t maxSegmentSize() const final {
        if (_maxSegmentSize == 0) const_cast<DisplaySpec *>(this)->calculateMaxSegmentSize();
        return _maxSegmentSize;
    };

    virtual uint16_t nbSegments(const uint16_t layoutIndex) const = 0;

    virtual uint16_t nbPixels(const uint16_t layoutIndex, const uint16_t segmentIndex) const = 0;

    virtual void setColour(
        const uint16_t layoutIndex,
        const uint16_t segmentIndex,
        const uint16_t pixelIndex,
        const uint16_t frameIndex,
        CRGB *outputArray,
        const CRGB colour
    ) const = 0;

    const LayoutCatalog &catalog() const {
        return _catalog;
    }

    virtual ~DisplaySpec() = default;
};

#endif //LED_SEGMENTS_DISPLAYSPEC_H
